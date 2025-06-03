cmake_minimum_required(VERSION 3.15...4.00)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gradebook-tests
        LANGUAGES C CXX)

file(GLOB tests *.cpp)
add_executable(gradebook-tests ${tests})
target_include_directories(gradebook-tests PRIVATE ../include ../lib/fff)

file(GLOB src_files ../src/*.c)
add_library(src ${src_files})
target_include_directories(src PRIVATE ../include)

find_package(GTest CONFIG REQUIRED)
enable_testing()
target_link_libraries(gradebook-tests GTest::gtest src sqlite3)
