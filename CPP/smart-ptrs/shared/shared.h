#pragma once

#include "sw_fwd.h"  // Forward declaration

#include <cstddef>  // std::nullptr_t

// https://en.cppreference.com/w/cpp/memory/shared_ptr

struct BaseControl {
    virtual size_t Count() = 0;
    virtual void Inc() = 0;
    virtual void Dec() = 0;
    virtual ~BaseControl() = default;
};

template <typename U>
struct ControlBlock : BaseControl {
    template <typename... Args>
    explicit ControlBlock(Args&&... args) : block(std::forward<Args>(args)...) {
        shared_counter = 1;
    }

    size_t Count() override {
        return shared_counter;
    }

    void Inc() override {
        ++shared_counter;
    }

    void Dec() override {
        --shared_counter;
    }

    U* GetPtr() {
        return &block;
    }

    size_t shared_counter = 0;
    U block;
};

template <typename U>
struct ControlBlockPtr : BaseControl {
    explicit ControlBlockPtr(U* pointer) {
        ptr = pointer;
        counter = 1;
    }

    size_t Count() override {
        return counter;
    }

    void Inc() override {
        ++counter;
    }

    void Dec() override {
        --counter;
    }

    ~ControlBlockPtr() override {
        delete ptr;
        ptr = nullptr;
    }

    size_t counter;
    U* ptr;
};

template <typename T>
class SharedPtr {
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors

    SharedPtr() : block_(nullptr), ptr_(nullptr) {
    }

    SharedPtr(std::nullptr_t) : block_(nullptr), ptr_(nullptr) {
    }

    SharedPtr(T* ptr) : block_(new ControlBlockPtr<T>(ptr)), ptr_(ptr) {
    }

    template <typename S>
    SharedPtr(S* ptr) : block_(new ControlBlockPtr<S>(ptr)), ptr_(ptr) {
    }

    SharedPtr(const SharedPtr& other) : block_(other.block_), ptr_(other.ptr_) {
        if (block_) {
            block_->Inc();
        }
    }

    template <typename V>
    SharedPtr(const SharedPtr<V>& other) : block_(other.block_), ptr_(other.ptr_) {
        if (block_) {
            block_->Inc();
        }
    }

    template <typename O>
    SharedPtr(SharedPtr<O>&& other) : block_(other.block_), ptr_(other.ptr_) {
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }

    SharedPtr(SharedPtr&& other) : block_(other.block_), ptr_(other.ptr_) {
        other.block_ = nullptr;
        other.ptr_ = nullptr;
    }

    // Aliasing constructor
    // #8 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    template <typename S>
    SharedPtr(const SharedPtr<S>& other, T* ptr) {
        ptr_ = ptr;
        block_ = other.block_;
        if (block_) {
            block_->Inc();
        }
    }

    // Promote `WeakPtr`
    // #11 from https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
    explicit SharedPtr(const WeakPtr<T>& other) : block_(nullptr), ptr_(nullptr) {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // `operator=`-s

    SharedPtr& operator=(const SharedPtr& other) {
        if (other.block_) {
            if (block_ != other.block_) {
                Reset();
                ptr_ = other.ptr_;
                block_ = other.block_;
                block_->Inc();
            }
        } else {
            Reset();
        }
        return *this;
    }

    template <typename V>
    SharedPtr& operator=(const SharedPtr<V>& other) {
        if (other.block_) {
            if (block_ != other.block_) {
                Reset();
                ptr_ = other.ptr_;
                block_ = other.block_;
                block_->Inc();
            }
        } else {
            Reset();
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
        return *this;
    }

    template <typename O>
    SharedPtr& operator=(SharedPtr<O>&& other) {
        if (other.block_) {
            if (block_ != other.block_) {
                Reset();
                ptr_ = other.ptr_;
                block_ = other.block_;
                block_->Inc();
                other.Reset();
            }
        } else {
            Reset();
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor

    ~SharedPtr() {
        Reset();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    void Reset() {
        if (block_) {
            if (UseCount() == 1) {
                delete block_;
            } else {
                block_->Dec();
            }
            block_ = nullptr;
            ptr_ = nullptr;
        }
    }

    void Reset(T* ptr) {
        if (block_) {
            if (UseCount() == 1) {
                delete block_;
            } else {
                block_->Dec();
            }
            block_ = nullptr;
            ptr_ = nullptr;
        }
        ptr_ = ptr;
        block_ = new ControlBlockPtr<T>(ptr);
    }

    template <typename Y>
    void Reset(Y* ptr) {
        if (block_) {
            if (UseCount() == 1) {
                delete block_;
            } else {
                block_->Dec();
            }
            block_ = nullptr;
            ptr_ = nullptr;
        }
        ptr_ = ptr;
        block_ = new ControlBlockPtr<Y>(ptr);
    }

    void Swap(SharedPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Observers

    T* Get() const {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    size_t UseCount() const {
        if (block_) {
            return block_->Count();
        }
        return 0;
    }

    explicit operator bool() const {
        if (ptr_) {
            return true;
        }
        return false;
    }

    BaseControl* block_;
    T* ptr_;
};

template <typename T, typename U>
bool operator==(const SharedPtr<T>& left, const SharedPtr<U>& right) {
    return left.ptr == right.ptr_;
}

// Allocate memory only once
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    SharedPtr<T> new_shared;
    ControlBlock<T>* new_block = new ControlBlock<T>(std::forward<Args>(args)...);
    new_shared.block_ = new_block;
    new_shared.ptr_ = new_block->GetPtr();
    return new_shared;
}

// Look for usage examples in tests
template <typename T>
class EnableSharedFromThis {
public:
    SharedPtr<T> SharedFromThis();
    SharedPtr<const T> SharedFromThis() const;

    WeakPtr<T> WeakFromThis() noexcept;
    WeakPtr<const T> WeakFromThis() const noexcept;
};
