#include <catch.hpp>
#include <red_black_tree.h>

using namespace ez;
TEST_CASE("test rb_node","[class]"){
    
    SECTION("test init"){
        int key = 0;
        char val = '0'; 
        rb_node<int,char> test(key,val);
        CHECK(test.size == 1);
        CHECK(test.color == rb_node<int,char>::red);
        CHECK(test.k == key);
        CHECK(test.v == val);
        CHECK(test.left == nullptr);
        CHECK(test.right == nullptr);
    }
}

TEST_CASE("test red_black_tree","[class]"){

    SECTION("test init"){
        red_black_tree<int,char> test;
        CHECK(test.size() == 0);
    }








}