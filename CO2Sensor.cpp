#include "CO2Sensor.h"
#include <Arduino.h>  // Для использования Serial

// Конструктор
CO2Sensor::CO2Sensor(int rxPin, int txPin) {
    serial = new SoftwareSerial(rxPin, txPin);  // Создание объекта SoftwareSerial для общения с датчиком
    serial->begin(9600);
    mhz19.begin(*serial);
    callbackTime = 60000; // 60 секунд по умолчанию
    sensorTime = 15000;   // 15 секунд по умолчанию
    lastSensorRead = 0;
    lastCallback = 0;
    callback = nullptr;
    lastCO2 = 0;
    lastRawCO2 = 0;
    lastTemperature = 0;
}

// Функция инициализации с параметрами или дефолтными значениями
void CO2Sensor::begin(unsigned long cbTime, unsigned long sTime, void (*cb)(float, float, float)) {
    callbackTime = cbTime;
    sensorTime = sTime;
    callback = cb;
    lastSensorRead = millis();
    lastCallback = millis();
}

// Установка времени вызова коллбека
void CO2Sensor::setCallbackTime(unsigned long cbTime) {
    callbackTime = cbTime;
}

// Установка времени опроса сенсора
void CO2Sensor::setSensorTime(unsigned long sTime) {
    sensorTime = sTime;
}

// Функция для работы в основном цикле программы (должна вызываться в loop)
void CO2Sensor::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastSensorRead >= sensorTime) {
        lastSensorRead = currentMillis;

        // Считываем последние значения с сенсора
        lastCO2 = mhz19.getCO2();
        lastRawCO2 = mhz19.getCO2Raw();
        lastTemperature = mhz19.getTemperature();

        // Проверяем и выводим CO2
        if (lastCO2 >= 300 && lastCO2 <= 5000) {
            CO2data.push_back(lastCO2);
        } else {
            Serial.print("Получено недопустимое значение CO2: ");
        }
        Serial.print("CO2: ");
        Serial.println(lastCO2);

        // Проверяем и выводим raw CO2
        if (lastRawCO2 >= 0 && lastRawCO2 <= 100000) {
            rawCO2Data.push_back(lastRawCO2);
        } else {
            Serial.print("Получено недопустимое значение raw CO2: ");
        }
        Serial.print("raw CO2: ");
        Serial.println(lastRawCO2);

        // Проверяем и выводим температуру
        if (lastTemperature >= 0 && lastTemperature <= 100) {
            temperatureData.push_back(lastTemperature);
        } else {
            Serial.print("Получено недопустимое значение температуры: ");
        }
        Serial.print("Температура: ");
        Serial.println(lastTemperature);
    }

    if (callback && currentMillis - lastCallback >= callbackTime) {
        lastCallback = currentMillis;

        // Вычисляем средние значения CO2, raw CO2 и температуры
        float avgCO2 = average(CO2data);
        float avgRawCO2 = average(rawCO2Data);
        float avgTemp = average(temperatureData);

        // Выводим средние данные перед вызовом коллбека
        Serial.println("Calling callback with averaged data...");
        Serial.print("Avg CO2: ");
        Serial.print(avgCO2);
        Serial.print(" ppm, Avg raw CO2: ");
        Serial.print(avgRawCO2);
        Serial.print(" ppm, Avg Temperature: ");
        Serial.print(avgTemp);
        Serial.println(" °C");

        // Вызываем коллбек с передачей средних значений
        callback(avgCO2, avgTemp, avgRawCO2);

        // Очищаем массивы
        CO2data.clear();
        temperatureData.clear();
        rawCO2Data.clear();
    }
}

// Метод для получения последней температуры
float CO2Sensor::getLastTemperature() const {
    return lastTemperature;
}

// Метод для получения последнего значения CO2
int CO2Sensor::getLastCO2() const {
    return lastCO2;
}

// Вспомогательная функция для вычисления среднего значения
float CO2Sensor::average(const std::vector<int>& data) {
    if (data.empty()) return 0;
    int sum = 0;
    for (int value : data) {
        sum += value;
    }
    return static_cast<float>(sum) / data.size();
}

String CO2Sensor::getHTMLStat() const {
    String html = "<div style='background-color: #f0f0f0; padding: 15px; border-radius: 8px; margin-bottom: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);'>";
    
    html += "<h2 style='font-size: 1.5em; color: #333;'>CO2 Sensor Stats</h2>";
    
    // Добавляем название сенсора
    html += "<p style='font-size: 1.2em; color: #555;'>Sensor: CO2 Sensor</p>";
    
    // Добавляем последние считанные значения
    html += "<p style='font-size: 1.2em; color: #555;'>Last CO2 Reading: " + String(lastCO2) + " ppm</p>";
    html += "<p style='font-size: 1.2em; color: #555;'>Last Raw CO2 Reading: " + String(lastRawCO2) + " ppm</p>";
    html += "<p style='font-size: 1.2em; color: #555;'>Last Temperature Reading: " + String(lastTemperature) + " °C</p>";

    // Добавляем массив последних значений
    html += "<h3 style='font-size: 1.2em; color: #333;'>Recent CO2 Readings:</h3>";
    html += "<ul>";
    for (int value : CO2data) {
        html += "<li>" + String(value) + " ppm</li>";
    }
    html += "</ul>";
    
    html += "<h3 style='font-size: 1.2em; color: #333;'>Recent Raw CO2 Readings:</h3>";
    html += "<ul>";
    for (int value : rawCO2Data) {
        html += "<li>" + String(value) + " ppm</li>";
    }
    html += "</ul>";

    html += "<h3 style='font-size: 1.2em; color: #333;'>Recent Temperature Readings:</h3>";
    html += "<ul>";
    for (int value : temperatureData) {
        html += "<li>" + String(value) + " °C</li>";
    }
    html += "</ul>";

    // Добавляем кнопку обновления
    html += "<form action='/stats' method='get'>";
    html += "<input type='submit' value='Update' style='padding: 10px 20px; font-size: 1.2em; color: white; background-color: #4CAF50; border: none; border-radius: 5px; cursor: pointer;'>";
    html += "</form>";

    html += "</div>";

    return html;
}
