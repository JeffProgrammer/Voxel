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

#include "game/camera.hpp"

Camera::Camera() {
	mFrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	
	mYaw = -90.0f;
	mPitch = 0.0f;

	mMove = { false, false, false, false };
}

void Camera::getYawPitch(float &yaw, float &pitch) const {
	yaw = mYaw;
	pitch = mPitch;
}

glm::vec3 Camera::getFrontVector() const {
	return mFrontVector;
}

glm::vec3 Camera::getUpVector() const {
	return mUpVector;
}

void Camera::processMouseMovement(const MouseMovementEvent &e) {
	mYaw += e.mouseDelta.x * MOUSE_SENSATIVITY;
	mPitch += e.mouseDelta.y * MOUSE_SENSATIVITY;
	
	// Cap pitch
	mPitch = glm::clamp(mPitch, -89.999f, 89.999f);
	
	// convert yaw/pitch to radians.
	float yaw = glm::radians(mYaw);
	float pitch = glm::radians(mPitch);
	
	// Adjust front vector
	glm::vec3 front(0.0f);
	front.x = glm::cos(yaw) * glm::cos(pitch);
	front.y = glm::sin(pitch);
	front.z = glm::sin(yaw) * glm::cos(pitch);
	mFrontVector = glm::normalize(front);
}

void Camera::processKeyboard(const KeyboardEvent &e) {
	switch (e.scanCode) {
		case SDL_SCANCODE_W:
			mMove.forward = e.isPressedDown;
			break;
		case SDL_SCANCODE_S:
			mMove.backward = e.isPressedDown;
			break;
		case SDL_SCANCODE_A:
			mMove.left = e.isPressedDown;
			break;
		case SDL_SCANCODE_D:
			mMove.right = e.isPressedDown;
			break;
		default:
			// Nothing.
			break;
	}
}

void Camera::update(const double &delta) {
	float dt = static_cast<float>(delta);

	if (mMove.forward)
		mPosition += CAMERA_MOVEMENT_SPEED * dt * mFrontVector;
	if (mMove.backward)
		mPosition -= CAMERA_MOVEMENT_SPEED * dt * mFrontVector;
	if (mMove.left)
		mPosition -= CAMERA_MOVEMENT_SPEED * dt * glm::normalize(glm::cross(mFrontVector, mUpVector));
	if (mMove.right)
		mPosition += CAMERA_MOVEMENT_SPEED * dt * glm::normalize(glm::cross(mFrontVector, mUpVector));
}