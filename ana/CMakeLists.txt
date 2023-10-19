# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(IonShield)

set(CMAKE_CXX_STANDARD 20)

find_package(ROOT REQUIRED Core Tree)

include(${ROOT_USE_FILE})

file(GLOB MACROS *.root)
file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})

set(EXECUTABLE_NAME Ana)
include_directories(${PROJECT_SOURCE_DIR}/ana)
add_executable(${EXECUTABLE_NAME} ana.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${ROOT_LIBRARIES})
