cmake_minimum_required(VERSION 3.16)
# System Introspection Functions(https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks#system-introspection-functions)
INCLUDE (CheckIncludeFiles)
INCLUDE(CheckFunctionExists)
INCLUDE (CheckSymbolExists)
INCLUDE (CheckLibraryExists)

# see https://stackoverflow.com/questions/9160335/os-specific-instructions-in-cmake-how-to
if(UNIX AND NOT APPLE)
  set(CMAKE_SYSTEM_NAME Linux)
elseif(WIN32)
  set(CMAKE_SYSTEM_NAME Windows)
elseif(APPLE)
  set(CMAKE_SYSTEM_NAME Darwin)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_C_COMPILER "clang-9")
  set(CMAKE_CXX_COMPILER "clang++-9")
endif()

project(test_stl CXX)
set(CMAKE_CXX_FLAGS "-Wall")

# CHECK_FUNCTION_EXISTS(function variable)
CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)

# CHECK_FUNCTION_EXISTS(function variable)
CHECK_FUNCTION_EXISTS(madvise HAVE_MADVISE)

# CHECK_SYMBOL_EXISTS(symbol headers variable)
CHECK_SYMBOL_EXISTS(LC_MESSAGES "locale.h" HAVE_LC_MESSAGES)

# CHECK_LIBRARY_EXISTS(library function location variable)
CHECK_LIBRARY_EXISTS(volmgt volmgt_running "" HAVE_VOLMGT)


add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_executable(test_set test_set.cpp)
add_executable(test_multiset test_multiset.cpp)
add_executable(test_unordered_set test_unordered_set.cpp)
add_executable(test_map test_map.cpp)
add_executable(test_attribute test_attribute.cpp)
