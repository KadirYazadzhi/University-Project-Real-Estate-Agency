

#include <iostream>

#include "sort.h"

using namespace std;


void sortPropertiesArray(struct Property arr[], int size, bool sortAscending) {


    quickSort(arr, 0, size - 1, sortAscending);
}


void swap(struct Property* a, struct Property* b) {
    Property temp = *a;
    *a = *b;
    *b = temp;
}


int partition(struct Property arr[], int low, int high, bool sortAscending) {

    int i = (low - 1);

    for (int j = low; j <= high- 1; j++) {
        bool condition;


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


void quickSort(struct Property arr[], int low, int high, bool sortAscending) {



    int pi = partition(arr, low, high, sortAscending);


    quickSort(arr, low, pi - 1, sortAscending);
    quickSort(arr, pi + 1, high, sortAscending);
}
