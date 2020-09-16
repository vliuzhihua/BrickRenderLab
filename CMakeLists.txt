set(CMAKE_CXX_STANDARD 17)
PROJECT(BrickGE)


CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "ROOT_DIR " ${ROOT_DIR})

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory("3rdpartylib/glfw")
add_subdirectory("3rdpartylib/glad")
add_subdirectory("3rdpartylib/tinyobjloader/tinyobjloader")
add_subdirectory("client")


