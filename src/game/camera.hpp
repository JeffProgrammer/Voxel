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

#ifndef _GAME_CAMERA_HPP_
#define _GAME_CAMERA_HPP_

#include "game/gameObject.hpp"
#include "platform/event/interface/IMouseMovementEvent.hpp"
#include "platform/event/interface/IKeyboardEvent.hpp"

class Camera : public GameObject, public IMouseMovementEvent, public IKeyboardEvent {
public:
	const F32 MOUSE_SENSATIVITY = 0.15f;
	const F32 CAMERA_MOVEMENT_SPEED = 7.5f;
	
	Camera();
	
	virtual void processMouseMovement(const MouseMovementEvent &e) override;
	virtual void processKeyboard(const KeyboardEvent &e) override;
	
	void getYawPitch(float &yaw, float &pitch) const;
	
	glm::vec3 getFrontVector() const;
	glm::vec3 getUpVector() const;

	virtual void update(const double &delta) override;
	
private:
	F32 mPitch;
	F32 mYaw;
	
	struct Move {
		bool forward;
		bool backward;
		bool left;
		bool right;
	} mMove;
	
	glm::vec3 mFrontVector;
	glm::vec3 mUpVector;
};

#endif // _GAME_CAMERA_HPP_