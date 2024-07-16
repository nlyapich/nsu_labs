cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(This CSVParser)

project(${This} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(Headers
  src/CSVParser.hpp
  src/tupleUtility.hpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${Headers})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
