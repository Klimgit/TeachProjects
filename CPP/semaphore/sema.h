#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    explicit Semaphore(const int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock lock(mutex_);
        ++count_;

        if (!queue_.empty()) {
            queue_.front()->notify_one();
        }
    }

    template <class Func>
    void Enter(Func callback) {
        std::condition_variable cv;
        std::unique_lock lock(mutex_);

        queue_.push(&cv);
        while (true) {
            if (count_ > 0 && &cv == queue_.front()) {
                callback(count_);
                queue_.pop();
                return;
            }

            cv.wait(lock);
        }
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::queue<std::condition_variable*> queue_;
    int count_ = 0;
};
