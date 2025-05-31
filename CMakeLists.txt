cmake_minimum_required(VERSION 3.15...4.00)

project(gradebook
        DESCRIPTION "A grade book to handle grades of students"
        VERSION 0.00
        LANGUAGES C)

file(GLOB src src/*.c)
add_executable(gradebook ${src})

target_include_directories(gradebook PRIVATE include)
