/**
 * @file reports.cpp
 * @brief Съдържа функции за генериране на различни видове справки.
 */

#include <iostream>
#include <cstring>

#include "reports.h"
#include "structs.h"
#include "sort.h"
#include "display.h"
#include "colors.h"

using namespace std;

// Дефинираме константа за максимален брой уникални брокери, равна на макс. брой имоти.
#define MAX_UNIQUE_BROKERS MAX_PROPERTIES

/**
 * @brief Намира и извежда най-скъпия имот в зададен от потребителя район.
 */
void mostExpensiveInArea() {
    cout << CYAN << "Въведете района за който желаете да бъде направена справката: " << RESET;
    char searchArea[50];
    cin.getline(searchArea, 50);

    double maxPrice = -1.0;
    int mostExpensiveIndex = -1;

    // Обхождаме всички имоти и търсим този с най-висока цена в дадения район
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
        cout << RED << "Не беше намерен имот в този район." << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- Най-скъпият имот в район " << searchArea << " ---" << RESET << endl;
    displayPropertyDetails(properties[mostExpensiveIndex]);
}

/**
 * @brief Изчислява и извежда средната цена на имотите в зададен от потребителя район.
 */
void averagePriceInArea() {
    cout << CYAN << "Въведете района за който желаете да бъде направена справката: " << RESET;
    char searchArea[50];
    cin.getline(searchArea, 50);

    double totalPrice = 0.0;
    int totalPropertyCount = 0;

    // Сумираме цените на всички имоти в дадения район и броим колко са
    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(properties[i].area, searchArea) == 0) {
            totalPrice += properties[i].price;
            totalPropertyCount++;
        }
    }

    if (totalPropertyCount == 0) {
        cout << RED << "Не бяха намерени имоти в този район." << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- Бяха намерени " << totalPropertyCount << " имота с обща стойност " << totalPrice << " ---" << RESET << endl;
    cout << "Средната цена за имот в " << searchArea << " район е " << totalPrice / totalPropertyCount << "." << endl;
}

/**
 * @brief Изчислява и извежда процента на продадените имоти за всеки брокер.
 */
void soldPercentagePerBroker() {
    char uniqueBrokers[MAX_UNIQUE_BROKERS][50];
    int uniqueBrokerCount = 0;

    // Първа стъпка: намираме всички уникални имена на брокери
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
            else break; // Достигнат е лимитът на уникални брокери
        }
    }

    cout << YELLOW << "\n--- Процент Продадени Имоти по Брокер ---" << RESET << endl;

    // Втора стъпка: за всеки уникален брокер, броим общия и продадения брой имоти
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

        cout << "Брокер: " << uniqueBrokers[i] << endl;
        cout << "   Общо имоти: " << totalBrokerProperties << endl;
        cout << "   Продадени: " << soldBrokerProperties << endl;
        cout << "   Процент продадени: " << soldBrokerPercentage << "%" << endl;
    }
}