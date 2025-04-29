#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <queue>

template<typename Key, typename Value>

class FIFOCache : public CacheBase<Key, Value> 
{

public:
    void put(const Key& key, const Value& value) override;
    Value* get(const Key& key) override;

private:
    std::unordered_map<Key, Value> dataCache;
    //this will keep track of which entry was queried 10 searches ago to evict upon full
    std::queue<Key> order;
};

template<typename Key, typename Value>
void FIFOCache<Key, Value>::put(const Key& key, const Value& value)
{
	//same as LFU, update value and return early if found
	if (dataCache.find(key) != dataCache.end())
	{
		dataCache[key] = value;
		return;
	}

	if (dataCache.size() >= CacheBase<Key, Value>::CAPACITY)
	{
		Key oldest = order.front();
		order.pop();
		//this goes to the map and tells it to remove the oldest value
		dataCache.erase(oldest);
	}

	//update the map bucket
	dataCache[key] = value;
	//add the new term to our fifo
	order.push(key);
}

//Do not consider access frequency, only insertion order matters.
template<typename Key, typename Value>
Value* FIFOCache<Key, Value>::get(const Key& key)
{
	auto it = dataCache.find(key);
	if (it == dataCache.end())
	{
		return nullptr;
	}
	return &it->second;
}
