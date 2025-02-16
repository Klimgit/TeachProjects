#pragma once

#include <atomic>
#include <thread>

struct RWSpinLock {
    void LockRead() {
        while (true) {
            uint32_t current_state = state_;
            if (current_state & kWriteFlag) {
                std::this_thread::yield();
            } else {
                if (state_.compare_exchange_weak(current_state, current_state + kReaderCounter)) {
                    break;
                }
            }
        }
    }

    void UnlockRead() {
        state_.fetch_sub(kReaderCounter, std::memory_order_release);
    }

    void LockWrite() {
        uint32_t expected = 0;
        while (!state_.compare_exchange_weak(expected, kWriteFlag)) {
            expected = 0;
            std::this_thread::yield();
        }
    }

    void UnlockWrite() {
        state_ = 0;
    }

private:
    static constexpr uint32_t kWriteFlag = 1;
    static constexpr uint32_t kReaderCounter = 2;

    std::atomic<uint32_t> state_{0};
};
