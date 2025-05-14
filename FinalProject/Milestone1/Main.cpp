#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <memory>
#include <random>
#include <unordered_set>
#include <algorithm>

#include "CacheBase.h"
#include "LFU.h"
#include "FIFOCache.h"
#include "RandomCache.h"

struct CityData {
    std::string name;
    std::string country;
    int population;

    friend std::ostream& operator<<(std::ostream& os, const CityData& data) {
        os << data.name << ", " << data.country << " (pop: " << data.population << ")";
        return os;
    }
};

std::vector<CityData> loadCities(const std::string& filename) {
    std::vector<CityData> cities;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string country, name, popStr;

        std::getline(ss, country, ',');
        std::getline(ss, name, ',');
        std::getline(ss, popStr, ',');

        CityData city = {
            name,
            country,
            std::stoi(popStr)
        };
        cities.push_back(city);
    }

    return cities;
}

enum class CacheType {
    LFU,
    FIFO,
    RANDOM
};

std::unique_ptr<CacheBase<std::string, CityData>> createCache(CacheType type, size_t capacity) {
    switch (type) {
    case CacheType::LFU:
        return std::make_unique<LFUCache<std::string, CityData>>(capacity);
    case CacheType::FIFO:
        return std::make_unique<FIFOCache<std::string, CityData>>(capacity);
    case CacheType::RANDOM:
        return std::make_unique<RandomCache<std::string, CityData>>(capacity);
    default:
        throw std::runtime_error("Invalid cache type");
    }
}

std::vector<CityData> generateLookupPool(const std::vector<CityData>& allCities, int hotCityCount, int hotCityDuplicates) {
    std::vector<CityData> pool;
    std::default_random_engine engine(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, allCities.size() - 1);

    std::unordered_set<std::string> usedNames;
    std::vector<CityData> hotCities;
    while (hotCities.size() < static_cast<size_t>(hotCityCount)) 
    {
        const CityData& candidate = allCities[dist(engine)];
        if (usedNames.insert(candidate.name).second)
        {
            hotCities.push_back(candidate);
        }
    }

    for (int i = 0; i < 1000; ++i)
    {
        pool.push_back(allCities[dist(engine)]);
    }

    for (const auto& hot : hotCities) 
    {
        for (int i = 0; i < hotCityDuplicates; ++i) {
            pool.push_back(hot);
        }
    }

    // Shuffle the full workload
    std::shuffle(pool.begin(), pool.end(), engine);
    return pool;
}

int main()
{
    const std::string filename = "world_cities.csv";
    const size_t cacheSize = 10;
    const int hotCityCount = 10;         // Number of hot cities
    const int hotCityDuplicates = 100;    // Times each hot city is duplicated

    std::vector<CityData> cities = loadCities(filename);
    if (cities.empty()) 
    {
        std::cerr << "Failed to load cities.\n";
        return 1;
    }

    std::vector<CityData> lookupPool = generateLookupPool(cities, hotCityCount, hotCityDuplicates);

    for (CacheType type : {CacheType::LFU, CacheType::FIFO, CacheType::RANDOM}) 
    {
        auto cache = createCache(type, cacheSize);

        std::cout << "\nTesting cache type: ";
        switch (type) {
        case CacheType::LFU: std::cout << "LFU\n"; break;
        case CacheType::FIFO: std::cout << "FIFO\n"; break;
        case CacheType::RANDOM: std::cout << "Random\n"; break;
        }

        size_t hits = 0;
        size_t misses = 0;
        double totalTimeNs = 0;

        for (const auto& city : lookupPool) 
        {
            const std::string& key = city.name;

            auto start = std::chrono::high_resolution_clock::now();
            CityData* cached = cache->get(key);
            auto end = std::chrono::high_resolution_clock::now();

            if (cached) {
                ++hits;
            }
            else
            {
                ++misses;
                cache->put(key, city);
            }

            totalTimeNs += std::chrono::duration<double, std::nano>(end - start).count();
        }

        double avgLookupTimeUs = totalTimeNs / lookupPool.size() / 1000.0;

        std::cout << "Total lookups: " << lookupPool.size() << "\n";
        std::cout << "Cache size: " << cacheSize << "\n";
        std::cout << "Hot cities: " << hotCityCount << ", each duplicated " << hotCityDuplicates << " times\n";
        std::cout << "Hits: " << hits << "\n";
        std::cout << "Misses: " << misses << "\n";
        std::cout << "Average lookup time: " << avgLookupTimeUs << " µs\n";
    }

    return 0;
}
