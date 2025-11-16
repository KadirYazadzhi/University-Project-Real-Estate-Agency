

#include <iostream>
#include <limits>
#include <cstring>
#include <type_traits>
#include "add.h"
#include "structs.h"
#include "update.h"
#include "file.h"
#include "colors.h"

using namespace std;


void sanitizeString(char* str) {
    int len = strlen(str);
    int k = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] != '|') {
            str[k++] = str[i];
        }
    }
    str[k] = '\0';
}


void addSingleProperty() {
    if (isCapacityReached()) return;

    Property newProperty;

    newProperty.refNumber = getValidNumericInput<int>(CYAN "Enter reference number: " RESET);


    if (getPropertyByRefNumber(newProperty.refNumber) != -1) {
        cout << RED << "Invalid data." << RESET << endl;
        cout << RED << "A property with this reference number already exists." << RESET << endl;
        cout << RED << "Failed to add property." << RESET << endl;
        return;
    }

    newProperty.price = getValidNumericInput<double>(CYAN "Enter property price: " RESET);
    newProperty.totalArea = getValidNumericInput<double>(CYAN "Enter total area of the property: " RESET);
    newProperty.rooms = getValidNumericInput<int>(CYAN "Enter number of rooms: " RESET);
    newProperty.floor = getValidNumericInput<int>(CYAN "Enter floor: " RESET);

    cout << CYAN << "Enter broker name: " << RESET;
    cin.getline(newProperty.broker, 50);
    sanitizeString(newProperty.broker);

    cout << CYAN << "Enter property type: " << RESET;
    cin.getline(newProperty.type, 50);
    sanitizeString(newProperty.type);

    cout << CYAN << "Enter property area: " << RESET;
    cin.getline(newProperty.area, 50);
    sanitizeString(newProperty.area);

    cout << CYAN << "Enter property exposition: " << RESET;
    cin.getline(newProperty.exposition, 20);
    sanitizeString(newProperty.exposition);


    properties[propertyCount] = newProperty;
    propertyCount++;

    cout << endl;
    cout << GREEN << "Property successfully added." << RESET << endl;

}


void addMultipleProperties() {
    int n = getValidNumericInput<int>(CYAN "Enter the number of properties you wish to add: " RESET);

    if (propertyCount + n > MAX_PROPERTIES) {
        int availableSpace = MAX_PROPERTIES - propertyCount;
        cout << RED << "The number of properties you wish to enter exceeds the maximum of 100 properties." << RESET << endl;
        cout << YELLOW << "The system has free space for " << availableSpace << " properties." << RESET << endl;

        n = availableSpace;

        if (n <= 0) {
            isCapacityReached();
            return;
        }
    }

    for (int i = 0; i < n; i++) {

        addSingleProperty();
    }
}


template <typename T>
T getValidNumericInput(const char* prompt, const char* errorMsg = "Invalid input. Please try again: ") {


    T value;
    cout << prompt;

    while (!(cin >> value)) {
        cout << errorMsg;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}


bool isCapacityReached() {
    if (propertyCount >= MAX_PROPERTIES) {
        cout << RED << "Adding a property is not possible." << RESET << endl;
        cout << RED << "The properties you have entered so far have reached the maximum of 100 properties." << RESET << endl;

        return true;
    }

    return false;
}