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