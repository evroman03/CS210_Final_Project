#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <queue>

template<typename Key, typename Value>
class FIFOCache : public CacheBase<Key, Value>
{
public:
    FIFOCache(size_t capacity) { this->capacity = capacity; }

    void put(const Key& key, const Value& value) override;
    Value* get(const Key& key) override;

private:
    std::unordered_map<Key, Value> dataCache;
    std::queue<Key> order;
};

template<typename Key, typename Value>
void FIFOCache<Key, Value>::put(const Key& key, const Value& value)
{
    if (dataCache.find(key) != dataCache.end()) {
        dataCache[key] = value;
        return;
    }

    if (dataCache.size() >= this->capacity) {
        Key oldest = order.front();
        order.pop();
        dataCache.erase(oldest);
    }

    dataCache[key] = value;
    order.push(key);
}

template<typename Key, typename Value>
Value* FIFOCache<Key, Value>::get(const Key& key)
{
    auto it = dataCache.find(key);
    if (it == dataCache.end()) return nullptr;
    return &it->second;
}
