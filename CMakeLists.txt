cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(IonShield)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core Tree)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h)

file(GLOB MACROS *.mac *.sh)

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${EXT_DATA} DESTINATION ${PROJECT_BINARY_DIR})

set(EXECUTABLE_NAME IonShield)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(${EXECUTABLE_NAME} main.cpp ${sources} ${headers})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE DEBUG=1)
else()
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE DEBUG=0)
endif()

target_link_libraries(${EXECUTABLE_NAME} ${Geant4_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${ROOT_LIBRARIES})

add_subdirectory(neutronAna)