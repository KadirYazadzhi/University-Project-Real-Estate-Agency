# Real Estate Agency Information System

```
██████╗ ███████╗ █████╗ ██╗         ███████╗███████╗████████╗ █████╗ ████████╗███████╗     █████╗  ██████╗ ███████╗███╗   ██╗ ██████╗██╗   ██╗
██╔══██╗██╔════╝██╔══██╗██║         ██╔════╝██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██╔════╝    ██╔══██╗██╔════╝ ██╔════╝████╗  ██║██╔════╝╚██╗ ██╔╝
██████╔╝█████╗  ███████║██║         █████╗  ███████╗   ██║   ███████║   ██║   █████╗      ███████║██║  ███╗█████╗  ██╔██╗ ██║██║      ╚████╔╝
██╔══██╗██╔══╝  ██╔══██║██║         ██╔══╝  ╚════██║   ██║   ██╔══██║   ██║   ██╔══╝      ██╔══██║██║   ██║██╔══╝  ██║╚██╗██║██║       ╚██╔╝
██║  ██║███████╗██║  ██║███████╗    ███████╗███████║   ██║   ██║  ██║   ██║   ███████╗    ██║  ██║╚██████╔╝███████╗██║ ╚████║╚██████╗   ██║
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝    ╚══════╝╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚══════╝    ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝   ╚═╝
        Информационна система 'Агенция за недвижими имоти'                                                     realestateagency - @kadir_
```

## 1. Project Goal

The primary goal of this project is to develop a console-based information system for a real estate agency. The system is designed to manage a portfolio of properties, allowing agents to perform various operations such as adding, displaying, searching, sorting, and updating property data. The application also provides reporting functionalities and ensures data persistence between sessions.

This project is developed as a semester assignment for a university-level programming course. It demonstrates fundamental programming concepts in C++, including data structures, modular programming, file I/O, and basic algorithms.

## 2. Requirements

The application is built to meet the following specifications:

### Base Task

*   **A: Main Menu:** A user-friendly main menu to navigate through the system's functions.
*   **B: Add New Property:**
    *   Add a single new property.
    *   Add a list of 'n' properties.
*   **C: Display Properties:** Display all properties in a well-formatted manner.
*   **D: Search and Display:**
    *   Display properties with the largest total area.
    *   Display all sold properties.
*   **E: Sort Properties:** Sort the main array of properties by price in ascending order without displaying them immediately.
*   **F: File Management (Binary):**
    *   Save the property data to a binary file.
    *   Load the property data from a binary file.

### First Extension

*   **G: Sub-menu for Sorting:**
    *   Extract and sort by price all properties for a given broker.
    *   Extract and sort from most expensive to cheapest all properties with a specific number of rooms.

### Second Extension

*   **H: Update Property Data:**
    *   A sold property cannot be modified.
    *   If a property's status is changed to "reserved", the price is updated to reflect a 20% down payment.
*   **I: Reports:**
    *   Find and display the most expensive property in a given area.
    *   Calculate and display the average property price in a given area.
    *   Calculate and display the percentage of sold properties for each broker.

### Third Extension

*   **J: Data Persistence (Text File):** The application's data can be saved to a text file to ensure persistence between sessions.

## 3. Project Structure

The project is organized into a `src` directory for source files, an `include` directory for header files, a `data` directory for data files, and a `lang` directory for language files. This separation of concerns improves code readability and maintainability.

```
/
|-- /src
|   |-- main.cpp
|   |-- add.cpp
|   |-- display.cpp
|   |-- ...
|-- /include
|   |-- add.h
|   |-- ...
|-- /data
|   |-- properties_backup.dat
|   |-- properties_report.txt
|   |-- /recovery
|   |   |-- sync.txt
|   |   |-- sync.dat
|-- /lang
|   |-- en.txt
|   |-- bg.txt
```

## 4. Implementation Details

### Data Structures

The core data structure of the application is the `Property` struct, defined in `include/structs.h`. It holds all the information for a single property. A global array `properties[MAX_PROPERTIES]` is used to store up to 100 property records.

### Core Logic and Techniques

*   **Modular Programming:** The code is split into multiple files, each responsible for a specific set of functionalities (e.g., `add.cpp` for adding properties, `file.cpp` for file operations). This makes the code easier to manage and debug.
*   **Robust Data Persistence:** The application ensures data integrity and persistence through a multi-layered strategy:
    1.  **Automatic Recovery System:** On every data modification (add, update, sort), the system automatically saves the current data to two separate recovery files: `data/recovery/sync.txt` and `data/recovery/sync.dat`.
    2.  **Automatic Loading:** On startup, the application automatically loads data from `sync.txt`, ensuring the most recent state is always available.
    3.  **Manual Backup:** The user can manually choose to save the current state to a user-facing binary file (`data/properties_backup.dat`) or load data from it.
*   **Input Validation:** The system validates all numeric inputs to prevent crashes from non-numeric entries. It also sanitizes all string inputs to remove special characters (`|`) that could corrupt the data files.
*   **Sorting Algorithm:** The project utilizes the **Quicksort** algorithm for sorting properties by price. This is an efficient, in-place sorting algorithm with an average time complexity of O(n log n).
*   **User Interface:** The console UI is enhanced with colors to improve user experience, with different colors for prompts, success messages, and errors.
*   **Mixed I/O Fundamentals:** The project utilizes a mix of C-style file I/O (`fopen`, `fwrite`, `fread`, `fprintf`, `fscanf`) and C++ file I/O (`ifstream`, `ofstream`). C-style functions are primarily used for binary data serialization and writing formatted reports, while C++ streams are employed for line-by-line reading in the localization and data recovery systems, offering a balance of performance and convenience.

## 5. File and Function Breakdown

This section provides a detailed description of each file and its functions.

### `src/main.cpp`
*   **Purpose:** Serves as the entry point of the application, handles language selection, and orchestrates calls to the main menu.
*   **Functions:**
    *   `main()`: Initializes the application, loads default translations, prompts the user to select a language, prints a banner, loads data from the recovery file, and enters the `mainMenu()`).

### `src/menu.cpp`
*   **Purpose:** Manages the user interface by displaying menus and handling user choices.
*   **Functions:**
    *   `mainMenu()`: The central navigation hub.
    *   `addPropertyMenu()`, `deletePropertyMenu()`, `displayMenu()`, `searchMenu()`, `sortMenu()`, `fileMenu()`, `reportsMenu()`: Sub-menus for each specific functionality.

### `src/add.cpp`
*   **Purpose:** Handles the logic for adding new properties.
*   **Functions:**
    *   `addSingleProperty()`, `addMultipleProperties()`: Guide the user through adding properties, with validation.
    *   `getValidNumericInput<T>()`: Template function for robust numeric input.
    *   `getValidStringInput()`: Handles string input with sanitization.
    *   `isCapacityReached()`: Checks if the property array is full.
    *   `sanitizeString()`: Removes `|` characters to prevent data file corruption.

### `src/update.cpp`
*   **Purpose:** Handles modifications of existing property data.
*   **Functions:**
    *   `updateProperty()`: Main function to select a property and choose a field to edit.
    *   `updateRefNumber()`, `updateStringField()`, `updateNumericField<T>()`, `updateStatus()`: Functions to update specific fields, with business logic for status changes.

### `src/delete.cpp`
*   **Purpose:** Manages the removal of properties from the system.
*   **Functions:**
    *   `DeleteProperty()`: Deletes a single property by its reference number.
    *   `DeleteAllProperties()`: Clears all properties from the system after confirmation.

### `src/display.cpp`
*   **Purpose:** Responsible for presenting property information to the user.
*   **Functions:**
    *   `displayAllProperties()`, `displaySoldProperties()`, `displayLargestProperties()`: Display properties based on different criteria.
    *   `displayPropertyDetails()`: Formats and prints the details of a single property.
    *   `isPropertiesEmpty()`: Checks if there are any properties to display.

### `src/search.cpp`
*   **Purpose:** Implements search functionalities to filter properties.
*   **Functions:**
    *   `searchByBroker()`: Finds and displays properties managed by a specific broker.
    *   `searchByRooms()`: Finds and displays properties with a specific number of rooms.

### `src/sort.cpp`
*   **Purpose:** Contains the implementation of the Quicksort algorithm for sorting properties.
*   **Functions:**
    *   `sortPropertiesArray()`: The main entry point for sorting.
    *   `quickSort()`, `partition()`, `swap()`: The core components of the Quicksort algorithm.

### `src/reports.cpp`
*   **Purpose:** Generates analytical reports from the property data.
*   **Functions:**
    *   `mostExpensiveInArea()`, `averagePriceInArea()`: Provide market insights for specific areas.
    *   `soldPercentagePerBroker()`: Calculates and displays sales performance for each broker.

### `src/file.cpp`
*   **Purpose:** Manages all file I/O operations, including backups and recovery.
*   **Functions:**
    *   `saveToBinaryFile()`, `loadFromBinaryFile()`: Handle manual backups.
    *   `saveToUserFriendlyTextFile()`: Exports data to a human-readable text report.
    *   `saveToSyncTextFile()`, `loadFromSyncTextFile()`: Manage the automatic recovery system using a simple text format.
    *   `saveToRecoveryBinaryFile()`: Saves a binary recovery file.
    *   `syncDataToRecoveryFiles()`: A central function that updates all recovery files.
    *   `confirmOverwrite()`: Prompts the user before overwriting an existing file.

### `src/localization.cpp`
*   **Purpose:** Manages multi-language support.
*   **Functions:**
    *   `loadTranslations()`: Reads key-value pairs from language files (`lang/*.txt`).
    *   `getTranslatedString()`: Retrieves a translated string for a given key.
    *   `selectLanguage()`: Prompts the user to choose a language at startup.

### `src/utils.cpp`
*   **Purpose:** Provides miscellaneous utility functions used throughout the application.
*   **Functions:**
    *   `clearConsole()`: Clears the terminal screen.
    *   `printBanner()`: Displays the application's ASCII art banner.
    *   `ensureDirectoryExists()`: Creates a directory if it doesn't exist.
    *   `getMenuChoice()`: Gets and validates user input for menus.
    *   `getConfirmation()`: Gets a 'y/n' confirmation from the user.

### `src/structs.cpp`
*   **Purpose:** Provides helper functions related to the core data structures.
*   **Functions:**
    *   `getStatusString()`: Converts the `Status` enum to a localized, human-readable string.

## 6. Project Statistics

*   **Total Lines of Code:** 1157

| File | Lines |
| :--- | :--- |
| `src/add.cpp` | 122 |
| `src/display.cpp` | 78 |
| `src/file.cpp` | 205 |
| `src/main.cpp` | 205 |
| `src/reports.cpp` | 112 |
| `src/search.cpp` | 87 |
| `src/sort.cpp` | 46 |
| `src/structs.cpp` | 16 |
| `src/update.cpp` | 160 |
| `include/add.h` | 11 |
| `include/colors.h` | 11 |
| `include/display.h` | 11 |
| `include/file.h` | 14 |
| `include/main.h` | 14 |
| `include/reports.h` | 8 |
| `include/search.h` | 7 |
| `include/sort.h` | 11 |
| `include/structs.h` | 26 |
| `include/update.h` | 13 |
