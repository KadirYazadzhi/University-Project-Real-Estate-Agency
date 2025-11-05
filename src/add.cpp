/**
 * @file add.cpp
 * @brief Съдържа функциите за добавяне на нови имоти в системата.
 *
 * Включва логика за добавяне на единични и множествено имоти, както и помощни функции
 * за валидация на потребителския вход.
 */

#include <iostream>
#include <limits>
#include <cstring>
#include "add.h"
#include "structs.h"
#include "update.h"
#include "file.h"
#include "colors.h"

using namespace std;

/**
 * @brief Премахва символа '|' от подаден символен низ.
 * 
 * Функцията е необходима, за да се предотврати повреждане на структурата на
 * текстовия recovery файл, който използва този символ за разделител.
 * @param str Указател към символния низ за "почистване".
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
 * @brief Добавя един нов имот в системата.
 * 
 * Изисква от потребителя да въведе всички данни за имота, като използва
 * помощни функции за валидация на числовия вход и почистване на текстовия.
 * След успешно добавяне, автоматично синхронизира промените с recovery файловете.
 */
void addSingleProperty() {
    if (isCapacityReached()) return;

    Property newProperty;

    newProperty.refNumber = getValidIntInput(CYAN "Въведете реф. номер: " RESET);

    // Проверка дали вече съществува имот със същия референтен номер
    if (getPropertyByRefNumber(newProperty.refNumber) != -1) {
        cout << RED << "Невалидни данни." << RESET << endl;
        cout << RED << "Вече съществува имот с този реф. номер." << RESET << endl;
        cout << RED << "Неуспешно добавяне на имот." << RESET << endl;
        return;
    }

    newProperty.price = getValidDoubleInput(CYAN "Въведете цена на имота: " RESET);
    newProperty.totalArea = getValidDoubleInput(CYAN "Въведете обща площ на имота: " RESET);
    newProperty.rooms = getValidIntInput(CYAN "Въведете брой стаи: " RESET);
    newProperty.floor = getValidIntInput(CYAN "Въведете етаж: " RESET);

    cout << CYAN << "Въведете име на брокера: " << RESET;
    cin.getline(newProperty.broker, 50);
    sanitizeString(newProperty.broker);

    cout << CYAN << "Въведете тип на имота: " << RESET;
    cin.getline(newProperty.type, 50);
    sanitizeString(newProperty.type);

    cout << CYAN << "Въведете район на имота: " << RESET;
    cin.getline(newProperty.area, 50);
    sanitizeString(newProperty.area);

    cout << CYAN << "Въведете изложение на имота: " << RESET;
    cin.getline(newProperty.exposition, 20);
    sanitizeString(newProperty.exposition);

    newProperty.status = AVAILABLE; // По подразбиране нов имот е със статус "Свободен"
    properties[propertyCount] = newProperty;
    propertyCount++;

    cout << endl;
    cout << GREEN << "Имота беше успешно въведен." << RESET << endl;
    syncDataToRecoveryFiles(); // Автоматично запазване на промените
}

/**
 * @brief Добавя множество нови имоти.
 * 
 * Потребителят въвежда брой имоти 'n', след което функцията извиква
 * `addSingleProperty()` в цикъл n на брой пъти.
 */
void addMultipleProperties() {
    int n = getValidIntInput(CYAN "Въведете броя на имотите, които желаете да добавите: " RESET);

    if (propertyCount + n > MAX_PROPERTIES) {
        int availableSpace = MAX_PROPERTIES - propertyCount;
        cout << RED << "Броя имоти, които желаете да въведете надвишават максимума от 100 имота." << RESET << endl;
        cout << YELLOW << "Системата има свободно място за " << availableSpace << " имота." << RESET << endl;

        n = availableSpace;

        if (n <= 0) {
            isCapacityReached();
            return;
        }
    }

    for (int i = 0; i < n; i++) {
        cout << YELLOW << "\n--- Въвеждане на имот #" << (i + 1) << " от " << n << " ---" << RESET << endl;
        addSingleProperty();
    }
}

/**
 * @brief Приема и валидира вход за цяло число (int).
 * 
 * В цикъл изисква вход от потребителя, докато не бъде въведено валидно цяло число.
 * @param prompt Съобщение, което да се изведе на потребителя.
 * @return int Валидираната стойност.
 */
int getValidIntInput(const char* prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << RED << "Невалиден вход. Моля, въведете цяло число: " << RESET;
        cin.clear(); // Изчиства флаговете за грешка на cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорира остатъка от грешния вход в буфера
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Почиства буфера след успешно четене
    return value;
}

/**
 * @brief Приема и валидира вход за реално число (double).
 * 
 * В цикъл изисква вход от потребителя, докато не бъде въведено валидно реално число.
 * @param prompt Съобщение, което да се изведе на потребителя.
 * @return double Валидираната стойност.
 */
double getValidDoubleInput(const char* prompt) {
    double value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << RED << "Невалиден вход. Моля, въведете число: " << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

/**
 * @brief Проверява дали е достигнат максималният капацитет на масива с имоти.
 * 
 * @return true Ако капацитетът е достигнат.
 * @return false Ако все още има свободно място.
 */
bool isCapacityReached() {
    if (propertyCount >= MAX_PROPERTIES) {
        cout << RED << "Добавянето на имот не е възможно." << RESET << endl;
        cout << RED << "Имотите, които до тук сте въвели са достигнали максимума от 100 имота." << RESET << endl;

        return true;
    }

    return false;
}