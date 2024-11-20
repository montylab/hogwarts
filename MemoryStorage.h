#ifndef MEMORY_STORAGE_H
#define MEMORY_STORAGE_H

#include <EEPROM.h>
#include <ArduinoJson.h>

class MemoryStorage {
public:
    static void begin(int eepromSize);  // Инициализация EEPROM

    // Методы для сохранения параметров
    static void saveParam(String key, String value);
    static void saveParam(String key, int value);
    static void saveParam(int key, int value);
    static void saveParam(int key, String value);

    // Методы для чтения параметров
    static String readStringParam(String key);
    static String readStringParam(int key);
    static int readIntParam(String key);
    static int readIntParam(int key);

private:
    static int eepromSize;

    static void loadFromEEPROM(DynamicJsonDocument& doc);
    static void saveToEEPROM(DynamicJsonDocument& doc);
};

#endif
