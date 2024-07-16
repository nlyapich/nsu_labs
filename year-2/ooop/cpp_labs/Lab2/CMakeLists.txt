cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(This GameOfLife)

project(${This} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(Headers
  src/gameOfLife.hpp
  src/universe.hpp
  src/rules.hpp
  src/cell.hpp
)

set(Sources
  src/gameOfLife.cpp
  src/universe.cpp
  src/rules.cpp
  src/cell.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${Headers} ${Sources})

if (BUILD_TESTING)
  add_subdirectory(googletest)

  enable_testing()
  add_subdirectory(tests)
endif()
