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
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/OpenGL/GLRenderer.hpp"
#include "core/cube.hpp"
#include "game/camera.hpp"

// temporary for a single cube until I figure out how to manage materials and
// shaders.
GLuint buffer, ibo;
GLuint cubeVAO;

GLuint cubeVert = 0;
GLuint fragVert = 0;
GLuint singleCubeProgram;

GLuint locationPosition;

GLuint ubo;
GLuint uboBlockIndex;

struct UBO {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
} uniformData;

struct PointLights {
	glm::vec3 position;
	glm::vec3 color;
};

#define LIGHT_COUNT 4

PointLights lights[LIGHT_COUNT];

struct LightData {
	GLuint position;
	GLuint color;
};
LightData lightsGLSL[LIGHT_COUNT];

const char *vertSingleCubeSrc =
"#version 330 core\n"

"layout (location = 0) in vec3 position;\n"

"layout (std140) uniform matrices {\n"
"   mat4 model;\n"
"   mat4 view;\n"
"   mat4 projection;\n"
"};"

"out vec3 fragPosition;"

"void main() {\n"
"   mat4 mvp = projection * view * model;\n"
"   gl_Position = mvp * vec4(position, 1);\n"
"   fragPosition = vec3(model * vec4(position, 1));"
"}";

const char *fragSingleCubeSrc =
"#version 330 core\n"

"struct PointLight {\n"
"   vec3 position;\n"
"   vec3 color;\n"
"};\n"

"#define LIGHT_COUNT 4\n"
"uniform PointLight lights[LIGHT_COUNT];\n"

"in vec3 fragPosition;\n"
"out vec4 frag_color;\n"

"void main() {\n"
"   vec3 ac = vec3(0,0,0);\n"
"   for (int i = 0; i < LIGHT_COUNT; i++) {\n"
"      float distance = length(lights[i].position - fragPosition);\n"
"      if (distance <= 4.0) {\n"
"         float attenuation = 1.0 / (distance * distance);\n"
"         vec3 lightColor = lights[i].color * attenuation;\n"
"         ac += lightColor;\n"
"      }\n"
"   }\n"
"   frag_color = vec4(0.25, 0, 0, 1) + vec4(ac, 0);\n"
"}";

static void checkError(const char *fn) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL %s Error: %d\n", fn, err);
	}
}

void GLRenderer::initRenderer() {
	mCamera = nullptr;
	
	// The core profile requires a VAO to be bound before quite a bit of specific GL calls are made.
	// We'll just create a global state VAO for now so that we can just call GL functions.
	glGenVertexArrays(1, &mGlobalVAO);
	glBindVertexArray(mGlobalVAO);
	
	// Set Clear color
	glClearColor(0.0f, 1.0f, 1.0f, 0.5f);
	
	// Set viewport
	glViewport(0, 0, 1440, 900);

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
			
			char *log = new char[shaderErrorLogLength];
			glGetShaderInfoLog(cubeVert, shaderErrorLogLength, &shaderErrorLogLength, log);
			printf("Error compiling vertex shader: %s\n", log);
			delete[] log;

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
			
			char *log = new char[shaderErrorLogLength];
			glGetShaderInfoLog(fragVert, shaderErrorLogLength, &shaderErrorLogLength, log);
			printf("Error compiling fragment shader: %s\n", log);
			delete[] log;

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
			
			char *log = new char[shaderErrorLogLength];
			glGetProgramInfoLog(singleCubeProgram, shaderErrorLogLength, &shaderErrorLogLength, log);
			printf("Error linking program: %s\n", log);
			delete[] log;

			glDeleteProgram(singleCubeProgram);
			glDeleteShader(cubeVert);
			glDeleteShader(fragVert);
			return;
		}
		
		glDetachShader(singleCubeProgram, cubeVert);
		glDetachShader(singleCubeProgram, fragVert);
	}

	uboBlockIndex = glGetUniformBlockIndex(singleCubeProgram, "matrices");

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformData), &uniformData, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	{
		glGenBuffers(1, &buffer);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24 * 3, cubeVertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 36, cubeIndices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(locationPosition);
		glVertexAttribPointer(locationPosition, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	}
	glBindVertexArray(mGlobalVAO);

	// Lights.
	for (U32 i = 0; i < LIGHT_COUNT; ++i) {
		std::string pos = "lights[" + std::to_string(i) + "].position";
		std::string col = "lights[" + std::to_string(i) + "].color";
		lightsGLSL[i].position = glGetUniformLocation(singleCubeProgram, pos.c_str());
		lightsGLSL[i].color = glGetUniformLocation(singleCubeProgram, col.c_str());
	}

	lights[0].color = glm::vec3(0.0f, 0.3f, 0.0f);
	lights[0].position = glm::vec3(3.0f, 0.0f, 3.0f);
	lights[1].color = glm::vec3(0.0f, 0.0f, 0.3f);
	lights[1].position = glm::vec3(5.0f, 0.0f, 3.0f);
	lights[2].color = glm::vec3(0.0f, 3.0f, 0.3f);
	lights[2].position = glm::vec3(5.0f, 0.0f, 5.0f);
	lights[3].color = glm::vec3(3.0f, 0.0f, 0.3f);
	lights[3].position = glm::vec3(3.0f, 1.0f, 5.0f);
}

void GLRenderer::destroyRenderer() {
	glDeleteVertexArrays(1, &cubeVAO);

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
	if (mCamera == nullptr)
		return;
	
	glm::mat4 view = glm::lookAt(mCamera->getPosition(), mCamera->getPosition() + mCamera->getFrontVector(), mCamera->getUpVector());
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1440.f/900.f, 0.02f, 200.0f);
	
	glUseProgram(singleCubeProgram);
	
	// Update the UBO's data.
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UBO, view), sizeof(glm::mat4), &view[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UBO, projection), sizeof(glm::mat4), &proj[0][0]);

	// Bind buffers to shaders
	glBindVertexArray(cubeVAO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo); // bind to register 0
	glUniformBlockBinding(singleCubeProgram, uboBlockIndex, 0); // bind to register 0

	// Bind lights
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		glUniform3fv(lightsGLSL[i].position, 1, &lights[i].position[0]);
		glUniform3fv(lightsGLSL[i].color, 1, &lights[i].color[0]);
	}

	for (int x = 0; x < 16; ++x) {
		for (int z = 0; z < 16; ++z) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(float(x), 0.0f, float(z)));
			glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UBO, model), sizeof(glm::mat4), &model[0][0]);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		}
	}
}

void GLRenderer::setActiveSceneCamera(Camera *camera) {
	mCamera = camera;
}