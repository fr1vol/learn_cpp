cmake_minimum_required(VERSION 3.10)

project(practice)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES 
        src/practice.cpp
        src/main.cpp
)

add_executable(test_practice ${SOURCES})

target_include_directories(test_practice 
    PRIVATE
        include
)
target_compile_definitions(test_practice
    PRIVATE Werror
)