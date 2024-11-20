#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ConfigStorage.h"
#include "./WiFiHandler.h"

WiFiHandler::WiFiHandler() 
    : status(CONNECTING), connectionAttemptStart(0), maxAttempts(20000), onConnect(nullptr) {}

// Метод для инициализации и начала подключения
void WiFiHandler::begin(void (*onConnectCallback)()) {
    onConnect = onConnectCallback;
    String ssid1 = ConfigStorage::getWifi1Network();
    String password1 = ConfigStorage::getWifi1Password();
    String ssid2 = ConfigStorage::getWifi2Network();
    String password2 = ConfigStorage::getWifi2Password();

    // Попытка подключения к первичной сети
    Serial.print("\nTrying to connect to ");
    Serial.println(ssid1);
    WiFi.begin(ssid1, password1);
    status = CONNECTING;
    connectionAttemptStart = millis();
    primaryNetwork = true;  // Изначально подключаемся к основной сети
}

// Метод для обновления статуса подключения
void WiFiHandler::update() {
    if (status == CONNECTING) {
        // Если прошло слишком много времени, пытаемся переключиться на вторую сеть
        if (millis() - connectionAttemptStart >= maxAttempts) {
            if (primaryNetwork) {
                // Попытка подключения ко вторичной сети
                String ssid2 = ConfigStorage::getWifi2Network();
                String password2 = ConfigStorage::getWifi2Password();
                Serial.print("Trying to connect to ");
                Serial.println(ssid2);
                WiFi.begin(ssid2, password2);
                primaryNetwork = false;
                connectionAttemptStart = millis();  // Перезапуск таймера
            } else {
                // Если ни одна сеть не подошла, запускаем режим точки доступа
                startAPMode();
            }
        } else if (WiFi.status() == WL_CONNECTED) {
            // Подключение успешно
            status = CONNECTED;
            Serial.print("Connected! IP Address: ");
            Serial.println(WiFi.localIP());

            // Вызываем функцию коллбек, если она установлена
            if (onConnect) {
                onConnect();
            }
        }
    }
}

// Метод для получения статуса подключения
WiFiHandler::WiFiStatus WiFiHandler::getStatus() const {
    return status;
}

// Метод для запуска режима точки доступа
void WiFiHandler::startAPMode() {
    Serial.println("Starting Access Point...");
    WiFi.softAP("ConfigAP_harry_sensor");
    status = SETUP_AP;
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());
}

// Новый метод: Получение IP-адреса
String WiFiHandler::getIPAddress() const {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.localIP().toString();  // Возвращаем IP-адрес в виде строки
    } else {
        return "N/A";  // Если IP недоступен, возвращаем "N/A"
    }
}

// Новый метод: Получение имени текущей сети
String WiFiHandler::getCurrentSSID() const {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.SSID();  // Возвращаем имя сети (SSID)
    } else {
        return "N/A";  // Если сеть не подключена, возвращаем "N/A"
    }
}

// Новый метод: Получение уровня сигнала Wi-Fi (0-3)
int WiFiHandler::getSignalStrength() const {
    if (WiFi.status() == WL_CONNECTED) {
        int32_t rssi = WiFi.RSSI();  // Получаем уровень сигнала в дБ

        // Преобразуем RSSI в более простую шкалу (0-3)
        if (rssi >= -55) {
            return 3;  // Хороший сигнал
        } else if (rssi >= -70) {
            return 2;  // Средний сигнал
        } else if (rssi >= -85) {
            return 1;  // Плохой сигнал
        } else {
            return 0;  // Очень плохой сигнал
        }
    } else {
        return 0;  // Если не подключен, уровень сигнала 0
    }
}

// Новый метод: Получение текущего статуса подключения в виде строки
String WiFiHandler::getStatusString() const {
    switch (status) {
        case CONNECTING:
            return "Connecting";
        case CONNECTED:
            return "Connected";
        case SETUP_AP:
            return "AP Mode";
        default:
            return "Unknown";
    }
}
