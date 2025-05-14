#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <vector>
#include <cstdlib>

template<typename Key, typename Value>
class RandomCache : public CacheBase<Key, Value>
{
public:
    RandomCache(size_t capacity) { this->capacity = capacity; }

    void put(const Key& key, const Value& value) override;
    Value* get(const Key& key) override;

private:
    std::unordered_map<Key, Value> cache;
    std::vector<Key> keys;
};

template<typename Key, typename Value>
void RandomCache<Key, Value>::put(const Key& key, const Value& value)
{
    if (cache.find(key) != cache.end()) {
        cache[key] = value;
        return;
    }

    if (cache.size() >= this->capacity) {
        int num = rand() % keys.size();
        Key evictKey = keys[num];
        cache.erase(evictKey);
        keys.erase(keys.begin() + num);
    }

    cache[key] = value;
    keys.push_back(key);
}

template<typename Key, typename Value>
Value* RandomCache<Key, Value>::get(const Key& key)
{
    auto it = cache.find(key);
    if (it == cache.end()) return nullptr;
    return &it->second;
}
