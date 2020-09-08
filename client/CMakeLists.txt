PROJECT(client)
set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")

CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

include_directories("${ROOT_DIR}/3rdpartylib/GL/include")
include_directories("${ROOT_DIR}/3rdpartylib")
include_directories("${ROOT_DIR}/3rdpartylib/FreeImage/Dist/x32")
#include_directories("${ROOT_DIR}/3rdpartylib/libjpeg")
link_directories(${ROOT_DIR}/3rdpartylib/GL/lib)
#link_directories(${ROOT_DIR}/3rdpartylib/libjpeg)
link_directories(${ROOT_DIR}/3rdpartylib/FreeImage/Dist/x32)

FILE(GLOB DIR_SRCS "*.cpp" "*.h")

message("client code: " "${DIR_SRCS}")
message("current path: " "${${CMAKE_CURRENT_SOURCE_DIR}}")


# source_group(md5 FILES ${MD5_FILE})


ADD_EXECUTABLE(client ${DIR_SRCS})

#target_link_libraries需要在add_executable之后!!!
#add_library(mxnet STATIC IMPORTED)
#target_link_libraries(client libjpeg-turbo)
#target_link_libraries(client libjpeg)
target_link_libraries(client FreeImage)
target_link_libraries(client glfw)
target_link_libraries(client opengl32)
target_link_libraries(client tinyobjloader)
#target_link_libraries(client eigen)


target_link_libraries(client glew32)
target_link_libraries(client glut32)