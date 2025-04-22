#include <iostream>
#include "CSVReader.h"
#include "LRU.h"

using namespace std;

int main()
{
    int choice;
    string csvPath = "world_cities.csv";
    string cityName, countryCode;

    LRUCache cache(10);
    CSVReader reader(csvPath);

    while (true)
    {
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
                string population = cache.get(key);

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
                        cache.put(key, population);
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
