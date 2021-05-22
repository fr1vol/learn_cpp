#pragma once

#include <cstddef>
#include <cstring>
#include <ostream>
#include <tuple>
#include <cassert>
namespace ez
{
    

    class string
    {
        static constexpr std::size_t buffer_size = 15;
        
        char*        m_start;
        std::size_t  m_length;
        union
        {
            std::size_t   m_capacity;
            char m_buffer[buffer_size+1] = {0};
        };


        void set_local(std::size_t n){
            m_start = m_buffer;
            m_length = n;
        }

        void set_heap(char* p, std::size_t len, std::size_t cap){
            m_start = p;
            m_length = len;
            m_capacity = cap;
        }

        std::tuple<char*,std::size_t,std::size_t> release(){
        
            if(!is_local()){
                char * p = m_start;
                std::size_t len = m_length;
                std::size_t cap = m_capacity;
                set_local(0);
                std::make_tuple(p,len,cap);
            }
            return std::make_tuple(nullptr,0,0);
            
        }

   public:

        string():m_start(m_buffer),m_length(0){}
        string(const char* str,std::size_t n){
            assert(str != nullptr);
            if(n <= buffer_size){
                set_local(n);
            }else{
                auto p = new char[n+1];
                set_heap(p,n,n);
            }
            strncpy(m_start,str,n);    
        }

        string(const char* str):string(str,str == nullptr?0:strlen(str)){}
        string(const string& other){
            if(other.size() <= buffer_size){
                set_local(other.size());
            }else{
                auto p = new char[other.size()+1];
                set_heap(p,other.size(),other.size());
            }
            strncpy(m_start,other.m_start,other.size());
        }
        string(string&& other){
            if(other.is_local()){
                set_local(other.size());
                strncpy(m_start,other.m_start,other.size());
            }else{
                auto [p,len,cap] = other.release();
                set_heap(p,len,cap);
            }
        }

        string& operator=(const string& other){
            if (other.size() > capacity()){
                if(!is_local()){
                    delete[] m_start;
                }
                auto p = new char[other.size()+1];
                set_heap(p,other.size(),other.size());   
            }
            strncpy(m_start,other.m_start,other.size());
            return *this;
        }

        string& operator=(string&& other){
            if(other.is_local()){
                strncpy(m_start,other.m_start,other.size());
            }else{
                if(!is_local()){
                    delete[] m_start;
                }
                auto [p,len,cap] = other.release();
                set_heap(p,len,cap);   
            }
            return *this;
        }

        void swap(string& other){   
            if(other.is_local()){
                if(is_local()){
                    char tmp[buffer_size+1]= {0};
                    auto n1 = size();
                    auto n2 = other.size();
                    strncpy(tmp,m_start,n1);
                    strncpy(m_start,other.m_start,n2);
                    strncpy(other.m_start,tmp,n1);
                    set_local(n2);
                    other.set_local(n1);
                }else {
                    auto n2 = other.size();
                    strncpy(m_start,other.m_start, n2);
                    auto [p,len,cap] = release();
                    set_local(n2);

                    other.set_heap(p,len,cap);
                }
            }else{
                if(is_local()){
                    auto n = size();
                    strncpy(other.m_start,m_start, n);
                    auto [p,len,cap] = other.release();
                    other.set_local(n);

                    set_heap(p,len,cap);
                }else{
                    auto [p1,len1,cap1] = other.release();
                    auto [p2,len2,cap2] = release();

                    other.set_heap(p2,len2,cap2);
                    set_heap(p1,len1,cap1);
                }
            }
        }
        
        ~string(){
            if(!is_local()){
                delete[] m_start;
            }
        }

        bool is_local() const{
            return  m_start == m_buffer;
        }

        std::size_t size() const {
            return m_length;
        }

        bool empty() const {
            return m_length == 0;
        }

        std::size_t capacity() const {
            if(is_local()){
                return buffer_size;
            }else{
                return  m_capacity;
            }
        }

        friend std::ostream& operator<<(std::ostream& os,const string& str){
            int size = str.size();
            for(int i =0; i < size;++i){
                os<<*(str.m_start + i);
            }
            return os;
        }
    };

    inline void swap(string& a,string& b){
        a.swap(b);
    }
};