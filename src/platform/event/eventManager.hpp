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

#ifndef _PLATFORM_EVENT_EVENTMANAGER_HPP_
#define _PLATFORM_EVENT_EVENTMANAGER_HPP_

#include <vector>
#include <SDL.h>

class IKeyboardEvent;
class IWindowEvent;
class IMouseMovementEvent;
class IMouseButtonEvent;

class EventManager {
public:
	bool pullEvents() const;
	
	void dispatchKeyEvent(const SDL_Event &e) const;
	void dispatchMouseButtonEvent(const SDL_Event &e) const;
	void dispatchMouseMotionEvent(const SDL_Event &e) const;
	void dispatchWindowEvent(const SDL_Event &e) const;

	void addEvent(const IKeyboardEvent *inputEvent);
	void addEvent(const IWindowEvent *windowEvent);
	void addEvent(const IMouseMovementEvent *mouseMovementEvent);
	void addEvent(const IMouseButtonEvent *mouseButtonEvent);
	void removeEvent(const IKeyboardEvent *inputEvent);
	void removeEvent(const IWindowEvent *windowEvent);
	void removeEvent(const IMouseMovementEvent *mouseMovementEvent);
	void removeEvent(const IMouseButtonEvent *mouseButtonEvent);

private:
	std::vector<const IKeyboardEvent*> mKeyboardEvents;
	std::vector<const IMouseMovementEvent*> mMouseMovmentEvents;
	std::vector<const IMouseButtonEvent*> mMouseButtonEvents;
	std::vector<const IWindowEvent*> mWindowEvents;
};

extern EventManager gEventManager;

#endif // _PLATFORM_EVENT_EVENTMANAGER_HPP_