#include <heap.h>
#include <practice.h>
#include <limits.h>

using namespace ez;

    
    //method
    void heap::insert(int key){
        int index = size();
        data.push_back(INT_MIN);
        increase_key(index,key);
    }   
    int heap::extract_max(){
        assert(!empty());
        int max_num = maximum();
        std::swap(data.front(),data.back());
        data.pop_back();
        max_heapify(data,0);
        return max_num;
    }
    void  heap::increase_key(int index,int key){
        assert(data[index] < key);
        data[index] = key;
        while(index > 0 && data[parent(index)] < data[index]){
            std::swap( data[parent(index)],data[index]);
            index = parent(index);
        }
    }

    //query
    int heap::maximum(){
        assert(!empty());
        return data.front();
    }

    std::size_t heap::size(){
        return data.size();
    }

    std::size_t heap::capacity(){
        return data.capacity();
    }
    bool heap::empty(){
        return data.empty();
    }
