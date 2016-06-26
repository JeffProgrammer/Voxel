#include <SDL.h>
#include "platform/window.hpp"
#undef main

int main(int argc, const char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// create window and pause for 1 second.
	Window window("Test", 1440, 900, Window::Flags::NONE, ContextAPI::OpenGL);
	SDL_Delay(1000);

	SDL_Quit();
	return 0;
}