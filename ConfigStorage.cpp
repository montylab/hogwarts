#include "ConfigStorage.h"

// Инициализация статической переменной
MemoryStorage ConfigStorage::storage;

// Инициализация статического метода
void ConfigStorage::begin() {
    storage.begin(512);  // Инициализация EEPROM с размером 512

    // WiFi параметры
    ConfigStorage::ensureStringParam(WIFI_1_NETWORK, "dom_jamnika");
    ConfigStorage::ensureStringParam(WIFI_1_PASSWORD, "Freewifi12");
    ConfigStorage::ensureStringParam(WIFI_2_NETWORK, "monty's portable");
    ConfigStorage::ensureStringParam(WIFI_2_PASSWORD, "freewifi");

    // Firebase параметры
    ConfigStorage::ensureIntParam(FIREBASE_SYNC_TIME, 60000);

    // CO2 параметры
    ConfigStorage::ensureIntParam(CO2_SENSOR_POLL_TIME, 15000);
    ConfigStorage::ensureIntParam(CO2_SENSOR_CO2_RANGE_0, 300);
    ConfigStorage::ensureIntParam(CO2_SENSOR_CO2_RANGE_1, 5000);
    ConfigStorage::ensureIntParam(CO2_SENSOR_CO2_RAW_RANGE_0, 0);
    ConfigStorage::ensureIntParam(CO2_SENSOR_CO2_RAW_RANGE_1, 100000);
    ConfigStorage::ensureIntParam(CO2_SENSOR_TEMPERATURE_RANGE_0, 0);
    ConfigStorage::ensureIntParam(CO2_SENSOR_TEMPERATURE_RANGE_1, 100);

    ConfigStorage::ensureIntParam(TFT_BACKLIGHT, 128);
}

// Геттеры
String ConfigStorage::getWifi1Network() {
    return storage.readStringParam(WIFI_1_NETWORK);
}

String ConfigStorage::getWifi1Password() {
    return storage.readStringParam(WIFI_1_PASSWORD);
}

String ConfigStorage::getWifi2Network() {
    return storage.readStringParam(WIFI_2_NETWORK);
}

String ConfigStorage::getWifi2Password() {
    return storage.readStringParam(WIFI_2_PASSWORD);
}

int ConfigStorage::getFirebaseSyncTime() {
    return storage.readIntParam(FIREBASE_SYNC_TIME);
}

int ConfigStorage::getCO2SensorPollTime() {
    return storage.readIntParam(CO2_SENSOR_POLL_TIME);
}

int ConfigStorage::getCO2SensorCO2Range0() {
    return storage.readIntParam(CO2_SENSOR_CO2_RANGE_0);
}

int ConfigStorage::getCO2SensorCO2Range1() {
    return storage.readIntParam(CO2_SENSOR_CO2_RANGE_1);
}

int ConfigStorage::getCO2SensorCO2RawRange0() {
    return storage.readIntParam(CO2_SENSOR_CO2_RAW_RANGE_0);
}

int ConfigStorage::getCO2SensorCO2RawRange1() {
    return storage.readIntParam(CO2_SENSOR_CO2_RAW_RANGE_1);
}

int ConfigStorage::getCO2SensorTemperatureRange0() {
    return storage.readIntParam(CO2_SENSOR_TEMPERATURE_RANGE_0);
}

int ConfigStorage::getCO2SensorTemperatureRange1() {
    return storage.readIntParam(CO2_SENSOR_TEMPERATURE_RANGE_1);
}

int ConfigStorage::getTFTBacklight() {
    return storage.readIntParam(TFT_BACKLIGHT);
}

// Сеттеры
void ConfigStorage::setWifi1Network(String value) {
    storage.saveParam(WIFI_1_NETWORK, value);
}

void ConfigStorage::setWifi1Password(String value) {
    storage.saveParam(WIFI_1_PASSWORD, value);
}

void ConfigStorage::setWifi2Network(String value) {
    storage.saveParam(WIFI_2_NETWORK, value);
}

void ConfigStorage::setWifi2Password(String value) {
    storage.saveParam(WIFI_2_PASSWORD, value);
}

void ConfigStorage::setFirebaseSyncTime(int value) {
    storage.saveParam(FIREBASE_SYNC_TIME, value);
}

void ConfigStorage::setCO2SensorPollTime(int value) {
    storage.saveParam(CO2_SENSOR_POLL_TIME, value);
}

void ConfigStorage::setCO2SensorCO2Range0(int value) {
    storage.saveParam(CO2_SENSOR_CO2_RANGE_0, value);
}

void ConfigStorage::setCO2SensorCO2Range1(int value) {
    storage.saveParam(CO2_SENSOR_CO2_RANGE_1, value);
}

void ConfigStorage::setCO2SensorCO2RawRange0(int value) {
    storage.saveParam(CO2_SENSOR_CO2_RAW_RANGE_0, value);
}

void ConfigStorage::setCO2SensorCO2RawRange1(int value) {
    storage.saveParam(CO2_SENSOR_CO2_RAW_RANGE_1, value);
}

void ConfigStorage::setCO2SensorTemperatureRange0(int value) {
    storage.saveParam(CO2_SENSOR_TEMPERATURE_RANGE_0, value);
}

void ConfigStorage::setCO2SensorTemperatureRange1(int value) {
    storage.saveParam(CO2_SENSOR_TEMPERATURE_RANGE_1, value);
}

void ConfigStorage::setTFTBacklight(int value) {
    storage.saveParam(TFT_BACKLIGHT, value);
}

// Метод для перезагрузки конфигурации
void ConfigStorage::restart() {
    // Перезагрузка устройства или другие действия
}

// Проверка и запись строки
void ConfigStorage::ensureStringParam(ParamKey key, String defaultValue) {
    if (storage.readStringParam(key) == "") {
        storage.saveParam(key, defaultValue);
    }
}

// Проверка и запись целого числа
void ConfigStorage::ensureIntParam(ParamKey key, int defaultValue) {
    if (storage.readIntParam(key) == 0) {
        storage.saveParam(key, defaultValue);
    }
}
