#include <iostream>
#include "CSVReader.h"
#include "LRU.h"

using namespace std;

int main() 
{
    int choice;
    LRUCache cache(10);
    CSVReader reader("cities.csv");
    
    while (true)
    {
        // Display menu
        cout << "\nSchool Management Menu\n";
        cout << "1. Load a CSV File\n";
        cout << "2. Search for a School\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
