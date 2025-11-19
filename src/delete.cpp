/**
 * @file delete.cpp
 * @brief Implementation of functions for deleting property records from the system.
 *
 * This file provides the concrete implementation for functions declared in `delete.h`,
 * enabling users to remove individual properties by reference number or to clear
 * all properties from the system, with appropriate user confirmations and
 * data synchronization for recovery.
 */

#include <iostream>    
#include <cctype>      
#include <limits>      
#include <string>      

#include "delete.h"    
#include "structs.h"   
#include "update.h"    
#include "file.h"      
#include "colors.h"    
#include "add.h"       
#include "utils.h"     
#include "localization.h" 

using namespace std;

/**
 * @brief Изтрива един имот от системата въз основа на неговия референтен номер.
 *
 * Тази функция първо проверява дали има имоти за изтриване. След това тя
 * подканва потребителя за референтен номер на имот, търси го и
 * ако бъде намерен, иска потвърждение, преди да продължи с изтриването.
 * При потвърждение, имотът се премахва чрез изместване на следващите елементи
 * в масива, а `propertyCount` се намалява. След това данните се
 * синхронизират за целите на възстановяването.
 *
 * @param properties Масив от структури `Property`, от които да се изтрива.
 * @param propertyCount Референция към цяло число, представляващо текущия брой имоти.
 */
void DeleteProperty(Property properties[], int& propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_TO_DELETE") << RESET << endl;
        return;
    }

    
    int refNumber = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_REF_NUMBER_TO_DELETE") + RESET).c_str());

    
    int index = getPropertyByRefNumber(refNumber, properties, propertyCount);
    if (index == -1) {
        cout << RED << getTranslatedString("ERROR_PROPERTY_NOT_FOUND_REF") << RESET << endl;
        return;
    }

    
    if (!getConfirmation((getTranslatedString("CONFIRM_DELETE_PROPERTY_PART1") + " - " + to_string(refNumber) + "? [y/n]: ").c_str())) {
        cout << YELLOW << getTranslatedString("DELETE_CANCELLED") << RESET << endl;
        return;
    }

    
    for (int i = index; i < propertyCount - 1; i++) {
        properties[i] = properties[i + 1];
    }

    propertyCount--; 

    cout << GREEN << getTranslatedString("PROPERTY_DELETED_SUCCESS_PART1") << " " << refNumber << " " << getTranslatedString("PROPERTY_DELETED_SUCCESS_PART2") << RESET << endl;
    syncDataToRecoveryFiles(properties, propertyCount); 
}

/**
 * @brief Изтрива всички имоти, съхранени в момента в системата.
 *
 * Тази функция първо проверява дали има имоти за изтриване. След това тя
 * иска изрично потвърждение от потребителя, преди да изчисти всички имоти.
 * При потвърждение, `propertyCount` се нулира до нула, което ефективно изчиства
 * масива от имоти. След това данните се синхронизират за целите на възстановяването.
 * Тази операция е необратима без възстановяване от резервни файлове.
 *
 * @param properties Масив от структури `Property`, които да бъдат изчистени. (Съдържанието е логически изчистено чрез propertyCount = 0)
 * @param propertyCount Референция към цяло число, представляващо текущия брой имоти.
 */
void DeleteAllProperties(Property properties[], int& propertyCount) {
    if (propertyCount == 0) {
        cout << YELLOW << getTranslatedString("NO_PROPERTIES_TO_DELETE") << RESET << endl;
        return;
    }

    
    if (!getConfirmation(getTranslatedString("CONFIRM_DELETE_ALL_PROPERTIES").c_str())) {
        cout << YELLOW << getTranslatedString("DELETE_ALL_CANCELLED") << RESET << endl;
        return;
    }

    propertyCount = 0; 

    cout << GREEN << getTranslatedString("ALL_PROPERTIES_DELETED_SUCCESS") << RESET << endl;
    syncDataToRecoveryFiles(properties, propertyCount); 
}