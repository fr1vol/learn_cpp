#include <catch.hpp>
#include <heap_object.hpp>


TEST_CASE("test heap object","[class]"){

    auto t = heap_object::create(2,4);
    CHECK(t->val == 2);
    CHECK(t->val2 == 4);
}