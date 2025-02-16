#pragma once

#include <typeinfo>
#include <utility>

struct Base {
    virtual ~Base() {};
    virtual Base* Clone() const = 0;
};

template <typename T>
struct Holder final : Base {
    T value;

    explicit Holder(const T& val) : value(val) {
    }

    Base* Clone() const override {
        return new Holder(value);
    }
};

class Any {
public:
    Any() : data_(nullptr) {
    }

    template <typename T>
    Any(const T& value) : data_(new Holder<T>(value)) {
    }

    Any(const Any& rhs) : data_(rhs.data_ ? rhs.data_->Clone() : nullptr) {
    }

    Any& operator=(Any rhs) {
        Swap(rhs);
        return *this;
    }

    ~Any() {
        Clear();
    }

    bool Empty() const {
        return data_ == nullptr;
    }

    void Clear() {
        delete data_;
        data_ = nullptr;
    }

    void Swap(Any& rhs) {
        std::swap(data_, rhs.data_);
    }

    template <typename T>
    const T& GetValue() const {
        if (!data_) {
            throw;
        }

        Holder<T>* holder = dynamic_cast<Holder<T>*>(data_);
        if (!holder) {
            throw std::bad_cast();
        }

        return holder->value;
    }

private:
    Base* data_;
};
