#pragma once

#include <iterator>
#include <cassert>
namespace ez
{

    template<typename Iter>
    Iter random_select(Iter begin, Iter end)
    {
        using diff_t = typename std::iterator_traits<Iter>::difference_type;
        diff_t count = end - begin;
        diff_t step = rand()%count;
        return begin+step;
    }

    template<typename Iter>
    Iter quick_select(Iter begin, Iter end){
        assert(begin != end);
        using diff_t = typename std::iterator_traits<Iter>::difference_type;
        using value_t = typename std::iterator_traits<Iter>::value_type;
        auto it_key = random_select(begin,end);
        value_t key = *it_key;
        std::iter_swap(begin,it_key);

        while(begin+1 != end){
            while(begin+1 != end && *(end-1) >= key ){
                --end;
            }
            std::iter_swap(begin,end-1);
            while( begin +1 != end && *begin <= key){
                ++begin;
            }
            std::iter_swap(begin,end-1);
        }
        return begin;
    }

    template<typename Iter>
    void quick_sort(Iter begin, Iter end){
        if(begin != end){
            auto it = quick_select(begin,end);
            quick_sort(begin,it);
            quick_sort(it+1,end);
        }
    }    

}