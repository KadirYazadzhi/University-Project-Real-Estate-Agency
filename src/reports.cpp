

#include <iostream>
#include <cstring>
#include <iomanip>

#include "reports.h"
#include "structs.h"
#include "sort.h"
#include "display.h"
#include "colors.h"

using namespace std;


#define MAX_UNIQUE_BROKERS MAX_PROPERTIES


void mostExpensiveInArea() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties entered into the system." << RESET << endl;
        return;
    }

    cout << CYAN << "Enter the area for which you want to generate the report: " << RESET;
    char searchArea[50];
    cin.getline(searchArea, 50);

    double maxPrice = -1.0;
    int mostExpensiveIndex = -1;


    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(properties[i].area, searchArea) == 0) {
            if (properties[i].price > maxPrice) {
                maxPrice = properties[i].price;
                mostExpensiveIndex = i;
            }
        }
    }

    if (mostExpensiveIndex == -1) {
        cout << endl;
        cout << RED << "No property found in this area." << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- Most expensive property in area " << searchArea << " ---" << RESET << endl;
    displayPropertyDetails(properties[mostExpensiveIndex]);
}


void averagePriceInArea() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties entered into the system." << RESET << endl;
        return;
    }

    cout << CYAN << "Enter the area for which you want to generate the report: " << RESET;
    char searchArea[50];
    cin.getline(searchArea, 50);

    double totalPrice = 0.0;
    int totalPropertyCount = 0;


    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(properties[i].area, searchArea) == 0) {
            totalPrice += properties[i].price;
            totalPropertyCount++;
        }
    }

    if (totalPropertyCount == 0) {
        cout << RED << "No properties found in this area." << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- Found " << totalPropertyCount << " properties with a total value of " << totalPrice << " ---" << RESET << endl;
    cout << "The average price per property in area '" << searchArea << "' is " << fixed << setprecision(2) << totalPrice / totalPropertyCount << "." << endl;
}


void soldPercentagePerBroker() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties entered into the system." << RESET << endl;
        return;
    }

    char uniqueBrokers[MAX_UNIQUE_BROKERS][50];
    int uniqueBrokerCount = 0;


    for (int i = 0; i < propertyCount; i++) {
        bool uniqueBrokerFound = true;
        for (int j = 0; j < uniqueBrokerCount; j++) {
            if (strcmp(properties[i].broker, uniqueBrokers[j]) == 0) {
                uniqueBrokerFound = false;
                break;
            }
        }

        if (uniqueBrokerFound) {
            if (uniqueBrokerCount < MAX_UNIQUE_BROKERS) {
                strcpy(uniqueBrokers[uniqueBrokerCount], properties[i].broker);
                uniqueBrokerCount++;
            }

        }
    }

    cout << YELLOW << "\n--- Percentage of Properties Sold per Broker ---" << RESET << endl;


    for (int i = 0; i < uniqueBrokerCount; i++) {
        int totalBrokerProperties = 0;
        int soldBrokerProperties = 0;

        for (int j = 0; j < propertyCount; j++) {
            if (strcmp(properties[j].broker, uniqueBrokers[i]) == 0) {
                totalBrokerProperties++;
                if (properties[j].status == SOLD) {
                    soldBrokerProperties++;
                }
            }
        }

        double soldBrokerPercentage = 0.0;
        if (totalBrokerProperties > 0) {
            soldBrokerPercentage = ((double)soldBrokerProperties / totalBrokerProperties) * 100.0;
        }

        cout << "Broker: " << uniqueBrokers[i] << endl;
        cout << left;
        cout << "  " << setw(20) << "Total Properties:" << totalBrokerProperties << endl;
        cout << "  " << setw(20) << "Sold:" << soldBrokerProperties << endl;
        cout << "  " << setw(20) << "Percentage Sold:" << fixed << setprecision(2) << soldBrokerPercentage << "%" << endl;
        cout << endl;
    }
}