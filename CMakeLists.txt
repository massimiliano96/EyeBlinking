cmake_minimum_required(VERSION 3.5.1)
project(exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_EXTENSIONS OFF)

#find_package(MPI REQUIRED)
#include_directories(${MPI_INCLUDE_DIRS})

# events
set(EVENTS_PATH "events" CACHE STRING "Events folder")
include_directories(${EVENTS_PATH})
file(GLOB_RECURSE APP_EVENTS "${EVENTS_PATH}/*.hpp" "${EVENTS_PATH}/*.cpp")

#Tools
set(TOOLS_PATH "tools" CACHE STRING "Tools folder")
include_directories(${TOOLS_PATH})
file(GLOB_RECURSE APP_TOOLS "${TOOLS_PATH}/*.h" "${TOOLS_PATH}/*.cpp" "${TOOLS_PATH}/*.hpp")

# Cpp
set(CPP_PATH "cpp" CACHE STRING "cpp folder")
include_directories(${CPP_PATH})
file(GLOB_RECURSE APP_CPP "${CPP_PATH}/*.h" "${CPP_PATH}/*.cpp" "${CPP_PATH}/*.hpp")

add_executable(exe ${APP_CPP} ${APP_TOOLS} ${APP_EVENTS}) 
#target_include_directories(Test PRIVATE ${MPI_CXX_INCLUDE_PATH})
#target_compile_options(Test PRIVATE ${MPI_CXX_COMPILE_FLAGS})

# Opencv
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

#Dllib
find_package(dlib REQUIRED)

target_link_libraries(exe -lboost_system -lboost_filesystem dlib::dlib ${OpenCV_LIBS})# ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS}

