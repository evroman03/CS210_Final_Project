#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

CSVReader::CSVReader(const std::string& filePath) : filePath(filePath) {}

int CSVReader::getPopulation(const std::string& cityName, const std::string& countryCode) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open CSV file.\n";
        return -1;
    }

    std::string line; //one full line in the CSV file

    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string city, code, populationStr;

        std::getline(ss, code, ',');
        std::getline(ss, city, ',');
        std::getline(ss, populationStr, ',');
        if (city == cityName && code == countryCode) 
        {
            return std::stoi(populationStr); // "string to integer" 
        }
    }

    std::cout << "No match found.\n";
    return -1;
}
