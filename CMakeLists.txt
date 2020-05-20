PROJECT(BrickGE)


CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

#AUX_SOURCE_DIRECTORY(client DIR_SRCS)
#MESSAGE(STATUS "CLIENT_DIR_SRCS::: \n" ${DIR_SRCS} )

#set(LIB_SRC rapidjson)

#add_subdirectory(rapidjson)
#add_library(rapidjson STATIC ${LIB_SRC})

#aux_source_directory(. DIR_SRCS)
#message(STATUS "client code: " "${DIR_SRCS}")

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory("glfw/glfw-3.2.1")
add_subdirectory("tinyobjloader/tinyobjloader")
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

# #target_link_libraries需要在add_executable之后!!!
# TARGET_LINK_LIBRARIES(test_proj d3d9)
# TARGET_LINK_LIBRARIES(test_proj d3dx9)
# TARGET_LINK_LIBRARIES(test_proj dxerr)
# TARGET_LINK_LIBRARIES(test_proj DXGI)
# TARGET_LINK_LIBRARIES(test_proj DInput8)

# target_link_libraries(test_proj dxguid)
# TARGET_LINK_LIBRARIES(test_proj dxguid)
# target_link_libraries(test_proj winmm)

# target_link_libraries(test_proj legacy_stdio_definitions)