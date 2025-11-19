/**
* @file search.cpp
 * @brief Имплементация на функции за търсене и филтриране на имоти по различни критерии.
 *
 * Този файл предоставя конкретната имплементация на функциите, декларирани в `search.h`,
 * позволявайки на потребителите да търсят имоти по име на брокер или по брой стаи.
 * Резултатите от търсенето могат да бъдат сортирани по цена, за да помогнат на потребителите
 * да анализират и сравняват обявите.
 */

#include <iostream>    
#include <limits>      
#include <cstring>     
#include <algorithm>   

#include "search.h"    
#include "structs.h"   
#include "display.h"   
#include "sort.h"      
#include "colors.h"    
#include "add.h"       
#include "localization.h" 

using namespace std;

/**
 * @brief Търси и показва имоти, обработени от конкретен брокер.
 *
 * Тази функция подканва потребителя за име на брокер. След това тя итерира през
 * масива от имоти, копирайки всички имоти, управлявани от указания брокер,
 * във временен масив. Ако са намерени имоти, тя допълнително подканва потребителя
 * да избере ред на сортиране (възходящ или низходящ по цена) и след това показва
 * намерените имоти.
 *
 * @param properties Масив от структури `Property` за търсене.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void searchByBroker(Property properties[], int propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_IN_SYSTEM") << RESET << endl;
        return;
    }

    char searchBroker[50];
    
    getValidStringInput((CYAN + getTranslatedString("PROMPT_SEARCH_BROKER_NAME") + RESET).c_str(), searchBroker, sizeof(searchBroker));

    Property tempProperties[MAX_PROPERTIES]; 
    int tempCount = 0;                       
    bool isFound = false;                    

    
    for (int i = 0; i < propertyCount; i++) {
        if (strcmp(searchBroker, properties[i].broker) == 0) {
            tempProperties[tempCount] = properties[i]; 
            tempCount++;
            isFound = true; 
        }
    }

    
    if (!isFound) {
        cout << RED << getTranslatedString("NO_PROPERTIES_FOUND_FOR_BROKER") << " '" << searchBroker << "'." << RESET << endl;
        return;
    }

    cout << CYAN << getTranslatedString("PROMPT_SORT_ORDER_BY_PRICE") << RESET << endl;
    cout << getTranslatedString("SORT_ORDER_ASCENDING") << endl;
    cout << getTranslatedString("SORT_ORDER_DESCENDING") << endl;
    
    int choice;
    bool isAscending;
    
    while (true) {
        choice = getValidNumericInput<int>((CYAN + getTranslatedString("ENTER_YOUR_CHOICE") + RESET).c_str());
        if (choice == 1 || choice == 2) {
            isAscending = (choice == 1); 
            break;
        }
        cout << RED << getTranslatedString("ERROR_INVALID_SORT_CHOICE") << RESET << endl;
    }

    
    sortPropertiesArray(tempProperties, tempCount, isAscending);

    cout << YELLOW << "\n--- " << getTranslatedString("SEARCH_RESULTS_FOR_BROKER") << " '" << searchBroker << "' (" << getTranslatedString("SORTED_BY_PRICE") << ") ---" << RESET << endl;
    
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- " << getTranslatedString("PROPERTY_HEADER") << " #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}

/**
 * @brief Търси и показва имоти въз основа на броя стаи.
 *
 * Тази функция подканва потребителя за брой стаи. След това тя итерира през
 * масива от имоти, копирайки всички имоти, които съвпадат с указания брой
 * стаи във временен масив. Ако са намерени имоти, те се сортират
 * по цена (по подразбиране низходящо, както е указано от `false` в извикването на `sortPropertiesArray`)
 * и след това се показват на потребителя.
 *
 * @param properties Масив от структури `Property` за търсене.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void searchByRooms(Property properties[], int propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_IN_SYSTEM") << RESET << endl;
        return;
    }

    int roomsCount = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_SEARCH_ROOMS_COUNT") + RESET).c_str());

    Property tempProperties[MAX_PROPERTIES]; 
    int tempCount = 0;                       
    bool isFound = false;                    

    
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].rooms == roomsCount) {
            tempProperties[tempCount] = properties[i]; 
            tempCount++;
            isFound = true; 
        }
    }

    
    if (!isFound) {
        cout << RED << getTranslatedString("NO_PROPERTIES_FOUND_FOR_ROOMS_COUNT_PART1") << " " << roomsCount << " " << getTranslatedString("NO_PROPERTIES_FOUND_FOR_ROOMS_COUNT_PART2") << RESET << endl;
        return;
    }

    
    sortPropertiesArray(tempProperties, tempCount, false);
    
    cout << YELLOW << "\n--- " << getTranslatedString("SEARCH_RESULTS_FOR_ROOMS_COUNT_PART1") << " " << roomsCount << " " << getTranslatedString("SEARCH_RESULTS_FOR_ROOMS_COUNT_PART2") << " (" << getTranslatedString("SORTED_BY_PRICE") << ") ---" << RESET << endl;
    
    for (int i = 0; i < tempCount; i++) {
        cout << YELLOW << "--- " << getTranslatedString("PROPERTY_HEADER") << " #" << i + 1 << RESET << endl;
        displayPropertyDetails(tempProperties[i]);
    }
}
