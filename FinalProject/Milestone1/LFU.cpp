#include "LFU.h"
#include "iostream"

template<typename Key, typename Value>
void LFUCache<Key, Value>::put(const Key& key, const Value& value) 
{
    if (CacheBase<Key, Value>::CAPACITY == 0)
    {
        cout << "Non-zero capacity required to input something"
        return;
    }

    //update value and return early if found
    if (get(key)) 
    {
        cache[key].value = value;
        return;
    }

    //otherwise, THEN the cache must be full
    if (cache.size() >= CacheBase<Key, Value>::CAPACITY) 
    {
        //use minFreq to get the frontmost, FIRSTmost in the freqMap
        Key evictKey = freqMap[minFreq].front();
        freqMap[minFreq].pop_front();
        cache.erase(evictKey);
    }

    freqMap[1].push_back(key);
    //make a value with the freq set to 1 and its pointer to the first bucket's end, but one to the left/positive/-1
    cache[key] = { value, 1, --freqMap[1].end() };
    minFreq = 1;
}

template<typename Key, typename Value>
Value* LFUCache<Key, Value>::get(const Key& key) 
{
    auto it = cache.find(key);

    //cache.end is the "cliff" marker that indicates a not found
    if (it == cache.end()) 
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

// Force template instantiation if needed elsewhere
//template class LFUCache<std::string, int>; // Example types
