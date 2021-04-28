#include <catch.hpp>
#include <shared_ptr.hpp>

using namespace ez;

TEST_CASE("control_block","[class]"){


    SECTION("inti"){
        control_block_impl<int> cb;
        CHECK(cb.weak_count() == 1);
        CHECK(cb.owner_count() == 1);
        CHECK(cb.get() == nullptr);

        
        control_block_impl<int> cb_2(nullptr);
        CHECK(cb_2.weak_count() == 1);
        CHECK(cb_2.owner_count() == 1);
        CHECK(cb_2.get() == nullptr);

        int* a = new int(4);
        control_block_impl<int> cb_3(a);
        CHECK(cb_3.weak_count() == 1);
        CHECK(cb_3.owner_count() == 1);
        CHECK(cb_3.get() == a);

    }
    
    SECTION("increase"){
        int* a = new int(4);
        control_block_impl<int> cb(a);

        cb.increase_owner();
        cb.increase_weak();

        CHECK(cb.weak_count() == 2);
        CHECK(cb.owner_count() == 2);
    }


    SECTION("decrease"){
        struct A{
            ~A(){
                std::cout<<"~A\n";
            }
            int a = 0;
        };
        A* a = new A();
        auto p_cb = new control_block_impl<A>(a);

        p_cb->increase_owner();
        p_cb->increase_weak();
        
        p_cb->decrease_weak();
        CHECK(p_cb->weak_count() == 1);

        p_cb->decrease_owner();
        CHECK(p_cb->owner_count() == 1);

    }

}


TEST_CASE("shared_ptr","[class]"){

    
        struct A{
            int a = 0;
        };

        struct B: A{
            int b =1;
        };

    SECTION("init"){
        shared_ptr<int> sp;
        CHECK(sp.get() == nullptr);
        CHECK(sp.use_count() == 0);

        shared_ptr<int> sp1(new int(5));
        CHECK(sp1.get() != nullptr);
        CHECK(sp1.use_count() == 1);
        CHECK(*sp1 == 5);
        
        shared_ptr<int> cp = sp1;
        CHECK(cp.get() != nullptr);
        CHECK(cp.use_count() == 2);
        CHECK(sp1.use_count() == 2);
        CHECK(*cp == 5);

        shared_ptr<A> a(new B);
        CHECK(a.get() != nullptr);
        CHECK(a.use_count() == 1);
        CHECK(a->a == 0);
        CHECK((*a).a == 0);

        shared_ptr<B> b = a;
        CHECK(b.get() != nullptr);
        CHECK(b.use_count() == 2);
        CHECK(a.use_count() == 2);
        CHECK(b->a == 0);
        CHECK((*b).a == 0);
        CHECK(b->b == 1);
    }

    SECTION("function : swap"){
        shared_ptr<int> a(new int(1));
        shared_ptr<int> b(new int(5));

        a.swap(b);
        CHECK(*a == 5);
        CHECK(*b == 1);

    }

    SECTION("function : reset "){
        shared_ptr<int> a(new int(1));
        auto a2 = a;

        CHECK(a2.use_count() == 2);//before
        a.reset();
        CHECK(a.get() == nullptr);
        CHECK(a.use_count() == 0);
        CHECK(a2.get() != nullptr);
        CHECK(a2.use_count() == 1);
        CHECK(*a2 == 1);

        auto p = new int(4);
        a.reset(p);
        CHECK(*a == 4);
    }
}