#include <SDL.h>
#include "platform/window.hpp"
#include "platform/timer.hpp"
#include "platform/event/eventManager.hpp"
#include "game/camera.hpp"
#undef main

#include <Windows.h>

int main(int argc, const char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	ContextAPI api = ContextAPI::OpenGL;

	// Need Windows 8 SDK to compile with D3D11 runtime we use.
#if _WIN32_WINNT >= 0x602
	for (int i = 0; i < argc; ++i) {
		if (SDL_strcasecmp(argv[i], "-d3d11") == 0) {
			// If we pass -d3d11 then run in DX11 mode.
			api = ContextAPI::D3D11;
			break;
		}
	}
#endif

	// create window and pause for 1 second.
	Window *window = new Window("Test", 1440, 900, Window::Flags::NONE, ContextAPI::OpenGL);
	Camera camera;
	camera.setPosition(glm::vec3(3.0f, 3.0f, -3.0f));
	RENDERER->setActiveSceneCamera(&camera);
	
	Timer timer;
	
	while (gEventManager.pullEvents(timer.getDelta())) {
		camera.update(timer.getDelta());

		timer.start();
		RENDERER->beginFrame();
		RENDERER->renderSingleCube();
		RENDERER->endFrame();
		window->swapBuffers();
		timer.stop();
	}
	delete window;
	
	SDL_Quit();
	return 0;
}