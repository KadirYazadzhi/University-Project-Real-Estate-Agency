/**
 * @file sort.cpp
 * @brief Имплементация на алгоритъма Quicksort за сортиране на масив от структури Property.
 *
 * Този файл предоставя конкретната имплементация за сортиране на данни за имоти
 * въз основа на полето `price`, във възходящ или низходящ ред.
 * Той включва основната функция за сортиране `sortPropertiesArray` и нейните
 * помощни функции: `swap`, `partition` и рекурсивната `quickSort`.
 */

#include <iostream> 

#include "sort.h"    
#include "structs.h" 

using namespace std;

/**
 * @brief Публична функция за иницииране на сортирането на масив от имоти.
 *
 * Тази функция служи като входна точка за сортиране. Тя проверява за базови случаи
 * (масив с 0 или 1 елемент вече е сортиран) и след това извиква рекурсивната
 * функция `quickSort` за извършване на действителната операция по сортиране. В момента
 * имотите се сортират въз основа на тяхната `price`.
 *
 * @param arr Масивът от структури `Property`, който ще бъде сортиран.
 * @param size Броят на елементите в масива.
 * @param sortAscending Булев флаг: `true` за сортиране във възходящ ред, `false` за низходящ.
 */
void sortPropertiesArray(struct Property arr[], int size, bool sortAscending) {
    if (size <= 1) return; 

    quickSort(arr, 0, size - 1, sortAscending); 
}

/**
 * @brief Разменя местата на две структури `Property` в паметта.
 *
 * Тази помощна функция се използва от алгоритъма за сортиране за ефективна размяна
 * на позициите на два елемента.
 *
 * @param a Указател към първата структура `Property`.
 * @param b Указател към втората структура `Property`.
 */
void swap(struct Property* a, struct Property* b) {
    Property temp = *a; 
    *a = *b;            
    *b = temp;          
}

/**
 * @brief Разделя подмасив около елемент на опора като част от алгоритъма Quicksort.
 *
 * Тази функция избира последния елемент за опора. Тя пренарежда елементите
 * в подмасива така, че всички елементи, удовлетворяващи условието за сортиране (напр.
 * по-малки от опората за възходящо сортиране) идват преди опората, а всички елементи,
 * които не удовлетворяват условието, идват след нея. Окончателната позиция на опората
 * след това се връща.
 *
 * @param arr Масивът от структури `Property`, който ще бъде разделен.
 * @param low Началният индекс на подмасива за разделяне.
 * @param high Крайният индекс на подмасива за разделяне.
 * @param sortAscending Булев флаг, указващ посоката на сортиране (`true` за възходящ).
 * @return Индексът на елемента на опора след разделянето.
 */
int partition(struct Property arr[], int low, int high, bool sortAscending) {
    double pivotPrice = arr[high].price; 
    int i = (low - 1); 

    
    for (int j = low; j <= high - 1; j++) {
        bool condition;

        
        if (sortAscending) {
            condition = (arr[j].price < pivotPrice); 
        } else {
            condition = (arr[j].price > pivotPrice); 
        }

        
        if (condition) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
}

/**
 * @brief Рекурсивна имплементация на алгоритъма Quicksort.
 *
 * Тази функция сортира подмасив от структури `Property` на място. Тя работи чрез
 * разделяне на масива около елемент на опора и след това рекурсивно извикване
 * на себе си за подмасивите преди и след опората. Базовият случай на рекурсията
 * е, когато подмасивът има 0 или 1 елемент.
 *
 * @param arr Масивът от структури `Property`, който ще бъде сортиран.
 * @param low Началният индекс на подмасива.
 * @param high Крайният индекс на подмасива.
 * @param sortAscending Булев флаг, указващ посоката на сортиране.
 */
void quickSort(struct Property arr[], int low, int high, bool sortAscending) {
    if (low >= high) return; 

    
    int pi = partition(arr, low, high, sortAscending);

    
    quickSort(arr, low, pi - 1, sortAscending);   
    quickSort(arr, pi + 1, high, sortAscending); 
}
