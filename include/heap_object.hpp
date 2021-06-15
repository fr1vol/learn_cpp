#pragma once
#include <memory>

struct heap_object
{
    class token
    {
    private:
        token(){}
        friend heap_object;
    };

    heap_object(token,const int& v1,const int& v2):val(v1),val2(v2){}
    static std::shared_ptr<heap_object> create(const int& a,const int& b){
        return std::make_shared<heap_object>(token{},a,b);
    }
    int val;
    int val2;
private:
    heap_object(){}
};