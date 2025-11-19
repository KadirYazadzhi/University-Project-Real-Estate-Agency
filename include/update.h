/**
 * @file update.h
 * @brief Този файл декларира функции, отговорни за модифициране на съществуващи записи на имоти.
 *
 * Той предоставя функционалности за търсене на имот по неговия референтен номер
 * и след това актуализиране на различни атрибути на този имот, осигурявайки целостта на данните
 * чрез валидиране.
 */

#ifndef UPDATE_H
#define UPDATE_H

#include "structs.h" 
#include <iostream>  
#include <limits>    


#define MAX_STRING_SIZE 51 

/**
 * @brief Основна функция за актуализиране на детайлите на съществуващ имот.
 * @param properties Масив от структури Property.
 * @param propertyCount Текущият брой имоти в системата.
 *
 * Тази функция подканва потребителя да въведе референтен номер, търси имота
 * и след това позволява на потребителя да избере кои полета да актуализира.
 */
void updateProperty(Property properties[], int& propertyCount);

/**
 * @brief Търси имот по неговия референтен номер.
 * @param refNumber Референтният номер на имота, който да се намери.
 * @param properties Масив от структури Property, в който да се търси.
 * @param propertyCount Броят на имотите в масива.
 * @return Индексът на имота в масива, ако е намерен, в противен случай -1.
 *
 * Тази помощна функция е от решаващо значение за различни операции, които изискват локализиране на конкретен имот.
 */
int getPropertyByRefNumber(int refNumber, const Property properties[], int propertyCount);

/**
 * @brief Актуализира референтния номер на имот.
 * @param properties Масив от структури Property.
 * @param propertyCount Броят на имотите в масива.
 * @param index Индексът на имота за актуализиране.
 * @return true, ако референтният номер е успешно актуализиран, в противен случай false.
 *
 * Необходимо е специално обработване за refNumber, тъй като той трябва да остане уникален.
 */
bool updateRefNumber(Property properties[], int propertyCount, int index);

/**
 * @brief Актуализира текстово поле на имот с нов потребителски вход.
 * @param fieldPtr Указател към полето от символен масив, което да бъде актуализирано.
 * @param maxSize Максималният размер на символен масив (включително терминиращата нула).
 * @param fieldName Името на полето, което се актуализира (за потребителски подсказки).
 * @return true, ако полето е актуализирано, false, ако потребителят е избрал да не го променя.
 *
 * Този общ помощник опростява актуализирането на различни текстови атрибути на имот.
 */
bool updateStringField(char* fieldPtr, int maxSize, const char* fieldName);

/**
 * @brief Шаблонна функция за актуализиране на числово поле на имот с нов потребителски вход.
 * @tparam T Числовият тип на полето (напр. int, double).
 * @param fieldPtr Указател към числовото поле, което да бъде актуализирано.
 * @param fieldName Името на полето, което се актуализира (за потребителски подсказки).
 * @return true, ако полето е актуализирано, false, ако потребителят е избрал да не го променя.
 *
 * Осигурява валиден числов вход и предоставя последователен интерфейс за актуализиране на числови полета.
 */
template <typename T>
bool updateNumericField(T* fieldPtr, const char* fieldName) {
    std::cout << "Въведете нова стойност за " << fieldName << " (текуща: " << *fieldPtr << ", въведете -1 за запазване): ";
    T newValue;
    std::cin >> newValue;

    if (std::cin.fail()) {
        std::cout << "Невалиден вход. Запазена е текущата стойност.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (newValue != -1) { 
            *fieldPtr = newValue;
            return true;
        }
    }
    return false;
}

/**
 * @brief Актуализира статуса на даден имот.
 * @param propertyToUpdate Референция към структурата Property, чийто статус трябва да бъде актуализиран.
 * @return true, ако статусът е актуализиран, false, ако потребителят е избрал да не го променя или входът е невалиден.
 *
 * Предоставя ръководен процес за промяна на статуса на наличност на имота.
 */
bool updateStatus(Property& propertyToUpdate);

#endif 
