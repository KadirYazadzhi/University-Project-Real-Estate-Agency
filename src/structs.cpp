/**
 * @file structs.cpp
 * @brief Дефинира глобалните променливи и помощни функции, свързани със структурите.
 */

#include "structs.h"

// Глобален масив, съхраняващ всички имоти в системата.
Property properties[MAX_PROPERTIES];

// Глобална променлива, която следи текущия брой на имотите в масива.
int propertyCount = 0;

/**
 * @brief Преобразува стойност от изброимия тип Status в четим низ.
 * 
 * @param status Статусът за преобразуване.
 * @return const char* Низ, представляващ статуса на български език.
 */
const char* getStatusString(enum Status status) {
    switch (status) {
        case SOLD:
            return "Продаден";
        case RESERVED:
            return "Капариран";
        case AVAILABLE:
            return "Свободен";
        default:
            return "Неизвестен";
    }
}