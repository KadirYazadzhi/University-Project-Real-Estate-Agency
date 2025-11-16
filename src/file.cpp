

#include <iostream>
#include "structs.h"
#include "file.h"
#include "colors.h"

#include <cstring>
#include <fstream>
#include <limits>

using namespace std;




void saveToBinaryFile() {
    if (!confirmOverwrite(USER_BINARY_FILENAME)) {
        cout << YELLOW << "Write operation cancelled." << RESET << endl;
        return;
    }

    FILE* binaryFile = fopen(USER_BINARY_FILENAME, "wb");

    if (binaryFile == NULL) {
        cout << RED << "Could not open file for writing." << RESET << endl;
        return;
    }


    if (propertyCount > 0) fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    fclose(binaryFile);

    cout << GREEN << "Property data successfully written to file '" << USER_BINARY_FILENAME << "'." << RESET << endl;
}


void loadFromBinaryFile() {
    FILE* binaryFile = fopen(USER_BINARY_FILENAME, "rb");
    if (binaryFile == NULL) {
        cout << RED << "Data file not found. Starting with an empty database." << RESET << endl;
        propertyCount = 0;
        return;
    }

    size_t readCount = fread(&propertyCount, sizeof(int), 1, binaryFile);

    if (readCount != 1) {
        cout << RED << "Data file is empty or corrupted." << RESET << endl;
        fclose(binaryFile);
        propertyCount = 0;
        return;
    }

    if (propertyCount > 0 && propertyCount <= MAX_PROPERTIES) {
        fread(properties, sizeof(Property), propertyCount, binaryFile);
        cout << GREEN << "Successfully loaded " << propertyCount << " properties from file '" << USER_BINARY_FILENAME << "'." << RESET << endl;
    }
    else if (propertyCount > MAX_PROPERTIES) {
        cout << RED << "Error: Number of properties in file (" << propertyCount << ") exceeds maximum capacity." << RESET << endl;
        propertyCount = 0;
    }
    else {
        cout << YELLOW << "File is empty. 0 properties loaded." << RESET << endl;
        propertyCount = 0;
    }

    fclose(binaryFile);
}


void saveToUserFriendlyTextFile() {
    if (!confirmOverwrite(USER_TEXT_FILENAME)) return;

    FILE* file = fopen(USER_TEXT_FILENAME, "w");

    if (file == NULL) {
        cout << RED << "Could not open text file for writing." << RESET << endl;
        return;
    }

    fprintf(file, "--- REPORT: All Properties (%d) ---\n", propertyCount);
    fprintf(file, "---------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(file, "| Ref | Broker | Type | Area | Expos | Price | Total Area | Rooms | Floor | Status |\n");
    fprintf(file, "---------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < propertyCount; i++) {
        Property property = properties[i];

        fprintf(file, "| %-3d | %-6s | %-4s | %-4s | %-4s | %.2f | %.2f | %-5d | %-5d | %-10s |\n",
            property.refNumber,
            property.broker,
            property.type,
            property.area,
            property.exposition,
            property.price,
            property.totalArea,
            property.rooms,
            property.floor,
            getStatusString(property.status)
        );
    }

    fclose(file);
    cout << GREEN << "Property data successfully exported to file '" << USER_TEXT_FILENAME << "'." << RESET << endl;
}


void saveToSyncTextFile() {
    FILE* file = fopen(SYNC_TEXT_FILENAME, "w");

    if (file == NULL) return;

    for (int i = 0; i < propertyCount; i++) {
        Property property = properties[i];

        fprintf(file, "%d|%s|%s|%s|%s|%.2f|%.2f|%d|%d|%d\n",
                property.refNumber,
                property.broker,
                property.type,
                property.area,
                property.exposition,
                property.price,
                property.totalArea,
                property.rooms,
                property.floor,
                property.status
        );
    }

    fclose(file);
}


void loadFromSyncTextFile() {
    FILE* file = fopen(SYNC_TEXT_FILENAME, "r");

    if (file == NULL) {
        propertyCount = 0;
        return;
    }

    propertyCount = 0;

    // Четем файла ред по ред, докато не достигнем края или максималния капацитет
    while (propertyCount < MAX_PROPERTIES) {
        int ref, rooms, floor, statusInt;
        double price, totalArea;

        char broker[50], type[50], area[50], exposition[20];


        int itemsRead = fscanf(file, "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%lf|%lf|%d|%d|%d\n",
            &ref,
            broker, type, area, exposition,
            &price, &totalArea,
            &rooms, &floor,
            &statusInt
        );

        if (itemsRead == 10) {
            properties[propertyCount].refNumber = ref;
            strcpy(properties[propertyCount].broker, broker);
            strcpy(properties[propertyCount].type, type);
            strcpy(properties[propertyCount].area, area);
            strcpy(properties[propertyCount].exposition, exposition);
            properties[propertyCount].price = price;
            properties[propertyCount].totalArea = totalArea;
            properties[propertyCount].rooms = rooms;
            properties[propertyCount].floor = floor;
            properties[propertyCount].status = (Status)statusInt;

            propertyCount++;
        }

    }

    fclose(file);
    if (propertyCount > 0) {
        cout << GREEN << "Successfully loaded " << propertyCount << " properties from the sync text file." << RESET << endl;
    }
}


void syncDataToRecoveryFiles() {
    saveToSyncTextFile();
    saveToRecoveryBinaryFile();
}


void saveToRecoveryBinaryFile() {
    FILE* binaryFile = fopen(SYNC_BINARY_FILENAME, "wb");
    if (binaryFile == NULL) return;
    fwrite(&propertyCount, sizeof(int), 1, binaryFile);
    if (propertyCount > 0) fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    fclose(binaryFile);
}


bool confirmOverwrite(const char* filename) {
    ifstream fileCheck(filename);


    fileCheck.close();

    cout << YELLOW << "File '" << filename << "' already exists. Do you want to overwrite it (Y/N)? " << RESET;
    char choice;

    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cin >> choice;

    if (choice == 'y' || choice == 'Y') return true;
    else return false;
}
