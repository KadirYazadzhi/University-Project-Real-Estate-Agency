/**
 * @file display.cpp
 * @brief Implementation of functions responsible for displaying property information to the console.
 *
 * This file provides the concrete implementation for functions declared in `display.h`,
 * enabling the presentation of property details in various formats, such as
 * listing all properties, filtering sold properties, identifying the largest properties,
 * and displaying individual property details. It also includes a utility to check
 * if the property list is empty before attempting display operations.
 */

#include <iostream>    
#include <iomanip>     

#include "display.h"   
#include "structs.h"   
#include "colors.h"    
#include "localization.h" 

using namespace std;

/**
 * @brief Проверява дали в системата са заредени имоти.
 *
 * Тази функция е обща защита за операции за показване и други свързани с имоти
 * операции. Ако `propertyCount` е нула, тя отпечатва локализирано съобщение за грешка,
 * указващо, че няма налични имоти за заявената операция.
 *
 * @param propertyCount Текущият брой имоти в системата.
 * @return `true`, ако не са заредени имоти (propertyCount е 0); `false` в противен случай.
 */
bool isPropertiesEmpty(int propertyCount) {
    if (propertyCount == 0) {
        cout << RED << getTranslatedString("ERROR_OPERATION_NOT_POSSIBLE_NO_PROPERTIES") << RESET << endl;
        return true;
    }
    return false;
}

/**
 * @brief Показва подробната информация за един имот на конзолата.
 *
 * Тази функция форматира и отпечатва всички атрибути на дадена структура `Property`.
 * Тя използва `iomanip` за последователно разстояние и прецизност на плаващата запетая, и
 * `localization.h` за показване на имена на полета и статус на текущия език.
 * Това осигурява стандартизирано и удобно за потребителя представяне на данните за имоти.
 *
 * @param prop Постоянна референция към структурата `Property`, чиито детайли трябва да бъдат показани.
 */
void displayPropertyDetails(const Property &prop) {
    cout << left; 
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_REF_NUMBER") << prop.refNumber << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_BROKER_NAME") << prop.broker << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_TYPE") << prop.type << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_AREA") << prop.area << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_EXPOSITION") << prop.exposition << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_PRICE") << fixed << setprecision(2) << prop.price << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_TOTAL_AREA") << fixed << setprecision(2) << prop.totalArea << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_ROOMS") << prop.rooms << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_FLOOR") << prop.floor << endl;
    cout << "  " << setw(15) << getTranslatedString("PROPERTY_DETAILS_STATUS") << getStatusString(prop.status) << endl;
    cout << endl;
}

/**
 * @brief Показва всички имоти, които в момента са съхранени в системата.
 *
 * Тази функция първо проверява дали списъкът с имоти е празен. Ако не е,
 * тя итерира през целия масив от имоти и извиква
 * `displayPropertyDetails` за всеки от тях. Всеки имот е предшестван от
 * локализирано заглавие, указващо неговия пореден номер в списъка.
 *
 * @param properties Постоянен масив от структури `Property` за показване.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void displayAllProperties(const Property properties[], int propertyCount) {
    if (isPropertiesEmpty(propertyCount)) return; 

    
    for (int i = 0; i < propertyCount; i++) {
        cout << YELLOW << "--- " << getTranslatedString("PROPERTY_HEADER") << " #" << i + 1 << " ---" << RESET << endl;
        displayPropertyDetails(properties[i]);
    }
}

/**
 * @brief Показва само имотите, които са маркирани като ПРОДАДЕНИ.
 *
 * Тази функция проверява дали списъкът с имоти е празен. Ако не е, тя итерира
 * през всички имоти, показвайки само тези, чийто `status` е `SOLD`.
 * Тя включва локализирано заглавие и брои намерените продадени имоти.
 * Ако не бъдат намерени продадени имоти, се показва специфично локализирано съобщение.
 *
 * @param properties Постоянен масив от структури `Property` за филтриране и показване.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void displaySoldProperties(const Property properties[], int propertyCount) {
    if (isPropertiesEmpty(propertyCount)) return; 

    bool isFound = false; 
    int foundCount = 0;   

    
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].status == SOLD) {
            if (!isFound) { 
                cout << YELLOW << "--- " << getTranslatedString("SOLD_PROPERTIES_HEADER") << " ---" << RESET << endl;
                isFound = true;
            }
            cout << YELLOW << "#" << ++foundCount << RESET << endl; 
            displayPropertyDetails(properties[i]);
        }
    }

    
    if (!isFound) {
        cout << YELLOW << getTranslatedString("NO_SOLD_PROPERTIES_CURRENTLY") << RESET << endl;
    }
}

/**
 * @brief Показва имоти с най-голяма обща площ.
 *
 * Тази функция първо проверява дали списъкът с имоти е празен. Ако не е, тя определя
 * максималната `totalArea` сред всички имоти. След това тя итерира през
 * имотите отново, показвайки детайлите на всички имоти, които съвпадат с тази
 * максимална площ. Тя обработва случаи, когато няма имоти с положителна площ.
 *
 * @param properties Постоянен масив от структури `Property` за анализ и показване.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void displayLargestProperties(const Property properties[], int propertyCount) {
    if (isPropertiesEmpty(propertyCount)) return; 

    double maxTotalArea = 0.0; 
    
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea > maxTotalArea) {
            maxTotalArea = properties[i].totalArea;
        }
    }

    
    if (maxTotalArea == 0.0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_WITH_AREA_GREATER_THAN_ZERO") << RESET << endl;
        return;
    }

    cout << YELLOW << "--- " << getTranslatedString("LARGEST_PROPERTIES_HEADER") << " (" << fixed << setprecision(2) << maxTotalArea << " " << getTranslatedString("SQ_M") << ") ---" << RESET << endl;
    int foundCount = 0; 
    
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].totalArea == maxTotalArea) {
            cout << YELLOW << "#" << ++foundCount << RESET << endl; 
            displayPropertyDetails(properties[i]);
        }
    }
}