#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <list>

template<typename Key, typename Value>
class LFUCache : public CacheBase<Key, Value> 
{

public:
    void put(const Key& key, const Value& value) override;
    Value* get(const Key& key) override;

private:
    struct CacheEntry 
    {
        Value value;
        int frequency;

        //iterator to the freqMap
        typename std::list<Key>::iterator listIter;
    };

    //our hashmap using the cache entry. Every time an entry is used, its freq goes up
    std::unordered_map<Key, CacheEntry> dataCache;
    //This is a "frequency list" that tracks the accesses/frequency of least used keys
    //When we access a value with a key, the key is moved from the [nth] bucket to the [n+1] bucket
    std::unordered_map<int, std::list<Key>> freqMap;
    //Minfreq keeps track of which buckets of freqList are the least used
    int minFreq = 0;
};
