#include "RandomCache.h"

template<typename Key, typename Value>

void RandomCache<Key, Value>::put(const Key& key, const Value& value)
{
	if (cache.find(key) != cache.end)
	{
		cache[key] = value;
		return;
	}

	if (cache.size() >= CacheBase<Key, Value>::CAPACITY)
	{
		//rand generates a psuedo random number, within the key size
		int num = rand() % keys.size();
		Key evictKey = keys[num];

		cache.erase(evictKey);
		keys.erase(keys.begin() + num);
	}

	cache[key] = value;
	keys.pushback(key);
}

template<typename Key, typename Value>
Value* RandomCache<Key, Value>::get(const Key& key)
{
	auto valuePtr = cache.find(key);
	if (valuePtr == cache.end())
	{
		return nullptr;
	}
	return &valuePtr->second;
}