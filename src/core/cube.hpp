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

#ifndef _CORE_CUBE_HPP_
#define _CORE_CUBE_HPP_

#include "core/types.hpp"

// Cube data comes from learnopengl.com which is Public Domain code:
// I index'd the data for more efficient GPU cache usage.

// 24 cube vertices.
static F32 cubeVertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f, // Bottom-left 0
	0.5f, -0.5f, -0.5f,  // bottom-right 1
	0.5f,  0.5f, -0.5f,  // top-right 2
	-0.5f,  0.5f, -0.5f, // top-left 3
	
	// Front face
	-0.5f, -0.5f,  0.5f, // bottom-left 4
	0.5f,  0.5f,  0.5f,  // top-right 5
	0.5f, -0.5f,  0.5f,  // bottom-right 6
	-0.5f,  0.5f,  0.5f, // top-left 7
	
	// Left face
	-0.5f,  0.5f,  0.5f, // top-right 8
	-0.5f, -0.5f, -0.5f, // bottom-left 9
	-0.5f,  0.5f, -0.5f, // top-left 10
	-0.5f, -0.5f,  0.5f, // bottom-right 11
	
	// Right face
	0.5f,  0.5f,  0.5f,  // top-left 12
	0.5f,  0.5f, -0.5f,  // top-right 13
	0.5f, -0.5f, -0.5f,  // bottom-right 14
	0.5f, -0.5f,  0.5f,  // bottom-left 15
	
	// Bottom face
	-0.5f, -0.5f, -0.5f, // top-right 16
	0.5f, -0.5f,  0.5f,  // bottom-left 17
	0.5f, -0.5f, -0.5f,  // top-left 18
	-0.5f, -0.5f,  0.5f, // bottom-right 19
	
	// Top face
	-0.5f,  0.5f, -0.5f, // top-left 20
	0.5f,  0.5f, -0.5f,  // top-right 21
	0.5f,  0.5f,  0.5f,  // bottom-right 22
	-0.5f,  0.5f,  0.5f, // bottom-left 23
};

// 36 cube indices.
static U16 cubeIndices[] = {
	// Back face
	0, 1, 2,
	2, 3, 0,
	
	// Front Face
	4, 5, 6,
	5, 4, 7,
	
	// Left Face
	8, 9, 10,
	9, 8, 11,
	
	// Right Face
	12, 13, 14,
	14, 15, 12,
	
	// Bottom face
	16, 17, 18,
	17, 16, 19,
	
	// Top face
	20, 21, 22,
	22, 23, 20
};


#endif // _CORE_CUBE_HPP_