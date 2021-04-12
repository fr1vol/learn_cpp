#define  CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <practice.h>

TEST_CASE("test quick_sort function ","[function]"){
    std::vector<int> v = {4,6,2,7,1,9,8,3,5,0};

    SECTION( "test all ranges " ) {
        
        int start = 0;
        int end = v.size()-1;
        ez::quick_sort(v,start,end);
        std::vector<int> test = {0,1,2,3,4,5,6,7,8,9};
        CHECK(v == test);
    }
    SECTION( "test the local ranges " ) {
        
        int start = 0;
        int end = 3;
        ez::quick_sort(v,start,end);
        std::vector<int> test = {2,4,6,7,1,9,8,3,5,0};
        CHECK(v == test);

    }

    SECTION("test start >= end"){
        int start = 2;
        int end = 0;
        ez::quick_sort(v,start,end);
        std::vector<int> test = {4,6,2,7,1,9,8,3,5,0};
        CHECK(v == test);
        ez::quick_sort(v,start,start);
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

    std::vector<int> test = {1,2,3,4,5,6,7,7,8,9,10,11,12,13};
    
    SECTION( "has key"){
        auto it = ez::lower_bound(test.begin(),test.end(),7);
        CHECK(it != test.end());
        CHECK(*it == 7);
        CHECK(it-test.begin()+1 == 7);
    }

    SECTION( "has not key"){
        auto it = ez::lower_bound(test.begin(),test.end(),16);
        CHECK(it == test.end());
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
