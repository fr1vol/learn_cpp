#ifndef EASY_SAFE_QUEUE_H
#define EASY_SAFE_QUEUE_H
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <deque>

namespace ez
{
    template<typename T>
    class mt_queue
    {
        mutable std::mutex m_mutex;
        std::condition_variable m_cond;
        std::deque<T > m_items;
    public:
        void push(const T&  v)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_items.emplace_back(v);
            m_cond.notify_one();
        }
        
        void push(T&&  v)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_items.emplace_back( std::forward<T>(v) );
            m_cond.notify_one();
        }
        void push_front(T&  v)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_items.emplace_front(std::forward<T>(v));
            m_cond.notify_one();
        }
        void push_front(T&&  v)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_items.emplace_front(std::forward<T>(v));
            m_cond.notify_one();
        }
        bool wait_for(int timeout_ms = 0)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if(m_items.empty())
                m_cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [&]{return !m_items.empty();});
            return !m_items.empty();
        }
        
        std::pair<T, bool> wait_pop(int timeout_ms = 0)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if(m_items.empty())
                m_cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [&]{return !m_items.empty();});
            return pop_();
        }
        
        std::pair<T, bool> pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return pop_();
        }
        
        bool empty() const
        {
           std::lock_guard<std::mutex> lock(m_mutex);
            return m_items.empty();
        }
        
        size_t size() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_items.size();
        }
        void notify()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_cond.notify_one();
        }
    private:
        std::pair<T, bool> pop_()
        {
            if (!m_items.empty())
            {
                auto r = m_items.front();
                m_items.pop_front();
                return {r, true};
            }
            else {
                return {T(), false};
            }
        }
    };

    

}
#endif
