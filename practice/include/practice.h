#ifndef CODE_PRACTICE_H
#define CODE_PRACTICE_H

#include <vector>
#include <cstddef>
#include <string>
#include <iterator>
namespace ez{

    // ------------------------ helper -------------------------
    int get_digits(int val);
    //-------------------------- sort --------------------------
    void quick_sort(std::vector<int>& v,int start, int end);
    
    //-------------------------- heap --------------------------
    int left(int index); 
    int right(int index);
    int parent(int index);
    void max_heapify(std::vector<int>& v,int index);
    void build_heap(std::vector<int>& v);
    std::vector<int> heap_sort(std::vector<int>& v);


    //-------------------------- string ------------------------
    std::size_t strlen(const char* str);
    int strcmp(const char* str1,const char* str2);
    char* strcpy(char* des,const char* src);
    int strstr( const  std::string& str, const std::string& substr);
    
  
    //------------------------ tree ----------------------------
    struct node
    {
        int val;
        node* left;
        node* right;

        node(int v):val(v),left(nullptr),right(nullptr){}
    };

    std::vector<int> pre_order(node* root);
    std::vector<int> in_order(node* root);
    std::vector<int> post_order(node* root);
    std::vector<std::vector<int>> level_order(node* tree);


    //------------------------ search -------------------------
    template<typename Iter,typename T>
    Iter lower_bound(Iter first,Iter last, const T& key){
        
        using diff_t = typename std::iterator_traits<Iter>::difference_type; 
        Iter  it;
        diff_t step;
        diff_t count = std::distance(first,last);
        while(count > 0){
            it = first;
            step = count/2;
            std::advance(it,step);
            if(*it < key) {
                first = ++it; 
                count -= step + 1; 
                }
            else
                count = step;
        }
        return first;
    }

    template<typename Iter,typename T>
    Iter upper_bound(Iter first,Iter last, const T& key){
        
        using diff_t = typename std::iterator_traits<Iter>::difference_type; 
        Iter  it;
        diff_t step;
        diff_t count = std::distance(first,last);
        while(count > 0){
            it = first;
            step = count/2;
            std::advance(it,step);
            if(*it <= key) {
                first = ++it; 
                count -= step + 1; 
                }
            else
                count = step;
        }
        return first;
    }


} //namespace ez
#endif //CODE_PRACTICE_H