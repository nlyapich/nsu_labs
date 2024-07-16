cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(gameOfLifeTest)

enable_testing()

add_library(gameOfLifeLib
  ./../src/gameOfLife.hpp
  ./../src/gameOfLife.cpp
  ./../src/rules.hpp
  ./../src/rules.cpp
  ./../src/universe.hpp
  ./../src/universe.cpp
  ./../src/cell.hpp
  ./../src/cell.cpp
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../modules")
include(common_lab)

add_executable(mechanicsTest mechanicsTest.cpp)

target_link_libraries(mechanicsTest gameOfLifeLib)
target_link_libraries(mechanicsTest gtest)
target_link_libraries(mechanicsTest gtest_main)

add_test(
  NAME mechanicsTest
  COMMAND mechanicsTest
)
