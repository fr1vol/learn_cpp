#include <crtp.hpp>
#include <catch.hpp>

#include <memory>
using namespace ez;
TEST_CASE("CRTP","[class]"){

    SECTION("static polymorphism"){
        std::shared_ptr<Base> b (new Derived_1());
        auto ans = b->func();
        CHECK( ans ==1);

        std::shared_ptr<Base> b2 (new Derived_2());
        auto ans2 = b2->func();
        CHECK( ans2 ==2);
    }
}