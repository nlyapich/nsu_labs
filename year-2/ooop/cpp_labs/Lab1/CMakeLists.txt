cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(This BigInt)

project(${This} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (BUILD_TESTING)
  add_subdirectory(googletest)

  enable_testing()
  add_subdirectory(tests)
endif()
