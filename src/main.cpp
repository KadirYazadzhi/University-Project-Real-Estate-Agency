

#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <iomanip>

#include "main.h"
#include "add.h"
#include "display.h"
#include "delete.h"
#include "file.h"
#include "reports.h"
#include "search.h"
#include "sort.h"
#include "structs.h"
#include "update.h"
#include "colors.h"

using namespace std;


int main() {
  loadFromSyncTextFile();
  printBanner();
  mainMenu();

  return 0;
}


void mainMenu() {
  while (true) {
    cout << CYAN << "Main Menu:" << RESET << endl;
    cout << left;
    cout << "  " << setw(25) << "1. Add Property/ies" << "5. Sort" << endl;
    cout << "  " << setw(25) << "2. Delete Property" << "6. File Operations" << endl;
    cout << "  " << setw(25) << "3. Display Properties" << "7. Data Correction" << endl;
    cout << "  " << setw(25) << "4. Search" << "8. Reports" << endl;
    cout << "  " << "0. Exit" << endl;
    
    int choice;
    cout << CYAN << "Enter your choice: " << RESET;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
      case 0:

        cout << YELLOW << "Thank you for using our system." << RESET << endl;
        exit(0);
      case 1: addPropertyMenu(); break;
      case 2: deletePropertyMenu(); break;
      case 3: displayMenu(); break;
      case 4: searchMenu(); break;
      case 5: sortMenu(); break;
      case 6: fileMenu(); break;
      case 7: updateProperty(); break;
      case 8: reportsMenu(); break;
      default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
    }
  }
}


void addPropertyMenu() {
  clearConsole();
  cout << CYAN << "Add Menu:" << RESET << endl;
  cout << "  1. Add a single property" << endl;
  cout << "  2. Add multiple properties" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: addSingleProperty(); break;
    case 2: addMultipleProperties(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void deletePropertyMenu() {
  clearConsole();
  cout << CYAN << "Delete Menu:" << RESET << endl;
  cout << "  1. Delete a single property (by ref. number)" << endl;
  cout << "  2. Delete all properties" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: DeleteProperty(); break;
    case 2: DeleteAllProperties(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void displayMenu() {
  clearConsole();
  cout << CYAN << "Display Menu:" << RESET << endl;
  cout << "  1. All properties" << endl;
  cout << "  2. Sold properties" << endl;
  cout << "  3. Properties with largest area" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: displayAllProperties(); break;
    case 2: displaySoldProperties(); break;
    case 3: displayLargestProperties(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void searchMenu() {
  clearConsole();
  cout << CYAN << "Search Menu:" << RESET << endl;
  cout << "  1. By broker" << endl;
  cout << "  2. By number of rooms" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: searchByBroker(); break;
    case 2: searchByRooms(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void sortMenu() {
  clearConsole();
  cout << CYAN << "Sort Menu:" << RESET << endl;
  cout << "  1. Sort properties of a given broker by price" << endl;
  cout << "  2. Search properties by number of rooms and sort by price" << endl;
  cout << "  3. Sort all properties by price (ascending order)" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: searchByBroker(); break;
    case 2: searchByRooms(); break;
    case 3:
        sortPropertiesArray(properties, propertyCount, true);
        syncDataToRecoveryFiles();
        cout << GREEN << "All properties were sorted by price." << RESET << endl;
        break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void fileMenu() {
  clearConsole();
  cout << CYAN << "File Operations Menu:" << RESET << endl;
  cout << "  1. Write to file (binary)" << endl;
  cout << "  2. Load from file (binary)" << endl;
  cout << "  3. Write to text file" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: saveToBinaryFile(); break;
    case 2: loadFromBinaryFile(); break;
    case 3: saveToUserFriendlyTextFile(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void reportsMenu() {
  clearConsole();
  cout << CYAN << "Reports Menu:" << RESET << endl;
  cout << "  1. Most expensive property in a given area" << endl;
  cout << "  2. Average property price in a given area" << endl;
  cout << "  3. Percentage of sold properties per broker" << endl;
  cout << "  0. Back" << endl;

  int choice;
  cout << CYAN << "Enter your choice: " << RESET;
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  switch (choice) {
    case 0: return;
    case 1: mostExpensiveInArea(); break;
    case 2: averagePriceInArea(); break;
    case 3: soldPercentagePerBroker(); break;
    default: cout << RED << "Invalid option. Please try again." << RESET << endl; break;
  }
}


void clearConsole() {
  cout << "\033[2J\033[1;1H";
}


void printBanner() {
  cout << R"(
██████╗ ███████╗ █████╗ ██╗         ███████╗███████╗████████╗ █████╗ ████████╗███████╗     █████╗  ██████╗ ███████╗███╗   ██╗ ██████╗██╗   ██╗
██╔══██╗██╔════╝██╔══██╗██║         ██╔════╝██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██╔════╝    ██╔══██╗██╔════╝ ██╔════╝████╗  ██║██╔════╝╚██╗ ██╔╝
██████╔╝█████╗  ███████║██║         █████╗  ███████╗   ██║   ███████║   ██║   █████╗      ███████║██║  ███╗█████╗  ██╔██╗ ██║██║      ╚████╔╝
██╔══██╗██╔══╝  ██╔══██║██║         ██╔══╝  ╚════██║   ██║   ██╔══██║   ██║   ██╔══╝      ██╔══██║██║   ██║██╔══╝  ██║╚██╗██║██║       ╚██╔╝
██║  ██║███████╗██║  ██║███████╗    ███████╗███████║   ██║   ██║  ██║   ██║   ███████╗    ██║  ██║╚██████╔╝███████╗██║ ╚████║╚██████╗   ██║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝    ╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚══════╝    ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝   ╚═╝
        Real Estate Agency Information System                                                     realestateagency - @kadir_
  )" << endl;
  cout << endl;
}
