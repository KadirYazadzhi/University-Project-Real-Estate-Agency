/**
 * @file file.cpp
 * @brief Управлява всички файлови операции.
 *
 * Този файл съдържа логиката за запис и четене от файлове. Разделя файловете на две групи:
 * 1. Потребителски - ръчно създадени справки и бекъпи (в папка data/).
 * 2. Синхронизиращи - автоматично запазени файлове за възстановяване на състоянието (в папка data/recovery/).
 */

#include <iostream>
#include "structs.h"
#include "file.h"
#include "colors.h"

#include <cstring>
#include <fstream>
#include <limits>

using namespace std;

// Дефиниция на пътищата до файловете
const char* USER_BINARY_FILENAME = "data/properties_backup.dat";      // За ръчен бекъп от потребителя
const char* USER_TEXT_FILENAME = "data/properties_report.txt";        // За ръчна тексова справка от потребителя
const char* SYNC_TEXT_FILENAME = "data/recovery/sync.txt";            // За автоматична синхронизация (основен)
const char* SYNC_BINARY_FILENAME = "data/recovery/sync.dat";          // За автоматична синхронизация (резервен)

/**
 * @brief Запазва текущите данни в двоичен файл, видим за потребителя (бекъп).
 */
void saveToBinaryFile() {
    if (!confirmOverwrite(USER_BINARY_FILENAME)) {
        cout << YELLOW << "Записът е отменен." << RESET << endl;
        return;
    }

    FILE* binaryFile = fopen(USER_BINARY_FILENAME, "wb");

    if (binaryFile == NULL) {
        cout << RED << "Не може да се отвори файлът за запис." << RESET << endl;
        return;
    }

    // Първо записваме броя на имотите, за да знаем колко да четем при зареждане
    fwrite(&propertyCount, sizeof(int), 1, binaryFile);
    // След това записваме целия масив с данни
    if (propertyCount > 0) fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    fclose(binaryFile);

    cout << GREEN << "Данните за имотите бяха успешно записани във файл '" << USER_BINARY_FILENAME << "'." << RESET << endl;
}

/**
 * @brief Зарежда данни от потребителски двоичен файл (бекъп).
 */
void loadFromBinaryFile() {
    FILE* binaryFile = fopen(USER_BINARY_FILENAME, "rb");
    if (binaryFile == NULL) {
        cout << RED << "Файлът с данни не е намерен. Започва се с празна база." << RESET << endl;
        propertyCount = 0;
        return;
    }

    size_t readCount = fread(&propertyCount, sizeof(int), 1, binaryFile);

    if (readCount != 1) {
        cout << RED << "Файлът с данни е празен или повреден." << RESET << endl;
        fclose(binaryFile);
        propertyCount = 0;
        return;
    }

    if (propertyCount > 0 && propertyCount <= MAX_PROPERTIES) {
        fread(properties, sizeof(Property), propertyCount, binaryFile);
        cout << GREEN << "Успешно заредени " << propertyCount << " имота от файл '" << USER_BINARY_FILENAME << "'." << RESET << endl;
    }
    else if (propertyCount > MAX_PROPERTIES) {
        cout << RED << "Грешка: Броят имоти във файла (" << propertyCount << ") надвишава максималния капацитет." << RESET << endl;
        propertyCount = 0;
    }
    else {
        cout << YELLOW << "Файлът е празен. Заредени са 0 имота." << RESET << endl;
        propertyCount = 0;
    }

    fclose(binaryFile);
}

/**
 * @brief Запазва данните в четим от човека текстов файл (справка).
 */
void saveToUserFriendlyTextFile() {
    if (!confirmOverwrite(USER_TEXT_FILENAME)) return;

    FILE* file = fopen(USER_TEXT_FILENAME, "w");

    if (file == NULL) {
        cout << RED << "Не може да се отвори текстов файл за запис." << RESET << endl;
        return;
    }

    fprintf(file, "--- СПРАВКА: Всички Имоти (%d) ---\n", propertyCount);
    fprintf(file, "---------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(file, "| Реф | Брокер | Тип | Район | Излж | Цена | Обща Площ | Стаи | Етаж | Статус |\n");
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
    cout << GREEN << "Данните за имотите бяха успешно експортирани във файл '" << USER_TEXT_FILENAME << "'." << RESET << endl;
}

/**
 * @brief Запазва данните в текстов файл за синхронизация.
 * Форматът е с разделител '|', за да може лесно да се чете ред по ред.
 */
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

/**
 * @brief Зарежда данните от текстовия файл за синхронизация при стартиране на програмата.
 */
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
        char broker[50], type[50], area[50], exposition[50];

        // fscanf връща броя на успешно прочетените елементи
        int itemsRead = fscanf(file, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%lf|%lf|%d|%d|%d\n",
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
        else break; // Ако не са прочетени 10 елемента, значи редът е грешен или сме в края на файла
    }

    fclose(file);
    if (propertyCount > 0) {
        cout << GREEN << "Успешно заредени " << propertyCount << " имота от синхронизиращия текстов файл." << RESET << endl;
    }
}

/**
 * @brief Централна функция за синхронизация. Извиква запис и в двата recovery файла.
 */
void syncDataToRecoveryFiles() {
    saveToSyncTextFile();
    saveToRecoveryBinaryFile();
}

/**
 * @brief Запазва данните в двоичен recovery файл (без потвърждение).
 */
void saveToRecoveryBinaryFile() {
    FILE* binaryFile = fopen(SYNC_BINARY_FILENAME, "wb");
    if (binaryFile == NULL) return;
    fwrite(&propertyCount, sizeof(int), 1, binaryFile);
    if (propertyCount > 0) fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    fclose(binaryFile);
}

/**
 * @brief Иска потвърждение от потребителя, ако файлът вече съществува.
 * 
 * @param filename Пътят до файла, който се проверява.
 * @return true Ако потребителят потвърди презаписването или ако файлът не съществува.
 * @return false Ако потребителят откаже презаписването.
 */
bool confirmOverwrite(const char* filename) {
    ifstream fileCheck(filename);

    if (!fileCheck.good()) return true; // Ако файлът не съществува, позволяваме запис
    fileCheck.close();

    cout << YELLOW << "Файлът '" << filename << "' вече съществува. Искате ли да го презапишете (Y/N)? " << RESET;
    char choice;

    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cin >> choice;

    if (choice == 'y' || choice == 'Y') return true;
    else return false;
}
