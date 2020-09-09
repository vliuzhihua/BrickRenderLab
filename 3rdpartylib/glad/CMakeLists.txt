PROJECT(glad)
set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")

CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

include_directories("./include")

add_library(glad STATIC "src/glad.c")

#target_link_libraries需要在add_executable之后!!!