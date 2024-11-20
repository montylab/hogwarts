#include <EEPROM.h>
#include <ArduinoJson.h>

class MemoryStorage {
public:
  MemoryStorage(int eepromSize) {
    EEPROM.begin(eepromSize);
    this->eepromSize = eepromSize;
  }

  void saveParam(String key, String value) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    doc[key] = value;
    saveToEEPROM(doc);
  }

  void saveParam(String key, int value) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    doc[key] = value;
    saveToEEPROM(doc);
  }

  void saveParam(int key, int value) {
    saveParam(String(key), value);
  }
    void saveParam(int key, String value) {
    saveParam(String(key), value);
  }

  String readStringParam(String key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(key)) {
      return doc[key].as<String>();
    }
    return "";
  }

  String readStringParam(int key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(String(key))) {
      return doc[key].as<String>();
    }
    return "";
  }

  int readIntParam(String key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(key)) {
      return doc[key].as<int>();
    }
    return 0;
  }

  int readIntParam(int key) {
    DynamicJsonDocument doc(512);
    loadFromEEPROM(doc);
    if (doc.containsKey(String(key))) {
      return doc[key].as<int>();
    }
    return 0;
  }

private:
  int eepromSize;

  void loadFromEEPROM(DynamicJsonDocument& doc) {
    String jsonString;
    for (int i = 0; i < eepromSize; i++) {
      jsonString += char(EEPROM.read(i));
    }
    if (jsonString.length() > 0) {
      deserializeJson(doc, jsonString);
    }
  }

  void saveToEEPROM(DynamicJsonDocument& doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    for (int i = 0; i < eepromSize; i++) {
      EEPROM.write(i, i < jsonString.length() ? jsonString[i] : 0);
    }
    EEPROM.commit();
  }
};
