cmake_minimum_required(VERSION 3.16)
project(server)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -Wall")
set(CMAKE_C_FLAGS_Release "${CMAKE_C_FLAGS_Release} -O3")

file(GLOB USER_LIBS_PATH "./*.c")
include_directories(../lib)
#message("USER_LIBS_PATH : ${USER_LIBS_PATH}")
add_executable(${CMAKE_PROJECT_NAME}  ${USER_LIBS_PATH})

find_library(PTHREAD pthread)
if(PTHREAD)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${PTHREAD})
    message(${PTHREAD})
endif()


