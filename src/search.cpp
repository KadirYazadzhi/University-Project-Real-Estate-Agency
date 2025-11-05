/**
 * @file search.cpp
 * @brief Съдържа функции за търсене, филтриране и сортиране на имоти по зададени критерии.
 *
 * Функциите в този файл не променят основния масив с данни, а създават временни копия,
 * които се обработват и извеждат на екрана.
 */

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

/**
 * @brief Търси имоти по име на брокер, след което ги сортира по цена.
 */
void searchByBroker() {
    cout << CYAN << "Въведете името на брокера за който желаете да се направи търсенето: " << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    char searchBroker[50];
    cin.getline(searchBroker, 50);

    Property tempProperties[MAX_PROPERTIES];
    int tempCount = 0;
    bool isFound = false;

    // 1. Копираме всички имоти на дадения брокер във временен масив
    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(searchBroker, properties[i].broker) == 0) {
            tempProperties[tempCount] = properties[i];
            tempCount++;
            isFound = true;
        }
    }

    if (!isFound) {
        cout << RED << "Не са намерени имоти за брокер " << searchBroker << "." << RESET << endl;
        return;
    }

    // 2. Потребителят избира посока на сортиране
    cout << CYAN << "Изберете как да бъде подреден масива по цена [Възходящ/Низходящ] ред: " << RESET;
    string searchType;
    cin >> searchType;

    transform(searchType.begin(), searchType.end(), searchType.begin(),
        [](unsigned char c){ return std::tolower(c); });

    bool isAscending = true;

    if (searchType == "низходящ") isAscending = false;
    else if (searchType == "възходящ") isAscending = true;
    else cout << YELLOW << "Невалиден избор. Сортирането ще бъде изпълнено във възходящ ред." << RESET << endl;

    // 3. Сортираме временния масив
    sortPropertiesArray(tempProperties, tempCount, isAscending);

    // 4. Извеждаме сортираните резултати
    cout << YELLOW << "\n--- Резултати за брокер " << searchBroker << " (Сортирани) ---" << RESET << endl;
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- Имот #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}

/**
 * @brief Търси имоти по брой стаи, след което ги сортира по цена в низходящ ред.
 */
void searchByRooms() {
    cout << CYAN << "Въведете броя на стаите, за които да бъде направено търсенето: " << RESET;
    int roomsCount;
    cin >> roomsCount;

    Property tempProperties[MAX_PROPERTIES];
    int tempCount = 0;
    bool isFound = false;

    // 1. Копираме всички имоти с дадения брой стаи във временен масив
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].rooms == roomsCount) {
            tempProperties[tempCount] = properties[i];
            tempCount++;
            isFound = true;
        }
    }

    if (!isFound) {
        cout << RED << "Не са намерени имоти с " << roomsCount << " броя стаи." << RESET << endl;
        return;
    }

    // 2. Сортираме временния масив по цена в низходящ ред (от най-скъп към най-евтин)
    sortPropertiesArray(tempProperties, tempCount, false);
    
    // 3. Извеждаме резултата
    cout << YELLOW << "\n--- Резултати за имотите с " << roomsCount << " броя стаи (Сортирани) ---" << RESET << endl;
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- Имот #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}