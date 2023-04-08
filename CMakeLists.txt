cmake_minimum_required(VERSION 3.5.1)
project(exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Dlib
find_package(dlib REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(SOURCES_PATH "src/eyeblinking" CACHE STRING "SOURCES folder")
include_directories(${SOURCES_PATH})
file(GLOB_RECURSE APP_SOURCES "${SOURCES_PATH}/*.hpp" "${SOURCES_PATH}/*.cpp")

add_executable(exe "src/main.cpp" ${APP_SOURCES})

target_link_libraries(exe ${OpenCV_LIBS} dlib::dlib)

file(COPY "models" DESTINATION ".")

add_test(NAME array_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND array_tests)