/**
 * @file add.cpp
 * @brief Имплементация на функции за добавяне на нови имоти в системата за управление на недвижими имоти.
 *
 * Този файл предоставя конкретната имплементация за добавяне на единични или множество имоти,
 * включително валидиране на вход, проверки на капацитета и санитизация на данни, за да се гарантира целостта на данните.
 */

#include <iostream>    
#include <limits>      
#include <cstring>     
#include <type_traits> 
#include <string>      
#include <algorithm>   

#include "add.h"       
#include "structs.h"   
#include "update.h"    
#include "file.h"      
#include "colors.h"    
#include "localization.h" 

using namespace std;

/**
 * @brief Санитизира C-стил низ, като премахва символи '|' (вертикална черта).
 *
 * Тази функция итерира през входния низ и конструира нов низ
 * на място, който изключва всички символи '|'. Това е от решаващо значение за предотвратяване на
 * проблеми при записване на данни в текстови файлове, разделени с '|' (напр. sync.txt),
 * тъй като символ '|' в поле за данни може да повреди файловия формат по време на анализиране.
 *
 * @param str C-стил низът, който да бъде санитизиран. Той се променя на място.
 */
void sanitizeString(char* str) {
    int len = strlen(str);
    int k = 0; 
    for (int i = 0; i < len; i++) {
        if (str[i] != '|') { 
            str[k++] = str[i];
        }
    }
    str[k] = '\0'; 
}

/**
 * @brief Ръководи потребителя при добавяне на единичен нов имот в системата.
 *
 * Тази функция подканва потребителя за всички необходими детайли на имот,
 * валидира входа, проверява за уникални референтни номера и след това добавя
 * имота към масива `properties`. Тя обработва потенциални грешки като
 * дублиращи се референтни номера или достигане на максимален капацитет.
 * След успешно добавяне, тя задейства синхронизация на данните за възстановяване.
 *
 * @param properties Масив от структури `Property`, където ще бъде добавен новият имот.
 * @param propertyCount Референция към цяло число, представляващо текущия брой имоти.
 */
void addSingleProperty(Property properties[], int& propertyCount) {
    
    if (isCapacityReached(propertyCount)) return;

    Property newProperty; 

    
    newProperty.refNumber = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_REF_NUMBER") + RESET).c_str());

    
    if (getPropertyByRefNumber(newProperty.refNumber, properties, propertyCount) != -1) {
        cout << RED << getTranslatedString("INVALID_DATA") << RESET << endl;
        cout << RED << getTranslatedString("ERROR_REF_NUMBER_EXISTS") << RESET << endl;
        cout << RED << getTranslatedString("ERROR_ADD_PROPERTY_FAILED") << RESET << endl;
        return; 
    }

    
    newProperty.price = getValidNumericInput<double>((CYAN + getTranslatedString("PROMPT_PRICE") + RESET).c_str());
    newProperty.totalArea = getValidNumericInput<double>((CYAN + getTranslatedString("PROMPT_TOTAL_AREA") + RESET).c_str());
    newProperty.rooms = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_ROOMS") + RESET).c_str());
    newProperty.floor = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_FLOOR") + RESET).c_str());

    
    
    getValidStringInput((CYAN + getTranslatedString("PROMPT_BROKER_NAME") + RESET).c_str(), newProperty.broker, sizeof(newProperty.broker));
    getValidStringInput((CYAN + getTranslatedString("PROMPT_PROPERTY_TYPE") + RESET).c_str(), newProperty.type, sizeof(newProperty.type));
    getValidStringInput((CYAN + getTranslatedString("PROMPT_PROPERTY_AREA") + RESET).c_str(), newProperty.area, sizeof(newProperty.area));
    getValidStringInput((CYAN + getTranslatedString("PROMPT_PROPERTY_EXPOSITION") + RESET).c_str(), newProperty.exposition, sizeof(newProperty.exposition));

    newProperty.status = AVAILABLE; 
    properties[propertyCount] = newProperty; 
    propertyCount++; 

    cout << endl;
    cout << GREEN << getTranslatedString("PROPERTY_ADDED_SUCCESS") << RESET << endl;
    syncDataToRecoveryFiles(properties, propertyCount); 
}

/**
 * @brief Позволява на потребителя да добавя множество имоти в системата в една сесия.
 *
 * Тази функция първо подканва потребителя за броя имоти, които желае да добави.
 * След това проверява дали добавянето на толкова имоти би надхвърлило максималния капацитет на системата.
 * Ако капацитетът би бил надхвърлен, тя информира потребителя и коригира броя на имотите,
 * които да се добавят, за да се поберат в наличното пространство. Тя многократно извиква `addSingleProperty` за всеки
 * имот, който трябва да бъде добавен.
 *
 * @param properties Масив от структури `Property`, където ще бъдат добавени новите имоти.
 * @param propertyCount Референция към цяло число, представляващо текущия брой имоти.
 */
void addMultipleProperties(Property properties[], int& propertyCount) {
    
    int n = getValidNumericInput<int>((CYAN + getTranslatedString("PROMPT_NUM_PROPERTIES_TO_ADD") + RESET).c_str());

    
    if (propertyCount + n > MAX_PROPERTIES) {
        int availableSpace = MAX_PROPERTIES - propertyCount;
        cout << RED << getTranslatedString("ERROR_ADD_EXCEEDS_MAX_PART1") << RESET << endl;
        cout << YELLOW << getTranslatedString("ERROR_ADD_EXCEEDS_MAX_PART2") << " " << availableSpace << " " << getTranslatedString("ERROR_ADD_EXCEEDS_MAX_PART3") << RESET << endl;
        n = availableSpace; 
    }
    
    
    if (n <= 0) {
        isCapacityReached(propertyCount);
        return;
    }

    
    for (int i = 0; i < n; i++) {
        cout << YELLOW << "\n--- " << getTranslatedString("PROMPT_ENTERING_PROPERTY") << " #" << (i + 1) << " " << getTranslatedString("PROMPT_OF") << " " << n << " ---" << RESET << endl;
        addSingleProperty(properties, propertyCount); 
    }
}

/**
 * @brief Проверява дали е достигнат максималният брой имоти (MAX_PROPERTIES).
 *
 * Тази функция сравнява текущия `propertyCount` с `MAX_PROPERTIES`.
 * Ако лимитът е достигнат, тя показва локализирани съобщения за грешка, указващи,
 * че не могат да бъдат добавени повече имоти.
 *
 * @param propertyCount Текущият брой имоти в системата.
 * @return `true`, ако капацитетът е достигнат; `false` в противен случай.
 */
bool isCapacityReached(int propertyCount) {
    if (propertyCount >= MAX_PROPERTIES) {
        cout << RED << getTranslatedString("ERROR_ADD_NOT_POSSIBLE") << RESET << endl;
        cout << RED << getTranslatedString("ERROR_MAX_CAPACITY_REACHED") << RESET << endl;
        return true;
    }
    return false;
}

/**
 * @brief Подканва потребителя за въвеждане на низ и го съхранява в буфер, със санитизация.
 *
 * Тази функция използва `cin.getline` за прочитане на ред текст от потребителя,
 * предотвратявайки препълване на буфера чрез ограничаване на входа до `bufferSize`. Ако входът
 * надвиши `bufferSize`, тя показва предупреждение. След прочитане, входният низ
 * се предава на `sanitizeString` за премахване на всякакви проблемни символи (като '|').
 *
 * @param prompt Съобщението, показвано на потребителя при поискване на вход.
 * @param buffer Буферът от символи за съхраняване на входния низ.
 * @param bufferSize Максималният размер на буфера, включително терминиращата нула.
 */
void getValidStringInput(const char* prompt, char* buffer, int bufferSize) {
    cout << prompt; 
    cin.getline(buffer, bufferSize); 
    if (cin.fail()) { 
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << YELLOW << getTranslatedString("WARNING_INPUT_TRUNCATED") << RESET << endl; 
    }
    sanitizeString(buffer); 
}
