#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <cassert>

class LruCache {
public:
    LruCache(size_t max_size);

    void Set(const std::string& key, const std::string& value);

    bool Get(const std::string& key, std::string* value);

    bool IsExist(const std::string& key);

private:
    size_t max_size_;
    std::list<std::pair<std::string, std::string>> cache_;
    std::unordered_map<std::string, decltype(cache_.begin())> map_;

    void Del();
};
