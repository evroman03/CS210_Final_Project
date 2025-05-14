#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <list>

template<typename Key, typename Value>
class LFUCache : public CacheBase<Key, Value>
{
public:
    LFUCache(size_t capacity) { this->capacity = capacity; }

    void put(const Key& key, const Value& value) override;
    Value* get(const Key& key) override;

private:
    struct CacheEntry
    {
        Value value;
        int frequency;
        typename std::list<Key>::iterator listIter;
    };

    std::unordered_map<Key, CacheEntry> dataCache;
    std::unordered_map<int, std::list<Key>> freqMap;
    int minFreq = 0;
};

template<typename Key, typename Value>
void LFUCache<Key, Value>::put(const Key& key, const Value& value)
{
    if (this->capacity == 0) return;

    Value* existing = get(key);
    if (existing) {
        *existing = value;
        return;
    }

    if (dataCache.size() >= this->capacity) {
        Key evictKey = freqMap[minFreq].front();
        freqMap[minFreq].pop_front();
        dataCache.erase(evictKey);
    }

    freqMap[1].push_back(key);
    dataCache[key] = { value, 1, --freqMap[1].end() };
    minFreq = 1;
}

template<typename Key, typename Value>
Value* LFUCache<Key, Value>::get(const Key& key)
{
    auto it = dataCache.find(key);
    if (it == dataCache.end()) return nullptr;

    int freq = it->second.frequency;
    freqMap[freq].erase(it->second.listIter);

    if (freqMap[freq].empty() && freq == minFreq) {
        ++minFreq;
    }

    freqMap[freq + 1].push_back(key);
    it->second.frequency++;
    it->second.listIter = --freqMap[freq + 1].end();

    return &it->second.value;
}
