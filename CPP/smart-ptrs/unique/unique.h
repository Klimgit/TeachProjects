#pragma once

#include "compressed_pair.h"
#include "deleters.h"
#include <cstddef>  // std::nullptr_t
#include <cstdlib>

struct Slug {};

// Primary template
template <typename T, typename Deleter = Slug>
class UniquePtr {
public:
    // Constructors
    explicit UniquePtr(T* ptr = nullptr) noexcept : my_ptr_(ptr, Deleter()) {
    }

    UniquePtr(T* ptr, Deleter deleter) noexcept : my_ptr_(ptr, std::move(deleter)) {
    }

    UniquePtr(UniquePtr&& other) noexcept
        : my_ptr_(std::move(other.Release()), std::move(other.GetDeleter())) {
    }

    // `operator=`-s
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            Reset(other.Release());
            GetDeleter() = std::move(other.GetDeleter());
        }
        return *this;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        Reset(nullptr);
        return *this;
    }

    template <typename U>
    explicit operator UniquePtr<U, Deleter>() const {
        return UniquePtr<U, Deleter>(Release(), GetDeleter());
    }

    template <typename U, typename NewDeleter>
    UniquePtr(UniquePtr<U, NewDeleter>&& other) noexcept
        : my_ptr_(other.Release(), std::move(other.GetDeleter())) {
    }

    // Destructor
    ~UniquePtr() noexcept {
        Reset();
    }

    // Modifiers
    T* Release() noexcept {
        return std::exchange(my_ptr_.GetFirst(), nullptr);
    }

    void Reset(T* ptr = nullptr) noexcept {
        if constexpr (std::is_void_v<T>) {
            free(std::exchange(my_ptr_.GetFirst(), ptr));
        } else {
            delete std::exchange(my_ptr_.GetFirst(), ptr);
        }
    }

    void Swap(UniquePtr& other) noexcept {
        std::swap(my_ptr_, other.my_ptr_);
    }

    // Observers
    T* Get() const noexcept {
        return my_ptr_.GetFirst();
    }

    Deleter& GetDeleter() noexcept {
        return my_ptr_.GetSecond();
    }

    const Deleter& GetDeleter() const noexcept {
        return my_ptr_.GetSecond();
    }

    explicit operator bool() const noexcept {
        return my_ptr_.GetFirst() != nullptr;
    }

    template <typename U = T>
    std::enable_if_t<!std::is_void_v<U>, U&> operator*() const noexcept {
        return *Get();
    }

    template <typename U = T>
    std::enable_if_t<!std::is_void_v<U>, U*> operator->() const noexcept {
        return Get();
    }

private:
    CompressedPair<T*, Deleter> my_ptr_;
};

// Specialization for arrays
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : my_ptr_(ptr, Deleter()) {
    }

    UniquePtr(T* ptr, Deleter deleter) noexcept : my_ptr_(ptr, std::move(deleter)) {
    }

    UniquePtr(UniquePtr&& other) noexcept
        : my_ptr_(std::exchange(other.my_ptr_.GetFirst(), nullptr), std::move(other.GetDeleter())) {
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            Reset(other.Release());
            GetDeleter() = std::move(other.GetDeleter());
        }
        return *this;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        Reset(nullptr);
        return *this;
    }

    ~UniquePtr() noexcept {
        Reset();
    }

    T* Release() noexcept {
        return std::exchange(my_ptr_.GetFirst(), nullptr);
    }

    void Reset(T* ptr = nullptr) noexcept {
        if constexpr (std::is_void_v<T>) {
            free(std::exchange(my_ptr_.GetFirst(), ptr));
        } else {
            delete[] std::exchange(my_ptr_.GetFirst(), ptr);
        }
    }

    void Swap(UniquePtr& other) noexcept {
        std::swap(my_ptr_, other.my_ptr_);
    }

    T* Get() const noexcept {
        return my_ptr_.GetFirst();
    }

    Deleter& GetDeleter() noexcept {
        return my_ptr_.GetSecond();
    }

    const Deleter& GetDeleter() const noexcept {
        return my_ptr_.GetSecond();
    }

    explicit operator bool() const noexcept {
        return my_ptr_.GetFirst() != nullptr;
    }

    template <typename U = T>
    std::enable_if_t<!std::is_void_v<U>, U&> operator*() const noexcept {
        return *Get();
    }

    template <typename U = T>
    std::enable_if_t<!std::is_void_v<U>, U*> operator->() const noexcept {
        return Get();
    }

    T& operator[](std::size_t index) const noexcept {
        return Get()[index];
    }

private:
    CompressedPair<T*, Deleter> my_ptr_;
};
