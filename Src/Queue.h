#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;

public:
 
    T pop() 
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        while(_queue.empty())
            _cond.wait(mlock);

        T val = _queue.front();
        _queue.pop();
        return val;
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(_mutex);
        _queue.push(item);
        mlock.unlock();
        _cond.notify_one();
    }

    Queue()=default;
    Queue(const Queue&) = delete;            // disable copying
    Queue& operator=(const Queue&) = delete; // disable assignment
};
