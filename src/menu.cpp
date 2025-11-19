/**
 * @file menu.cpp
 * @brief Имплементация на системата от менюта на приложението и потока на потребителско взаимодействие.
 *
 * Този файл съдържа конкретните имплементации за различни менюта, включително
 * главното меню и подменюта за добавяне, изтриване, показване, търсене,
 * сортиране, файлови операции и отчети. Тези функции ръководят потребителя
 * през функционалностите на приложението, обработват изборите от менюто и
 * делегират задачи на съответните модули.
 */

#include <iostream>  
#include <limits>    
#include <iomanip>   
#include <cstdlib>   

#include "menu.h"    
#include "utils.h"   
#include "add.h"     
#include "display.h" 
#include "delete.h"  
#include "file.h"    
#include "reports.h" 
#include "search.h"  
#include "sort.h"    
#include "update.h"  
#include "colors.h"  
#include "localization.h" 

using namespace std;

/**
 * @brief Показва главното меню на приложението и обработва изборите на потребителя на най-високо ниво.
 *
 * Тази функция непрекъснато цикли, показвайки опциите на главното меню на потребителя
 * и подканвайки за избор. Тя използва `getMenuChoice` за валидиране на входа и след това
 * извиква съответното подменю или функция въз основа на избора на потребителя.
 * Цикълът се прекъсва само когато потребителят избере да излезе от приложението.
 * Преди излизане, тя гарантира, че текущите данни са запазени за възстановяване.
 *
 * @param properties Масив от структури `Property`, представляващ основния набор от данни.
 * @param propertyCount Референция към цяло число, съдържащо текущия брой имоти.
 */
void mainMenu(Property properties[], int& propertyCount) {
  while (true) {
    clearConsole(); 
    cout << CYAN << getTranslatedString("MAIN_MENU_TITLE") << RESET << endl;
    cout << getTranslatedString("MAIN_MENU_ADD_PROPERTY") << endl;
    cout << getTranslatedString("MAIN_MENU_DELETE_PROPERTY") << endl;
    cout << getTranslatedString("MAIN_MENU_DISPLAY_PROPERTIES") << endl;
    cout << getTranslatedString("MAIN_MENU_SEARCH") << endl;
    cout << getTranslatedString("MAIN_MENU_SORT") << endl;
    cout << getTranslatedString("MAIN_MENU_FILE_OPERATIONS") << endl;
    cout << getTranslatedString("MAIN_MENU_UPDATE_DATA") << endl;
    cout << getTranslatedString("MAIN_MENU_REPORTS") << endl;
    cout << getTranslatedString("MAIN_MENU_EXIT") << endl;
    
    
    int choice = getMenuChoice(0, 8);

    switch (choice) {
      case 0:
        
        saveToSyncTextFile(properties, propertyCount); 
        cout << YELLOW << getTranslatedString("THANK_YOU_MESSAGE") << RESET << endl;
        exit(0); 
      case 1: addPropertyMenu(properties, propertyCount); break;      
      case 2: deletePropertyMenu(properties, propertyCount); break;   
      case 3: displayMenu(properties, propertyCount); break;          
      case 4: searchMenu(properties, propertyCount); break;           
      case 5: sortMenu(properties, propertyCount); break;             
      case 6: fileMenu(properties, propertyCount); break;             
      case 7: updateProperty(properties, propertyCount); break;       
      case 8: reportsMenu(properties, propertyCount); break;          
      default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
    }
  }
}

/**
 * @brief Показва менюто за добавяне на имоти и обработва изборите на потребителя.
 *
 * Това подменю предоставя опции за добавяне на единичен имот или множество имоти.
 * То изчиства конзолата, показва локализираните опции и след това извиква
 * съответната функция (`addSingleProperty` или `addMultipleProperties`)
 * въз основа на валидирания избор на потребителя.
 *
 * @param properties Масив от структури `Property`, където ще бъдат добавени новите имоти.
 * @param propertyCount Референция към цяло число, съдържащо текущия брой имоти.
 */
void addPropertyMenu(Property properties[], int& propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("ADD_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("ADD_MENU_SINGLE_PROPERTY") << endl;
  cout << getTranslatedString("ADD_MENU_MULTIPLE_PROPERTIES") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 2);

  switch (choice) {
    case 0: return; 
    case 1: addSingleProperty(properties, propertyCount); break;   
    case 2: addMultipleProperties(properties, propertyCount); break; 
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за изтриване на имоти и обработва изборите на потребителя.
 *
 * Това подменю предлага опции за изтриване на един имот по референтен номер
 * или за изчистване на всички имоти от системата. То използва `getMenuChoice` за
 * валидиране на входа и извиква съответната функция за изтриване.
 *
 * @param properties Масив от структури `Property`, от които ще бъдат изтрити имоти.
 * @param propertyCount Референция към цяло число, съдържащо текущия брой имоти.
 */
void deletePropertyMenu(Property properties[], int& propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("DELETE_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("DELETE_MENU_SINGLE_PROPERTY") << endl;
  cout << getTranslatedString("DELETE_MENU_ALL_PROPERTIES") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 2);

  switch (choice) {
    case 0: return; 
    case 1: DeleteProperty(properties, propertyCount); break;   
    case 2: DeleteAllProperties(properties, propertyCount); break; 
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за преглед на имоти и обработва изборите на потребителя.
 *
 * Това подменю представя опции за показване на всички имоти, само продадени имоти,
 * или имоти с най-голяма обща площ. То извиква съответната функция за показване
 * въз основа на валидирания вход на потребителя.
 *
 * @param properties Постоянен масив от структури `Property` за показване.
 * @param propertyCount Текущият брой имоти в системата.
 */
void displayMenu(const Property properties[], int propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("DISPLAY_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("DISPLAY_MENU_ALL_PROPERTIES") << endl;
  cout << getTranslatedString("DISPLAY_MENU_SOLD_PROPERTIES") << endl;
  cout << getTranslatedString("DISPLAY_MENU_LARGEST_PROPERTIES") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 3);

  switch (choice) {
    case 0: return; 
    case 1: displayAllProperties(properties, propertyCount); break;   
    case 2: displaySoldProperties(properties, propertyCount); break; 
    case 3: displayLargestProperties(properties, propertyCount); break; 
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за търсене на имоти и обработва изборите на потребителя.
 *
 * Това подменю позволява на потребителите да търсят имоти по име на брокер или
 * по брой стаи. То извиква съответната функция за търсене въз основа
 * на валидирания избор на потребителя.
 *
 * @param properties Постоянен масив от структури `Property` за търсене.
 * @param propertyCount Текущият брой имоти в системата.
 */
void searchMenu(Property properties[], int propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("SEARCH_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("SEARCH_MENU_BY_BROKER") << endl;
  cout << getTranslatedString("SEARCH_MENU_BY_ROOMS") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 2);

  switch (choice) {
    case 0: return; 
    case 1: searchByBroker(properties, propertyCount); break;   
    case 2: searchByRooms(properties, propertyCount); break;    
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за сортиране на имоти и обработва изборите на потребителя.
 *
 * Това подменю предоставя опции за сортиране на имоти въз основа на различни критерии.
 * Текущата имплементация позволява сортиране по цена за конкретни резултати от търсене
 * или сортиране на целия масив от имоти по цена във възходящ ред.
 *
 * @param properties Масив от структури `Property` за сортиране.
 * @param propertyCount Текущият брой имоти в системата.
 */
void sortMenu(Property properties[], int propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("SORT_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("SORT_MENU_BROKER_PRICE") << endl;
  cout << getTranslatedString("SORT_MENU_ROOMS_PRICE") << endl;
  cout << getTranslatedString("SORT_MENU_ALL_PRICE_ASC") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 3);

  switch (choice) {
    case 0: return; 
    
    
    case 1: searchByBroker(properties, propertyCount); break; 
    case 2: searchByRooms(properties, propertyCount); break; 
    case 3:
        
        sortPropertiesArray(properties, propertyCount, true);
        syncDataToRecoveryFiles(properties, propertyCount); 
        cout << GREEN << getTranslatedString("SORT_ALL_PROPERTIES_SUCCESS") << RESET << endl;
        break;
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за файлови операции (запис/зареждане) и обработва изборите на потребителя.
 *
 * Това подменю предоставя опции за записване на данни за имоти в двоичен файл,
 * зареждане на данни от двоичен файл или записване на данни в удобен за потребителя текстов файл за отчет.
 * То извиква съответната функция за обработка на файлове въз основа на валидирания вход на потребителя.
 *
 * @param properties Масив от структури `Property` за файлови операции.
 * @param propertyCount Референция към цяло число, съдържащо текущия брой имоти.
 */
void fileMenu(Property properties[], int& propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("FILE_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("FILE_MENU_SAVE_BINARY") << endl;
  cout << getTranslatedString("FILE_MENU_LOAD_BINARY") << endl;
  cout << getTranslatedString("FILE_MENU_SAVE_TEXT") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 3);

  switch (choice) {
    case 0: return; 
    case 1: saveToBinaryFile(properties, propertyCount); break;           
    case 2: loadFromBinaryFile(properties, propertyCount); break;         
    case 3: saveToUserFriendlyTextFile(properties, propertyCount); break; 
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}

/**
 * @brief Показва менюто за генериране на отчети и обработва изборите на потребителя.
 *
 * Това подменю предлага опции за генериране на различни аналитични отчети,
 * като намиране на най-скъпия имот в район, изчисляване на
 * средната цена в район или определяне на процентите на продадени от брокер.
 * То извиква съответната функция за генериране на отчети.
 *
 * @param properties Постоянен масив от структури `Property` за генериране на отчети.
 * @param propertyCount Текущият брой имоти в системата.
 */
void reportsMenu(const Property properties[], int propertyCount) {
  clearConsole();
  cout << CYAN << getTranslatedString("REPORTS_MENU_TITLE") << RESET << endl;
  cout << getTranslatedString("REPORTS_MENU_MOST_EXPENSIVE_IN_AREA") << endl;
  cout << getTranslatedString("REPORTS_MENU_AVERAGE_PRICE_IN_AREA") << endl;
  cout << getTranslatedString("REPORTS_MENU_SOLD_PERCENTAGE_PER_BROKER") << endl;
  cout << getTranslatedString("ADD_MENU_BACK") << endl; 

  
  int choice = getMenuChoice(0, 3);

  switch (choice) {
    case 0: return; 
    case 1: mostExpensiveInArea(properties, propertyCount); break;         
    case 2: averagePriceInArea(properties, propertyCount); break;           
    case 3: soldPercentagePerBroker(properties, propertyCount); break;     
    default: cout << RED << getTranslatedString("INVALID_OPTION") << RESET << endl; break; 
  }
}