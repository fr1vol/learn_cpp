#pragma once

#include <iostream>

namespace ez
{
    //静态多态

    struct Base
    {
        virtual int func() = 0;
    };

    template<typename T>
    struct CRTP: Base 
    {
        int func() override
        {   
            return impl().test();
        }

        T& impl(){
            return *static_cast<T*>(this);
        }
    };

    struct Derived_1 : CRTP<Derived_1>
    {
        int test(){
            std::cout<<"Derived 1 impl\n";
            return 1;
        }

        ~Derived_1(){
             std::cout<<"~ Derived 1 \n";
        }
    };

    struct Derived_2 : CRTP<Derived_2>
    {
        int test(){
            std::cout<<"Derived 2 impl\n";
            return 2;
        }

        ~Derived_2(){
             std::cout<<"~ Derived 2 \n";
        }
    };

}//namespace 