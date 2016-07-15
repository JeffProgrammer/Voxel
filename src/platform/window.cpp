//-----------------------------------------------------------------------------
// Copyright (c) 2016, Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the project nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "platform/window.hpp"

Window::Window(const std::string &title, S32 width, S32 height, Flags flags, ContextAPI api) {
	// Setup window flags
	U32 sdlFlags = SDL_WindowFlags::SDL_WINDOW_SHOWN;
	if (api == ContextAPI::OpenGL)
		sdlFlags |= SDL_WindowFlags::SDL_WINDOW_OPENGL;
	if (flags & Flags::FULLSCREEN)
		sdlFlags |= SDL_WindowFlags::SDL_WINDOW_FULLSCREEN;
	if (flags & Flags::HIGHDPI)
		sdlFlags |= SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI;

	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlFlags);
	
	SDL_GL_SetSwapInterval(1);

	// Create a context and initialize it.
	mContext = ContextFactory::createContext(api);
	mContext->init(this);

	// set the current context so that global variables can be used.
	ContextFactory::setCurrentContext(mContext);
}

Window::~Window() {
	// destroy context and window
	ContextFactory::releaseContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Window::swapBuffers() {
	mContext->swapBuffers();
}

void Window::lockCursor(bool lock) {
	SDL_ShowCursor(lock);
	SDL_SetRelativeMouseMode(lock ? SDL_TRUE : SDL_FALSE);
}

void Window::processKeyboard(const KeyboardEvent &keyboardEvent) {
	// toggle cursor state.
	if (keyboardEvent.isPressedDown && keyboardEvent.scanCode == SDL_SCANCODE_ESCAPE)
		lockCursor(!SDL_ShowCursor(SDL_QUERY));
}