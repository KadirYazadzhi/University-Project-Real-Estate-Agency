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

The project is organized into a `src` directory for source files, an `include` directory for header files, and a `data` directory for data files. This separation of concerns improves code readability and maintainability.

```
/solution
|-- /src
|   |-- main.cpp
|   |-- add.cpp
|   |-- display.cpp
|   |-- ...
|-- /include
|   |-- main.h
|   |-- add.h
|   |-- ...
|-- /data
|   |-- properties_backup.dat
|   |-- properties_report.txt
|   |-- /recovery
|   |   |-- sync.txt
|   |   |-- sync.dat
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
*   **C-style Fundamentals:** The project adheres to fundamental C++ concepts, primarily using character arrays (`char[]`) for strings and C-style file I/O (`fopen`, `fwrite`, `fread`, `fprintf`, `fscanf`).

## 5. File and Function Breakdown

This section provides a detailed description of each file and its functions.

### `src/main.cpp`
*   **Purpose:** Serves as the entry point of the application, handles menu navigation, and orchestrates calls to other modules.
*   **Functions:**
    *   `main()`: The entry point. Calls `loadFromSyncTextFile()` to load data, prints a banner, and enters the `mainMenu()`.
    *   `mainMenu()`: Displays the main menu. On exit, it calls `saveToSyncTextFile()` to ensure data persistence.
    *   `sortMenu()`: Now includes an option to sort the main `properties` array by price, which also triggers an automatic data sync.

### `src/add.cpp`
*   **Purpose:** Handles the logic for adding new properties to the system.
*   **Functions:**
    *   `addSingleProperty()`: Prompts the user for property details. It now uses validation and sanitization functions to ensure data integrity. After a successful addition, it calls `syncDataToRecoveryFiles()`.
    *   `getValidIntInput()`, `getValidDoubleInput()`: Helper functions that ensure the user enters a valid number.
    *   `sanitizeString()`: A helper function that removes `|` characters from string inputs to prevent file corruption.

### `src/update.cpp`
*   **Purpose:** Handles modifications of existing property data.
*   **Functions:**
    *   `updateProperty()`: The main function for this module. It finds a property and presents a menu of fields to edit.
    *   `updateRefNumber()`, `updateStringField()`, `updateNumericField()`, `updateStatus()`: These functions now call `syncDataToRecoveryFiles()` after every successful modification to ensure data is always saved.

### `src/file.cpp`
*   **Purpose:** Manages all file I/O operations, now with a clear separation between user-facing files and internal recovery files.
*   **Functions:**
    *   `saveToBinaryFile()` / `loadFromBinaryFile()`: Handle manual backup and restore from `data/properties_backup.dat`.
    *   `saveToUserFriendlyTextFile()`: Exports property data to a human-readable report in `data/properties_report.txt`.
    *   `saveToSyncTextFile()` / `loadFromSyncTextFile()`: Handle automatic persistence to/from `data/recovery/sync.txt`.
    *   `saveToRecoveryBinaryFile()`: Saves a binary backup for recovery to `data/recovery/sync.dat`.
    *   `syncDataToRecoveryFiles()`: A new central function that calls both `saveToSyncTextFile()` and `saveToRecoveryBinaryFile()` to ensure a robust auto-save mechanism.

*(Descriptions for `display.cpp`, `search.cpp`, `sort.cpp`, `reports.cpp`, and `structs.cpp` remain largely the same as they were not part of the major refactoring.)*

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
