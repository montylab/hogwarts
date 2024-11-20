#ifndef DHT22SENSOR_H
#define DHT22SENSOR_H

#include "DHT22.h"  // Include the DHT library
#include <vector>
#include "HasHTMLStat.h"  // Include the interface header for HTML stats

class DHT22Sensor : public HasHTMLStat {
private:
    int pin;
    DHT22 dht;

    unsigned long callbackTime;
    unsigned long sensorTime;
    unsigned long lastSensorRead;
    unsigned long lastCallback;

    std::vector<float> temperatureData;
    std::vector<float> humidityData;

    float lastTemperature;
    float lastHumidity;

    void (*callback)(float, float);

    float average(const std::vector<float>& data);

public:
    // Constructor
    DHT22Sensor(int pin);

    // Initialization function
    void begin(unsigned long cbTime, unsigned long sTime, void (*cb)(float, float));

    // Update function
    void update();

    // Get the last readings
    float getLastTemperature() const;
    float getLastHumidity() const;

    // Implement the HTML generation method from HasHTMLStat
    String getHTMLStat() const override;
};

#endif
