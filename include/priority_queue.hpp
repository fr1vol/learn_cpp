#pragma once
#include <vector>
#include <utility>
#include <cassert>
namespace ez{

class priority_queue
{
    std::vector<int> c;

    using idx_t = unsigned int;
private:
    idx_t left(idx_t idx){
        return idx*2+1;
    }

    idx_t right(idx_t idx){
        return idx*2+2;
    }

    idx_t parent(idx_t idx){
        return idx == 0? 0 :(idx-1)/2;
    }

    void max_heapify(idx_t idx){
        assert(idx < c.size());
        auto l = left(idx);
        auto r = right(idx);

        idx_t largest_idx = idx;
        if(l < c.size() && c[l] > c[largest_idx]){
            largest_idx = l;
        }
        if(r < c.size() && c[r] > c[largest_idx]){
            largest_idx = r;
        }
        if(largest_idx != idx){
            std::swap(c[largest_idx],c[idx]);
            max_heapify(largest_idx);
        }
    }
    
    void build_heap(){
        if(c.empty()) return ;
        idx_t last_idx = c.size()-1;
        auto p = parent(last_idx);
        for(int i = p; i >= 0; --i){
            max_heapify(i);
        }

    }

public:

    priority_queue():c(){}
    priority_queue(const std::vector<int>& v):c(v){
        build_heap();
    }

    priority_queue(std::vector<int>&& v):c(std::move(v)){
        build_heap();
    }

    const int& top() const
    {
        return c.front();
    };

    std::size_t size() const
    {
        return c.size();
    }

    bool empty() const
    {
        return c.empty();
    }

    void push(const int& val)
    {
        c.push_back(val);
        build_heap();
    }

    void pop()
    {
        assert(!c.empty());
        std::swap(c.front(),c.back());
        c.pop_back();
        max_heapify(0);
    }

};


};