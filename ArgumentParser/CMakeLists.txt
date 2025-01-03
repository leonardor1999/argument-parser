cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(
  ArgumentParser
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wunused -Werror)
endif()

add_library(ArgumentParser STATIC argument_parser.cpp)

add_executable(main main.cpp)

target_link_libraries(main ArgumentParser)
