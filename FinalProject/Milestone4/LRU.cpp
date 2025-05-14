#include "LRU.h"

//constructor
LRUCache::LRUCache(int capacity) : capacity(capacity) {}

std::string LRUCache::get(const std::string& key)
{
    auto it = cache.find(key);
    //if we found the value using the key (!= cache.end)
    if (it != cache.end()) 
    {
        // splice() moves elements in a list or from one to another in constant time(O(1)) by redoing the pointers
        // Destination, the source list, the iterator to the element
        recentList.splice(recentList.begin(), recentList, it->second);
        return it->second->data;
    }
    return "Not Found";
}

void LRUCache::put(const std::string& key, const std::string& data) 
{
    auto it = cache.find(key);
    if (it != cache.end()) 
    {
        //updating data and move to frontt
        it->second->data = data;
        recentList.splice(recentList.begin(), recentList, it->second);
    }

    //this is reached if the hashmap .find returned the "end" or no value. 
    //Thus, if the value doesnt exist, we add it, and remove the lru if the recent list is full
    else 
    {
        //if the linkedList is full, remove the least recently used entry
        if (recentList.size() == capacity) 
        {
            auto& lru = recentList.back();
            cache.erase(lru.key);
            recentList.pop_back();
        }
        //adding new entry to the front
        recentList.push_front({ key, data });
        cache[key] = recentList.begin();
    }
}
