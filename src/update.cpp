

#include <iostream>
#include <cstring>
#include <limits>
#include <string>
#include <iomanip>

#include "structs.h"
#include "update.h"
#include "file.h"
#include "colors.h"


using namespace std;


void updateProperty() {
    int refNumber = getValidNumericInput<int>(CYAN "Enter the reference number of the property you want to edit: " RESET);

    int index = getPropertyByRefNumber(refNumber);
    if (index == -1) {
        cout << RED << "No property found with ref. number - " << refNumber << "." << RESET << endl;
        return;
    }


    if (properties[index].status == SOLD) {
        cout << RED << "Invalid operation, the property is already sold - (A sold property cannot be edited)." << RESET << endl;
        return;
    }

    cout << CYAN << "Choose what you want to change: " << RESET << endl;
    cout << left;
    cout << "  " << setw(15) << "1. Ref. Number" << "6. Price" << endl;
    cout << "  " << setw(15) << "2. Broker" << "7. Total Area" << endl;
    cout << "  " << setw(15) << "3. Type" << "8. Number of Rooms" << endl;
    cout << "  " << setw(15) << "4. Area" << "9. Floor" << endl;
    cout << "  " << setw(15) << "5. Exposition" << "10. Status" << endl;
    cout << "  " << "0. Back" << endl;

    int choice = getValidNumericInput<int>(CYAN "Enter your choice: " RESET);

    switch (choice) {
        case 1: updateRefNumber(index); break;
        case 2: updateStringField(index, properties[index].broker, 50, "брокер"); break;
        case 3: updateStringField(index, properties[index].type, 50, "тип"); break;
        case 4: updateStringField(index, properties[index].area, 50, "район"); break;
        case 5: updateStringField(index, properties[index].exposition, 20, "изложение"); break;
        case 6: updateNumericField(&properties[index].price, "цена"); break;
        case 7: updateNumericField(&properties[index].totalArea, "обща площ"); break;
        case 8: updateNumericField(&properties[index].rooms, "брой стаи"); break;
        case 9: updateNumericField(&properties[index].floor, "етаж"); break;
        case 10: updateStatus(index); break;
        case 0: return;
        default:
            cout << RED << "Invalid option." << RESET << endl;
            break;
    }
}


void updateRefNumber(int index) {
    int newRefNumber = getValidNumericInput<int>(CYAN "Enter the new reference number: " RESET);

    if (getPropertyByRefNumber(newRefNumber) != -1) {
        cout << RED << "Invalid operation. A property with this reference number already exists." << RESET << endl;
        return;
    }

    properties[index].refNumber = newRefNumber;
    cout << GREEN << "Reference number successfully changed." << RESET << endl;
    syncDataToRecoveryFiles();
}


void updateStringField(int index, char* fieldPtr, int maxSize, const char* fieldName) {
    cout << CYAN << "Enter new value for " << fieldName << " (up to " << maxSize -1 << " characters): " << RESET;
    
    char newValue[MAX_STRING_SIZE];
    cin.getline(newValue, sizeof(newValue));

        cout << YELLOW << "The entered value is identical to the current one. No change was made." << RESET << endl;
        return;
    }

    cout << GREEN << "Value for " << fieldName << " successfully changed." << RESET << endl;
    syncDataToRecoveryFiles();
}


template <typename T>
void updateNumericField(T* fieldPtr, const char* fieldName) {
    string prompt = string(CYAN) + "Enter new value for " + fieldName + ": " + RESET;
    T newValue = getValidNumericInput<T>(prompt.c_str());

    if (*fieldPtr == newValue) {
        cout << YELLOW << "The entered value is identical to the current one. No change was made." << RESET << endl;
        return;
    }

    *fieldPtr = newValue;
    cout << GREEN << "Value for " << fieldName << " successfully changed." << RESET << endl;
    syncDataToRecoveryFiles();
}


void updateStatus(int index) {
    Status oldStatus = properties[index].status;

    cout << CYAN << "Choose new status: " << RESET << endl;
    cout << left;
    cout << "  " << setw(15) << "1. Available" << endl;
    cout << "  " << setw(15) << "2. Sold" << endl;
    cout << "  " << setw(15) << "3. Reserved" << endl;
    
    int choice;
    while (true) {
        choice = getValidNumericInput<int>(CYAN "Enter your choice: " RESET);
        if (choice >= 1 && choice <= 3) {
            break;
        }
        cout << RED << "Invalid choice. Please choose 1, 2 or 3." << RESET << endl;
    }

    Status newStatus;
    switch (choice) {
        case 1: newStatus = AVAILABLE; break;
        case 2: newStatus = SOLD; break;
        case 3: newStatus = RESERVED; break;
    }

    if (oldStatus == newStatus) {
        cout << YELLOW << "The property is already in the selected status. No change was made." << RESET << endl;
        return;
    }


    if (oldStatus == AVAILABLE && newStatus == RESERVED) {
        properties[index].price *= 0.8;
        cout << YELLOW << "The price of the property has been reduced by 20% due to the status change to 'Reserved'." << RESET << endl;
    }

    properties[index].status = newStatus;
    cout << GREEN << "Status successfully changed." << RESET << endl;
    syncDataToRecoveryFiles();
}


int getPropertyByRefNumber(int refNumber) {
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].refNumber == refNumber) return i;
    }

    return -1;
}
