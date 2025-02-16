#pragma once

#include <string>
#include <cstring>
#include <stdexcept>

class StringView {
public:
    StringView(const std::string& str, std::size_t idx = 0, std::size_t len = std::string::npos) {
        if (idx > str.size()) {
            throw std::out_of_range("Index is out of range");
        }
        str_ = str.data() + idx;
        len_ = std::min(len, str.size() - idx);
    }

    StringView(const char* cstr) : str_(cstr), len_(std::strlen(cstr)) {
    }

    StringView(const char* cstr, std::size_t len) : str_(cstr), len_(len) {
    }

    const char& operator[](std::size_t i) const {
        if (i >= len_) {
            throw std::out_of_range("Index is out of range");
        }
        return str_[i];
    }

    std::size_t Size() const {
        return len_;
    }

private:
    const char* str_;
    std::size_t len_;
};
