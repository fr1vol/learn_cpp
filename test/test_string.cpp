#include <catch.hpp>
#include <string.hpp>
#include <iostream>
using namespace std;


TEST_CASE("test ez::string ","[class]"){

    

    SECTION("init")
    {
        
        CHECK(sizeof(ez::string) == 32);
        
        ez::string a;
        
        CHECK(a.size() == 0);
        CHECK(a.is_local());
        CHECK(a.empty());

        ez::string b = "123456789012345";
        CHECK(b.size() == 15);
        CHECK(b.is_local());
        CHECK(!b.empty());

        ez::string c = "1234567890123456";
        CHECK(c.size() == 16);
        CHECK(!c.is_local());
        CHECK(!c.empty());

        ez::string d = b;
        CHECK(d.size() == 15);
        CHECK(b.is_local());
        CHECK(!b.empty());

        ez::string e = c;
        CHECK(e.size() == 16);
        CHECK(!e.is_local());
        CHECK(!e.empty());

        ez::string a1("");
        
        CHECK(a1.size() == 0);
        CHECK(a1.is_local());
        CHECK(a1.empty());

    }
}