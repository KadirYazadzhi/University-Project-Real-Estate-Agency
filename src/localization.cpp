/**
 * @file localization.cpp
 * @brief Имплементация на помощни функции за локализация за управление на езиковите настройки на приложението.
 *
 * Този файл предоставя конкретната имплементация за зареждане на файлове с преводи,
 * извличане на преведени низове и управление на избора на език от потребителя.
 * Той използва глобална карта за съхраняване на двойки ключ-стойност за преводи, което позволява
 * динамично превключване на езика и поддръжка на множество езици.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits> 
#include <string>   

#include "localization.h"
#include "utils.h" 
#include "colors.h" 



#ifdef PROJECT_ROOT_DIR
const std::string BASE_DIR = PROJECT_ROOT_DIR;
#else


const std::string BASE_DIR = ".";
#endif





std::map<std::string, std::string> g_translations;


std::string currentLanguage = "bg"; 

/**
 * @brief Зарежда преводи от указан езиков файл в глобалната карта за преводи.
 *
 * Тази функция изчиства всички предишно заредени преводи и след това анализира текстов файл,
 * намиращ се в директорията `lang/`. Всеки ред във файла трябва да бъде във формат
 * `KEY=VALUE`. Ако файлът не може да бъде отворен, той отпечатва грешка и
 * се опитва да зареди езика по подразбиране български като резервен вариант, за да осигури основна
 * функционалност.
 *
 * @param langCode Низ, представляващ езиковия код (напр. "en" за английски, "bg" за български).
 *                  Функцията очаква да съществува файл с име `{langCode}.txt` в поддиректорията `lang/`
 *                  на `BASE_DIR`.
 */
void loadTranslations(const std::string& langCode) {
    g_translations.clear(); 

    
    std::string filename = BASE_DIR + "/lang/" + langCode + ".txt"; 
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << RED << "Грешка: Не може да се отвори файлът с преводи: " << filename << RESET << std::endl;
        
        
        if (langCode != "bg") {
            currentLanguage = "bg"; 
            loadTranslations("bg");
        }
        return; 
    }

    std::string line;
    
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('='); 
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos); 
            std::string value = line.substr(delimiterPos + 1); 
            g_translations[key] = value; 
        }
    }
    file.close(); 
    currentLanguage = langCode; 
}

/**
 * @brief Извлича преведен низ за даден ключ от глобалната карта за преводи.
 *
 * Ако ключът съществува в картата `g_translations`, се връща съответният преведен низ.
 * Ако ключът не бъде намерен, се отпечатва предупредително съобщение до `cerr`
 * и се връща самият ключ. Този резервен механизъм гарантира, че приложението
 * все още може да показва някакъв текст, дори ако преводът липсва, предотвратявайки сривове.
 *
 * @param key Ключът (оригиналният идентификатор на низ), който да се търси в картата за преводи.
 * @return Постоянна референция към преведения низ, ако е намерен, в противен случай постоянна
 *         референция към самия ключ (заедно с предупреждение).
 */
const std::string& getTranslatedString(const std::string& key) {
    if (g_translations.count(key)) {
        return g_translations[key]; 
    }
    
    std::cerr << YELLOW << "Предупреждение: Ключ за превод '" << key << "' не е намерен за език '" << currentLanguage << "'." << RESET << std::endl;
    return key;
}

/**
 * @brief Насочва потребителя при избор на език на приложението при стартиране.
 *
 * Тази функция изчиства конзолата, показва списък с налични езици (английски и български)
 * и подканва потребителя да направи избор. Тя включва валидиране на входа, за да се гарантира, че потребителят
 * въвежда валидна опция (1 или 2). След като бъде направен валиден избор, тя извиква `loadTranslations`
 * за зареждане на низовете на избрания език и потвърждава настройката на езика на потребителя.
 */
void selectLanguage() {
    clearConsole(); 
    std::cout << "Select Language / Изберете език:" << std::endl;
    std::cout << "  1. English (en)" << std::endl;
    std::cout << "  2. Български (bg)" << std::endl;
    std::cout << "Enter your choice / Въведете вашия избор: ";

    int choice;
    
    while (!(std::cin >> choice) || (choice < 1 || choice > 2)) {
        std::cout << RED << "Невалиден избор. Моля, въведете 1 или 2. / Невалиден избор. Моля, въведете 1 или 2." << RESET << std::endl;
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "Enter your choice / Въведете вашия избор: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    
    if (choice == 1) {
        loadTranslations("en");
    } else { 
        loadTranslations("bg");
    }
    
    std::cout << GREEN << getTranslatedString("LANGUAGE_SET_SUCCESS") << RESET << std::endl;
}
