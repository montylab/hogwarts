#ifndef CONFIG_STORAGE_H
#define CONFIG_STORAGE_H

#include "MemoryStorage.h"

class ConfigStorage {
public:
    enum ParamKey {
        WIFI_1_NETWORK = 1,
        WIFI_1_PASSWORD,
        WIFI_2_NETWORK,
        WIFI_2_PASSWORD,
        FIREBASE_SYNC_TIME,
        CO2_SENSOR_POLL_TIME,
        CO2_SENSOR_CO2_RANGE_0,
        CO2_SENSOR_CO2_RANGE_1,
        CO2_SENSOR_CO2_RAW_RANGE_0,
        CO2_SENSOR_CO2_RAW_RANGE_1,
        CO2_SENSOR_TEMPERATURE_RANGE_0,
        CO2_SENSOR_TEMPERATURE_RANGE_1,
        TFT_BACKLIGHT
    };

    static void begin();
    
    // Геттеры
    static String getWifi1Network();
    static String getWifi1Password();
    static String getWifi2Network();
    static String getWifi2Password();
    static int getFirebaseSyncTime();
    static int getCO2SensorPollTime();
    static int getCO2SensorCO2Range0();
    static int getCO2SensorCO2Range1();
    static int getCO2SensorCO2RawRange0();
    static int getCO2SensorCO2RawRange1();
    static int getCO2SensorTemperatureRange0();
    static int getCO2SensorTemperatureRange1();
    static int getTFTBacklight();

    // Сеттеры
    static void setWifi1Network(String value);
    static void setWifi1Password(String value);
    static void setWifi2Network(String value);
    static void setWifi2Password(String value);
    static void setFirebaseSyncTime(int value);
    static void setCO2SensorPollTime(int value);
    static void setCO2SensorCO2Range0(int value);
    static void setCO2SensorCO2Range1(int value);
    static void setCO2SensorCO2RawRange0(int value);
    static void setCO2SensorCO2RawRange1(int value);
    static void setCO2SensorTemperatureRange0(int value);
    static void setCO2SensorTemperatureRange1(int value);
    static void setTFTBacklight(int value);


    static void restart();

private:
    static MemoryStorage storage;
    
    static void ensureStringParam(ParamKey key, String defaultValue);
    static void ensureIntParam(ParamKey key, int defaultValue);
};

#endif
