#ifndef EZ_TRIE_H
#define EZ_TRIE_H

#include <string>
#include <iostream>
#include <vector>
namespace ez{

struct node
{

    node* ch[26] = {nullptr};
    bool end = false;

    void add(char i){
        node* n = new node();
        ch[i-'a'] = n;
    }

    bool has(char i){
        return ch[i-'a'] != nullptr;
    }

    node* next(char i){
        return ch[i-'a'];
    }

    
};

class trie
{
    node  root;
public:
    void insert(std::string word) {
        node* tmp = &root;
        for(char i : word){
            if(tmp != nullptr && tmp->has(i)){
                tmp = tmp->next(i);
            }else{
                tmp->add(i);
                tmp = tmp->next(i);
            }
        }
        tmp->end = true;
    }
    
    bool search(std::string word) {
        node* tmp = &root;
        for(char i : word){
            if(tmp != nullptr && tmp->has(i)){
                tmp = tmp->next(i);
            }else{
                return false;
            }
        }
        return tmp->end;
    }
    
    bool startsWith(std::string prefix) {
        node* tmp = &root;
        for(char i : prefix){
            if(tmp != nullptr && tmp->has(i)){
                tmp = tmp->next(i);
            }else{
                return false;
            }
        }
        return true;
    }

};

}//namespace ez
#endif //EZ_TRIE_H