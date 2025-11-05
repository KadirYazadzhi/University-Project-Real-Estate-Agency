/**
 * @file display.cpp
 * @brief Съдържа функции за извеждане на информация за имотите на конзолата.
 */

#include <iostream>
#include <iomanip>
#include "display.h"
#include "structs.h"
#include "colors.h"

using namespace std;

/**
 * @brief Проверява дали има въведени имоти в системата.
 * 
 * @return true Ако няма имоти.
 * @return false Ако има поне един имот.
 */
bool isPropertiesEmpty() {
    if (propertyCount == 0) {
        cout << RED << "Не е възможно операцията да се изпълни." << RESET << endl;
        cout << RED << "Няма записани имоти." << RESET << endl;

        return true;
    }

    return false;
}

/**
 * @brief Извежда детайлна информация за един имот.
 * 
 * @param prop Референция към обект от тип Property, чиито данни ще бъдат изведени.
 */
void displayPropertyDetails(const Property &prop) {
    cout << left << setw(10) << YELLOW << "Реф. номер: " << RESET << prop.refNumber << endl;
    cout << left << setw(10) << YELLOW << "Име на брокер: " << RESET << prop.broker << endl;
    cout << left << setw(10) << YELLOW << "Тип: " << RESET << prop.type << endl;
    cout << left << setw(10) << YELLOW << "Район: " << RESET << prop.area << endl;
    cout << left << setw(10) << YELLOW << "Изложение: " << RESET << prop.exposition << endl;
    cout << left << setw(10) << YELLOW << "Цена: " << RESET << prop.price << endl;
    cout << left << setw(10) << YELLOW << "Обща площ: " << RESET << prop.totalArea << endl;
    cout << left << setw(10) << YELLOW << "Брой стаи: " << RESET << prop.rooms << endl;
    cout << left << setw(10) << YELLOW << "Етаж: " << RESET << prop.floor << endl;
    cout << left << setw(10) << YELLOW << "Статус: " << RESET << getStatusString(prop.status) << endl;
    cout << endl;
}

/**
 * @brief Извежда всички имоти, налични в системата.
 */
void displayAllProperties() {
    if (isPropertiesEmpty()) return;

    for (int i = 0; i < propertyCount; i++) {
        cout << YELLOW << "--- Имот #" << i + 1 << RESET << endl;
        displayPropertyDetails(properties[i]);
    }
}

/**
 * @brief Извежда само продадените имоти.
 */
void displaySoldProperties() {
    if (isPropertiesEmpty()) return;

    bool isFound = false;

    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].status == SOLD) {
            cout << YELLOW << "--- Имот #" << i + 1 << RESET << endl;
            displayPropertyDetails(properties[i]);
            isFound = true;
        }
    }

    if (!isFound) cout << YELLOW << "В момента няма продадени имоти в системата." << RESET << endl;
}

/**
 * @brief Извежда имотите с най-голяма обща площ.
 */
void displayLargestProperties() {
    if (isPropertiesEmpty()) return;

    double maxTotalArea = 0.0;

    // Първо намираме коя е най-голямата площ
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea > maxTotalArea) {
            maxTotalArea = properties[i].totalArea;
        }
    }

    cout << YELLOW << "--- Имоти с най-голяма площ (" << maxTotalArea << ") ---" << RESET << endl;
    // След това извеждаме всички имоти, които имат тази площ
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea < maxTotalArea) continue;

        cout << YELLOW << "--- Имот #" << i + 1 << RESET << endl;
        displayPropertyDetails(properties[i]);
    }
}
