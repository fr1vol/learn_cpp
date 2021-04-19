#include <catch.hpp>
#include <trie.h>


TEST_CASE("test node ","[class]"){

    SECTION("init"){
        ez::node* test = new ez::node();
        CHECK(test->end == false);
        for(auto i : test->ch){
            CHECK (i == nullptr);
        }
    }

    SECTION("add"){
        ez::node test;
        test.add('a');
        
        CHECK(test.has('a'));
        
    }

}

TEST_CASE("test trie","[class]"){

    
    ez::trie* test = new ez::trie();
    test->insert("abcf");
    test->insert("abcde");
    test->insert("bbcde");
    test->insert("apple");
    test->insert("app");
    

    SECTION("search"){

        CHECK(test->search("app"));
        CHECK(test->search("abcf"));
        CHECK(!test->search("f"));
    }

    SECTION("search"){

        CHECK(test->startsWith("app"));
        CHECK(test->startsWith("ab"));
        CHECK(!test->startsWith("f"));
    }
}