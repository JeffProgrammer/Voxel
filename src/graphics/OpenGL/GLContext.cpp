//-----------------------------------------------------------------------------
// Copyright (c) 2014-2016, Jeff Hutchinson
// Copyright (c) 2014-2016 Glenn Smith
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

#include <assert.h>
#include <SDL.h>
#include "graphics/OpenGL/GLContext.hpp"
#include "graphics/OpenGL/GLRenderer.hpp"
#include "platform/window.hpp"

GLContext::GLContext() : mContext(nullptr), mGlobalVAO(0) {

}

void GLContext::initContext() {
	mRenderer = new GLRenderer();

	// Set core profile flags for OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// set other window hints.
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	mContext = SDL_GL_CreateContext(mWindow->getSDLWindow());
	if (mContext == NULL) {
		assert(false);
		return;
	}

	SDL_GL_MakeCurrent(mWindow->getSDLWindow(), mContext);

	// Initialize glad
	if (!gladLoadGL()) {
		assert(false);
		return;
	}

	// The core profile requires a VAO to be bound before quite a bit of specific GL calls are made.
	// We'll just create a global state VAO for now so that we can just call GL functions.
	glGenVertexArrays(1, &mGlobalVAO);
	glBindVertexArray(mGlobalVAO);

	// Set Clear color
	glClearColor(0.0f, 1.0f, 1.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLContext::destroy() {
	if (mContext != nullptr) {
		SDL_GL_DeleteContext(mContext);
		SDL_GL_MakeCurrent(NULL, NULL);

		// Delete the VAO
		if (glIsVertexArray(mGlobalVAO)) {
			glDeleteVertexArrays(1, &mGlobalVAO);
		}
	}
	delete mRenderer;
}

void GLContext::swapBuffers() const {
	SDL_GL_SwapWindow(mWindow->getSDLWindow());
}

Renderer* GLContext::getRenderer() const {
	return mRenderer;
}