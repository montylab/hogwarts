#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <ESP8266WiFi.h>

class WiFiHandler {
public:
    // Возможные статусы подключения Wi-Fi
    enum WiFiStatus {
        CONNECTING,
        CONNECTED,
        SETUP_AP
    };

    // Конструктор
    WiFiHandler();

    // Метод для инициализации подключения к Wi-Fi
    void begin(void (*onConnectCallback)() = nullptr);

    // Метод для обновления состояния подключения (вызывается в loop)
    void update();

    // Получение текущего статуса Wi-Fi подключения
    WiFiStatus getStatus() const;

    // Получение IP-адреса (или "N/A", если не подключено)
    String getIPAddress() const;

    // Получение имени текущей сети (SSID)
    String getCurrentSSID() const;

    // Получение уровня сигнала (0 - нет сигнала, 1 - плохой, 2 - средний, 3 - хороший)
    int getSignalStrength() const;

    // Получение статуса подключения в виде строки ("Connecting", "Connected", "AP Mode")
    String getStatusString() const;

private:
    WiFiStatus status;                   // Текущий статус Wi-Fi подключения
    unsigned long connectionAttemptStart; // Время начала попытки подключения
    unsigned long maxAttempts;            // Максимальное время ожидания подключения
    bool primaryNetwork;                  // Указывает, пытаемся ли подключиться к первичной сети
    void (*onConnect)();                  // Коллбек для успешного подключения

    // Внутренний метод для запуска режима точки доступа
    void startAPMode();
};

#endif
