/**
 * @file sort.cpp
 * @brief Съдържа реализация на Quicksort алгоритъм за сортиране на имоти.
 */

#include <iostream>

#include "sort.h"

using namespace std;

/**
 * @brief Публична функция за стартиране на сортирането.
 * 
 * @param arr Масивът от имоти, който ще се сортира.
 * @param size Размерът на масива.
 * @param sortAscending true за сортиране в нарастващ ред, false за намаляващ.
 */
void sortPropertiesArray(struct Property arr[], int size, bool sortAscending) {
    if (size <= 1) return; // Масив с 0 или 1 елемент вече е сортиран

    quickSort(arr, 0, size - 1, sortAscending);
}

/**
 * @brief Помощна функция за размяна на два елемента.
 * 
 * @param a Указател към първия имот.
 * @param b Указател към втория имот.
 */
void swap(struct Property* a, struct Property* b) {
    Property temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Част от Quicksort: разделя масива спрямо избран елемент (pivot).
 * 
 * @param arr Масивът за разделяне.
 * @param low Долна граница на частта от масива.
 * @param high Горна граница на частта от масива.
 * @param sortAscending Посока на сортиране.
 * @return int Индексът на pivot елемента след разделянето.
 */
int partition(struct Property arr[], int low, int high, bool sortAscending) {
    double pivotPrice = arr[high].price; // Избираме последния елемент за pivot
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++) {
        bool condition;

        // Условието за размяна зависи от посоката на сортиране
        if (sortAscending) condition = (arr[j].price < pivotPrice);
        else condition = (arr[j].price > pivotPrice);

        if (condition) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * @brief Рекурсивна реализация на Quicksort алгоритъма.
 * 
 * @param arr Масивът за сортиране.
 * @param low Долна граница.
 * @param high Горна граница.
 * @param sortAscending Посока на сортиране.
 */
void quickSort(struct Property arr[], int low, int high, bool sortAscending) {
    if (low >= high) return; // Дъно на рекурсията

    // Намираме индекса на pivot елемента
    int pi = partition(arr, low, high, sortAscending);

    // Рекурсивно сортираме двете части на масива
    quickSort(arr, low, pi - 1, sortAscending);
    quickSort(arr, pi + 1, high, sortAscending);
}
