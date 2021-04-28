#pragma once

#include <cstddef> //std::nullptr_t
#include <tuple>
#include <iostream>
namespace ez
{

    template<typename T>
    struct default_delete
    {
        void operator()(T* ptr) const {
            delete ptr;
        }
    };

    struct control_block
    {
        virtual void increase_weak() = 0;
        virtual void decrease_weak() = 0;
        virtual void increase_owner() = 0;
        virtual void decrease_owner() = 0;
        virtual void destroy() =0;
        virtual void destroy_this() = 0;
        virtual int weak_count() = 0;
        virtual int owner_count() = 0;
        virtual ~control_block(){}
    };

    template<typename T,typename Delete = default_delete<T>>
    struct control_block_impl : control_block
    {
        using pointer =  T*;
        
        explicit control_block_impl():ptr(nullptr,Delete()),weak(1),owner(1){}
        explicit control_block_impl(std::nullptr_t ):control_block_impl(){}        
        control_block_impl(T* p, Delete&& d =Delete{}):ptr(p,d),weak(1),owner(1){}

        void increase_weak(){
            ++weak;
        }

        void increase_owner(){
            ++owner;
        }

        void decrease_weak(){
            --weak;
            if(weak == 0){
                destroy_this();
            }
        }

        void decrease_owner(){
            --owner;
            if(owner == 0){
                destroy();
                decrease_weak();
            }
        }

        void destroy(){
            pointer old_ptr = get();
            if(old_ptr != nullptr){
                get_del()(old_ptr);
                std::get<0>(ptr) = nullptr;
            }
        }

        void destroy_this(){
            delete  this;
        }

        pointer get() const {
            return std::get<0>(ptr);
        }
        Delete get_del(){
            return std::get<1>(ptr);
        }
        int weak_count(){
            return weak;
        }
        
        int owner_count(){
            return owner;
        }  

        std::tuple<T*,Delete> ptr;
        int weak;
        int owner;

    };


    template<typename T>
    class shared_ptr
    {
    public:
        using pointer = T*;
        //construct 
        shared_ptr():c_ptr(nullptr),m_ptr(nullptr){}
        explicit shared_ptr(std::nullptr_t):shared_ptr(){}
    
        template<typename T2,typename D = default_delete<T2>>
        explicit shared_ptr(T2* ptr,D d = D{}):c_ptr(new control_block_impl<T2>(ptr)),m_ptr(static_cast<pointer>(ptr)){}

        shared_ptr(const shared_ptr& other){
            this->copy_construct(other);
        }

        shared_ptr& operator=(const shared_ptr& other){
            shared_ptr(other).swap(*this);
            return *this;
        }


        template<typename T2>
        shared_ptr(const shared_ptr<T2>& other){
            this->copy_construct(other);
        }

        template<typename T2>
        shared_ptr& operator=(const shared_ptr<T2>& other){
            shared_ptr(other).swap(*this);
            return *this;
        }

        template<typename T2>
        shared_ptr& operator=(shared_ptr<T2>&& other){
            shared_ptr(std::move(other)).swap(*this);
            return *this;
        }

        template<typename T2>
        shared_ptr(shared_ptr<T2>&& other){
            this->move_construct(std::move(other));
        }

        T& operator*() const{
            return *m_ptr;
        }

        pointer operator->() const {
            return m_ptr;
        }

        void swap(shared_ptr& other)
        {
            using std::swap;
            swap(c_ptr,other.c_ptr);
            swap(m_ptr,other.m_ptr);
        }

        void reset(std::nullptr_t p =nullptr){
            shared_ptr<T>(nullptr).swap(*this); 
        }

        void reset(pointer ptr){
            shared_ptr<T>(ptr).swap(*this); 
        }

        int use_count(){
            int count = 0;
            if(c_ptr != nullptr){
                count = c_ptr->owner_count();
            }
            return count;
        }    

        pointer get() const {
            return m_ptr;
        }

        void increase_owner() const {
            if(c_ptr != nullptr){
                c_ptr->increase_owner();
            }
        }

        void decrease_owner(){
            if(c_ptr != nullptr){
                c_ptr->decrease_owner();
            }
        }
        
        template<typename T2>
        void move_construct(shared_ptr<T2>&& other){
            c_ptr = other.c_ptr;
            m_ptr = static_cast<T*>(other.m_ptr);
            other.c_ptr = nullptr;
            other.m_ptr = nullptr;
        }

        template<typename T2>
        void copy_construct(const shared_ptr<T2>& other){
            other.increase_owner();
            c_ptr = other.c_ptr;
            m_ptr = static_cast<T*>(other.m_ptr);
        }
        ~shared_ptr(){
            this->decrease_owner();
        }
    private:
        control_block* c_ptr;
        pointer m_ptr;  

        template<typename T2>
        friend class shared_ptr;
       
    };


};//namespace ez