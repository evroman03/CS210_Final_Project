#pragma once
#include <string>

class CSVReader 
{
public:
    CSVReader(const std::string& filePath);
    int getPopulation(const std::string& cityName, const std::string& countryCode);

private:
    std::string filePath;
};