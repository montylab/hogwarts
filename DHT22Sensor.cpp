#include "DHT22Sensor.h"
#include <Arduino.h>

// Constructor
DHT22Sensor::DHT22Sensor(int pin) : pin(pin), dht(pin) {
    callbackTime = 60000; // 60 seconds default
    sensorTime = 15000;   // 15 seconds default
    lastSensorRead = 0;
    lastCallback = 0;
    callback = nullptr;
    lastTemperature = 0.0;
    lastHumidity = 0.0;
}

// Initialization function
void DHT22Sensor::begin(unsigned long cbTime, unsigned long sTime, void (*cb)(float, float)) {
    callbackTime = cbTime;
    sensorTime = sTime;
    callback = cb;
    lastSensorRead = millis();
    lastCallback = millis();
}

// Update function
void DHT22Sensor::update() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastSensorRead >= sensorTime) {
        lastSensorRead = currentMillis;

        float temperature = dht.getTemperature();
        float humidity = dht.getHumidity();

        if (!isnan(temperature) && temperature >= -40 && temperature <= 80) {
            lastTemperature = temperature;
            temperatureData.push_back(temperature);
        } else {
            Serial.println("Invalid temperature reading.");
        }

        if (!isnan(humidity) && humidity >= 0 && humidity <= 100) {
            lastHumidity = humidity;
            humidityData.push_back(humidity);
        } else {
            Serial.println("Invalid humidity reading.");
        }

        Serial.print("Temperature: ");
        Serial.println(lastTemperature);
        Serial.print("Humidity: ");
        Serial.println(lastHumidity);
    }

    if (callback && currentMillis - lastCallback >= callbackTime) {
        lastCallback = currentMillis;

        float avgTemperature = average(temperatureData);
        float avgHumidity = average(humidityData);

        Serial.println("Calling callback with averaged data...");
        Serial.print("Avg Temperature: ");
        Serial.print(avgTemperature);
        Serial.print(" °C, Avg Humidity: ");
        Serial.print(avgHumidity);
        Serial.println(" %");

        callback(avgTemperature, avgHumidity);

        temperatureData.clear();
        humidityData.clear();
    }
}

// Get the last readings
float DHT22Sensor::getLastTemperature() const {
    return lastTemperature;
}

float DHT22Sensor::getLastHumidity() const {
    return lastHumidity;
}

// Compute the average of a vector
float DHT22Sensor::average(const std::vector<float>& data) {
    if (data.empty()) return 0.0;
    float sum = 0;
    for (float value : data) {
        sum += value;
    }
    return sum / data.size();
}

// Generate HTML representation of the sensor stats
String DHT22Sensor::getHTMLStat() const {
    String html = "<div style='background-color: #f0f0f0; padding: 15px; border-radius: 8px; margin-bottom: 10px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);'>";
    html += "<h2 style='font-size: 1.5em; color: #333;'>DHT22 Sensor Stats</h2>";
    html += "<p style='font-size: 1.2em; color: #555;'>Sensor: DHT22</p>";
    html += "<p style='font-size: 1.2em; color: #555;'>Last Temperature Reading: " + String(lastTemperature) + " °C</p>";
    html += "<p style='font-size: 1.2em; color: #555;'>Last Humidity Reading: " + String(lastHumidity) + " %</p>";

    html += "<h3 style='font-size: 1.2em; color: #333;'>Recent Temperature Readings:</h3><ul>";
    for (float value : temperatureData) {
        html += "<li>" + String(value) + " °C</li>";
    }
    html += "</ul>";

    html += "<h3 style='font-size: 1.2em; color: #333;'>Recent Humidity Readings:</h3><ul>";
    for (float value : humidityData) {
        html += "<li>" + String(value) + " %</li>";
    }
    html += "</ul>";

    html += "<form action='/stats' method='get'>";
    html += "<input type='submit' value='Update' style='padding: 10px 20px; font-size: 1.2em; color: white; background-color: #4CAF50; border: none; border-radius: 5px; cursor: pointer;'>";
    html += "</form></div>";

    return html;
}
