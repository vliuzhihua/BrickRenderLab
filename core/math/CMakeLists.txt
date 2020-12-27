PROJECT(math)
set(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")

CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE on)

include_directories("${ROOT_DIR}/3rdpartylib")
include_directories("${ROOT_DIR}/3rdpartylib/glm")

FILE(GLOB DIR_SRCS "*.cpp" "*.h")

message("${PROJECT_NAME} " "code: " "${DIR_SRCS}")

add_library(math ${DIR_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "runtime")

#set_target_properties(client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${ROOT_DIR}/project_space")
