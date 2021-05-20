#include <catch.hpp>
#include <N_queen.h>
#include <iostream>
using namespace ez;
using namespace std;
TEST_CASE("test N_queen ","[class]"){

    N_queen q;
    auto r = q.solveNQueens(4);
    for(auto i : r){
        for(auto j : i){
            cout<<j<<endl;
        }
        cout<<"=====\n";
    }
}