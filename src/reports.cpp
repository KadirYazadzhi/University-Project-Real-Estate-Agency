/**
 * @file reports.cpp
 * @brief Имплементация на функции за генериране на различни аналитични отчети за данни за имоти.
 *
 * Този файл предоставя конкретната имплементация за функции, декларирани в `reports.h`,
 * включително намиране на най-скъпия имот в дадена област, изчисляване на
 * средната цена на имотите в дадена област и определяне на процента на продадените
 * имоти от всеки брокер. Тези отчети предлагат ценни прозрения за пазара на недвижими имоти.
 */

#include <iostream>    
#include <cstring>     
#include <iomanip>     
#include <map>         

#include "reports.h"   
#include "structs.h"   
#include "display.h"   
#include "colors.h"    
#include "add.h"       
#include "localization.h" 

using namespace std;

/**
 * @brief Намира и показва най-скъпия имот в указан от потребителя район.
 *
 * Тази функция подканва потребителя да въведе конкретен район. След това тя итерира
 * през масива от имоти, за да намери този с най-висока цена,
 * който съвпада с посочения район. Ако не бъдат намерени имоти в района
 * или ако в системата не съществуват имоти, се показват подходящи съобщения.
 *
 * @param properties Постоянен масив от структури `Property` за анализ.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void mostExpensiveInArea(const Property properties[], int propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_IN_SYSTEM") << RESET << endl;
        return;
    }

    char searchArea[50];
    
    getValidStringInput((CYAN + getTranslatedString("PROMPT_REPORT_AREA") + RESET).c_str(), searchArea, sizeof(searchArea));

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
        cout << RED << getTranslatedString("NO_PROPERTY_FOUND_IN_AREA") << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- " << getTranslatedString("MOST_EXPENSIVE_PROPERTY_IN_AREA") << " " << searchArea << " ---" << RESET << endl;
    
    displayPropertyDetails(properties[mostExpensiveIndex]);
}

/**
 * @brief Изчислява и показва средната цена на имотите в указан от потребителя район.
 *
 * Тази функция подканва потребителя за район. След това тя итерира през всички имоти,
 * сумирайки цените на имотите в този район и преброявайки колко от тях
 * съществуват. Накрая, тя изчислява и показва средната цена,
 * или съобщение за грешка, ако не са намерени имоти в района или ако системата е празна.
 *
 * @param properties Постоянен масив от структури `Property` за анализ.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void averagePriceInArea(const Property properties[], int propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_IN_SYSTEM") << RESET << endl;
        return;
    }

    char searchArea[50];
    
    getValidStringInput((CYAN + getTranslatedString("PROMPT_REPORT_AREA") + RESET).c_str(), searchArea, sizeof(searchArea));

    double totalPrice = 0.0;     
    int propertiesInAreaCount = 0; 

    
    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(properties[i].area, searchArea) == 0) {
            totalPrice += properties[i].price;
            propertiesInAreaCount++;
        }
    }

    
    if (propertiesInAreaCount == 0) {
        cout << RED << getTranslatedString("NO_PROPERTIES_FOUND_IN_AREA") << RESET << endl;
        return;
    }

    cout << YELLOW << "\n--- " << getTranslatedString("FOUND_PROPERTIES_COUNT_PART1") << " " << propertiesInAreaCount << " " << getTranslatedString("FOUND_PROPERTIES_COUNT_PART2") << " " << totalPrice << " ---" << RESET << endl;
    cout << getTranslatedString("AVERAGE_PRICE_IN_AREA_PART1") << " '" << searchArea << "' " << getTranslatedString("AVERAGE_PRICE_IN_AREA_PART2") << " " << fixed << setprecision(2) << totalPrice / propertiesInAreaCount << "." << endl;
}

/**
 * @brief Изчислява и показва процента на продадените имоти за всеки уникален брокер.
 *
 * Тази функция ефективно събира статистика за всеки брокер, използвайки `std::map`.
 * Тя итерира през всички имоти, увеличавайки общия брой и броя на продадените имоти за всеки брокер.
 * Този подход динамично обработва произволен брой уникални брокери без ограниченията на масивите с фиксиран размер.
 * Накрая, тя представя процента на продадените имоти за всеки брокер, предлагайки ценни прозрения
 * за индивидуалната ефективност на брокерите.
 *
 * @param properties Постоянен масив от структури `Property` за анализ.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void soldPercentagePerBroker(const Property properties[], int propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_IN_SYSTEM") << RESET << endl;
        return;
    }

    
    
    std::map<std::string, std::pair<int, int>> brokerStats;

    
    for (int i = 0; i < propertyCount; i++) {
        const Property& prop = properties[i];
        std::string brokerName = prop.broker; 

        brokerStats[brokerName].first++; 
        if (prop.status == SOLD) {
            brokerStats[brokerName].second++; 
        }
    }

    cout << YELLOW << "\n--- " << getTranslatedString("SOLD_PERCENTAGE_PER_BROKER_HEADER") << " ---" << RESET << endl;

    
    
    for (const auto& pair : brokerStats) {
        const std::string& brokerName = pair.first;
        int totalBrokerProperties = pair.second.first;
        int soldBrokerProperties = pair.second.second;

        double soldBrokerPercentage = 0.0;
        if (totalBrokerProperties > 0) {
            soldBrokerPercentage = (static_cast<double>(soldBrokerProperties) / totalBrokerProperties) * 100.0;
        }

        cout << getTranslatedString("BROKER_LABEL") << ": " << brokerName << endl;
        cout << left; 
        cout << "  " << setw(20) << getTranslatedString("TOTAL_PROPERTIES_LABEL") << totalBrokerProperties << endl;
        cout << "  " << setw(20) << getTranslatedString("SOLD_PROPERTIES_LABEL") << soldBrokerProperties << endl;
        cout << "  " << setw(20) << getTranslatedString("SOLD_PERCENTAGE_LABEL") << fixed << setprecision(2) << soldBrokerPercentage << "%" << endl;
        cout << endl;
    }
}
