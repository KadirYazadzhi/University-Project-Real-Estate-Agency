

#include <iostream>
#include <iomanip>
#include "display.h"
#include "structs.h"
#include "colors.h"

using namespace std;


bool isPropertiesEmpty() {
    if (propertyCount == 0) {
        cout << RED << "Operation cannot be performed." << RESET << endl;
        cout << RED << "No properties recorded." << RESET << endl;

        return true;
    }

    return false;
}


void displayPropertyDetails(const Property &prop) {
    cout << left << setw(10) << YELLOW << "Ref. number: " << RESET << prop.refNumber << endl;
    cout << left << setw(10) << YELLOW << "Broker name: " << RESET << prop.broker << endl;
    cout << left << setw(10) << YELLOW << "Type: " << RESET << prop.type << endl;
    cout << left << setw(10) << YELLOW << "Area: " << RESET << prop.area << endl;
    cout << left << setw(10) << YELLOW << "Exposition: " << RESET << prop.exposition << endl;
    cout << left << setw(10) << YELLOW << "Price: " << RESET << prop.price << endl;
    cout << left << setw(10) << YELLOW << "Total area: " << RESET << prop.totalArea << endl;
    cout << left << setw(10) << YELLOW << "Number of rooms: " << RESET << prop.rooms << endl;
    cout << left << setw(10) << YELLOW << "Floor: " << RESET << prop.floor << endl;
    cout << left << setw(10) << YELLOW << "Status: " << RESET << getStatusString(prop.status) << endl;
    cout << endl;
}


void displayAllProperties() {
    if (isPropertiesEmpty()) return;

    for (int i = 0; i < propertyCount; i++) {
        cout << YELLOW << "--- Property #" << i + 1 << RESET << endl;
        displayPropertyDetails(properties[i]);
    }
}


void displaySoldProperties() {
    if (isPropertiesEmpty()) return;

    bool isFound = false;

    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].status == SOLD) {
            cout << YELLOW << "--- Property #" << i + 1 << RESET << endl;
            displayPropertyDetails(properties[i]);
            isFound = true;
        }
    }

    if (!isFound) cout << YELLOW << "Currently, there are no sold properties in the system." << RESET << endl;
}


void displayLargestProperties() {
    if (isPropertiesEmpty()) return;

    double maxTotalArea = 0.0;


    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea > maxTotalArea) {
            maxTotalArea = properties[i].totalArea;
        }
    }

    cout << YELLOW << "--- Properties with the largest area (" << maxTotalArea << ") ---" << RESET << endl;

    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea < maxTotalArea) continue;

        cout << YELLOW << "--- Property #" << i + 1 << RESET << endl;
        displayPropertyDetails(properties[i]);
    }
}
