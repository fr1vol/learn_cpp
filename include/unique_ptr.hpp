#ifndef EZ_UNIQUE_PTR_HPP
#define EZ_UNIQUE_PTR_HPP

#include <type_traits>
#include <tuple>
namespace ez
{

    template<typename T>
    struct default_delete
    {
        void operator()(T* ptr) const {
            delete ptr;
            ptr = nullptr;
        }
    };

    template<typename T, typename Delete = default_delete<T>>
    class unique_ptr
    {
    public:
        using pointer = T*;
        using element = T;
        using delete_type = Delete;

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr():unique_ptr(nullptr){}
        unique_ptr(std::nullptr_t ):m_ptr(std::make_tuple<T*,Delete>(nullptr,Delete{})){}
        unique_ptr(pointer p,Delete&& d = Delete{}): m_ptr(p,d){}
        unique_ptr(unique_ptr&& other){
            swap(m_ptr,other.m_ptr);
            other.reset(nullptr);
        }

        unique_ptr& operator=( unique_ptr&& r ) {
            reset(r.release());
            get_deleter() = r.get_deleter();
        }

        unique_ptr& operator=( std::nullptr_t) {
            reset();
        }

        T& operator*() const{
            return *get();
        }
        
        pointer operator->() const {
            return get();
        }
        

        explicit operator bool() const{
            return get() != nullptr;
        }

        ~unique_ptr(){
            reset(nullptr);
        }

        pointer get() const {
            return std::get<0>(m_ptr);
        }

        Delete& get_deleter()  {
            return std::get<1>(m_ptr);
        }

        void reset(pointer p = nullptr){
            pointer old_ptr = get();
            std::get<0>(m_ptr) = p;
            if(old_ptr != nullptr) {
                get_deleter()(old_ptr);
            }
        }

        pointer release(){
            auto old_ptr = get();
            std::get<0>(m_ptr) = nullptr;
            return old_ptr;
        }
    private:
        std::tuple<T*,Delete> m_ptr;
    };


};//namespace ez

#endif // EZ_UNIQUE_PTR_HPP