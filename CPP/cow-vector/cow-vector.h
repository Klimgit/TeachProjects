#pragma once

#include <string>
#include <vector>

struct State {
    int ref_count_;
    std::vector<std::string> data_;
};

class COWVector {
public:
    COWVector() : state_(new State()) {
        state_->ref_count_ = 1;
    };
    ~COWVector() {
        Remove();
    }

    COWVector(const COWVector& other) : state_(other.state_) {
        ++(state_->ref_count_);
    }

    COWVector& operator=(const COWVector& other) {
        if (this != &other) {
            Remove();
            state_ = other.state_;
            ++state_->ref_count_;
        }
        return *this;
    }

    // Rule of 5?

    size_t Size() const {
        return state_->data_.size();
    }

    void Resize(size_t size) {
        IsUnique();
        state_->data_.resize(size);
    }

    const std::string& Get(size_t at) {
        return state_->data_[at];
    }
    const std::string& Back() {
        return state_->data_.back();
    }

    void PushBack(const std::string& value) {
        IsUnique();
        state_->data_.push_back(value);
    }

    void Set(size_t at, const std::string& value) {
        IsUnique();
        state_->data_[at] = value;
    }

private:
    State* state_;

    void IsUnique() {
        if (state_->ref_count_ > 1) {
            --state_->ref_count_;
            state_ = new State(*state_);
            state_->ref_count_ = 1;
        }
    }

    void Remove() const {
        if (--state_->ref_count_ == 0) {
            delete state_;
        }
    }
};
