#pragma once

#include <type_traits>
#include <utility>

template<typename F, typename S,
    bool FirstEmpty = std::is_empty_v<F> && !std::is_final_v<F> && !std::is_same_v<F, S>,
    bool SecondEmpty = std::is_empty_v<S> && !std::is_final_v<S> && !std::is_same_v<F, S> >
class CompressedPair;

template<typename F, typename S>
class CompressedPair<F, S, false, false> {
public:
    CompressedPair() : first_(), second_() {
    }

    CompressedPair(const F &first, const S &second) : first_(first), second_(second) {
    }

    CompressedPair(const F &first, S &&second) : first_(first), second_(std::forward<S>(second)) {
    }

    CompressedPair(F &&first, const S &second) : first_(std::forward<F>(first)), second_(second) {
    }

    CompressedPair(F &&first, S &&second)
        : first_(std::forward<F>(first)), second_(std::forward<S>(second)) {
    }

    F &GetFirst() {
        return first_;
    }

    const F &GetFirst() const {
        return first_;
    }

    S &GetSecond() {
        return second_;
    }

    const S &GetSecond() const {
        return second_;
    }

private:
    F first_;
    S second_;
};

template<typename F, typename S>
class CompressedPair<F, S, true, false> : public F {
public:
    CompressedPair() : F(), second_() {
    }

    CompressedPair(const F &first, const S &second) : F(first), second_(second) {
    }

    CompressedPair(const F &first, S &&second) : F(first), second_(std::forward<S>(second)) {
    }

    CompressedPair(F &&first, const S &second) : F(std::forward<F>(first)), second_(second) {
    }

    CompressedPair(F &&first, S &&second)
        : F(std::forward<F>(first)), second_(std::forward<S>(second)) {
    }

    F &GetFirst() {
        return *this;
    }

    const F &GetFirst() const {
        return *this;
    }

    S &GetSecond() {
        return second_;
    }

    const S &GetSecond() const {
        return second_;
    }

private:
    S second_;
};

template<typename F, typename S>
class CompressedPair<F, S, false, true> : public S {
public:
    CompressedPair() : S(), first_() {
    }

    CompressedPair(const F &first, const S &second) : S(second), first_(first) {
    }

    CompressedPair(const F &first, S &&second) : S(std::forward<S>(second)), first_(first) {
    }

    CompressedPair(F &&first, const S &second) : S(second), first_(std::forward<F>(first)) {
    }

    CompressedPair(F &&first, S &&second)
        : S(std::forward<S>(second)), first_(std::forward<F>(first)) {
    }

    F &GetFirst() {
        return first_;
    }

    const F &GetFirst() const {
        return first_;
    }

    S &GetSecond() {
        return *this;
    }

    const S &GetSecond() const {
        return *this;
    }

private:
    F first_;
};

template<typename F, typename S>
class CompressedPair<F, S, true, true> : public F, public S {
public:
    CompressedPair() = default;

    CompressedPair(const F &first, const S &second) : F(first), S(second) {
    }

    CompressedPair(const F &first, S &&second) : F(first), S(std::forward<S>(second)) {
    }

    CompressedPair(F &&first, const S &second) : F(std::forward<F>(first)), S(second) {
    }

    CompressedPair(F &&first, S &&second) : F(std::forward<F>(first)), S(std::forward<S>(second)) {
    }

    F &GetFirst() {
        return *this;
    }

    const F &GetFirst() const {
        return *this;
    }

    S &GetSecond() {
        return *this;
    }

    const S &GetSecond() const {
        return *this;
    }
};

template<typename F>
class CompressedPair<F, F, true, true> : public F {
public:
    CompressedPair() = default;

    CompressedPair(const F &, const F &) {
    }

    CompressedPair(const F &, F &&) {
    }

    CompressedPair(F &&, const F &) {
    }

    CompressedPair(F &&, F &&) {
    }

    F &GetFirst() {
        return *this;
    }

    const F &GetFirst() const {
        return *this;
    }

    F &GetSecond() {
        return *this;
    }

    const F &GetSecond() const {
        return *this;
    }
};
