#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

    void Add(const T& item, TimePoint at) {
        std::unique_lock lock(mutex_);
        queue_.emplace(at, item);
        cv_.notify_all();
    }

    T Pop() {
        std::unique_lock lock(mutex_);

        while (true) {
            if (!queue_.empty()) {
                std::chrono::time_point<std::chrono::system_clock> now = Clock::now();
                auto it = queue_.begin();

                if (it->first <= now) {
                    T result = it->second;
                    queue_.erase(it);
                    return result;
                }

                cv_.wait_until(lock, it->first);
            } else {
                cv_.wait(lock);
            }
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::multimap<TimePoint, T> queue_;
};
