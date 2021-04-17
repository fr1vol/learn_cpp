#ifndef EZ_RED_BLACK_TREE_H
#define EZ_RED_BLACK_TREE_H

#include <memory>
#include <utility>
namespace ez 
{

template<typename Key,typename Value>
struct rb_node 
{
    enum Color : int{
        red,
        black
    };
    
    Key   k;
    Value v;
    Color  color;
    int size;
    rb_node* left;
    rb_node* right;
public:
    rb_node(Key key,Value value) : k(key),v(value),color(red),size(1),left(nullptr),right(nullptr){};
    
};


template<typename K,typename V>
class red_black_tree
{
public:
    using rb_node = rb_node<K,V>;


    red_black_tree():root(nullptr){}
    ~red_black_tree(){delete root;}

    int size(const rb_node* node) const{
        int left_size = node->left == nullptr ? 0 : node->left->size;
        int right_size = node->right == nullptr ? 0 : node->right->size;
        return left_size + right_size + 1;
    }

    int size() const {
        return size(root);
    }

    bool is_red(rb_node* node){
        if(node == nullptr) return false;
        return (node->color == rb_node::red);
    }

    std::pair<V,bool> get(const K& key){
        return get(root,key);
    }
    std::pair<V,bool> get(rb_node* node, const K& key){

        if(node == nullptr) return std::pair<V,bool>{V{},false};
        if(node->k == key){
            return std::pair<V,bool>{node->v,true};
        }else if(key < node->k ){
            return std::pair<V,bool>(node->left,key);
        }else{
            return std::pair<V,bool>(node->right,key);
        }

        return {V{},false};
    }

    rb_node min(rb_node* node){
        if(node->left == nullptr) return node;
        return min(node->left);
    }

    K min(){
        return min(root)->k;
    }


    

    rb_node* rotate_left(rb_node* node){

        rb_node* x = node->right;
        rb_node* x_left = x->left;
        node->right = x_left;
        x->left = node;
        x->color = node->color;
        node->color =rb_node:: red;
        x->size = node->size;
        node->size = size(node);
    }

    rb_node* rotate_right(rb_node* node){

        rb_node* x = node->left;
        rb_node* x_right = x->right;
        node->left = x_right;
        x->right = node;
        x->color = node->color;
        node->color = rb_node::red;
        x->size = node->size;
        node->size = size(node);
    }

    void flip_color(rb_node* node){
        node->color = rb_node::red;
        node->left->color = rb_node::black;
        node->right->color = rb_node::black;
    }
    
    void put(K key,V val){
        put(root,key,val);
        root->color = rb_node::black;
    }

    rb_node* put(rb_node* node, K key,V val){
        if(node == nullptr){
            node = new rb_node(key,val);//color: red ; size: 1 ;
            return node;
        }        

        if(key < node->k){
            node->left = put(node->left,key,val);
        }else if(key > node->k){
            node->right = put(node->right,key,val);
        }else{
            node->val = val;
        }
        
        if(is_red(node->right) && !is_red(node->left)) node = rotate_left(node);
        if(is_red(node->left) && is_red(node->left->left)) node = rotate_right(node);
        if(is_red(node->right) && is_red(node->left)) flip_color(node);

        node->size = size(node);

        return node;
    }

    rb_node* balance(rb_node* node){
        if(is_red(node->right) && !is_red(node->left)) node = rotate_left(node);
        if(is_red(node->left) && is_red(node->left->left)) node = rotate_right(node);
        if(is_red(node->right) && is_red(node->left)) flip_color(node);
        node->size = size(node);
        return node;
    }   

    rb_node* move_red_left(rb_node* node){
        flip_color(node);
        if(is_red(node->right->left)){
            node->right = rotate_right(node->right);
            node = rotate_left(node);
        }
        return node;
    }

    rb_node* delete_min(rb_node* node){
        if(node->left == nullptr){
            return nullptr;
        }

        if(!is_red(node->left) && !is_red(node->left->left)){
            node = move_red_left(node);
        }

        node->left = delete_min(node->left);
        return balance(node);
    }

    void delete_min(){
        if(!is_red(root->left) && !is_red(root->right)){
            root->color =rb_node::red;
        }
        root = delete_min(root);
        if(size() != 0){
            root->color = rb_node::black;
        }
    }

    rb_node* move_red_right(rb_node* node){
        flip_color(node);
        if(!is_red(node->left->left)){
            node->right = rotate_right(node->right);
        }
        return node;
    }

    rb_node* delete_max(rb_node* node){
       
        if(is_red(node->left)){
            node = rotate_right(node);
        }

        if(node->right == nullptr){
            return nullptr;
        }

        if(!is_red(node->right) && !is_red(node->right->left)){
            node = move_red_right(node);
        }

        node->right = delete_min(node->right);
        return balance(node);
    }

    void delete_max(){
        if(!is_red(root->left) && !is_red(root->right)){
            root->color =rb_node::red;
        }
        root = delete_max(root);
        if(size() != 0){
            root->color = rb_node::black;
        }
    }

    rb_node* delete_node(rb_node* node,K key){
        if(key < node->k){
            if(!is_red(node->left) && !is_red(node->left->left)){
                node = move_red_left(node);
            }
            node->left = delete_node(node->left,key);
        }else{
            if(is_red(node->left)){
                node = rotate_right(node);
            }
            if (key == node->k && node->right == nullptr){
                return nullptr;
            }

            if(!is_red(node->right) && !is_red(node->right->left)){
                node = move_red_right(node);
            }

            if(key == node->k){
                node->v = get(node->right,min(node->right)->k);
                node->key = min(node->right)->k;
                node->right = delete_min(node->right);
            }else {
                node->right = delete_node(node->right,key);
            }
            return balance(node);
        }
    }

    void delete_node(K key){
        if(!is_red(root->left) && !is_red(root->right)){
            root->color =rb_node::red;
        }
        root = delete_node(root,key);
        if(size() == 0) {
            root->color = rb_node::black;
        }
    }

private:
    rb_node* root;

};




}// namespace ez

#endif // EZ_READ_BLACK_TREE_H