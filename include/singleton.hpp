#ifndef EASY_SINGLETON_HPP
#define EASY_SINGLETON_HPP

namespace ez{
    class Singleton
    {
    private:
        Singleton(){};
        ~Singleton(){};
        Singleton(const Singleton&) = delete;
        Singleton& operator= (const Singleton&) = delete;
    public:
        static Singleton& get_instance(){
            static Singleton instance;
            return instance;
        }
    };
}

#endif