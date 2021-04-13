#include <catch.hpp>
#include <heap.h>

TEST_CASE("test class heap","[class]"){

    SECTION("init"){
        ez::heap h1;
        CHECK(h1.size() == 0);
        CHECK(h1.empty());

        ez::heap h(5);
        CHECK(h.size() == 0);
        CHECK(h.empty());
        CHECK(h.capacity() >= 5);
    }

    SECTION("insert"){
        ez::heap h(10);
        h.insert(0);
        CHECK(h.maximum() == 0);
        CHECK(h.size() == 1);
        h.insert(2);
        CHECK(h.maximum() == 2);
        CHECK(h.size() == 2);
        h.insert(4);
        CHECK(h.maximum() == 4);
        CHECK(h.size() == 3);
        h.insert(3);
        CHECK(h.maximum() == 4);
        CHECK(h.size() == 4);
        h.insert(3);
        CHECK(h.maximum() == 4);
        CHECK(h.size() == 5);
    }

    SECTION( "extract_max"){
        ez::heap h;
        
        for(int i : {1,2,3,4,6,6,7}){
            h.insert(i);
        }

        int n = h.extract_max();
        CHECK(n == 7);
        CHECK(h.size() ==6);

        n = h.extract_max();
        CHECK(n == 6);
        CHECK(h.size() ==5);

        n = h.extract_max();
        CHECK(n == 6);
        CHECK(h.size() ==4); 
    
    }

    SECTION("increase_key"){
        ez::heap h;
        for(int i : {1,2,3,4,6,6,7}){
            h.insert(i);
        }

        h.increase_key(4,8);
        CHECK(h.maximum() == 8);
        CHECK(h.size() == 7);

        h.increase_key(6,10);
        CHECK(h.maximum() == 10);
        CHECK(h.size() == 7);

    }
}