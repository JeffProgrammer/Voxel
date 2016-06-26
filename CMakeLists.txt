# Copyright 2016 Jeff Hutchinson
# Portions Copyright 2014-2016 Glenn Smith
# License: 3 clause BSD

cmake_minimum_required(VERSION 3.0.2)
project(VoxelGame)

if (NOT MSVC)
	# Force C++14 standard on compilers, MSVC already does this by default
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
	# Force multiple processors to be used to compile visual studio builds.
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Enable Static Linking the C++ ABI directly into the executables and libraries
if (MSVC)
	#---BEGIN COPY FROM BULLET CMAKE---
	#We statically link to reduce dependancies
	FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO )
		IF(${flag_var} MATCHES "/MD")
			STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MD")
		IF(${flag_var} MATCHES "/MDd")
			STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		ENDIF(${flag_var} MATCHES "/MDd")
	ENDFOREACH(flag_var)
	#---END COPY FROM BULLET CMAKE---
endif()

#------------------------------------------------------------------------------

include("cmake/glad.cmake")
include("cmake/openSimplexNoise.cmake")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2")

# Voxel Game
set(VOXEL_SRC
	src/main/main.cpp
)

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
	
	OpenGL32
)

add_executable(Voxel ${VOXEL_SRC})
target_link_libraries(Voxel ${VOXEL_LIBRARIES})
include_directories(${VOXEL_INCLUDE})