#pragma once

#include <string>

//allows other datatypes 
template<typename Key, typename Value>

class CacheBase 
{
public:
    virtual ~CacheBase() = default;

    //=0 marks this as a pure virtual function - it MUST be overridden in inherited classes
    virtual void put(const Key& key, const Value& value) = 0;

    virtual Value* get(const Key& key) = 0;

    size_t capacity = 100; // default fallback
};
