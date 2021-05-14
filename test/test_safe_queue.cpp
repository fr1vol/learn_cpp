#include <catch.hpp>
#include <safe_queue.hpp>

using namespace ez;
using namespace std;

TEST_CASE("test safe_queue","[class]"){
    
    SECTION("test init"){
        safe_queue<int> q;
        CHECK(q.size() == 0);
        CHECK(q.empty());

        int a = 1;
        q.push(std::move(a));
        CHECK(q.size() == 1);
        CHECK(!q.empty());


        q.push(2);
        CHECK(q.size() == 2);
        CHECK(!q.empty());

        auto [val , flag] = q.pop();

        CHECK(flag);
        CHECK(val == 1);
        CHECK(q.size() == 1);
        CHECK(!q.empty());

        auto [val2 , flag2] = q.wait_pop();
        CHECK(flag2);
        CHECK(val2 == 2);
        CHECK(q.size() == 0);
        CHECK(q.empty());
        
    }
}