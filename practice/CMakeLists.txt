cmake_minimum_required(VERSION 3.10)

project(practice)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES 
        src/practice.cpp
        src/heap.cpp
        src/trie.cpp 
        src/main.cpp
        src/red_black_tree.cpp
        test/test_practice.cpp
        test/test_heap.cpp
        test/test_trie.cpp
        
)

add_executable(test_practice ${SOURCES})

target_include_directories(test_practice 
    PRIVATE
        include
)
target_compile_definitions(test_practice
    PRIVATE Werror Wall
)