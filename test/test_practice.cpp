#include <catch.hpp>
#include <practice.h>
#include <limits.h>
#include <cstring>
#include <iostream>
using namespace std;

TEST_CASE("test quick_sort function ","[function]"){
    std::vector<int> v = {4,6,2,7,1,9,8,3,5,0};
    //has duplicate values
    std::vector<int> v2 = {4,4,4,3,6,2,7,1,9,8,3,5,0};
    

    SECTION("test quick_helper"){
        auto index = ez::quick_helper(v,0,v.size()-1);
        CHECK(index == 4);
    }

    
    SECTION( "test quick_sort2" ) {
        int start = 0;
        int end = v.size()-1;
        ez::quick_sort2(v,start,end);
        std::vector<int> test = {0,1,2,3,4,5,6,7,8,9};
        CHECK(v == test);

        start = 0;
        end = v2.size()-1;
        ez::quick_sort2(v2,start,end);
        std::vector<int> test2 = {0,1,2,3,3,4,4,4,5,6,7,8,9};
        CHECK(v2 == test2);

    }


    SECTION( "test quick_sort1 " ) {
        
        int start = 0;
        int end = v.size()-1;
        ez::quick_sort1(v,start,end);
        std::vector<int> test = {0,1,2,3,4,5,6,7,8,9};
        CHECK(v == test);

        start = 0;
        end = v2.size()-1;
        ez::quick_sort1(v2,start,end);
        std::vector<int> test2 = {0,1,2,3,3,4,4,4,5,6,7,8,9};
        CHECK(v2 == test2);
    }

    SECTION("test start >= end"){
        int start = 2;
        int end = 0;
        ez::quick_sort1(v,start,end);
        std::vector<int> test = {4,6,2,7,1,9,8,3,5,0};
        CHECK(v == test);
        ez::quick_sort1(v,start,start);
        CHECK(v == test);

        ez::quick_sort2(v,start,end);
        CHECK(v == test);
        ez::quick_sort2(v,start,start);
        CHECK(v == test);
    }
    
}

TEST_CASE("test strlen function","[function]"){
    const char* str = "123456";
    char str2[6] = {'1','2','3','4','5','6'}; 
    SECTION( "test const char*"){

        auto len = ez::strlen(str);
        CHECK(len == 6);
    }

    SECTION( "test char[]"){

        auto len = ez::strlen(str2);
        CHECK(len == 6);
    }

    SECTION( "test  bad char[]"){

        str2[3] = '\0';
        auto len = ez::strlen(str2);
        CHECK(len == 3);
    }
}

TEST_CASE("test strcmp","[function]"){
    const char* str1 = "abcdefg";
    const char* str2 = "abcdefg";
    const char* str3 = "abcDefg";
    const char* str4 = "abc";

    SECTION( " str1 == str2"){
        CHECK(ez::strcmp(str1, str2) == 0);
    }

    SECTION( " str1 != str3"){
        CHECK(ez::strcmp(str1, str3) > 0);
    }
    SECTION( " str3 != str1"){
        CHECK(ez::strcmp(str3, str1) < 0);
    }
    SECTION( "str4 != str1"){
        CHECK(ez::strcmp(str4, str1) < 0);
    }
}

TEST_CASE("test strcpy","[function]"){
    
    const char* src = "123456";
    const char* src2 = "123";
    char des[7]  = {0};
    
    SECTION("normal"){
        ez::strcpy(des,src);
        CHECK(std::strlen(des) == 6);
        CHECK(std::strcmp(des,src) == 0);
    }

    SECTION("mid \\0 "){
        ez::strcpy(des,src2);
        CHECK(std::strlen(des) == 3);
        CHECK(std::strcmp(des,src2) == 0);
    }

    
}

TEST_CASE("test strstr"){
    std::string str = "123456";

    SECTION( "has substr"){
        int index = ez::strstr(str,"456");
        CHECK(index == 3);
    }
    SECTION( "has not substr"){
        int index = ez::strstr(str,"789");
        CHECK(index == -1);
    }

    SECTION( "empty  string"){
        int index = ez::strstr(str,"");
        CHECK(index == 0);
    }
}

TEST_CASE("test lower_bound","[function]"){

    std::vector<int> test = {1,2,3,4,6,7,7,8,9,10,11,12,13};
    
    SECTION( "has key"){
        auto it = ez::lower_bound(test.begin(),test.end(),7);
        CHECK(it != test.end());
        CHECK(*it == 7);
        CHECK(it-test.begin()+1 == 6);
    }

    SECTION( "has not key, key > max"){
        auto it = ez::lower_bound(test.begin(),test.end(),16);
        CHECK(it == test.end());
    }
    SECTION( "has not key, min < key < max"){
        auto it = ez::lower_bound(test.begin(),test.end(),5);
        CHECK(*it == 6);
    }
    SECTION( "has not key, key < min"){
        auto it = ez::lower_bound(test.begin(),test.end(),0);
        CHECK(*it == 1);
    }

}

TEST_CASE("test upper_bound","[function]"){

    std::vector<int> test = {1,2,3,4,5,6,7,7,8,9,10,11,12,13};
    
    SECTION( "has key"){
        auto it = ez::upper_bound(test.begin(),test.end(),7);
        CHECK(it != test.end());
        CHECK(*it == 8);
        CHECK(it-test.begin()+1 == 9);
    }

    SECTION( "has not key"){
        auto it = ez::upper_bound(test.begin(),test.end(),16);
        CHECK(it == test.end());
    }
}


TEST_CASE("test tree order","[function]"){

    std::vector<ez::node> tree(8,0);
    for(int i=0;i<tree.size(); ++i){
        tree[i].val =  i+1;
    }
    /*
        test tree
            1
           / \
          2   3
         / \   \
        4   5   6
           / \
          7   8
    
    */
    tree[0].left = &tree[1];
    tree[0].right = &tree[2];
    tree[1].left = &tree[3];
    tree[1].right = &tree[4];
    tree[2].right = &tree[5];
    tree[4].left = &tree[6];
    tree[4].right = &tree[7];

    SECTION( "pre_order"){
        auto v = pre_order(&tree[0]);
        std::vector<int>  ans = {1,2,4,5,7,8,3,6};
        CHECK(v == ans);
    }

    SECTION( "in_order"){
        auto v = in_order(&tree[0]);
        std::vector<int>  ans = {4,2,7,5,8,1,3,6};
        CHECK(v == ans);
    }

    SECTION( "post_order"){
        auto v = post_order(&tree[0]);
        std::vector<int>  ans = {4,7,8,5,2,6,3,1};
        CHECK(v == ans);
    }

    SECTION( "level_order"){
        auto v = level_order(&tree[0]);
        std::vector<std::vector<int>> ans = {{1},{2,3},{4,5,6},{7,8}};
        CHECK(v == ans);
    }
    
    SECTION( "test nullptr"){
        ez::node* test = nullptr;
        auto v1 = pre_order(test);
        auto v2 = in_order(test);
        auto v3 = post_order(test);
        auto v4 = level_order(test);

        CHECK(v1.empty());
        CHECK(v2.empty());
        CHECK(v3.empty());
        CHECK(v4.empty());
    }
}

TEST_CASE("test get digits","[function]"){

    SECTION(" 0"){
        CHECK(ez::get_digits(0) == 1);
    }

    SECTION(" 1234567"){
        CHECK(ez::get_digits(1234567) == 7);
    }

    SECTION(" INT_MAX"){
        CHECK(ez::get_digits(INT_MAX) == 10);
    }

    SECTION(" -1"){
        CHECK(ez::get_digits(-1) == 1);
    }

    SECTION("-1234567"){
        CHECK(ez::get_digits(-1234567) == 7);
    }


    SECTION(" INT_MIN"){
        CHECK(ez::get_digits(INT_MIN) == 10);
    }
}

TEST_CASE("test left,right,parent","[function]"){

    SECTION("left"){
        CHECK( ez::left(0) == 1);
        CHECK( ez::left(1) == 3);
        CHECK( ez::left(2) == 5);
    }

    SECTION("right"){
        CHECK( ez::right(0) == 2);
        CHECK( ez::right(1) == 4);
        CHECK( ez::right(2) == 6);
    }

    SECTION("parent"){
        CHECK( ez::parent(3) == 1);
        CHECK( ez::parent(4) == 1);
        CHECK( ez::parent(2) == 0);
    }
}


TEST_CASE("test real_val & virtual_val","[function]"){
    
    SECTION( "real_val"){
        int i = ez::real_val(5,6);
        CHECK(i == 5);

        int i2 = ez::real_val(7,6);
        CHECK(i2 == 1);

        int i3 = ez::real_val(-1,9);
        CHECK(i3 == -1);
    }

    SECTION( "virtual_val"){
        int i = ez::virtual_val(5,6,7);
        CHECK(i == 47);

        int i2 = ez::virtual_val(7,6,8);
        CHECK(i2 == 55);

        int i3 = ez::virtual_val(-1,6,9);
        CHECK(i3 == 53);
    }
}

TEST_CASE("test heap  ","[function]"){


    SECTION( " max_heapify"){
        std::vector<int> test = {0,5,6,3,2,1,4};
        ez::max_heapify(test,0);
        std::vector<int> rlt = {6,5,4,3,2,1,0};
        CHECK(test == rlt);
    }

    SECTION( "build_heap"){
        std::vector<int> test = {0,2,5,4,6,1,3};
        ez::build_heap(test);
        std::vector<int> rlt = {6,4,5,0,2,1,3};
        CHECK(test == rlt);
    }

    SECTION( " heap_sort"){
        
        std::vector<int> test = {6,4,5,0,2,1,3};
        auto v = ez::heap_sort(test);
        std::vector<int> rlt = {6,5,4,3,2,1,0};
        CHECK(v == rlt);
    }
}




TEST_CASE("test merge sort","[function]"){

    SECTION( "merge1 : use tmp"){
        std::vector<int> test = {18,1,6,7,4,5,8,0};
        auto tmp = test;
        ez::merge(test,std::pair<int,int>{1,3},std::pair<int,int>{4,6},tmp);
        std::vector<int> rlt  = {18,1,4,5,6,7,8,0};
        CHECK(test == rlt);

        std::vector<int> test2 = {2,1};
        ez::merge(test2,std::pair<int,int>{0,0},std::pair<int,int>{1,1},tmp);
        std::vector<int> rlt2  = {1,2};
        CHECK(test2 == rlt2);

        std::vector<int> test3 = {18,1,6,8,2,3,4,0};
        ez::merge(test3,std::pair<int,int>{1,3},std::pair<int,int>{4,6},tmp);
        std::vector<int> rlt3  = {18,1,2,3,4,6,8,0};
        CHECK(test3 == rlt3);

    }

    SECTION( "merge2 : in palce"){
        std::vector<int> test = {18,1,6,7,4,5,8,0};
        ez::merge2(test,std::pair<int,int>{1,3},std::pair<int,int>{4,6});
        std::vector<int> rlt  = {18,1,4,5,6,7,8,0};
        CHECK(test == rlt);

        std::vector<int> test2 = {2,1};
        ez::merge2(test2,std::pair<int,int>{0,0},std::pair<int,int>{1,1});
        std::vector<int> rlt2  = {1,2};
        CHECK(test2 == rlt2);

        std::vector<int> test3 = {18,1,6,8,2,3,4,0};
        ez::merge2(test3,std::pair<int,int>{1,3},std::pair<int,int>{4,6});
        std::vector<int> rlt3  = {18,1,2,3,4,6,8,0};
        CHECK(test3 == rlt3);

    }
    SECTION( "merge sort"){
        std::vector<int> test = {6,1,2,9,3,7,4,5,8};
        ez::merge_sort(test,0,test.size()-1);
        std::vector<int> rlt  = {1,2,3,4,5,6,7,8,9};
        CHECK(test == rlt);

        std::vector<int> test2 = {6,-1,2,9,3,7,4,-5,8};
        ez::merge_sort(test2,0,test2.size()-1);
        std::vector<int> rlt2  = {-5,-1,2,3,4,6,7,8,9};
        CHECK(test2 == rlt2);

        std::vector<int> test3 = {-1,-2,-2,-3,-3,-7,-4,-5,-8};
        ez::merge_sort(test3,0,test3.size()-1);
        std::vector<int> rlt3  = {-8,-7,-5,-4,-3,-3,-2,-2,-1};
        CHECK(test3 == rlt3);
    }
}







