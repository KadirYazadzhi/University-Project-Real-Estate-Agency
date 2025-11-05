/**
 * @file main.cpp
 * @brief Главен файл на програмата.
 *
 * Този файл съдържа входната точка на приложението (main функцията),
 * както и функциите, отговорни за навигацията в главното меню и подменютата.
 */

#include <iostream>
#include <string>
#include <cstdlib>

#include "main.h"
#include "add.h"
#include "display.h"
#include "file.h"
#include "reports.h"
#include "search.h"
#include "sort.h"
#include "structs.h"
#include "update.h"
#include "colors.h"

using namespace std;

/**
 * @brief Входна точка на програмата.
 * 
 * Извиква функциите за зареждане на данни от файл, извеждане на банер и стартиране на главното меню.
 * @return int Връща 0 при успешно завършване.
 */
int main() {
  loadFromSyncTextFile();
  printBanner();
  mainMenu();

  return 0;
}

/**
 * @brief Управлява главното меню на приложението.
 * 
 * В безкраен цикъл извежда опциите, приема избора на потребителя и извиква съответната функция.
 */
void mainMenu() {
  while (true) {
    cout << CYAN << "Изберете опция: " << RESET << endl;
    cout << "  1. Добавяне на имот/и." << endl;
    cout << "  2. Извеждане на имоти." << endl;
    cout << "  3. Търсене." << endl;
    cout << "  4. Сортиране." << endl;
    cout << "  5. Файлови операции." << endl;
    cout << "  6. Корекция на данни." << endl;
    cout << "  7. Справки" << endl;
    cout << "  0. Изход." << endl;
    cout << CYAN << "Въведете вашия избор: " << RESET;

    int choice;
    cin >> choice;

    switch (choice) {
      case 0:
        saveToSyncTextFile(); // Запазваме данните преди изход
        cout << YELLOW << "Благодарим ви че използвахте системата ни." << RESET << endl;
        exit(0);
      case 1: addPropertyMenu(); break;
      case 2: displayMenu(); break;
      case 3: searchMenu(); break;
      case 4: sortMenu(); break;
      case 5: fileMenu(); break;
      case 6: updateProperty(); break;
      case 7: reportsMenu(); break;
      default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
    }
  }
}

/**
 * @brief Управлява подменюто за добавяне на имоти.
 */
void addPropertyMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. Добавяне на един имот." << endl;
  cout << "  2. Добавяне на няколко имота." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: addSingleProperty(); break;
    case 2: addMultipleProperties(); break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Управлява подменюто за извеждане на имоти.
 */
void displayMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. Всички имоти." << endl;
  cout << "  2. Продадени имоти." << endl;
  cout << "  3. Имоти с най-голяма площ." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: displayAllProperties(); break;
    case 2: displaySoldProperties(); break;
    case 3: displayLargestProperties(); break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Управлява подменюто за търсене.
 */
void searchMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. По брокер." << endl;
  cout << "  2. По брой стаи." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: searchByBroker(); break;
    case 2: searchByRooms(); break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Управлява подменюто за сортиране.
 */
void sortMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. Сортиране на имотите на даден брокер по цена." << endl;
  cout << "  2. Търсене на имоти по брой стаи и сортиране по цена." << endl;
  cout << "  3. Сортиране на всички имоти по цена (възходящ ред)." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: searchByBroker(); break;
    case 2: searchByRooms(); break;
    case 3:
        sortPropertiesArray(properties, propertyCount, true);
        syncDataToRecoveryFiles();
        cout << GREEN << "Всички имоти бяха сортирани по цена." << RESET << endl;
        break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Управлява подменюто за файлови операции (ръчни).
 */
void fileMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. Запис във файл (двоичен)." << endl;
  cout << "  2. Зареждане от файл (двоичен)." << endl;
  cout << "  3. Запис в текстов файл." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: saveToBinaryFile(); break;
    case 2: loadFromBinaryFile(); break;
    case 3: saveToUserFriendlyTextFile(); break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Управлява подменюто за генериране на справки.
 */
void reportsMenu() {
  clearConsole();

  cout << CYAN << "Изберете опция: " << RESET << endl;
  cout << "  1. Най-скъп имот в даден район." << endl;
  cout << "  2. Средна цена на имот в даден район." << endl;
  cout << "  3. Процент на продадените имоти на всеки брокер." << endl;
  cout << "  0. Назад." << endl;
  cout << CYAN << "Въведете вашия избор: " << RESET;

  int choice;
  cin >> choice;

  switch (choice) {
    case 0: mainMenu(); break;
    case 1: mostExpensiveInArea(); break;
    case 2: averagePriceInArea(); break;
    case 3: soldPercentagePerBroker(); break;
    default: cout << RED << "Невалидна опция. Моля, опитайте отново." << RESET << endl; break;
  }
}

/**
 * @brief Изчиства екрана на конзолата.
 */
void clearConsole() {
  cout << "\033[2J\033[1;1H";
}

/**
 * @brief Извежда ASCII арт банер при стартиране на програмата.
 */
void printBanner() {
  cout << R"(
██████╗ ███████╗ █████╗ ██╗         ███████╗███████╗████████╗ █████╗ ████████╗███████╗     █████╗  ██████╗ ███████╗███╗   ██╗ ██████╗██╗   ██╗
██╔══██╗██╔════╝██╔══██╗██║         ██╔════╝██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██╔════╝    ██╔══██╗██╔════╝ ██╔════╝████╗  ██║██╔════╝╚██╗ ██╔╝
██████╔╝█████╗  ███████║██║         █████╗  ███████╗   ██║   ███████║   ██║   █████╗      ███████║██║  ███╗█████╗  ██╔██╗ ██║██║      ╚████╔╝
██╔══██╗██╔══╝  ██╔══██║██║         ██╔══╝  ╚════██║   ██║   ██╔══██║   ██║   ██╔══╝      ██╔══██║██║   ██║██╔══╝  ██║╚██╗██║██║       ╚██╔╝
██║  ██║███████╗██║  ██║███████╗    ███████╗███████║   ██║   ██║  ██║   ██║   ███████╗    ██║  ██║╚██████╔╝███████╗██║ ╚████║╚██████╗   ██║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝    ╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚══════╝    ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝   ╚═╝
        Информационна система 'Агенция за недвижими имоти'                                                     realestateagency - @kadir_
  )" << endl;
  cout << endl;
}