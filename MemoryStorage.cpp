#include "MemoryStorage.h"

// Инициализация статической переменной
int MemoryStorage::eepromSize = 0;

void MemoryStorage::begin(int size) {
    eepromSize = size;
    EEPROM.begin(eepromSize);
}

void MemoryStorage::saveParam(String key, String value) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    doc[key] = value;
    saveToEEPROM(doc);
}

void MemoryStorage::saveParam(String key, int value) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    doc[key] = value;
    saveToEEPROM(doc);
}

void MemoryStorage::saveParam(int key, int value) {
    saveParam(String(key), value);
}

void MemoryStorage::saveParam(int key, String value) {
    saveParam(String(key), value);
}

String MemoryStorage::readStringParam(String key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(key)) {
        return doc[key].as<String>();
    }
    return "";
}

String MemoryStorage::readStringParam(int key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(String(key))) {
        return doc[String(key)].as<String>();
    }
    return "";
}

int MemoryStorage::readIntParam(String key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(key)) {
        return doc[key].as<int>();
    }
    return 0;
}

int MemoryStorage::readIntParam(int key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(String(key))) {
        return doc[String(key)].as<int>();
    }
    return 0;
}

void MemoryStorage::loadFromEEPROM(DynamicJsonDocument& doc) {
    String jsonString;
    for (int i = 0; i < eepromSize; i++) {
        jsonString += char(EEPROM.read(i));
    }
    if (jsonString.length() > 0) {
        deserializeJson(doc, jsonString);
    }
}

void MemoryStorage::saveToEEPROM(DynamicJsonDocument& doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    for (int i = 0; i < eepromSize; i++) {
        EEPROM.write(i, i < jsonString.length() ? jsonString[i] : 0);
    }
    EEPROM.commit();
}
