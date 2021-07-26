cmake_minimum_required(VERSION 3.16)
project(test_opencv CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Debug as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(ENV{CONAN_REVISIONS_ENABLED} 1)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.fastgit.org/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
set(CONAN_PROFILE default)

conan_add_remote(NAME conan-center
        				 INDEX 0
        				 URL https://conan.bintray.com
        				 VERIFY_SSL True)

conan_cmake_run(REQUIRES opencv/4.5.0
								REQUIRES spdlog/1.8.1
								REQUIRES catch2/2.13.2
								REQUIRES eigen/3.3.7
	            	#REQUIRES ceres-solver/1.14.0
			    			#REQUIRES toml11/3.6.0
								#REQUIRES fmt/7.0.3 OPTIONS fmt:header_only=True
			    			#REQUIRES sophus/1.0.0
								#REQUIRES boost/1.74.0 OPTIONS boost:header_only=True
			    			BASIC_SETUP CMAKE_TARGETS
			    			SETTINGS build_type=Release 
								BUILD missing)
								
add_executable(test_opencv test_opencv.cpp unit_test.cpp)
target_link_libraries(test_opencv 
                      CONAN_PKG::opencv 
                      CONAN_PKG::spdlog 
                      CONAN_PKG::catch2)      
