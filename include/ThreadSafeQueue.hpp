#pragma once

#include <algorithm>
#include <mutex>
#include <queue>


template <typename T>
class ThreadSafeQueue {
    private:
    mutable std::mutex mtx_;
    std::queue<T> queue_;

    public:
    void push(T value){
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(std::move(value));
    }

    bool try_pop(T& value){
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }
};

