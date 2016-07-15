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
#include "core/algorithm.hpp"
#include "core/types.hpp"
#include "platform/event/eventManager.hpp"
#include "platform/event/interface/IKeyboardEvent.hpp"
#include "platform/event/interface/IMouseButtonEvent.hpp"
#include "platform/event/interface/IMouseMovementEvent.hpp"
#include "platform/event/interface/IWindowEvent.hpp"

EventManager gEventManager;

bool EventManager::pullEvents(const F64 &delta) const {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_EventType::SDL_KEYDOWN:
			case SDL_EventType::SDL_KEYUP:
				dispatchKeyEvent(e, delta);
				break;
			case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			case SDL_EventType::SDL_MOUSEBUTTONUP:
				dispatchMouseButtonEvent(e, delta);
				break;
			case SDL_EventType::SDL_MOUSEMOTION:
				dispatchMouseMotionEvent(e, delta);
				break;
			case SDL_EventType::SDL_WINDOWEVENT:
				dispatchWindowEvent(e, delta);
				break;
			case SDL_EventType::SDL_QUIT:
				return false;
		}
	}
	return true;
}

void EventManager::dispatchKeyEvent(const SDL_Event &e, const F64 &delta) const {
	KeyboardEvent ev;
	ev.frameDelta = delta;
	ev.isPressedDown = (e.type == SDL_EventType::SDL_KEYDOWN);
	ev.scanCode = e.key.keysym.scancode;
	
	for (const auto i : mKeyboardEvents) {
		i->processKeyboard(ev);
	}
}

void EventManager::dispatchMouseButtonEvent(const SDL_Event &e, const F64 &delta) const {
	MouseButtonEvent ev;
	ev.frameDelta = delta;
	ev.leftClick = e.button.button == SDL_BUTTON_LEFT;
	ev.rightClick = e.button.button == SDL_BUTTON_RIGHT;
	ev.middleClick = e.button.button == SDL_BUTTON_MIDDLE;
	ev.doubleClick = e.button.clicks >= 2;
	
	for (const auto i : mMouseButtonEvents) {
		i->processMouseButton(ev);
	}
}

void EventManager::dispatchMouseMotionEvent(const SDL_Event &e, const F64 &delta) const {
	MouseMovementEvent ev;
	ev.frameDelta = delta;
	ev.mousePosition = glm::vec2(e.motion.x, e.motion.y);
	ev.mouseDelta = glm::vec2(e.motion.xrel, e.motion.yrel);
	
	for (const auto i : mMouseMovmentEvents) {
		i->processMouseMovement(ev);
	}
}

void EventManager::dispatchWindowEvent(const SDL_Event &e, const F64 &delta) const {
	WindowEvent ev;
	ev.frameDelta = delta;
	ev.gainedFocus = e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED;
	ev.lostFocus = e.window.event == SDL_WINDOWEVENT_FOCUS_LOST;
	
	// TODO: dispatch window events.
}

//-----------------------------------------------------------------------------

void EventManager::addEvent(IWindowEvent *windowEvent) {
	assert(!Algorithm::contains(mWindowEvents, windowEvent));
	mWindowEvents.push_back(windowEvent);
}

void EventManager::addEvent(IKeyboardEvent *inputEvent) {
	assert(!Algorithm::contains(mKeyboardEvents, inputEvent));
	mKeyboardEvents.push_back(inputEvent);
}

void EventManager::addEvent(IMouseMovementEvent *mouseMovementEvent) {
	assert(!Algorithm::contains(mMouseMovmentEvents, mouseMovementEvent));
	mMouseMovmentEvents.push_back(mouseMovementEvent);
}

void EventManager::addEvent(IMouseButtonEvent *mouseButtonEvent) {
	assert(!Algorithm::contains(mMouseButtonEvents, mouseButtonEvent));
	mMouseButtonEvents.push_back(mouseButtonEvent);
}

void EventManager::removeEvent(IWindowEvent *windowEvent) {
	assert(Algorithm::contains(mWindowEvents, windowEvent));
	mWindowEvents.erase(std::find(mWindowEvents.begin(), mWindowEvents.end(), windowEvent));
}

void EventManager::removeEvent(IKeyboardEvent *inputEvent) {
	assert(Algorithm::contains(mKeyboardEvents, inputEvent));
	mKeyboardEvents.erase(std::find(mKeyboardEvents.begin(), mKeyboardEvents.end(), inputEvent));
}

void EventManager::removeEvent(IMouseMovementEvent *mouseMovementEvent) {
	assert(Algorithm::contains(mMouseMovmentEvents, mouseMovementEvent));
	mMouseMovmentEvents.erase(std::find(mMouseMovmentEvents.begin(), mMouseMovmentEvents.end(), mouseMovementEvent));
}

void EventManager::removeEvent(IMouseButtonEvent *mouseButtonEvent) {
	assert(Algorithm::contains(mMouseButtonEvents, mouseButtonEvent));
	mMouseButtonEvents.erase(std::find(mMouseButtonEvents.begin(), mMouseButtonEvents.end(), mouseButtonEvent));
}