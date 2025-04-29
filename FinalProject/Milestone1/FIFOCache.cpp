#include "FIFOCache.h"

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