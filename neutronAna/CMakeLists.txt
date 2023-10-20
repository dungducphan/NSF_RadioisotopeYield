# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)

find_package(ROOT REQUIRED Core Tree)

include(${ROOT_USE_FILE})

file(GLOB MACROS Data/*.root)
file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR}/neutronAna/Data)

set(EXECUTABLE_NAME NeutronAna)
include_directories(${PROJECT_SOURCE_DIR}/neutronAna)
add_executable(${EXECUTABLE_NAME} ana.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${ROOT_LIBRARIES})
