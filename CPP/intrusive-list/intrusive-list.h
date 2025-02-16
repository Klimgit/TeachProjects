#pragma once

#include <iterator>

class ListHook {
public:
    ListHook() : next_(this), prev_(this){};

    bool IsLinked() const {
        return next_ != this && prev_ != this;
    }

    void Unlink() {
        if (IsLinked()) {
            prev_->next_ = next_;
            next_->prev_ = prev_;
            next_ = this;
            prev_ = this;
        }
    }

    // Must unlink element from list
    ~ListHook() {
        Unlink();
    }

    ListHook(const ListHook&) = delete;
    ListHook& operator=(const ListHook&) = delete;

private:
    template <class T>
    friend class List;
    ListHook* next_;
    ListHook* prev_;

    // that helper function might be useful
    void LinkBefore(ListHook* other) {
        Unlink();
        next_ = other;
        prev_ = other->prev_;
        other->prev_->next_ = this;
        other->prev_ = this;
    };
};

template <typename T>
class List {
public:
    class Iterator {
        using IteratorTag = std::bidirectional_iterator_tag;

    public:
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef IteratorTag iterator_category;

        Iterator(ListHook* node) : node_(node) {
        }

        Iterator& operator++() {
            node_ = node_->next_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        T& operator*() const {
            return *static_cast<T*>(node_);
        }
        T* operator->() const {
            return static_cast<T*>(node_);
        }

        bool operator==(const Iterator& rhs) const {
            return node_ == rhs.node_;
        }
        bool operator!=(const Iterator& rhs) const {
            return node_ != rhs.node_;
        }

    private:
        ListHook* node_;
    };

    List() {
        dummy_.next_ = &dummy_;
        dummy_.prev_ = &dummy_;
    }
    List(const List&) = delete;
    List(List&& other) {
        if (!other.IsEmpty()) {
            dummy_.next_ = other.dummy_.next_;
            dummy_.prev_ = other.dummy_.prev_;
            dummy_.next_->prev_ = &dummy_;
            dummy_.prev_->prev_ = &dummy_;
            other.dummy_.next_ = &other.dummy_;
            other.dummy_.prev_ = &other.dummy_;
        }
    }

    // must unlink all elements from list
    ~List() {
        while (!IsEmpty()) {
            PopFront();
        }
    }

    List& operator=(const List&) = delete;
    List& operator=(List&& other) {
        if (this != &other) {
            while (!IsEmpty()) {
                PopFront();
            }
            if (!other.IsEmpty()) {
                dummy_.next_ = other.dummy_.next_;
                dummy_.prev_ = other.dummy_.prev_;
                dummy_.next_->prev_ = &dummy_;
                dummy_.prev_->next_ = &dummy_;
                other.dummy_.next_ = &other.dummy_;
                other.dummy_.prev_ = &other.dummy_;
            }
        }
        return *this;
    }

    bool IsEmpty() const {
        return dummy_.next_ == &dummy_;
    }
    // that method is allowed to be O(n)
    size_t Size() const {
        size_t size = 0;
        ListHook* current = dummy_.next_;
        while (current != &dummy_) {
            ++size;
            current = current->next_;
        }
        return size;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        elem->LinkBefore(&dummy_);
    }
    void PushFront(T* elem) {
        elem->LinkBefore(dummy_.next_);
    }

    T& Front() {
        return *static_cast<T*>(dummy_.next_);
    }
    const T& Front() const {
        return *static_cast<const T*>(dummy_.next_);
    }

    T& Back() {
        return *static_cast<T*>(dummy_.prev_);
    }
    const T& Back() const {
        return *static_cast<const T*>(dummy_.prev_);
    }

    void PopBack() {
        dummy_.prev_->Unlink();
    }
    void PopFront() {
        dummy_.next_->Unlink();
    }

    Iterator Begin() {
        return Iterator(dummy_.next_);
    }

    Iterator End() {
        return Iterator(&dummy_);
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        return Iterator(static_cast<ListHook*>(element));
    }

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
