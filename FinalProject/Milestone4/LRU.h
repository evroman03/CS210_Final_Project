#pragma once

#include <unordered_map>
#include <list>
#include <string>

struct CSVEntry 
{
    std::string key; //
    std::string data;
};

class LRUCache {
public:
    LRUCache(int capacity);

    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& data);

private:
    int capacity;
    //this is an unorderedMap (kvp structure) with a string matching an iterato that points directly to the correct node in memory

    /* 
    - With a std::vector, we could use indexes — but std::list has no index - based access, so (list[3] is not valid).
    - Additionally, in an LRU cache, we want to move elements around(to front, to back), but doing that with a vector would be O(n) because moving elements takes time.
    - But, with a list + iterator, we can move any node to the front / back in O(1) time using splice(), because we have its iterator already. 

    - This iterator lets us directly modify the recentList list
    */
    std::unordered_map<std::string, std::list<CSVEntry>::iterator> cache;
    std::list<CSVEntry> recentList;
};
