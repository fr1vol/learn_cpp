#ifndef EZ_MATH_H
#define EZ_MATH_H
#include <cmath>

namespace ez
{
    /*
        s = a^2 + b 
        连分数法
    */
    template<int N = 10>
    double square_root(int x){
        int a = 1;
        while( a * a < x ){
            ++a;
        };
        int b = x - a*a;

        int time = N;
        double tmp = 0;
        while(time--){
            tmp = tmp + 2 * a;
            tmp = b /tmp;
        }
        return a+tmp;
    }    

    /*
    newtow
    */
    double suqare_root_newtow(int x){
          
        double eps = 1e-12;
        double t = x;
        while (std::abs(t - x/t) > eps * t) {
            t = (t + x / t) / 2.0;
        }
        return t;
    }

   


}//namespace ez

#endif//EZ_MATH_H