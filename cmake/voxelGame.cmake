# Voxel Game
set(VOXEL_SRC
	src/main/main.cpp
	
	src/platform/window.cpp
	src/platform/window.hpp
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
