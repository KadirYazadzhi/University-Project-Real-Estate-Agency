#ifndef FILE_H
#define FILE_H


void saveToBinaryFile();
void loadFromBinaryFile();
void saveToRecoveryBinaryFile();
void saveToUserFriendlyTextFile();
void saveToSyncTextFile();
void loadFromSyncTextFile();
bool confirmOverwrite(const char* filename);
void syncDataToRecoveryFiles();

#endif
