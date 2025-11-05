#ifndef UPDATE_H
#define UPDATE_H

#define MAX_STRING_SIZE 51 // 50 за данни + 1 за '\0'

void updateProperty();
int getPropertyByRefNumber(int refNumber);
void updateRefNumber(int index);
void updateStringField(int index, char* fieldPtr, int maxSize, const char* fieldName);
template <typename T>void updateNumericField(T* fieldPtr, const char* fieldName);
void updateStatus(int index);

#endif //UPDATE_H
