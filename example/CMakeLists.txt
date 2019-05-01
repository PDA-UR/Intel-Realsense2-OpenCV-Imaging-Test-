cmake_minimum_required(VERSION 3.13)
project(realsensetest)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

find_library(REALSENSE2_FOUND realsense2 HINTS ${LIBRARY_DIR} REQUIRED)
if(NOT REALSENSE2_FOUND)
    SET(REALSENSE2_FOUND "realsense2")
    message(WARN "Failed to find_library(realsense2)")
endif()

add_executable(realsensetest main.cpp)
target_link_libraries(realsensetest  ${OpenCV_LIBS} ${REALSENSE2_FOUND})