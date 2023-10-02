cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(IonShield)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core Tree)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h)

file(GLOB MACROS *.mac)

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${EXT_DATA} DESTINATION ${PROJECT_BINARY_DIR})

set(EXECUTABLE_NAME IonShield)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(${EXECUTABLE_NAME} main.cpp ${sources} ${headers})
target_link_libraries(${EXECUTABLE_NAME} ${Geant4_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${ROOT_LIBRARIES})