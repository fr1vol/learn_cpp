#include <catch.hpp>
#include <priority_queue.hpp>

using namespace ez;

TEST_CASE("test ez::priority_queue ","[class]"){

    SECTION("init"){
        priority_queue q;
        CHECK(q.size() == 0);
        CHECK(q.empty());
    }

    SECTION("init vector"){
        std::vector<int> test = {9,2,3,4,5,1,0,7,6,8};
        priority_queue q(test);

        CHECK(q.size() == 10);

        CHECK(q.top() == 9);
        q.pop();
        CHECK(q.top() == 8);
        CHECK(q.size() == 9);
    }

}