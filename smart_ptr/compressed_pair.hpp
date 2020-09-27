#ifndef EASY_COMPRESSED_PAIR_H
#define EASY_COMPRESSED_PAIR_H

#include <type_traits>
#include <utility>

#include <iostream>
namespace ez{

template<typename T1, typename T2>
struct compressed_pair;



template<typename T1, typename T2, bool is_same, bool first_empty, bool second_empty>
struct compressed_pair_switch;

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, false , false , false>{ static constexpr int value = 0;};

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, false , true  , false>{ static constexpr int value = 1;};

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, false , false , true >{ static constexpr int value = 2;};

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, false , true  , true >{ static constexpr int value = 3;};

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, true  , false , false>{ static constexpr int value = 4;};

template<typename T1, typename T2>
struct compressed_pair_switch<T1,T2, true  , true  , true >{ static constexpr int value = 5;};

template<typename T>
void cp_swap(T& t1,T& t2)
{
    using std::swap;
    swap(t1,t2);
    std::cout<<"use swap"<<std::endl;
}


template<typename T1,typename T2,int switch_case>
class compressed_pair_imp;

template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,0>
{
public:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;

private:
    T1_param m_first;
    T2_param m_second;

public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):m_first(std::move(t1)) {}

    explicit compressed_pair_imp(T2_param  t2):m_second(std::move(t2)) {}
    
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :m_first( std::move(t1)),m_second( std::move(t2)) {}

    T1_reference  first() noexcept { return m_first;}
    T2_reference  second() noexcept { return m_second;}
    T1_const_reference  first() const noexcept {return m_first;}
    T2_const_reference  second() const  noexcept { return m_second;}

    void swap(compressed_pair_imp& other) noexcept
    {
        cp_swap(m_first,other.first());
        cp_swap(m_second,other.second());
    }
};

template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,1> : private std::remove_reference<T1>::type
{

    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;

private:    
    T2_param m_second;
public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):T1_param(std::move(t1)) {}

    explicit compressed_pair_imp(T2_param  t2):m_second(std::move(t2)) {}
    
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :T1_param( std::move(t1)),m_second( std::move(t2)) {}

    T1_reference  first() noexcept { return *this;}
    T2_reference  second() noexcept { return m_second;}
    T1_const_reference  first() const noexcept {return *this;}
    T2_const_reference  second() const  noexcept { return m_second;}

    void swap(compressed_pair_imp& other) noexcept
    {
        cp_swap(m_second,other.second());
    }
};

template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,2> : private std::remove_reference<T2>::type
{

public:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;

private:   
    T1_param m_first;
public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):m_first(std::move(t1)) {}

    explicit compressed_pair_imp(T2_param  t2):T2_param(std::move(t2)) {}
    
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :m_first( std::move(t1)),T2_param( std::move(t2)) {}

    T1_reference  first() noexcept { return m_first;}
    T2_reference  second() noexcept { return *this;}
    T1_const_reference  first() const noexcept {return m_first;}
    T2_const_reference  second() const  noexcept { return *this;}

    void swap(compressed_pair_imp& other) noexcept
    {
        cp_swap(m_first,other.first());
    }
};

template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,3> : private std::remove_cv<T2>::type
{
public:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;
public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):T1_param( std::move(t1)) {}

    explicit compressed_pair_imp(T2_param  t2):T2_param( std::move(t2)) {}
    
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :T1_param( std::move(t1)),T2_param( std::move(t2)) {}

    T1_reference  first() noexcept { return *this;}
    T2_reference  second() noexcept { return *this;}
    T1_const_reference  first() const noexcept {return *this;}
    T2_const_reference  second() const  noexcept { return *this;}

    void swap(compressed_pair_imp& other) noexcept
    {
    }
};


template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,4> 
{

public:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;
private:
    T1_param m_first;
    T2_param m_second;
public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):m_first(std::move(t1)) {}
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :m_first(std::move(t1)),m_second(std::move(t2)) {}

    T1_reference  first() noexcept { return m_first;}
    T2_reference  second() noexcept { return *this;}
    T1_const_reference  first() const noexcept {return m_first;}
    T2_const_reference  second() const  noexcept { return *this;}

    void swap(compressed_pair_imp& other) noexcept
    {
        cp_swap(m_first,other.first());
        cp_swap(m_second,other.second());
    }
};

template<typename T1,typename T2>
class compressed_pair_imp<T1,T2,5> : private std::remove_cv<T2>::type
{

public:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;

private:
    T1_param m_first;
public:
    compressed_pair_imp() {}

    explicit compressed_pair_imp(T1_param  t1):m_first(std::move(t1)) {}
    
    explicit compressed_pair_imp(T1_param  t1,T2_param  t2)
        :m_first(std::move(t1)),T2_param(std::move(t2)) {}

    T1_reference  first() noexcept { return m_first;}
    T2_reference  second() noexcept { return *this;}
    T1_const_reference  first() const noexcept {return m_first;}
    T2_const_reference  second() const  noexcept { return *this;}

    void swap(compressed_pair_imp& other) noexcept
    {
    }
};



template<typename T1,typename T2>
class compressed_pair : private compressed_pair_imp<T1,T2,
                                compressed_pair_switch<T1,T2,std::is_same<typename std::remove_cv<T1>::type,T2>::value, 
                                std::is_empty<T1>::value,
                                std::is_empty<T2>::value>::value>
{
    using base = compressed_pair_imp<T1,T2,
                                compressed_pair_switch<T1,T2,std::is_same<typename std::remove_cv<T1>::type,T2>::value, 
                                std::is_empty<T1>::value,
                                std::is_empty<T2>::value>::value>;
private:
    typedef typename std::remove_cv<T1>::type T1_param;
    typedef typename std::remove_cv<T2>::type T2_param;

    typedef typename std::remove_cv<T1>::type& T1_reference;
    typedef typename std::remove_cv<T2>::type& T2_reference;

    typedef const typename std::remove_cv<T1>::type& T1_const_reference;
    typedef const typename std::remove_cv<T2>::type& T2_const_reference;
public:
    compressed_pair():base(){}
    compressed_pair(T1_param  x,T2_param  y):base(std::move(x),std::move(y)){}
    explicit compressed_pair(T1_param  x):base(std::move(x)){}
    explicit compressed_pair(T2_param  y):base(std::move(y)){}

    T1_reference  first()  { return base::first();}
    T2_reference  second()  { return base::second();}
    T1_const_reference  first() const  {return base::first();}
    T2_const_reference  second() const   { return base::second();}

    void swap(compressed_pair& y) { base::swap(y); }
};

template<typename T>
class compressed_pair<T,T> : private compressed_pair_imp<T,T,
                                compressed_pair_switch<T,T,true, 
                                std::is_empty<T>::value,
                                std::is_empty<T>::value>::value>
{
    using base = compressed_pair_imp<T,T,
                                compressed_pair_switch<T,T,true, 
                                std::is_empty<T>::value,
                                std::is_empty<T>::value>::value>;
private:
    typedef typename std::remove_cv<T>::type T1_param;
    typedef typename std::remove_cv<T>::type T2_param;

    typedef typename std::remove_cv<T>::type& T1_reference;
    typedef typename std::remove_cv<T>::type& T2_reference;

    typedef const typename std::remove_cv<T>::type& T1_const_reference;
    typedef const typename std::remove_cv<T>::type& T2_const_reference;
public:
    compressed_pair():base(){}
    compressed_pair(T1_param  x,T1_param  y):base(std::move(x),std::move(y)){}
    explicit compressed_pair(T1_param  x):base(std::move(x)){}

    T1_reference  first()  { return base::first();}
    T2_reference  second()  { return base::second();}
    T1_const_reference  first() const  {return base::first();}
    T2_const_reference  second() const   { return base::second();}

    void swap(compressed_pair<T,T>& y) { base::swap(y); }
};


template <typename T1, typename  T2>
inline void swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y)
{
   x.swap(y);
}

}//namespace ez
 
#endif //EASY_COMPRESSED_PAIR_H