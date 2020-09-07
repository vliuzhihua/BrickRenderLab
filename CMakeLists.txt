PROJECT(BrickGE)


CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "ppppppppppp " ${ROOT_DIR})

add_definitions(-DUNICODE -D_UNICODE)

# add_subdirectory("3rdpartylib/libjpeg-turbo")
#add_subdirectory("3rdpartylib/libjpeg")
add_subdirectory("3rdpartylib/glfw/glfw-3.2.1")
add_subdirectory("3rdpartylib/tinyobjloader/tinyobjloader")
add_subdirectory("client")


#about d3d
# set(DXSDK_DIR "$ENV{DXSDK_DIR}")
# set(DXSDK_INCLUDE_DIR "${DXSDK_DIR}/Include")
# include_directories("${DXSDK_INCLUDE_DIR}")
# if ("${CMAKE_GENERATOR}" MATCHES "Win64")
#     set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x64")
#     link_directories("${DXSDK_LIB_DIR}")
# else()
#     set(DXSDK_LIB_DIR "${DXSDK_DIR}/Lib/x86")
#     link_directories("${DXSDK_DIR}/Lib/x86")
# endif()
# message(status "dx include path: " "${DXSDK_INCLUDE_DIR}")
# message(status "dx lib path: " "${DXSDK_LIB_DIR}")

# #link_directories和include_directories需要在add_executable之前!!!
# ADD_EXECUTABLE(test_proj ${DIR_SRCS})
