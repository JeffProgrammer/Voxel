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

#include <assert.h>
#include <SDL.h>
#include "platform/event/eventManager.hpp"

EventManager gEventManager;

void EventManager::pullEvents() const {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

	}
}

void EventManager::addEvent(const IInputEvent *inputEvent) {
	mInputEvents.push_back(inputEvent);
}

void EventManager::addEvent(const IWindowEvent *windowEvent) {
	mWindowEvents.push_back(windowEvent);
}

void EventManager::removeEvent(const IInputEvent *inputEvent) {
	assert(std::find(mInputEvents.begin(), mInputEvents.end(), inputEvent) != mInputEvents.end());
	mInputEvents.erase(std::find(mInputEvents.begin(), mInputEvents.end(), inputEvent));
}

void EventManager::removeEvent(const IWindowEvent *windowEvent) {
	assert(std::find(mWindowEvents.begin(), mWindowEvents.end(), windowEvent) != mWindowEvents.end());
	mWindowEvents.erase(std::find(mWindowEvents.begin(), mWindowEvents.end(), windowEvent));
}