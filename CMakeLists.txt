cmake_minimum_required(VERSION 3.15...4.00)

project(gradebook
        DESCRIPTION "A grade book to handle grades of students"
        VERSION 1.00
        LANGUAGES C)

file(GLOB src src/*.c)
add_executable(gradebook ${src})

target_link_libraries(gradebook sqlite3)
target_include_directories(gradebook PRIVATE include)
