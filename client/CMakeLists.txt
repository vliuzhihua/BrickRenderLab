PROJECT(client)

CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

aux_source_directory(. DIR_SRCS)

message("client code: " "${DIR_SRCS}")

#ADD_EXECUTABLE(client ${DIR_SRCS})
ADD_EXECUTABLE(client main.cpp)

#target_link_libraries需要在add_executable之后!!!
#add_library(mxnet STATIC IMPORTED)
target_link_libraries(client glfw)
target_link_libraries(client opengl32)
# TARGET_LINK_LIBRARIES(test_proj d3d9)