

#include <iostream>
#include <limits>
#include <cstring>
#include <algorithm>

#include "structs.h"
#include "search.h"
#include "display.h"
#include "sort.h"
#include "colors.h"


using namespace std;


void searchByBroker() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties entered into the system." << RESET << endl;
        return;
    }

    cout << CYAN << "Enter the name of the broker you wish to search for: " << RESET;
    char searchBroker[50];
    cin.getline(searchBroker, 50);

    Property tempProperties[MAX_PROPERTIES];
    int tempCount = 0;
    bool isFound = false;


    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(searchBroker, properties[i].broker) == 0) {
            tempProperties[tempCount] = properties[i];
            tempCount++;
            isFound = true;
        }
    }

    if (!isFound) {
        cout << RED << "No properties found for broker '" << searchBroker << "'." << RESET << endl;
        return;
    }


    cout << CYAN << "Select sorting order by price:" << RESET << endl;
    cout << "  1. Ascending (lowest to highest)" << endl;
    cout << "  2. Descending (highest to lowest)" << endl;
    
    int choice;
    bool isAscending;
    while (true) {
        choice = getValidNumericInput<int>(CYAN "Enter your choice: " RESET);
        if (choice == 1 || choice == 2) {
            isAscending = (choice == 1);
            break;
        }
        cout << RED << "Invalid choice. Please choose 1 or 2." << RESET << endl;
    }


    sortPropertiesArray(tempProperties, tempCount, isAscending);


    cout << YELLOW << "\n--- Results for broker '" << searchBroker << "' (Sorted by price) ---" << RESET << endl;
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- Property #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}


void searchByRooms() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties entered into the system." << RESET << endl;
        return;
    }

    int roomsCount = getValidNumericInput<int>(CYAN "Enter the number of rooms to search for: " RESET);

    Property tempProperties[MAX_PROPERTIES];
    int tempCount = 0;
    bool isFound = false;


    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].rooms == roomsCount) {
            tempProperties[tempCount] = properties[i];
            tempCount++;
            isFound = true;
        }
    }

    if (!isFound) {
        cout << RED << "No properties found with " << roomsCount << " rooms." << RESET << endl;
        return;
    }


    sortPropertiesArray(tempProperties, tempCount, false);
    

    cout << YELLOW << "\n--- Results for properties with " << roomsCount << " rooms (Sorted by price) ---" << RESET << endl;
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- Property #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}