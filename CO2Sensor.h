#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include <MHZ19.h>
#include <SoftwareSerial.h>
#include "HasHTMLStat.h"
#include <vector>

class CO2Sensor: public HasHTMLStat {
private:
    MHZ19 mhz19;                  // Экземпляр сенсора
    SoftwareSerial* serial;        // Для работы с сенсором через определенные пины
    unsigned long callbackTime;    // Время между вызовами коллбека
    unsigned long sensorTime;      // Время между опросами сенсора
    unsigned long lastSensorRead;  // Время последнего опроса сенсора
    unsigned long lastCallback;    // Время последнего вызова коллбека
    
    std::vector<int> CO2data;      // Массив данных CO2
    std::vector<int> temperatureData; // Массив данных температуры
    std::vector<int> rawCO2Data;   // Массив данных raw CO2

    void (*callback)(float, float, float); // Коллбек функция
    
    // Вспомогательная функция для вычисления среднего значения
    float average(const std::vector<int>& data);
    
public:
    // Конструктор
    CO2Sensor(int rxPin, int txPin);

    // Функция инициализации с параметрами или дефолтными значениями
    void begin(unsigned long cbTime = 60000, unsigned long sTime = 15000, void (*cb)(float, float, float) = nullptr);

    // Установка времени вызова коллбека
    void setCallbackTime(unsigned long cbTime);

    // Установка времени опроса сенсора
    void setSensorTime(unsigned long sTime);

    int getLastCO2() const;
    float getLastTemperature() const;

    int lastCO2;
    int lastRawCO2;
    int lastTemperature;


    String getHTMLStat() const override;


    // Функция для работы в основном цикле программы (должна вызываться в loop)
    void update();
};

#endif // CO2SENSOR_H
