#pragma once

#include <atomic>
#include <optional>
#include <utility>

template <class T>
class MPSCStack {
public:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& val) : value(val), next(nullptr) {
        }
    };

    MPSCStack() : head_(nullptr) {
    }

    ~MPSCStack() {
        while (Pop()) {
        }
    }

    void Push(const T& value) {
        Node* new_node = new Node(value);
        Node* old = head_;
        do {
            new_node->next = old;
        } while (!head_.compare_exchange_weak(old, new_node));
    }

    std::optional<T> Pop() {
        Node* top = head_;
        if (!top) {
            return std::nullopt;
        }

        while (!head_.compare_exchange_weak(top, top->next)) {
            if (!top) {
                return std::nullopt;
            }
        }

        std::optional<T> result = std::move(top->value);
        delete top;
        return result;
    }

    template <class TFn>
    void DequeueAll(const TFn& cb) {
        Node* top = head_.exchange(nullptr);
        while (top) {
            Node* next = top->next;
            cb(std::move(top->value));
            delete top;
            top = next;
        }
    }

private:
    std::atomic<Node*> head_;
};