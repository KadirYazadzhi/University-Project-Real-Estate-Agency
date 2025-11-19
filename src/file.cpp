/**
 * @file file.cpp
 * @brief Имплементация на всички файлови операции за системата за управление на недвижими имоти.
 *
 * Този файл обработва записването и зареждането на данни за имоти от и във различни файлови формати,
 * включително двоични файлове за ефективно съхранение, удобни за потребителя текстови файлове за отчети,
 * и файлове за синхронизация за възстановяване на данни. Той също така включва помощни програми за
 * управление на директории и потвърждение за презаписване на съществуващи файлове.
 */

#include <iostream>    
#include <cstring>     
#include <fstream>     
#include <limits>      
#include <cstdlib>     
#include <string>      
#include <algorithm>   

#include "file.h"      
#include "structs.h"   
#include "colors.h"    
#include "utils.h"     
#include "localization.h" 


using namespace std;




#ifdef PROJECT_ROOT_DIR
const string BASE_DIR = PROJECT_ROOT_DIR;
#else


const string BASE_DIR = ".";
#endif


const string USER_BINARY_FILENAME = BASE_DIR + "/data/properties_backup.dat"; 
const string USER_TEXT_FILENAME = BASE_DIR + "/data/properties_report.txt";   
const string SYNC_TEXT_FILENAME = BASE_DIR + "/data/recovery/sync.txt";       
const string SYNC_BINARY_FILENAME = BASE_DIR + "/data/recovery/sync.dat";     

/**
 * @brief Записва текущия масив от имоти в указан от потребителя двоичен файл.
 *
 * Тази функция първо гарантира, че целевата директория съществува. След това подканва
 * потребителя за потвърждение, ако файлът вече съществува, за да се предотвратят случайни презаписвания.
 * Данните за имотите се записват в суров двоичен формат, започвайки с `propertyCount`
 * и последвани от масива от структури `Property`.
 *
 * @param properties Постоянен масив от структури `Property` за запис.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void saveToBinaryFile(const Property properties[], int propertyCount) {
    
    ensureDirectoryExists((BASE_DIR + "/data").c_str());

    
    if (!confirmOverwrite(USER_BINARY_FILENAME.c_str())) {
        cout << YELLOW << getTranslatedString("SAVE_CANCELLED") << RESET << endl;
        return; 
    }

    
    FILE* binaryFile = fopen(USER_BINARY_FILENAME.c_str(), "wb");
    if (binaryFile == NULL) {
        cout << RED << getTranslatedString("ERROR_OPEN_FILE_WRITE") << RESET << endl;
        return; 
    }

    
    fwrite(&propertyCount, sizeof(int), 1, binaryFile);
    
    if (propertyCount > 0) {
        fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    }
    fclose(binaryFile); 

    cout << GREEN << getTranslatedString("DATA_SAVED_SUCCESS") << " '" << USER_BINARY_FILENAME << "'." << RESET << endl;
}

/**
 * @brief Зарежда данни за имоти от указан от потребителя двоичен файл в паметта на приложението.
 *
 * Тази функция се опитва да отвори двоичен файл и да прочете данни за имоти от него.
 * Тя прочита броя на имотите първо, след това масива от структури `Property`.
 * Включена е надеждна проверка за грешки за обработка на случаи, когато файлът не е намерен,
 * е повреден или ако броят на имотите надвишава капацитета на приложението.
 *
 * @param properties Масив от структури `Property`, където ще бъдат съхранени заредените данни.
 * @param propertyCount Референция към цяло число, което ще бъде актуализирано с броя на заредените имоти.
 */
void loadFromBinaryFile(Property properties[], int& propertyCount) {
    
    FILE* binaryFile = fopen(USER_BINARY_FILENAME.c_str(), "rb");
    if (binaryFile == NULL) {
        cout << RED << getTranslatedString("ERROR_FILE_NOT_FOUND") << RESET << endl;
        propertyCount = 0; 
        return;
    }

    
    size_t readCount = fread(&propertyCount, sizeof(int), 1, binaryFile);
    
    if (readCount != 1 || propertyCount < 0) {
        cout << RED << getTranslatedString("ERROR_FILE_CORRUPTED") << RESET << endl;
        fclose(binaryFile);
        propertyCount = 0;
        return;
    }

    
    if (propertyCount > 0 && propertyCount <= MAX_PROPERTIES) {
        size_t readProps = fread(properties, sizeof(Property), propertyCount, binaryFile);
        
        if (readProps != (size_t)propertyCount) {
            cout << RED << getTranslatedString("ERROR_FILE_CORRUPTED_INCOMPLETE") << RESET << endl;
            propertyCount = 0;
            return;
        }

        cout << GREEN << getTranslatedString("DATA_LOADED_SUCCESS_COUNT") << " " << propertyCount << " " << getTranslatedString("DATA_LOADED_SUCCESS_FROM_FILE") << " '" << USER_BINARY_FILENAME << "'." << RESET << endl;
    } else if (propertyCount > MAX_PROPERTIES) {
        
        cout << RED << getTranslatedString("ERROR_CAPACITY_EXCEEDED_FILE_COUNT") << " (" << propertyCount << ") " << getTranslatedString("ERROR_CAPACITY_EXCEEDED_MAX_CAPACITY") << RESET << endl;
        propertyCount = 0; 
    } else { 
        cout << YELLOW << getTranslatedString("FILE_EMPTY_LOADED_ZERO") << RESET << endl;
        propertyCount = 0;
    }

    fclose(binaryFile); 
}

/**
 * @brief Записва текущия масив от имоти в четим текстов файл като отчет.
 *
 * Тази функция форматира данните за имоти в табличен текстов отчет. Тя включва
 * заглавки и разделители за четимост и използва локализирани низове за имена на колони.
 * Подобно на `saveToBinaryFile`, тя проверява за съществуване на директория и потвърждава презаписвания.
 *
 * @param properties Постоянен масив от структури `Property` за докладване.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void saveToUserFriendlyTextFile(const Property properties[], int propertyCount) {
    
    ensureDirectoryExists((BASE_DIR + "/data").c_str());
    
    if (!confirmOverwrite(USER_TEXT_FILENAME.c_str())) return;

    
    FILE* file = fopen(USER_TEXT_FILENAME.c_str(), "w");
    if (file == NULL) {
        cout << RED << getTranslatedString("ERROR_OPEN_TEXT_FILE_WRITE") << RESET << endl;
        return;
    }

    
    fprintf(file, "---------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(file, "| %-4s | %-15s | %-15s | %-15s | %-10s | %-12s | %-10s | %-5s | %-5s | %-10s |\n",
            getTranslatedString("REF_SHORT").c_str(), getTranslatedString("BROKER_SHORT").c_str(), getTranslatedString("TYPE_SHORT").c_str(), getTranslatedString("AREA_SHORT").c_str(), getTranslatedString("EXPOSITION_SHORT").c_str(),
            getTranslatedString("PRICE_SHORT").c_str(), getTranslatedString("TOTAL_AREA_SHORT").c_str(), getTranslatedString("ROOMS_SHORT").c_str(), getTranslatedString("FLOOR_SHORT").c_str(), getTranslatedString("STATUS_SHORT").c_str());
    fprintf(file, "---------------------------------------------------------------------------------------------------------------------------------\n");

    
    for (int i = 0; i < propertyCount; i++) {
        const Property& prop = properties[i];
        fprintf(file, "| %-4d | %-15s | %-15s | %-15s | %-10s | %-12.2f | %-10.2f | %-5d | %-5d | %-10s |\n",
                prop.refNumber, prop.broker, prop.type, prop.area, prop.exposition,
                prop.price, prop.totalArea, prop.rooms, prop.floor, getStatusString(prop.status).c_str());
    }

    fclose(file); 
    cout << GREEN << getTranslatedString("DATA_EXPORTED_SUCCESS") << " '" << USER_TEXT_FILENAME << "'." << RESET << endl;
}

/**
 * @brief Записва текущия масив от имоти в обикновен текстов файл за синхронизация/възстановяване.
 *
 * Тази функция записва данни за имоти в текстов файл във формат, разделен с | (pipe).
 * Този формат е оптимизиран за програмно анализиране по време на зареждане/възстановяване на данни,
 * а не за човешка четимост. Обработката на грешки при отваряне на файл е минимална, тъй като
 * това е предимно вътрешен механизъм за възстановяване.
 *
 * @param properties Постоянен масив от структури `Property` за синхронизация.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void saveToSyncTextFile(const Property properties[], int propertyCount) {
    
    FILE* file = fopen(SYNC_TEXT_FILENAME.c_str(), "w");
    if (file == NULL) return; 

    
    for (int i = 0; i < propertyCount; i++) {
        const Property& prop = properties[i];
        fprintf(file, "%d|%s|%s|%s|%s|%.2f|%.2f|%d|%d|%d\n",
                prop.refNumber, prop.broker, prop.type, prop.area, prop.exposition,
                prop.price, prop.totalArea, prop.rooms, prop.floor, prop.status);
    }
    fclose(file); 
}

/**
 * @brief Зарежда данни за имоти от текстов файл за синхронизация в паметта на приложението.
 *
 * Тази функция прочита данни за имоти от текстов файл, разделен с | (pipe). Тя анализира
 * всеки ред, конструира структури `Property` и ги съхранява във временен масив.
 * След зареждане, тя подканва потребителя за потвърждение за зареждане на тези имоти
 * в основните данни на приложението, осигурявайки механизъм за възстановяване.
 *
 * @param properties Масив от структури `Property`, където ще бъдат съхранени заредените данни.
 * @param propertyCount Референция към цяло число, което ще бъде актуализирано с броя на заредените имоти.
 */
void loadFromSyncTextFile(Property properties[], int& propertyCount) {
    
    FILE* file = fopen(SYNC_TEXT_FILENAME.c_str(), "r");
    if (!file) {
        propertyCount = 0; 
        return;
    }

    Property tempProps[MAX_PROPERTIES]; 
    int tempCount = 0;                  

    
    while (tempCount < MAX_PROPERTIES) {
        int ref, rooms, floor, statusInt;
        double price, totalArea;
        char broker[50], type[50], area[50], exposition[20];

        
        
        
        int itemsRead = fscanf(file,
            "%d|%49[^|]|%49[^|]|%49[^|]|%19[^|]|%lf|%lf|%d|%d|%d\n",
            &ref, broker, type, area, exposition,
            &price, &totalArea, &rooms, &floor, &statusInt);

        if (itemsRead != 10) {
            
            break;
        }

        
        Property& prop = tempProps[tempCount];
        prop.refNumber = ref;
        prop.price = price;
        prop.totalArea = totalArea;
        prop.rooms = rooms;
        prop.floor = floor;

        
        
        strncpy(prop.broker, broker, sizeof(prop.broker) - 1);
        prop.broker[sizeof(prop.broker) - 1] = '\0';
        strncpy(prop.type, type, sizeof(prop.type) - 1);
        prop.type[sizeof(prop.type) - 1] = '\0';
        strncpy(prop.area, area, sizeof(prop.area) - 1);
        prop.area[sizeof(prop.area) - 1] = '\0';
        strncpy(prop.exposition, exposition, sizeof(prop.exposition) - 1);
        prop.exposition[sizeof(prop.exposition) - 1] = '\0';


        
        if (statusInt >= SOLD && statusInt <= AVAILABLE) {
            prop.status = (Status)statusInt;
        } else {
            prop.status = AVAILABLE; 
        }

        tempCount++; 
    }

    fclose(file); 

    
    if (tempCount == 0) {
        propertyCount = 0;
        return;
    }

    
    cout << getTranslatedString("RECOVERY_PROMPT_PART1") << " " << tempCount << " " << getTranslatedString("RECOVERY_PROMPT_PART2") << " [y/n] ";

    string answer;
    
    getline(cin, answer);

    
    if (answer.empty() || tolower(answer[0]) != 'y') {
        propertyCount = 0;
        return;
    }

    
    for (int i = 0; i < tempCount; i++) {
        properties[i] = tempProps[i];
    }
    propertyCount = tempCount; 

    cout << GREEN << getTranslatedString("DATA_LOADED_SUCCESS_COUNT") << " " << tempCount << " " << getTranslatedString("DATA_LOADED_SUCCESS_FROM_SYNC_FILE") << RESET << endl;
}


/**
 * @brief Синхронизира текущите данни за имоти с текстови и двоични файлове за възстановяване.
 *
 * Тази функция гарантира устойчивост на данните, като записва текущото състояние на имотите
 * в определени файлове за възстановяване. Тя първо гарантира, че директорията за възстановяване съществува,
 * след това извиква `saveToSyncTextFile` и `saveToRecoveryBinaryFile`. Тази многоформатна
 * стратегия за възстановяване осигурява устойчивост срещу различни видове повреда на данните.
 *
 * @param properties Постоянен масив от структури `Property` за синхронизация.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void syncDataToRecoveryFiles(const Property properties[], int propertyCount) {
    
    ensureDirectoryExists((BASE_DIR + "/data/recovery").c_str());
    
    saveToSyncTextFile(properties, propertyCount);
    
    saveToRecoveryBinaryFile(properties, propertyCount);
}

/**
 * @brief Записва текущия масив от имоти в двоичен файл за възстановяване.
 *
 * Това е специализирана версия на двоично записване, предназначена за автоматично
 * фоново възстановяване. Тя директно записва броя и данните за имотите
 * в предварително дефиниран двоичен файл без потребителско взаимодействие или подканване за презаписване,
 * гарантирайки, че винаги е налично скорошно архивиране.
 *
 * @param properties Постоянен масив от структури `Property` за запис.
 * @param propertyCount Броят на валидните имоти в масива.
 */
void saveToRecoveryBinaryFile(const Property properties[], int propertyCount) {
    
    FILE* binaryFile = fopen(SYNC_BINARY_FILENAME.c_str(), "wb");
    if (binaryFile == NULL) return; 
    
    fwrite(&propertyCount, sizeof(int), 1, binaryFile);
    
    if (propertyCount > 0) {
        fwrite(properties, sizeof(Property), propertyCount, binaryFile);
    }
    fclose(binaryFile); 
}

/**
 * @brief Подканва потребителя за потвърждение преди презаписване на съществуващ файл.
 *
 * Тази функция проверява дали даден файл вече съществува. Ако съществува, тя показва
 * локализирано предупредително съобщение на потребителя и пита за изрично потвърждение
 * преди да продължи с операция, която би презаписала файла.
 * Това е критична мярка за безопасност за предотвратяване на неволна загуба на данни.
 *
 * @param filename C-стил низ път до въпросния файл.
 * @return `true`, ако файлът не съществува или ако потребителят потвърди презаписването; `false` в противен случай.
 */
bool confirmOverwrite(const char* filename) {
    
    ifstream fileCheck(filename);
    if (!fileCheck.good()) {
        return true; 
    }
    fileCheck.close(); 

    
    cout << YELLOW << getTranslatedString("FILE_EXISTS_OVERWRITE_PROMPT_PART1") << " '" << filename << "' " << getTranslatedString("FILE_EXISTS_OVERWRITE_PROMPT_PART2") << RESET;
    char choice;
    cin >> choice; 

    
    if (!cin) cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    
    return (choice == 'y' || choice == 'Y');
}