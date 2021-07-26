cmake_minimum_required(VERSION 3.16)
project(STL_test)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_executable(test_set test_set.cpp)
add_executable(test_multiset test_multiset.cpp)
add_executable(test_unordered_set test_unordered_set.cpp)
add_executable(test_map test_map.cpp)
