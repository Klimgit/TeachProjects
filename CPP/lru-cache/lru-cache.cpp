#include "lru-cache.h"

LruCache::LruCache(size_t max_size) : max_size_(max_size) {
}

void LruCache::Set(const std::string& key, const std::string& value) {
    auto item = map_.find(key);
    if (item != map_.end()) {
        cache_.erase(item->second);
    } else {
        if (cache_.size() >= max_size_) {
            Del();
        }
    }
    cache_.push_front(std::make_pair(key, value));
    map_[key] = cache_.begin();
}

void LruCache::Del() {
    std::pair<std::string, std::string> last_element = cache_.back();
    map_.erase(last_element.first);
    cache_.pop_back();
}

bool LruCache::Get(const std::string& key, std::string* value) {
    if (IsExist(key)) {
        auto item = map_.find(key);
        *value = item->second->second;
        cache_.splice(cache_.begin(), cache_, item->second);
        return true;
    }
    return false;
}

bool LruCache::IsExist(const std::string& key) {
    return map_.find(key) != map_.end();
}
