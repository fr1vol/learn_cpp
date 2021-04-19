#ifndef EZ_HEAP_HPP
#define EZ_HEAP_HPP

#include <vector>
#include <cassert>

namespace ez
{

class heap
{
public:
    heap():data(){};
    heap(int n):data(){data.reserve(n);};
    
    //method
    void insert(int key);
    int extract_max();
    void increase_key(int index,int key);

    //query
    int maximum();
    std::size_t size();
    std::size_t capacity();
    bool empty();

private:
    std::vector<int>  data;
};

}

#endif