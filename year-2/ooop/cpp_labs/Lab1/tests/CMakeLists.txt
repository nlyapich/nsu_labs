cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(bigIntTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

enable_testing()

add_library(BigIntLib ./../src/bigInt.hpp ./../src/bigInt.cpp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../modules")
include(common_lab)

add_executable(${PROJECT_NAME} bigIntTest.cpp)

target_link_libraries(bigIntTest BigIntLib)
target_link_libraries(bigIntTest gtest)
target_link_libraries(bigIntTest gtest_main)

add_test(
  NAME bigIntTest
  COMMAND bigIntTest
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/build_lcov_report.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
