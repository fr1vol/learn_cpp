#ifndef EASY_SAFE_QUEUE_HPP
#define EASY_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace ez
{

    template<typename T>
    class safe_queue
    {

    public:
        bool wait_for(int time_ms = 0){
            
            std::unique_lock<std::mutex> lock(m_mtx);
            if(m_queue.empty()){
                m_cv.wait_for(lock,std::chrono::milliseconds(time_ms),[&](){return !m_queue.empty();});
            }
            return !m_queue.empty();
        }

        std::pair<T,bool> wait_pop(int time_ms = 0){
            std::unique_lock<std::mutex> lock(m_mtx);
            if(m_queue.empty()){
                m_cv.wait_for(lock,std::chrono::milliseconds(time_ms),[&](){return !m_queue.empty();});
            }
            return inner_pop();
        }
        
        std::pair<T,bool> pop(){
            std::lock_guard<std::mutex> lock(m_mtx);
            return inner_pop();
        }

        void push(T&& t)
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_queue.emplace_back(std::forward<T>(t));
            m_cv.notify_one();
        }

        void push(const T& t){
            std::lock_guard<std::mutex> lock(m_mtx);
            m_queue.emplace_back(t);
            m_cv.notify_one();
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(m_mtx);
            return m_queue.size();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(m_mtx);
            return m_queue.empty();
        }

        void notify()
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_cv.notify_one();
        }
    private:
        std::pair<T,bool> inner_pop()
        {
            if(m_queue.empty()){
                return {T{},false}; 
            }else{
                auto r = m_queue.front();
                m_queue.pop_front();
                return {r,true};
            }
        }

    private:
        mutable std::mutex m_mtx;
        std::deque<T> m_queue;
        std::condition_variable m_cv;
    };

};//namespace ez

#endif// EASY_SAFE_QUEUE_HPP
