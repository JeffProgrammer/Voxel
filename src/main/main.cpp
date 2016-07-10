#include <SDL.h>
#include "platform/window.hpp"
#include "platform/event/eventManager.hpp"
#include "game/camera.hpp"
#undef main

int main(int argc, const char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// create window and pause for 1 second.
	Window window("Test", 1440, 900, Window::Flags::NONE, ContextAPI::OpenGL);
	Camera camera;
	while (gEventManager.pullEvents()) {
		RENDERER->beginFrame();
		RENDERER->renderSingleCube();
		RENDERER->endFrame();
		window.swapBuffers();
	}
	
	SDL_Quit();
	return 0;
}