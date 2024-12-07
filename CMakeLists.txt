cmake_minimum_required(VERSION 3.22)
project(bis)

include_directories("include/")
include_directories("include/min")
include_directories("include/InverseSolver")
include_directories("include/UniversalApproximation")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")

add_library(PALINV SHARED ${SOURCES})
