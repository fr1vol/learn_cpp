#ifndef EZ_SHARED_PTR_HPP
#define EZ_SHARED_PTR_HPP

#include <atomic>
#include <utility>
#include <memory>
#include <type_traits>
#include "compressed_pair.hpp"

namespace ez
{

class ref_count_base
{
public:
    ref_count_base() =default;

    virtual void destory() = 0;
    virtual void delete_this() = 0;

    virtual ~ref_count_base() {}

    bool incref_nz()
    {
        int count = uses;
        while(count != 0){
            const int old_value = uses.exchange(count+1);
            if(old_value == count){
                return true;
            }
            count = old_value;
        }
        return false;
    }

    void incref(){
        ++uses;
    }

    void incwref(){
        ++weaks;
    }

    void decref(){
        --uses;
        if(uses == 0){
            destory();
            decwref();
        }
    }

    void decwref(){
        --weaks;
        if(weaks == 0){
            delete_this();
        }
    }

    long use_count() const{
        return uses;
    }

private:
    std::atomic_int uses{1};
    std::atomic_int weaks{1};  
};



template<typename T>
class ref_count: public ref_count_base 
{
public:
    explicit ref_count(T* p) : ref_count_base(),ptr(p){}
private:
    virtual void destory() override{
        delete ptr;
    }

    virtual void delete_this() override{
        delete this;
    }
    T* ptr;
};

template<typename Resource, typename Deleter>
class ref_count_resource: public ref_count_base 
{
public:
    explicit ref_count_resource(Resource p,Deleter d) : ref_count_base(),ptr(p,std::move(d)){}
private:
    virtual void destory() override{
        ptr.second()(ptr.first());
    }

    virtual void delete_this() override{
        delete this;
    }
    ez::compressed_pair<Resource,Deleter> ptr;
};


template<typename Alloc>
void deallocate_pain(Alloc& a,typename Alloc::value_type*  const ptr)
{
    using Alloc_traits = std::allocator_traits<Alloc>;
    if constexpr (std::is_same<typename Alloc_traits::pointer, typename Alloc::value_type* >::value){
        Alloc_traits::deallocate(a,ptr,1);
    }else{
        using Ptr_traits = std::pointer_traits<typename Alloc_traits::pointer>;
        Alloc_traits::deallocate(a,Ptr_traits::pointer_to(*ptr),1);
    }
}


template<typename Resource,typename Deleter, typename Alloc>
class ref_count_resource_alloc : public ref_count_base
{
    using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<ref_count_resource_alloc> ;
    explicit ref_count_resource_alloc(Resource p,Deleter d, Alloc a):ref_count_base(),pair(ez::compressed_pair<Resource,alloc_type>(p,std::move(a)),std::move(d)) {}
public:

private:
    virtual void destory() override {
        pair.second()(pair.first().first());
    }

    virtual void delete_this() override {
        auto a = pair.first().second();
        this->~ref_count_resource_alloc();
        deallocate_pain(a,this);
    }

    ez::compressed_pair<ez::compressed_pair<Resource,alloc_type>,Deleter> pair;

};


template <class _Ty>
class shared_ptr;

template <class _Ty>
class weak_ptr;


template<typename T>
class Ptr_base{
public:
    using element_type = T;
    
    long use_count() const{
        return rep ? rep->use_count() : 0;
    }

    template<typename U>
    bool owner_before(const Ptr_base<U>& right) const {
        return rep < right.rep;
    }

    Ptr_base(const Ptr_base&) =delete;
    Ptr_base& operator==(const Ptr_base& ) =delete;

    Ptr_base() = default;
    ~Ptr_base() =default;

    template<typename U>
    void move_construct_from(Ptr_base<U>&& right){
        ptr = right.ptr;
        rep = right.rep;

        right.ptr = nullptr;
        right.rep = nullptr;
    }

    template<typename U>
    void copy_construct_from(const Ptr_base<U>& other){
        other.incref();

        ptr = other.ptr;
        rep = other.rep;
    }

    template<typename U>
    void alias_construct_from(const shared_ptr<U>& other,element_type* p){
        other.incref();

        ptr = p;
        rep = other.rep;
    }

    template<typename U>
    void alias_move_construct_from(shared_ptr<U>&& other,element_type* p){

        ptr = p;
        rep = other.rep;

        other.ptr = nullptr;
        other.rep = nullptr;
    }

    template <class _Ty0>
    friend class weak_ptr;

    template<typename U>
    bool construct_from_weak(const weak_ptr<U> other) {
        if(other.rep && other.rep->incref_nz()){
            ptr = other.ptr;
            rep = other.rep;

            return true;
        }

        return false;
    }

    void incref() const{
        if(rep){
            rep->incref();
        }
    }

    void decref(){
        if(rep){
            rep->decref();
        }
    }

    void swap_ptr_base(Ptr_base& right) {
        using std::swap;
        swap(ptr,right.ptr);
        swap(rep,right.rep);
    }

    template<typename U>
    void weakly_construct_from(const Ptr_base<U>& other){
        if(other.rep){
            ptr = other.ptr;
            rep = other.ptr;
            rep->incwref();
        }
    }

    template<typename U>
    void weakly_convert_lvalue_avoiding_exprired_conversions(const Ptr_base<U>& other){
        if(other.rep){
            rep = other.rep;
            rep->incwref();

            if(rep->incref_nz()){   
                ptr = other.ptr;
                rep->decref();
            }
        }
    }

    template<typename U>
    void weakly_convert_rvalue_avoiding_expored_conversions(Ptr_base<U>&& other){
        rep = other.rep;
        other.rep = nullptr;

        if(rep && rep->incref_nz()){
            ptr = other.ptr;
            rep->decref();
        }

        other.ptr = nullptr;
    }

    void incwref(){
        if(rep){
            rep->incwref();
        }
    }

    void decwref(){
        if(rep){
            rep->decwref();
        }
    }

    element_type* get() const{
        return ptr;
    }
private:

    element_type* ptr{nullptr};
    ref_count_base* rep{nullptr};


    template<typename U>
    friend class Ptr_base;

    friend shared_ptr<T>;
};


template<typename T>
class shared_ptr : Ptr_base<T>
{
private:
    using base = Ptr_base<T>;

public:
    using element_type = typename base::element_type;
    using weak_type = weak_ptr<T>;
    
    //1
    constexpr shared_ptr()  = default;
    //2
    constexpr shared_ptr(std::nullptr_t)  {} 

    //3
    template<typename U>
    explicit shared_ptr( U* ptr )
    {
        std::unique_ptr<U> tmp_owner(ptr);
        set_ptr_rep_and_enable_shared(tmp_owner.get(),new ref_count<U>(tmp_owner.get()));
        tmp_owner.release();
    }

    //4
    template<typename U,typename Deleter >
    shared_ptr( U* ptr, Deleter d )
    {
        std::unique_ptr<U> tmp_owner(ptr);
        set_ptr_rep_and_enable_shared(tmp_owner.get(),new ref_count_resource<U*,Deleter>(tmp_owner.get(),std::move(d)));
        tmp_owner.release();
    }

    //5
    template<typename U,typename Deleter >
    shared_ptr(std::nullptr_t p, Deleter d )
    {
        set_ptr_rep_and_enable_shared(nullptr,new ref_count_resource<U*,Deleter>(nullptr,std::move(d)));
    }

    //6
    template<typename U,typename Deleter,typename Alloc >
    shared_ptr( U* p, Deleter d, Alloc alloc )
    {
        using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<ref_count_resource_alloc<U*,Deleter,Alloc>>;


        std::unique_ptr<U,Deleter> tmp_owner(p,d);
        
        alloc_type a(alloc);

        auto l = [a](typename alloc_type::pointer ptr){ if(ptr){a.deallocate(ptr,1);}};
        std::unique_ptr<ref_count_resource_alloc<U*,Deleter,Alloc>,decltype(l)> tmp_ref(a.allocate(1),l);

        construct_in_place(tmp_ref,p,d,std::move(alloc));
       
        set_ptr_rep_and_enable_shared(tmp_owner.get(),tmp_ref.get());

        tmp_owner.release();
        tmp_ref.release();
    }

    //7
    template<typename Deleter, typename Alloc >
    shared_ptr( std::nullptr_t , Deleter d, Alloc alloc )
    {
        using element_type = typename std::allocator_traits<Alloc>::value_type;
        using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<ref_count_resource_alloc<element_type*,Deleter,Alloc>>;

        alloc_type a(alloc);

        auto l = [a](typename alloc_type::pointer ptr){ if(ptr){a.deallocate(ptr,1);}};
        std::unique_ptr<ref_count_resource_alloc<element_type*,Deleter,Alloc>,decltype(l)> tmp_ref(a.allocate(1),l);

        construct_in_place(tmp_ref,nullptr,d,std::move(alloc));
       
        set_ptr_rep_and_enable_shared(nullptr,tmp_ref.get());

        tmp_ref.release();
    }

    //8
    template<class U>
    shared_ptr( const shared_ptr<U>& r, element_type* ptr )
    {
        this->alias_construct_from(r,ptr);
    }

    template<typename U>
    shared_ptr( shared_ptr<U>&& r, element_type* ptr )
    {
        this->alias_move_construct_from(std::move(r),ptr);
    }

    //9
    shared_ptr( const shared_ptr& r )
    {
        this->copy_construct_from(r);
    }

    template<typename U>
    shared_ptr( const shared_ptr<U>& r )
    {
        this->copy_construct_from(r);
    }

    //10
    shared_ptr( shared_ptr&& r )
    {
        this->move_construct_from(r);
    }

    template<typename U>
    shared_ptr(shared_ptr<U>&& r )
    {
        this->move_construct_from(r);
    }

    //11
    template<typename U>
    explicit shared_ptr( const std::weak_ptr<U>& r )
    {
        if(!this->construct_from_weak(r)) {
            std::cout<<"construct_from_weak"<<std::endl;
        }
    }

    //12
    template<typename U, typename Deleter >
    shared_ptr( std::unique_ptr<U,Deleter>&& r )
    {
        using pointer_type = typename std::unique_ptr<U,Deleter>::pointer;
        using element_type = typename std::unique_ptr<U,Deleter>::element_type;
        using deleter_type =  typename std::conditional<std::is_reference<Deleter>::value,decltype( std::ref(r.get_deleter())),Deleter >::type;

        const pointer_type  ptr = r.get();

        if(ptr){
            const element_type * raw_ptr = ptr;
            const auto rep = new ref_count_resource<pointer_type,deleter_type>(ptr,r.get_deleter());
            set_ptr_rep_and_enable_shared(raw_ptr,rep);
            r.release();
        }
    }

    ~shared_ptr()
    {
        this->decref();
    }

    shared_ptr& operator=(const shared_ptr& r)
    {
        shared_ptr(r).swap(*this);
        return *this;
    }

    template<typename U>
    shared_ptr& operator=(const shared_ptr<U>& r)
    {
        shared_ptr(r).swap(*this);
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& r)
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }

    template<typename U>
    shared_ptr& operator=(shared_ptr<U>&& r)
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }

    template<typename U,typename Deleter>
    shared_ptr& operator=(std::unique_ptr<U,Deleter>&& r)
    {
        shared_ptr(std::move(r)).swap(*this);
        return *this;
    }


    void swap(shared_ptr& other)
    {
        this->swap_ptr_base(other);
    }

    void reset()
    {
        shared_ptr().swap(*this);
    }

    template<typename U>
    void reset(U* p)
    {
        shared_ptr(p).swap(*this);
    }

    template <typename U, typename Deleter>
    void reset(U* p, Deleter d) { 
        shared_ptr(p, d).swap(*this);
    }

    template <typename U, typename Deleter, typename Alloc>
    void reset(U* p, Deleter d, Alloc alloc) { 
        shared_ptr(p, d, alloc).swap(*this);
    }

    using base::get;

    template<typename U= T>
    U& operator*() const {
        return *get();
    }

    template<typename U= T>
    U* operator->() const {
        return get();
    }

    explicit operator bool() const {
        return get() != nullptr;
    }

    bool unique() const {
        return this->use_count() == 1;
    }


private:
    template<typename U>
    void set_ptr_rep_and_enable_shared(U* const p, ref_count_base* const r){
        this->ptr = p;
        this->rep = r;

        // if(p && p->wptr.expired()){
        //     p->wptr = shared_ptr<std::remove_cv<U>::type>>(*this,const_cast<remove_cv<U>::type*>(p));
        // }
    }

    void set_ptr_rep_and_enable_shared(std::nullptr_t,ref_count_base* const r){
        this->ptr = nullptr;
        this->rep = r
;    }
};

    
template<typename T,typename U>
bool operator==(const shared_ptr<T>& left, const shared_ptr<U>& right)
{
    return left.get() == right.get();
}

template <typename T>
void swap(shared_ptr<T>& left, shared_ptr<T>& right) noexcept {
   left.swap(right);
}

template<typename T,typename... Ts>
void construct_in_place(T& obj,Ts&&... args)
{       
    ::new (const_cast<void*>(static_cast<const volatile void*>(std::addressof(obj)))) T(std::forward<Ts>(args)...);
}


template<typename T>
class weak_ptr : Ptr_base<T>
{
    weak_ptr(){};
    weak_ptr(const weak_ptr& other) {
        this->weakly_construct_from(other);
    }

    template<typename U>
    weak_ptr(const shared_ptr<U>& other){
        this->weakly_construct_from(other);
    }

    weak_ptr(weak_ptr&& other){
        this->weakly_convert_lvalue_avoiding_expired_conversions(std::move(other));
    }

    template<typename U>
    weak_ptr(weak_ptr<U>&& other){
        this->weakly_convert_lvalue_avoiding_expired_conversions(std::move(other));
    }

    ~weak_ptr(){
        this->decwref();
    }

    weak_ptr& operator=(const weak_ptr& other){
        weak_ptr(other).swap(*this);
        return *this;
    }

    template<typename U>
    weak_ptr& operator=(const weak_ptr<U>& other){
        weak_ptr(other).swap(*this);
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other){
        weak_ptr(std::move(other)).swap(*this);
        return *this;
    }

    template<typename U>
    weak_ptr& operator=(weak_ptr<U>&& other){
        weak_ptr(std::move(other)).swap(*this);
        return *this;
    }

    template<typename U>
    weak_ptr& operator=(const shared_ptr<U>& other){
        weak_ptr(other).swap(*this);
        return *this;
    }

    void reset()
    {
        weak_ptr{}.swap(*this);
    }

    void swap(weak_ptr& other) {
        this->swap_ptr_base(other);
    }
    bool expired() const {
        return this->use_count() == 0;
    }

    shared_ptr<T> lock()  const {
        shared_ptr<T> tmp;
        tmp.construct_from_weak(*this);
        return tmp;
    }
};

template <class T>
void swap(weak_ptr<T>& left, weak_ptr<T>& right) noexcept {
    left.swap(right);
}

template <typename T>
class enable_shared_from_this{

public:

    enable_shared_from_this() :ptr(){}
    ~enable_shared_from_this() =default;

    shared_ptr<T> shared_from_this() {
        shared_ptr<T> tmp(ptr);
        return tmp;
    }

    shared_ptr<const T> shared_from_this() const {
        shared_ptr<const T> tmp(ptr);
        return tmp;
    }

    weak_ptr<T> weak_from_this()  {
        return ptr;
    }

    weak_ptr<const T> weak_from_this() const  {
        return ptr;
    }

private:

    template<typename U>
    friend class shared_ptr;

    mutable weak_ptr<T> ptr;
};


}//namespace ez
#endif