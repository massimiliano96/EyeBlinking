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
file(GLOB_RECURSE EYEBLINKING_HEADERS "${SOURCES_PATH}/*.hpp" "${SOURCES_PATH}/*.h")

add_library(eyeblinking_cpp ${APP_SOURCES})
target_link_libraries(eyeblinking_cpp ${OpenCV_LIBS} dlib::dlib)

add_executable(exe "src/main.cpp")
target_link_libraries(exe eyeblinking_cpp ${OpenCV_LIBS} dlib::dlib)

include(GoogleTest)

enable_testing()

find_package(GTest REQUIRED)

file(COPY "models" DESTINATION ".")

set(TESTS_PATH "tests" CACHE STRING "TESTS folder")
include_directories(${TESTS_PATH})
file(GLOB_RECURSE APP_TESTS "${TESTS_PATH}/*.hpp" "${TESTS_PATH}/*.cpp")

add_executable(test_exe ${APP_TESTS})
target_link_libraries(test_exe eyeblinking_cpp GTest::gtest)
target_sources(test_exe
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${EYEBLINKING_HEADERS}
)
target_include_directories(test_exe PRIVATE ${SOURCES_PATH})

gtest_discover_tests(test_exe)
