/**
 * @file structs.h
 * @brief Този файл дефинира основните структури от данни и изброявания, използвани в системата за управление на недвижими имоти.
 *
 * Включва дефиницията за структурата Property, която представлява единична обява за недвижим имот,
 * и изброяването Status, което указва наличността на имота.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>


#define MAX_PROPERTIES 100

/**
 * @brief Изброява възможните статуси на недвижим имот.
 *
 * - SOLD: Имотът е продаден и вече не е наличен.
 * - RESERVED: Имотът е временно запазен за потенциален купувач.
 * - AVAILABLE: Имотът е в момента на пазара и може да бъде закупен.
 */
enum Status { SOLD, RESERVED, AVAILABLE };

/**
 * @brief Преобразува стойност от изброявания тип Status в съответното ѝ текстово представяне.
 * @param status Стойността на Status за преобразуване.
 * @return Постоянна референция към низ, представляващ статуса.
 */
const std::string& getStatusString(enum Status status);

/**
 * @brief Представлява единичен недвижим имот с различни атрибути.
 */
struct Property {
    int refNumber;      
    char broker[50];    
    char type[50];      
    char area[50];      
    char exposition[20];
    double price;       
    double totalArea;   
    int rooms;          
    int floor;          
    enum Status status; 
};

#endif 
