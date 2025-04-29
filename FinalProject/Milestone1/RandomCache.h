#pragma once

#include "CacheBase.h"
#include <unordered_map>
#include <vector>
//for random number generation
#include <cstdlib>

template<typename Key, typename Value>
class RandomCache : public CacheBase<Key, Value>
{
public: 
	void put(const Key& key, const Value& value) override;
	Value* get(const Key& key) override;

private:
	std::unordered_map<Key, Value> cache;
	//we need a separate keys vector because otherwise how would you pick a random bucket from the hashmap
	std::vector<Key> keys;
};

template<typename Key, typename Value>

void RandomCache<Key, Value>::put(const Key& key, const Value& value)
{
	if (cache.find(key) != cache.end())
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
	keys.push_back(key);
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