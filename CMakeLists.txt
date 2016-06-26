# Copyright 2016 Jeff Hutchinson
# Portions Copyright 2014-2016 Glenn Smith
# License: 3 clause BSD

cmake_minimum_required(VERSION 3.0.2)
project(VoxelGame)

include("cmake/compilerFlags.cmake")
include("cmake/glad.cmake")
include("cmake/openSimplexNoise.cmake")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2")
include("cmake/voxelGame.cmake")

# Enable Static Linking the C++ ABI directly into the executables and libraries
if (MSVC)
	include("cmake/msvcAbi.cmake")
endif()