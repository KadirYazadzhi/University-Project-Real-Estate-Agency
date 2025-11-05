#ifndef SORT_H
#define SORT_H

#include "structs.h"

void sortPropertiesArray(struct Property arr[], int size, bool sortAscending);
int partition(struct Property arr[], int low, int high, bool sortAscending);
void quickSort(struct Property arr[], int low, int high, bool sortAscending);
void swap(struct Property* a, struct Property* b);

#endif //SORT_H
