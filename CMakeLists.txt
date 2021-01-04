cmake_minimum_required(VERSION 3.11)
project(AOC2018)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3")

# find_package(PkgConfig)
# pkg_check_modules(PC_RapidJSON QUIET RapidJSON)

# set(BOOST_ROOT "/home/ov699/opt/")
find_package(Boost 1.67.0 COMPONENTS unit_test_framework REQUIRED)

#  include_directories(src ${Boost_INCLUDE_DIR})

add_executable(puzzle_09_2 puzzle_09_2.cc)
target_compile_features(puzzle_09_2 PRIVATE cxx_std_17)

add_executable(puzzle_15 puzzle_15.cc)
target_compile_features(puzzle_15 PRIVATE cxx_std_17)

add_executable(puzzle_16 puzzle_16.cc)
target_compile_features(puzzle_16 PRIVATE cxx_std_17)
