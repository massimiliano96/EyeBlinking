cmake_minimum_required(VERSION 3.5.1)
project(exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opencv
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(EYE_BLINKING_PATH "src/eyeblinking" CACHE STRING "EYE_BLINKING folder")
include_directories(${EYE_BLINKING_PATH})
file(GLOB_RECURSE APP_EYE_BLINKING "${EYE_BLINKING_PATH}/*.hpp" "${EYE_BLINKING_PATH}/*.cpp")

# Cpp
set(CPP_PATH "cpp" CACHE STRING "cpp folder")
include_directories(${CPP_PATH})
file(GLOB_RECURSE APP_CPP "${CPP_PATH}/*.h" "${CPP_PATH}/*.cpp" "${CPP_PATH}/*.hpp")

add_executable(exe "src/main.cpp" ${APP_EYE_BLINKING})

target_link_libraries(exe ${OpenCV_LIBS})

add_test(NAME array_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND array_tests)