#ifndef ADD_H
#define ADD_H

void addSingleProperty();
void addMultipleProperties();
int getValidIntInput(const char* prompt);
double getValidDoubleInput(const char* prompt);
bool isCapacityReached();
void sanitizeString(char* str);

#endif //ADD_H
