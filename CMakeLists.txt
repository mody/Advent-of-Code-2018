cmake_minimum_required(VERSION 3.11)
project(AOC2018)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3")

# find_package(PkgConfig)
# pkg_check_modules(PC_RapidJSON QUIET RapidJSON)

# set(BOOST_ROOT "/home/ov699/opt/")
find_package(Boost 1.67.0 COMPONENTS unit_test_framework REQUIRED)

include_directories(src ${Boost_INCLUDE_DIR})

add_executable(puzzle_09_2 puzzle_09_2.cc)
target_compile_features(puzzle_09_2 PRIVATE cxx_std_17)

add_executable(puzzle_15 puzzle_15.cc)
target_compile_features(puzzle_15 PRIVATE cxx_std_17)

add_executable(puzzle_16 puzzle_16.cc)
target_compile_features(puzzle_16 PRIVATE cxx_std_17)

add_executable(puzzle_17 puzzle_17.cc)
target_compile_features(puzzle_17 PRIVATE cxx_std_17)

add_executable(puzzle_18 puzzle_18.cc)
target_compile_features(puzzle_18 PRIVATE cxx_std_17)

add_executable(puzzle_19 puzzle_19.cc)
target_compile_features(puzzle_19 PRIVATE cxx_std_17)

add_executable(puzzle_20 puzzle_20.cc)
target_compile_features(puzzle_20 PRIVATE cxx_std_17)

add_executable(puzzle_22 puzzle_22.cc)
target_compile_features(puzzle_22 PRIVATE cxx_std_17)

add_executable(puzzle_23 puzzle_23.cc)
target_compile_features(puzzle_23 PRIVATE cxx_std_17)
