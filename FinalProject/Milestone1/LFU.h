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

template<typename Key, typename Value>
void LFUCache<Key, Value>::put(const Key& key, const Value& value)
{
    if (CacheBase<Key, Value>::CAPACITY == 0)
    {
        return;
    }

    //update value and return early if found
    Value* existing = get(key);
    if (existing)
    {
        *existing = value; // update the value directly
        return;
    }

    //otherwise, THEN the cache must be full
    if (dataCache.size() >= CacheBase<Key, Value>::CAPACITY)
    {
        //use minFreq to get the frontmost, FIRSTmost in the freqMap
        Key evictKey = freqMap[minFreq].front();
        freqMap[minFreq].pop_front();
        dataCache.erase(evictKey);
    }

    freqMap[1].push_back(key);
    //make a value with the freq set to 1 and its pointer to the first bucket's end, but one to the left/positive/-1
    dataCache[key] = { value, 1, --freqMap[1].end() };
    minFreq = 1;
}

template<typename Key, typename Value>
Value* LFUCache<Key, Value>::get(const Key& key)
{
    auto it = dataCache.find(key);

    //cache.end is the "cliff" marker that indicates a not found
    if (it == dataCache.end())
    {
        return nullptr;
    }

    //if we did find a value in the data map, store its frequency
    int freq = it->second.frequency;
    //remove this value from the freqMap because it is changing frequency
    freqMap[freq].erase(it->second.listIter);

    //
    if (freqMap[freq].empty() && freq == minFreq)
    {
        ++minFreq;
    }

    //re-add the value, but in a high frequency bucket
    freqMap[freq + 1].push_back(key);
    //increment the value's frequency
    it->second.frequency++;
    //set the listiter to the next frequency bucket, (freq+1), one before the end sentinal
    it->second.listIter = --freqMap[freq + 1].end();

    return &it->second.value;
}

