#pragma once

#include <queue>
#include <optional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(const int size) : capacity_(size), is_closed_(false) {
    }

    void Send(const T& value) {
        std::unique_lock lock(mutex_);
        cv_not_full_.wait(lock, [this]() { return is_closed_ || queue_.size() < capacity_; });

        if (is_closed_) {
            throw std::runtime_error("Channel is close");
        }

        queue_.push(value);
        cv_not_empty_.notify_one();
    }

    std::optional<T> Recv() {
        std::unique_lock lock(mutex_);

        cv_not_empty_.wait(lock, [this]() { return !queue_.empty() || is_closed_; });

        if (queue_.empty() && is_closed_) {
            return std::nullopt;
        }

        T value = std::move(queue_.front());
        queue_.pop();

        cv_not_full_.notify_one();
        return value;
    }

    void Close() {
        std::unique_lock lock(mutex_);
        is_closed_ = true;
        cv_not_full_.notify_all();
        cv_not_empty_.notify_all();
    }

private:
    const int capacity_;
    std::queue<T> queue_;
    std::mutex mutex_;

    std::condition_variable cv_not_full_;
    std::condition_variable cv_not_empty_;
    bool is_closed_;
};
