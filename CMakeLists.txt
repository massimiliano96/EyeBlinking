cmake_minimum_required(VERSION 3.5.1)
project(exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)


# Opencv
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

# events
set(EVENTS_PATH "src/events" CACHE STRING "Events folder")
include_directories(${EVENTS_PATH})
file(GLOB_RECURSE APP_EVENTS "${EVENTS_PATH}/*.hpp" "${EVENTS_PATH}/*.cpp")

#Detection
set(DETECTION_PATH "detection" CACHE STRING "Detection folder")
include_directories(${DETECTIONPATH})
file(GLOB_RECURSE APP_DETECTION "${DETECTION_PATH}/*.h" "${DETECTION_PATH}/*.cpp" "${DETECTION_PATH}/*.hpp")

# Cpp
set(CPP_PATH "cpp" CACHE STRING "cpp folder")
include_directories(${CPP_PATH})
file(GLOB_RECURSE APP_CPP "${CPP_PATH}/*.h" "${CPP_PATH}/*.cpp" "${CPP_PATH}/*.hpp")

add_executable(exe "src/main.cpp" ${APP_DETECTION} ${APP_EVENTS}) 

target_link_libraries(exe ${OpenCV_LIBS})

add_test(NAME array_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND array_tests)