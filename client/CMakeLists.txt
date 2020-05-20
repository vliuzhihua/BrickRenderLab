PROJECT(client)
set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")

CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(include)

aux_source_directory(. DIR_SRCS)

message("client code: " "${DIR_SRCS}")

link_directories(lib)

ADD_EXECUTABLE(client ${DIR_SRCS})

#target_link_libraries需要在add_executable之后!!!
#add_library(mxnet STATIC IMPORTED)
target_link_libraries(client glfw)
target_link_libraries(client opengl32)
target_link_libraries(client tinyobjloader)


target_link_libraries(client glew32)
target_link_libraries(client glut32)