#include <catch.hpp>
#include <red_black_tree.h>

#if 1
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

    SECTION("put"){
        red_black_tree<int,char> test;
        test.put(0,'0');
        test.put(1,'1');
        test.put(2,'2');
        test.put(3,'3');
        test.put(4,'4');
        test.put(5,'5');
        test.put(6,'6');
        test.put(7,'7');
        test.put(8,'8');
        CHECK(test.size() == 9);
        auto v = test.sort();
        for(int i = 0; i< 9; ++i){
            CHECK(v[i]->v == '0'+i);
        }
    }

    

    SECTION("delete"){
        red_black_tree<int,char> test;
        test.put(0,'0');
        test.put(1,'1');
        test.put(2,'2');
        test.put(3,'3');
        test.put(4,'4');
        test.put(5,'5');
        test.put(6,'6');
        test.put(7,'7');
        test.put(8,'8');
        CHECK(test.size() == 9);

        test.delete_node(8);
        CHECK(test.size() == 8);
        auto v = test.sort();
        for(int i = 0; i< 8; ++i){
            CHECK(v[i]->v == '0'+i);
        }
        test.delete_node(0);
        test.delete_node(1);
        test.delete_node(2);
        test.delete_node(3);
        test.delete_node(4);
        test.delete_node(5);
        test.delete_node(6);
        test.delete_node(7);

        CHECK(test.size() == 0);
    }




}

#endif