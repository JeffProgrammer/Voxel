#------------------------------------------------------------------------------
# Copyright (c) 2015-2016, Jeff Hutchinson
# Copyright (c) 2015-2016, Glenn Smith
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the project nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#------------------------------------------------------------------------------

set(VOXEL_SRC
	src/core/algorithm.hpp
	src/core/cube.hpp
	src/core/types.hpp

	src/game/camera.cpp
	src/game/camera.hpp
	src/game/gameObject.cpp
	src/game/gameObject.hpp

	src/graphics/context.cpp
	src/graphics/context.hpp
	src/graphics/renderer.cpp
	src/graphics/renderer.hpp
	src/graphics/OpenGL/GLContext.cpp
	src/graphics/OpenGL/GLContext.hpp
	src/graphics/OpenGL/GLRenderer.cpp
	src/graphics/OpenGL/GLRenderer.hpp

	src/main/main.cpp

	src/platform/event/eventManager.cpp
	src/platform/event/eventManager.hpp
	src/platform/event/eventTypes.hpp
	src/platform/event/interface/IKeyboardEvent.cpp
	src/platform/event/interface/IKeyboardEvent.hpp
	src/platform/event/interface/IMouseButtonEvent.cpp
	src/platform/event/interface/IMouseButtonEvent.hpp
	src/platform/event/interface/IMouseMovementEvent.cpp
	src/platform/event/interface/IMouseMovementEvent.hpp
	src/platform/event/interface/IWindowEvent.cpp
	src/platform/event/interface/IWindowEvent.hpp
	src/platform/timer.cpp
	src/platform/timer.hpp
	src/platform/window.cpp
	src/platform/window.hpp
)

if (WIN32)
	set (VOXEL_SRC
		${VOXEL_SRC}
		src/graphics/D3D11/D3D11Context.cpp
		src/graphics/D3D11/D3D11Context.hpp
		src/graphics/D3D11/D3D11Renderer.cpp
		src/graphics/D3D11/D3D11Renderer.hpp
	)
endif()

set (VOXEL_INCLUDE
	src
	thirdparty/glad/include
	thirdparty/glm
	thirdparty/SDL2/include
	thirdparty/openSimplexNoise
)

set (VOXEL_LIBRARIES
	Glad
	OpenSimplexNoise
	SDL2-static
)

if (WIN32)
	set (VOXEL_LIBRARIES
		${VOXEL_LIBRARIES}
		OpenGL32
		
		D3D11
		D3DCompiler
	)
endif()

add_executable(Voxel ${VOXEL_SRC})
target_link_libraries(Voxel ${VOXEL_LIBRARIES})
include_directories(include ${VOXEL_INCLUDE})

if (APPLE)
	target_link_libraries(Voxel "-framework OpenGL")
endif()

# Project solution folders organization
source_group("core" REGULAR_EXPRESSION core/.*)
source_group("game" REGULAR_EXPRESSION game/.*)
source_group("graphics" REGULAR_EXPRESSION graphics/.*)
source_group("graphics\\D3D11" REGULAR_EXPRESSION graphics/D3D11/.*)
source_group("graphics\\OpenGL" REGULAR_EXPRESSION graphics/OpenGL/.*)
source_group("main" REGULAR_EXPRESSION main/.*)
source_group("platform" REGULAR_EXPRESSION platform/.*)
source_group("platform\\event" REGULAR_EXPRESSION platform/event/.*)
source_group("platform\\event\\interface" REGULAR_EXPRESSION platform/event/interface/.*)