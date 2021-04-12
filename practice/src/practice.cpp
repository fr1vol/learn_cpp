
#include <practice.h>
#include <cassert>
#include <string>
#include <stack>
#include <iostream>
#include <queue>
namespace ez
{
    
    void quick_sort(std::vector<int>& v, int start, int end){
        assert(start >= 0 && static_cast<int>(v.size()) > end);
        
        if(start >= end){
            return ;
        }
        int left = start;
        int right = end;
        int key = v[left];

        while(left < right){
            while(left < right && v[right] > key){
                --right;
            }
            v[left] = v[right];

            while(left < right && v[left] < key){
                ++left;
            }
            v[right] = v[left]; 
        }
        v[left] = key;

        quick_sort(v,start, left-1);
        quick_sort(v,left+1,end);
        
    }


    std::size_t strlen(const char * str){
        assert(str != nullptr);
        auto end = str;
        while( *end != '\0'){
            ++end;
        }
        return end - str;
    }

    
    int strcmp(const char* str1,const char* str2){
        assert(str1 != nullptr || str2 != nullptr);

        while(*str1 == *str2)
        {
            if(*str1 == '\0'){
                return 0;
            }
            ++str1;
            ++str2;
        }

        return static_cast<int>(*str1 - *str2);
    }
    
    char* strcpy(char* des,const char* src){
        assert(des != nullptr || src != nullptr);

        char* start = des;
        while(*src != '\0'){
            *start = *src;
            ++start;
            ++src;
        }
        *start = '\0';
        return des;
    }

    std::vector<int> get_index(const std::string& str){
        if(str.length() < 2){
            return std::vector<int>(str.length(),0);
        }
        std::vector<int> ans(str.length(),0);
        for(int i =0,j=1; j < str.length();){
            
            if(str[i] == str[j]){
                ans[j] = ans[i] + 1;
                ++i;
                ++j;
            }else{
                if(i == 0){
                    ans[j] = 0;
                    ++j; 
                }else{
                    --i;
                    i = ans[i];
                }
            }
        }
        return ans;
    }


    int strstr( const  std::string& str, const std::string& substr ){
        auto index = get_index(substr);

        int k = 0;
        int n = str.length();
        int m = substr.length();
        if(m == 0) return 0;
        for(int i = 0; i < n; ++i){
            
            while(k > 0 && str[i] != substr[k]){
                k = index[k-1];
            }

            if(str[i] == substr[k]){
                ++k;
            }

            if(k == m) {return i-m+1;}

        }

        return -1;
    }

    std::vector<int> pre_order(node* root){
        if(root == nullptr) return{};
        std::vector<int> ans;
        std::stack<node*> s;
        s.push(root);
        while(!s.empty()){
            auto n = s.top();
            s.pop();
            ans.push_back(n->val);

            if(n->right != nullptr){
                s.push(n->right);
            }
            if(n->left != nullptr){
                s.push(n->left);
            }
        }
        return ans;
    }

    std::vector<int> in_order(node* root){
        if(root == nullptr) return{};
        std::stack<node*> s;
        std::vector<int> ans;
        while(1){
            if ( root != nullptr ) {
               s.push ( root ); 
               root = root->left; 
            } else if ( !s.empty() ) {
               root = s.top(); 
               s.pop();
               ans.push_back(root->val);
               root = root->right; 
            } else{
               break; 
            }
        }
        return ans;
    }

    std::vector<int> post_order(node* root){
        if(root == nullptr) return{};
        std::stack<node*> nodes1;
        std::stack<node*> nodes2;
        std::vector<int> ans;
        nodes1.push(root);
        while(!nodes1.empty()){
            auto tmp = nodes1.top();
            nodes1.pop();
            nodes2.push(tmp);
            if(tmp->left != nullptr){
                nodes1.push(tmp->left);
            }

            if(tmp->right != nullptr){
                nodes1.push(tmp->right);
            }
        }
        while(!nodes2.empty()){

            auto tmp  = nodes2.top();
            nodes2.pop();
            ans.push_back(tmp->val);
        }
        return ans;
    }

    //------------------------ BFS ----------------------------
    
    
    std::vector<std::vector<int>> level_order(node* tree){
        if(tree == nullptr) return {};

        std::vector<std::vector<int>>  ans;
        std::queue<node*> q;
        q.push(tree);

        while(!q.empty()){
            int size = q.size();
            std::vector<int> tmp;
            tmp.reserve(size);
            for(int i = 0 ; i< size ; ++i){
                
                auto n = q.front();
                tmp.push_back(n->val);
                q.pop();

                if(n->left != nullptr){
                    q.push(n->left);
                }

                if(n->right != nullptr){
                    q.push(n->right);
                }
            }
            ans.emplace_back(std::move(tmp));
        }
        return ans;
    }

}//namespace ez