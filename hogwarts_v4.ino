#define DISABLE_DEBUG

#include "./CO2Sensor.h"
#include "./WiFiHandler.h"
#include "./FirebaseHandler.cpp"
#include "./ConfigStorage.h"  
#include "./HarryServer.h"
#include "./TFT.cpp"

#define FIREBASE_DOMAIN "pippers-home.firebaseio.com"
#define FIREBASE_AUTH "cwHRVnHQkg9qcZXu6OzSPf9bILjr1YU8iZY6bSUN"

FirebaseHandler firebaseHandler(FIREBASE_DOMAIN);
CO2Sensor co2sensor(D2, D1); //RX и TX
WiFiHandler wifiHandler;

// Инициализация дисплея
#define __CS D0
#define __DC D4
#define __RST D8
#define __BLACKLIGHT D6
DisplayModule display(__CS, __DC, __RST, __BLACKLIGHT);

void setup() {
    Serial.print("SETUP \n");
    Serial.begin(115200);

    // Загружаем конфигурацию
    ConfigStorage::begin();

    display.begin(&co2sensor, &wifiHandler);

    // Настройка WiFi
    wifiHandler.begin(postWiFiSetup);
}

void postWiFiSetup() {
    Serial.print("POST WIFI SETUP \n");

      // Получаем параметры из конфигурации
    int firebaseSyncTime = ConfigStorage::getFirebaseSyncTime();
    int co2PollTime = ConfigStorage::getCO2SensorPollTime();

    // Настройка CO2 сенсора
    co2sensor.begin(firebaseSyncTime, co2PollTime, sensorCallback);

    // Настройка Firebase
    firebaseHandler.begin();  

    HarryServer::begin();
    HarryServer::addStatProvider(&co2sensor);
    //firebaseHandler.writeData("Local_IP", IP);
}

void loop() {
  display.updateDisplay();  // Обновляем дисплей
  co2sensor.update();

  if (wifiHandler.getStatus() == WiFiHandler::CONNECTED) {
    HarryServer::handleClient();
  } else {
    wifiHandler.update();
  }  
}

// Коллбек функция
void sensorCallback(float avgCO2, float avgTemp, float avgRawCO2) {
    Serial.print("Среднее CO2: ");
    Serial.println(avgCO2);
    Serial.print("Средний raw CO2: ");
    Serial.println(avgRawCO2);
    Serial.print("Средняя температура: ");
    Serial.println(avgTemp);

    // Отправка данных в Firebase
    firebaseHandler.writeData("co2", avgCO2);
    firebaseHandler.writeData("co2_raw", avgRawCO2);
    firebaseHandler.writeData("temperature", avgTemp);
}
