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

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/OpenGL/GLRenderer.hpp"
#include "core/cube.hpp"

// temporary for a single cube until I figure out how to manage materials and
// shaders.
GLuint buffer, ibo;

GLuint cubeVert = 0;
GLuint fragVert = 0;
GLuint singleCubeProgram;

GLuint locationModel;
GLuint locationView;
GLuint locationProj;
GLuint locationPosition;

const char *vertSingleCubeSrc =
	"#version 330 core\n"

	"layout (location = 0) in vec3 position;\n"

	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"

	"void main() {\n"
	"   mat4 mvp = projection * view * model;\n"
	"   gl_Position = mvp * vec4(position, 1);\n"
	"}";

const char *fragSingleCubeSrc =
	"#version 330 core\n"

	"out vec4 frag_color;\n"

	"void main() {\n"
	"   frag_color = vec4(1, 0, 0, 1);\n"
	"}";

static void checkError(const char *fn) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL %s Error: %d\n", fn, err);
	}
}

void GLRenderer::initRenderer() {
	// The core profile requires a VAO to be bound before quite a bit of specific GL calls are made.
	// We'll just create a global state VAO for now so that we can just call GL functions.
	glGenVertexArrays(1, &mGlobalVAO);
	glBindVertexArray(mGlobalVAO);
	
	// Set Clear color
	glClearColor(0.0f, 1.0f, 1.0f, 0.5f);
	
	glEnable(GL_DEPTH_TEST);
	checkError("glEnable-DepthTest");
	glDepthFunc(GL_LESS);
	checkError("glDepthFunc");
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Shaders.
	{
		GLint shaderCompileSuccess;
		GLint shaderErrorLogLength;
		
		// Create vertex shaders
		cubeVert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(cubeVert, 1, &vertSingleCubeSrc, NULL);
		glCompileShader(cubeVert);
		
		// Check shader error.
		glGetShaderiv(cubeVert, GL_COMPILE_STATUS, &shaderCompileSuccess);
		if (!shaderCompileSuccess) {
			glGetShaderiv(cubeVert, GL_INFO_LOG_LENGTH, &shaderErrorLogLength);
			
			std::vector<char> log;
			log.reserve(shaderErrorLogLength);
			glGetShaderInfoLog(cubeVert, shaderErrorLogLength, &shaderErrorLogLength, &log[0]);
			printf("Error compiling vertex shader: %s\n", &log[0]);
			
			glDeleteShader(cubeVert);
			return;
		}
		
		// Create fragment shader.
		fragVert = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragVert, 1, &fragSingleCubeSrc, NULL);
		glCompileShader(fragVert);
		
		// Check shader error.
		glGetShaderiv(fragVert, GL_COMPILE_STATUS, &shaderCompileSuccess);
		if (!shaderCompileSuccess) {
			glGetShaderiv(fragVert, GL_INFO_LOG_LENGTH, &shaderErrorLogLength);
			
			std::vector<GLchar> log;
			log.reserve(shaderErrorLogLength);
			glGetShaderInfoLog(fragVert, shaderErrorLogLength, &shaderErrorLogLength, &log[0]);
			printf("Error compiling fragment shader: %s\n", &log[0]);
			
			glDeleteShader(fragVert);
			return;
		}
		
		// now we can make a program.
		singleCubeProgram = glCreateProgram();
		glAttachShader(singleCubeProgram, cubeVert);
		glAttachShader(singleCubeProgram, fragVert);
		
		glLinkProgram(singleCubeProgram);
		
		// Make sure we linked successfully.
		GLint linked;
		glGetProgramiv(singleCubeProgram, GL_LINK_STATUS, &linked);
		if (!linked) {
			glGetProgramiv(singleCubeProgram, GL_INFO_LOG_LENGTH, &shaderErrorLogLength);
			
			std::vector<GLchar> log;
			log.reserve(shaderErrorLogLength);
			glGetProgramInfoLog(singleCubeProgram, shaderErrorLogLength, &shaderErrorLogLength, &log[0]);
			printf("Error linking program: %s\n", &log[0]);
			
			glDeleteProgram(singleCubeProgram);
			glDeleteShader(cubeVert);
			glDeleteShader(fragVert);
			return;
		}
		
		glDetachShader(singleCubeProgram, cubeVert);
		glDetachShader(singleCubeProgram, fragVert);
	}
	
	
	locationModel = glGetUniformLocation(singleCubeProgram, "model");
	checkError("glGetUniformLocation-model");
	locationView = glGetUniformLocation(singleCubeProgram, "view");
	checkError("glGetUniformLocation-view");
	locationProj = glGetUniformLocation(singleCubeProgram, "projection");
	checkError("glGetUniformLocation-projection");
	locationPosition = glGetAttribLocation(singleCubeProgram, "position");
	checkError("glGetAttribLocation-position");
	
	glGenBuffers(1, &buffer);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24 * 3, cubeVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 36, cubeIndices, GL_STATIC_DRAW);
}

void GLRenderer::destroyRenderer() {
	// Delete the VAO
	if (glIsVertexArray(mGlobalVAO)) {
		glDeleteVertexArrays(1, &mGlobalVAO);
	}
	
	glDeleteProgram(singleCubeProgram);
	glDeleteShader(cubeVert);
	glDeleteShader(fragVert);
	glDeleteBuffers(1, &buffer);
	glDeleteBuffers(1, &ibo);
}

void GLRenderer::beginFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderer::renderChunks() {

}

void GLRenderer::endFrame() {
	
}

void GLRenderer::renderSingleCube() {
	glm::mat4 view = glm::lookAt(glm::vec3(-1.5f, 1.5f, -1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 16.f/9.f, 0.02f, 200.0f);
	glm::mat4 model = glm::mat4(1.0f);
	
	glUseProgram(singleCubeProgram);
	checkError("glUseProgram");
	
	glEnableVertexAttribArray(locationPosition);
	glVertexAttribPointer(locationPosition, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	
	glUniformMatrix4fv(locationModel, 1, false, &model[0][0]);
	checkError("glUniformMatrix4fv-model");
	glUniformMatrix4fv(locationView, 1, false, &view[0][0]);
	checkError("glUniformMatrix4fv-view");
	glUniformMatrix4fv(locationProj, 1, false, &proj[0][0]);
	checkError("glUniformMatrix4fv-projection");
	
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	checkError("glBindBuffer-VBO");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	checkError("glBindBuffer-IBO");
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	checkError("glDrawElements");
}