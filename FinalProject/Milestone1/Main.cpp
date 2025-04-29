#include <iostream>
#include "CSVReader.h"
#include "LFU.h"
#include "FIFOCache.h"
#include "RandomCache.h"
#include <memory>

using namespace std;

int main()
{
    int strategyChoice;
    string csvPath = "world_cities.csv";
    string cityName, countryCode;

    cout << "Select a caching strategy:\n";
    cout << "1. LFU (Least Frequently Used)\n";
    cout << "2. FIFO (First-In, First-Out)\n";
    cout << "3. Random Replacement\n";
    cout << "Enter your choice: ";
    cin >> strategyChoice;
    cin.ignore();

    unique_ptr<CacheBase<string, string>> cache;

    switch (strategyChoice)
    {
    case 1:
        cache = make_unique<LFUCache<string, string>>();
        break;
    case 2:
        cache = make_unique<FIFOCache<string, string>>();
        break;
    case 3:
        cache = make_unique<RandomCache<string, string>>();
        break;
    default:
        cout << "Invalid choice. Defaulting to LFU.\n";
        cache = make_unique<LFUCache<string, string>>();
    }

    CSVReader reader(csvPath);

    while (true)
    {
        int choice;
        cout << "\nCountry Management Menu\n";
        cout << "1. Load a different CSV File\n";
        cout << "2. Find a city\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            cout << "Currently using CSV file: " << csvPath << endl;
            cout << "Would you like to use a new CSV file? (y/n): ";
            string response;
            getline(cin, response);

            if (response == "y")
            {
                cout << "Enter new CSV file path: ";
                getline(cin, csvPath);
                reader = CSVReader(csvPath);
                cout << "CSV file updated to: " << csvPath << endl;
            }
            break;
        }

        case 2:
        {
            cout << "Enter country code: ";
            getline(cin, countryCode);
            cout << "Enter city name: ";
            getline(cin, cityName);

            string key = countryCode + "," + cityName;
            string* result = cache->get(key);
            string population = result ? *result : "Not Found";

            if (population != "Not Found")
            {
                cout << "Population (from cache): " << population << endl;
            }
            else
            {
                int pop = reader.getPopulation(cityName, countryCode);
                if (pop != -1)
                {
                    population = to_string(pop);
                    cache->put(key, population);
                    cout << "Population (from file): " << population << endl;
                }
                else
                {
                    cout << "City not found in the CSV file." << endl;
                }
            }
            break;
        }

        case 3:
            cout << "Exiting program.\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

