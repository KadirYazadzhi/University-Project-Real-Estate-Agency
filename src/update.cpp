/**
 * @file update.cpp
 * @brief Съдържа функции за редакция на данни на съществуващи имоти.
 */

#include <iostream>
#include <cstring>
#include <limits>

#include "structs.h"
#include "update.h"
#include "file.h"
#include "colors.h"

using namespace std;

/**
 * @brief Главно меню за редакция на имот.
 * 
 * Намира имот по референтен номер и предоставя меню с опции за редакция на отделните му полета.
 */
void updateProperty() {
    cout << CYAN << "Въведете реф. номер на имота, който искате да коригирате: " << RESET;
    int refNumber;
    cin >> refNumber;

    int index = getPropertyByRefNumber(refNumber);
    if (index == -1) {
        cout << RED << "Не беше намерен имот с реф. номер - " << refNumber << "." << RESET << endl;
        return;
    }

    // Проверка дали имотът е продаден - продаден имот не може да се редактира.
    if (properties[index].status == SOLD) {
        cout << RED << "Невалидна операция, имота е вече продаден - (Продаден имот не може да бъде редактиран)." << RESET << endl;
        return;
    }

    cout << CYAN << "Изберете кое искате да промените: " << RESET << endl;
    cout << "1. Реф. Номер" << endl;
    cout << "2. Брокер" << endl;
    cout << "3. Тип" << endl;
    cout << "4. Район" << endl;
    cout << "5. Изложение" << endl;
    cout << "6. Цена" << endl;
    cout << "7. Обща площ" << endl;
    cout << "8. Брой стаи" << endl;
    cout << "9. Етаж" << endl;
    cout << "10. Статус" << endl;
    cout << "0. Назад" << endl;
    cout << CYAN << "Въведете вашия избор: " << RESET;

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: updateRefNumber(index); break;
        case 2: updateStringField(index, properties[index].broker, 50, "брокер"); break;
        case 3: updateStringField(index, properties[index].type, 50, "тип"); break;
        case 4: updateStringField(index, properties[index].area, 50, "район"); break;
        case 5: updateStringField(index, properties[index].exposition, 20, "изложение"); break;
        case 6: updateNumericField(&properties[index].price, "цена"); break;
        case 7: updateNumericField(&properties[index].totalArea, "обща площ"); break;
        case 8: updateNumericField(&properties[index].rooms, "брой стаи"); break;
        case 9: updateNumericField(&properties[index].floor, "етаж"); break;
        case 10: updateStatus(index); break;
        default: return;
    }
}

/**
 * @brief Актуализира референтния номер на имот.
 * @param index Индексът на имота в глобалния масив `properties`.
 */
void updateRefNumber(int index) {
    cout << CYAN << "Въведете новия референтен номер: " << RESET;
    int newRefNumber;
    cin >> newRefNumber;

    if (getPropertyByRefNumber(newRefNumber) != -1) {
        cout << RED << "Невалидна операция. Вече съществува имот с този референтен номер." << RESET << endl;
        return;
    }

    properties[index].refNumber = newRefNumber;
    syncDataToRecoveryFiles(); // Синхронизация след промяна
}

/**
 * @brief Актуализира текстово поле на имот.
 * @param index Индексът на имота.
 * @param fieldPtr Указател към полето (напр. `properties[index].broker`).
 * @param maxSize Максимален размер на полето.
 * @param fieldName Име на полето за извеждане в съобщенията.
 */
void updateStringField(int index, char* fieldPtr, int maxSize, const char* fieldName) {
    cout << CYAN << "Въведете нова стойност за " << fieldName << " (до " << maxSize << "символа): " << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    char newValue[MAX_STRING_SIZE];
    cin.getline(newValue, maxSize);

    if (strcmp(fieldPtr, newValue) == 0) {
        cout << YELLOW << "Въведената стойност е идентична с текущата. Не е направена промяна." << RESET << endl;
        return;
    }

    strcpy(fieldPtr, newValue);

    cout << GREEN << "Стойността за " << fieldName << " беше успешно променена." << RESET << endl;
    syncDataToRecoveryFiles(); // Синхронизация след промяна
}

/**
 * @brief Шаблонна функция за актуализация на числово поле.
 * @tparam T Типът на числовото поле (int, double).
 * @param fieldPtr Указател към полето.
 * @param fieldName Име на полето за извеждане в съобщенията.
 */
template <typename T>
void updateNumericField(T* fieldPtr, const char* fieldName) {
    cout << CYAN << "Въведете нова стойност за " << fieldName << ":" << RESET;

    T newValue;
    if (!(cin >> newValue)) {
        cout << RED << "Невалиден вход. Операцията е отменена." << RESET << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (*fieldPtr == newValue) {
        cout << YELLOW << "Въведената стойност е идентична с текущата. Не е направена промяна." << RESET << endl;
        return;
    }

    *fieldPtr = newValue;
    cout << GREEN << "Стойността за " << fieldName << " беше успешно променена." << RESET << endl;
    syncDataToRecoveryFiles(); // Синхронизация след промяна
}

/**
 * @brief Актуализира статуса на имот.
 * При промяна от AVAILABLE на RESERVED, цената се намалява с 20%.
 * @param index Индексът на имота.
 */
void updateStatus(int index) {
    Status oldStatus = properties[index].status;

    cout << CYAN << "Изберете новия статус: " << RESET;
    cout << "   1. Наличен" << endl;
    cout << "   2. Продаден" << endl;
    cout << "   3. Капариран" << endl;
    cout << CYAN << "Въведете вашия избор: " << RESET;

    int choice;
    if (!(cin >> choice) || choice < 0 || choice > 3) {
        cout << RED << "Невалиден избор. Операцията е отменена." << RESET << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    Status newStatus;

    switch (choice) {
        case 1: newStatus = AVAILABLE; break;
        case 2: newStatus = SOLD; break;
        case 3: newStatus = RESERVED; break;
        default: return;
    }

    if (oldStatus == newStatus) {
        cout << YELLOW << "Имотът вече е в избрания статус. Не беше направена промяна." << RESET << endl;
        return;
    }

    // Ако имотът се капарира, цената се актуализира с 20% надолу.
    if (oldStatus == AVAILABLE && newStatus == RESERVED) {
        properties[index].price *= 0.8;
    }

    properties[index].status = newStatus;
    cout << GREEN << "Статуса беше успешно променен" << RESET << endl;
    syncDataToRecoveryFiles(); // Синхронизация след промяна
}

/**
 * @brief Намира индекса на имот по неговия референтен номер.
 * 
 * @param refNumber Референтният номер за търсене.
 * @return int Индексът на имота в масива, или -1 ако не е намерен.
 */
int getPropertyByRefNumber(int refNumber) {
    for (int i = 0; i < propertyCount; i++) {
        if (properties[i].refNumber == refNumber) return i;
    }

    return -1;
}
