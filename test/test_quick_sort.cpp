#include <quick_sort.hpp>
#include <catch.hpp>
#include <vector>
TEST_CASE("quick_sort iterator","function"){

    std::vector<int> v1 = {4,4,4,3,6,2,7,1,9,8,3,5,0};
    std::vector<int> v2 = {0,1,2,3,3,4,4,4,5,6,7,8,9};
    ez::quick_sort(v1.begin(),v1.end());
    CHECK(v2 == v1); 
}