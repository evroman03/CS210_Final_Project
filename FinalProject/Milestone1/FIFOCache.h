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
    std::unordered_map<Key, Value> cache;
    //this will keep track of which entry was queried 10 searches ago to evict upon full
    std::queue<Key> order;
};
