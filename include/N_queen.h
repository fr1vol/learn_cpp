#pragma once
#include <vector>
#include <string>
namespace ez
{
class N_queen {
    void filled_col(std::vector<std::vector<int>>& v,int col){
        int n = v.size();
        if(col == n) return ;
        int pre_col = col -1;
        for(int i =0;i < n; ++i){
            if(v[i][pre_col] != 0){
                int val = v[i][pre_col];
                if(i > 0){
                    v[i-1][col] |= (val & 1);
                }
                    v[i][col] |= (val & (1<<1));
                if(i+1 < n){
                    v[i+1][col] |= (val & (1<<2));
                }
            }
        }
    }

    std::vector<int> select(std::vector<std::vector<int>>& v,int col){
        int n = v.size();
        std::vector<int> ans;
        for(int i =0;i < n; ++i){
            if(v[i][col] == 0){
                ans.push_back(i);
            }
        }
        return ans;
    }

    void clear_col(std::vector<std::vector<int>>& v,int col){
        if(col == v.size()) return ;
        int n = v.size();
        for(int i =0;i < n; ++i){
            v[i][col] = 0;
        }
    }

    
    void helper(std::vector<std::vector<int>>& v,std::vector<int>& tmp,int col){
        if(tmp.size() == v.size()){
            ans.push_back(tmp);
            return;
        }
        auto vec = select(v,col);
        for(auto row : vec){
            
            v[row][col] = 7;
            tmp.push_back(row);
            filled_col(v,col+1);
            helper(v,tmp,col+1);
            clear_col(v,col+1);
            v[row][col] = 0;
            tmp.pop_back();
        }
    }

    std::vector<std::string> vec_to_str(const std::vector<int>& v){
        std::string tmp(v.size(),'.');
        std::vector<std::string> ans(v.size(),tmp);
        for(int i = 0; i< v.size();++i){
            ans[v[i]][i] = 'Q';
        }
        return ans;
    }
    std::vector<std::vector<int>> ans;
public:
    std::vector<std::vector<std::string>> solveNQueens(int n) {
        std::vector<std::vector<int>> v(n,std::vector<int>(n,0));
        std::vector<int> indexs;
        helper(v,indexs,0);
        std::vector<std::vector<std::string>> rlt;
        for(const auto& v : ans){
            rlt.emplace_back(vec_to_str(v));
        }
        return rlt;
    }
};
    
}