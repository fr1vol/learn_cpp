#ifndef EASY_UNIQUE_PTR_H
#define EASY_UNIQUE_PTR_H

#include <memory>
#include <type_traits>
#include "compressed_pair.hpp"

namespace ez{

template <typename T>
struct default_delete { 
    constexpr default_delete()  = default;

    template <typename T2, std::enable_if_t<std::is_convertible<T2*, T*>::value, int> = 0>
    default_delete(const default_delete<T2>&)  {}

    void operator()(T* ptr) const  { 
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        delete ptr;
    }
};

template<typename T, typename Deleter, typename = void>
struct get_deleter_pointer_type
{
    using type = T*;
};

template<typename T,typename Deleter>
struct get_deleter_pointer_type<T,Deleter,std::void_t<typename Deleter::pointer>>
{
    using type = typename  Deleter::pointer;
};


template<typename T,typename Deleter = default_delete<T>>
class unique_ptr
{
 
public:
    using pointer = typename get_deleter_pointer_type<T,Deleter>::type;
    using element_type = T;
    using deleter_type = Deleter;

    constexpr unique_ptr()  :m_pair(nullptr){}
    constexpr unique_ptr(std::nullptr_t )  :m_pair(nullptr){}
    unique_ptr operator=(std::nullptr_t)
    {
        reset();
        return *this;
    }

    explicit unique_ptr(pointer p)  :m_pair(p){}

   
    unique_ptr(pointer p,typename std::conditional_t<std::is_reference<deleter_type>::value, deleter_type, const deleter_type& > d):m_pair(p,d) {}

    unique_ptr(pointer p, typename std::remove_reference_t<deleter_type>&& d):m_pair(std::move(p),std::move(d)){}

    unique_ptr( unique_ptr&& u )  :m_pair(u.release(), std::forward<Deleter>( u.get_deleter() )) {}

    unique_ptr& operator=( unique_ptr&& u ) 
    {
        reset(u.release());
        get_deleter() = std::forward<deleter_type>(u.get_deleter());
        return *this;
    }

    template<typename U, typename E >
    unique_ptr( unique_ptr<U, E>&& u )  :m_pair(u.release(), std::forward<E>( u.get_deleter() )) {}
    
    template< class U, class E >
    unique_ptr& operator=( unique_ptr<U,E>&& u) 
    {
        reset(u.release());
        get_deleter() = std::forward<deleter_type>(u.get_deleter());
        return *this;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;


    ~unique_ptr()
    {
        if(get() != nullptr)
        {
            get_deleter()(get());
            std::cout<<"~delete ptr"<<std::endl;
        }
    }

    void reset(pointer ptr = nullptr)
    {
        auto old_ptr = m_pair.first();
        using std::swap;
        swap(m_pair.first(), ptr);
    
        if(old_ptr != nullptr)
        {
            get_deleter()(old_ptr);
        }
    }

    void reset(std::nullptr_t p) 
    {
        reset();
    }

    pointer release() 
    {
        pointer p = get();
        m_pair.first() = nullptr;
        return p;
    }

    void swap(unique_ptr other)
    {
        using std::swap;
        swap(m_pair,other);
    }

    pointer get() const
    {
        return m_pair.first();
    }

    deleter_type& get_deleter()
    {
        return m_pair.second();
    }

    const deleter_type& get_deleter() const
    {
        return m_pair.second();
    }

    explicit operator bool() const
    {
        return get() != nullptr;
    }

    typename std::add_lvalue_reference<T>::type operator*() const
    {
        return *get();
    }

    pointer operator->() const
    {
        return get();
    }
private:
    compressed_pair<pointer,deleter_type> m_pair;
};

template<typename T,typename U>
inline void swap(unique_ptr<T,U>& x,unique_ptr<T,U>& y)
{
    x.swap(y);
}

template<typename T,typename U>
inline bool operator==(const unique_ptr<T,U>& x,const unique_ptr<T,U>& y)
{
    return (x.get() == y.get());
}

template<typename T,typename U>
inline bool operator==(const unique_ptr<T,U>& x,std::nullptr_t )
{
    return !x;
}

template<typename T,typename U>
inline bool operator==(std::nullptr_t,const unique_ptr<T,U>& y)
{
    return !y;
}


template<typename T,typename U>
inline bool operator!=(const unique_ptr<T,U>& x,const unique_ptr<T,U>& y)
{
    return (x.get() != y.get());
}

template<typename T,typename U>
inline bool operator!=(const unique_ptr<T,U>& x,std::nullptr_t )
{
    return (bool)x;
}

template<typename T,typename U>
inline bool operator!=(std::nullptr_t,const unique_ptr<T,U>& y)
{
    return (bool)y;
}


}// namespace ez

#endif //EASY_UNIQUE_PTR_H