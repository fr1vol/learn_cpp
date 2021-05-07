#include <catch.hpp>
#include <math.hpp>
#include <iostream>
using namespace ez;


TEST_CASE("square_root","[function]"){
    auto r = square_root<100>(150);
    Approx target = Approx(12.24744871391).epsilon(0.00001);
    CHECK(r == target);

    auto r2 = suqare_root_newtow(150);
    CHECK(r2 == target);
}


