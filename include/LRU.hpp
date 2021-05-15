#ifndef EASY_LRU_CACHE_HPP
#define EASY_LRU_CACHE_HPP
#include <iostream>
#include <unordered_map>
#include <cassert>
namespace ez{

class LRUCache {
    struct node{
        node():val(0),key(0),pre(nullptr),next(nullptr){}
        node(int key_,int val_):val(val_),key(key_),pre(nullptr),next(nullptr){}

        int val;
        int key;
        node* pre;
        node* next;
    };
    std::unordered_map<int,node*> m;
    node head;
    node tail;
    int cap ;
public:

    void print(){
        auto n = head.next;

        while(n != &tail){
            std::cout<<n->val<<" ";
            n = n->next;
        }
        std::cout<<std::endl;
    }
    void insert_head(node* n){
        assert(n != nullptr);
        auto first = head.next;
        head.next = n;
        n->next = first;
        n->pre = &head;
        first->pre = n;
    }

    void move_self(node* n){
        assert(n != nullptr && n != &head && n != &tail);
        auto pre_node = n->pre;
        auto next_node = n->next;
        pre_node->next = next_node;
        next_node->pre = pre_node;
        n->next = nullptr;
        n->pre = nullptr;
    }

    node* move_last(){
        assert(tail.pre != &head);
        auto n = tail.pre;
        move_self(n);
        return n;
    }
public:
    ~LRUCache(){
        auto n = head.next;
        while(n != &tail){
            auto next_node = n->next;
            delete n;
            n = next_node;
        }
    }

    int size() const {
        return m.size();
    }
public:
    LRUCache(int capacity):m(),head(),tail(),cap(capacity) {
        head.next = &tail;
        tail.pre = & head;
    }
    
    int get(int key) {
        if(auto it = m.find(key); it == m.end()){
            return -1;
        }else{
            auto n = it->second;
            move_self(n);
            insert_head(n);
            return n->val;
        }
    }
    
    void put(int key, int value) {
        if(auto it = m.find(key); it == m.end()){
            if(m.size() == cap){
                auto n = move_last();
                m.erase(n->key);

                n->key = key;
                n->val = value;
                m.emplace(key,n);
                insert_head(n);
            }else{
                auto n = new node(key,value);
                m.emplace(key,n);
                insert_head(n);
            }
        }else{
            auto n = it->second;
            n->val = value;
            move_self(n);
            insert_head(n);
        }
    }
};
} // namespace ez;
#endif // EASY_LRU_CACHE_HPP