
#include <practice.h>
#include <cassert>
#include <string>
#include <stack>
#include <queue>
#include <limits.h>
#include <algorithm>
namespace ez
{
    
    int quick_helper(std::vector<int>& v,int left, int right)
    {
        assert(left >= 0 && right < v.size() && left < right);
        int key = v[left];
        
        while( left < right){
            while(left < right && v[right] >= key){
                --right;
            }
            v[left] =  v[right];

            while(left < right && v[left] <= key){
                ++left;
            } 
            v[right] = v[left];
        }
        v[left] = key;
        return left;
    }


    void quick_sort1(std::vector<int>& v,int start, int end){
        if (start<end){
            auto index = quick_helper(v, start, end);
            quick_sort1(v,start,index-1);
            quick_sort1(v,index+1,end);
        }
    }
    
    void quick_sort2(std::vector<int>& v,int start, int end){
        
        std::stack<std::pair<int,int>> s;
        if (start<end) {
            s.push({start,end});
        }
        while(!s.empty()){
            auto [left,right] = s.top();
            s.pop();
            auto index = quick_helper(v,left,right);
            if(index-1 > left){
                s.push({left,index-1});
            }

            if(index+1 < right){
                s.push({index+1,right});
            }
        }
    }




    //--------------------- merge --------------------
    int real_val(int cur_val,int max_val) {
        return cur_val < max_val ?cur_val :(cur_val / max_val);
    }

    int virtual_val(int sort_val,int cur_val,int max_val) {
        return  sort_val + cur_val * max_val;
    }

    void merge(std::vector<int>& v,std::pair<int,int> range_left,std::pair<int,int> range_right){


        int i = range_left.first;
        int j = range_right.first;
        int pos = i;

        if(v[i] >= 0  && v[j] >= 0){
            
            int max_num = std::max(v[range_left.second],v[range_right.second]) + 1;
            while(i< range_left.second+1 && j < range_right.second+1){
                int real_i = real_val(v[i],max_num);
                int real_j = real_val(v[j],max_num);
                int real_pos = real_val(v[pos],max_num);
                if(real_i > real_j){
                    v[pos] = virtual_val(real_j,real_pos,max_num);
                    ++j;
                }else{
                    v[pos] = virtual_val(real_i,real_pos,max_num);
                    ++i;
                }
                ++pos;
            }

            for(;i < range_left.second+1;++i){
                v[pos] = real_val(v[i],max_num);
                ++pos;
            }
            


            for(int  i = range_left.first; i < range_right.second + 1; ++i){
                v[i] = (v[i]%max_num);
            }
        }else{
            std::vector<int> v1(v.begin()+range_left.first,v.begin()+range_left.second+1);
            std::vector<int> v2(v.begin()+range_right.first,v.begin()+range_right.second+1);

            int i = 0;
            int j = 0;
            while(i != v1.size() && j != v2.size()){
                if(v1[i] > v2[j]){
                    v[pos] = v2[j];
                    ++j;
                }else{
                    v[pos] = v1[i];
                    ++i;
                }
                ++pos;
            }
            for(;i != v1.size();++i){
                v[pos] = v1[i];
                ++pos;
            }
        }
    }

    void merge_sort(std::vector<int>& v,int start, int end){
        if(start < end){
            int mid = start + (end-start)/2;
            merge_sort(v,start,mid);
            merge_sort(v,mid+1,end);
            merge(v,{start,mid},{mid+1,end});
        }
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


    static int  sizeTable[10] = { -9, -99, -999, -9999, -99999, -999999, -9999999,
                                      -99999999, -999999999, INT_MIN };

    
    int get_digits(int x){
        if(x > 0) x =  -x; 
        for (int i=0; ; i++){
            if (x >= sizeTable[i])
                return i+1;
        }
    }

    
    int left(int index){
        assert(index >= 0);
        return index*2+1;
    }
    int right(int index){
        assert(index >= 0);
        return index*2+2;
    }
    int parent(int index){
        assert(index > 0);
        return (index-1)/2;
    }
    void max_heapify(std::vector<int>& v,int index){
        int l = left(index);
        int r = right(index);

        int largest = index;
        if(l < v.size() && v[l] > v[largest]){
            largest = l;
        }

        if(r < v.size() && v[r] > v[largest]){
            largest = r;
        }

        if(largest != index){
            std::swap(v[largest],v[index]);
            max_heapify(v,largest);
        }

    }

    void build_heap(std::vector<int>& v){
        if(v.empty())  return;

        int last = v.size()-1;
        int p = (last == 0)? 0 : parent(last);
        for(int i = p; i >= 0;--i ){
            max_heapify(v,i);
        }
    }

    std::vector<int> heap_sort(std::vector<int>& v){
        std::vector<int> ans;
        while(!v.empty()){
            ans.push_back(v.front());
            std::swap(v.front(), v.back());
            v.pop_back();
            max_heapify(v,0);
        }
        return ans;
    }


}//namespace ez