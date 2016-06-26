add_library(Glad STATIC thirdparty/glad/src/glad.c)
target_include_directories(Glad PUBLIC thirdparty/glad/include)