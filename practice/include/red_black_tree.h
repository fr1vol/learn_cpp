#ifndef EZ_RED_BLACK_TREE_H
#define EZ_RED_BLACK_TREE_H

#include <memory>
#include <utility>
#include <vector>
#include <queue>
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
    ~red_black_tree(){
        std::queue<rb_node*> q;
        if(root != nullptr){ 
            q.push(root);
        }

        while(!q.empty()){
            int size = q.size();
            for(int i = 0 ; i< size ; ++i){
                
                auto n = q.front();
                q.pop();

                if(n->left != nullptr){
                    q.push(n->left);
                }

                if(n->right != nullptr){
                    q.push(n->right);
                }

                delete n;
            }
        }
    }

    int size(const rb_node* node) const{
        if(node == nullptr) return 0;
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

        if(node == nullptr) return {V{},false};
        if(node->k == key){
            return {node->v,true};
        }else if(key < node->k ){
            return get(node->left,key);
        }else{
            return get(node->right,key);
        }

        return {V{},false};
    }

    rb_node* min(rb_node* node){
        if(node->left == nullptr) return node;
        return min(node->left);
    }

    K min(){
        return min(root)->k;
    }


    

    rb_node* rotate_left(rb_node* node){

        rb_node* x = node->right;
        if(x == nullptr) return node;
        rb_node* x_left = x->left;
        node->right = x_left;
        
        x->left = node;
        x->color = node->color;
        node->color =rb_node:: red;
        x->size = node->size;
        node->size = size(node);
        return x;
    }

    rb_node* rotate_right(rb_node* node){

        
        rb_node* x = node->left;
        if(x == nullptr) return node;
        rb_node* x_right = x->right;
        node->left = x_right;
        
        x->right = node;
        x->color = node->color;
        node->color = rb_node::red;
        x->size = node->size;
        node->size = size(node);
        return x;
    }

    void flip_color(rb_node* node){
        node->color = (node->color == rb_node::red)?rb_node::black : rb_node::red;
        node->left->color = (node->color == rb_node::red)?rb_node::black : rb_node::red;
        node->right->color = (node->color == rb_node::red)?rb_node::black : rb_node::red;
    }

  
    
    void put(K key,V val){
        root = put(root,key,val);
        root->color = rb_node::black;
    }

    rb_node* put(rb_node* node, K key,V val){
        if(node == nullptr){
            node = new rb_node(key,val);
            return node;
        }        

        if(key < node->k){
            node->left = put(node->left,key,val);
        }else if(key > node->k){
            node->right = put(node->right,key,val);
        }else{
            node->v= val;
        }
        
        if(is_red(node->right) && !is_red(node->left)) node = rotate_left(node);
        if(is_red(node->left) && is_red(node->left->left)) node = rotate_right(node);
        if(is_red(node->right) && is_red(node->left)) flip_color(node);

        node->size = size(node);

        return node;
    }

    rb_node* balance(rb_node* node){
        if(is_red(node->right)) node = rotate_left(node);
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
            delete node;
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
        if(root != nullptr){
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
            delete node;
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
        if(root != nullptr){
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
                
                auto tmp = get(node->right,min(node->right)->k);
                node->v = tmp.first;
                node->k = min(node->right)->k;
                node->right = delete_min(node->right);
            }else {
                node->right = delete_node(node->right,key);
            }
           
        }
        return balance(node);
    }

    void delete_node(K key){
        if(!is_red(root->left) && !is_red(root->right)){
            root->color =rb_node::red;
        }
        root = delete_node(root,key);
        if(root != nullptr) {
            root->color = rb_node::black;
        }
    }

    void in_order(rb_node* node,std::vector<rb_node*>&  v){
        if(node == nullptr){ return ;}
        in_order(node->left,v);
        v.push_back(node);
        in_order(node->right,v);
    }

    std::vector<rb_node*> sort(){
        std::vector<rb_node*> rlt;
        in_order(root,rlt);
        return rlt;
    }

   
private:
    rb_node* root;

};




}// namespace ez

#endif // EZ_READ_BLACK_TREE_H