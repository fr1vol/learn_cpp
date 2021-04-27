cmake_minimum_required(VERSION 3.10)

project(practice)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./src SOURCES)
aux_source_directory(./test SOURCES_UT)

add_executable(test_practice ${SOURCES} ${SOURCES_UT})

target_include_directories(test_practice 
    PRIVATE
        include
)
target_compile_definitions(test_practice
    PRIVATE Werror Wall
)