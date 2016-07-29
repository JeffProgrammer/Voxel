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
//
//
//
// NOTICE:
// Some algorithms used for screen space tiling comes from AMD's OpenGPU 
// initiative licensed under the MIT license. See this link for the source:
//
// https://github.com/GPUOpen-LibrariesAndSDKs/ForwardPlus11/blob/master/forwardplus11/src/Shaders/ForwardPlus11Tiling.hlsl
//
//
//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _CORE_SCREENSPACETILING_HPP_
#define _CORE_SCREENSPACETILING_HPP_

#include "core/types.hpp"
#include <glm/glm.hpp>

#define SCREESPACE_TILESIZE 16

namespace TiledCulling {
	/**
	 * Creates a standard Hessian-normal-form plane equation from three points
	 * a b c, where a is the origin.
	 */
	inline glm::vec3 createPlaneEquation(const glm::vec3 &b, const glm::vec3 &c) {
		return glm::normalize(glm::cross(b, c));
	}

	/**
	 * Tests to see if a point is inside of a frustrum by checking each plane.
	 */
	inline bool testFrustrumSides(const glm::vec3 &center, const F32 radius, const glm::vec3 &plane0, const glm::vec3 &plane1, const glm::vec3 &plane2, const glm::vec3 &plane3) {
		return glm::dot(plane0, center) < radius && 
				 glm::dot(plane1, center) < radius && 
				 glm::dot(plane2, center) < radius && 
				 glm::dot(plane3, center) < radius;
	}

	inline S32 getNumberOfTilesWidth(const S32 width) {
		return (width + SCREESPACE_TILESIZE - 1) / SCREESPACE_TILESIZE;
	}

	inline S32 getNumberOfTilesHeight(const S32 height) {
		return (height + SCREESPACE_TILESIZE - 1) / SCREESPACE_TILESIZE;
	}

	inline S32 getTileIndex(const glm::vec2 &pos, const S32 width) {
		S32 numCellsX = getNumberOfTilesWidth(width);
		return S32(floorf(pos.x / SCREESPACE_TILESIZE) + floorf(pos.y / SCREESPACE_TILESIZE) * numCellsX);
	}

	inline glm::vec4 projectionToView(const glm::vec4 &proj, const glm::vec4 &projInverse) {
		glm::vec4 view = proj * projInverse;
		view /= view.w;
		return view;
	}
}

#endif // _CORE_SCREENSPACETILING_HPP_