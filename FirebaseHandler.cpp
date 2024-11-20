#define DISABLE_DEBUG

#include <Arduino.h>
#include <FirebaseClient.h>
#include <WiFiClientSecure.h>

// Класс для работы с Firebase
class FirebaseHandler {
public:
    // Конструктор принимает URL базы данных Firebase
    FirebaseHandler(const char* firebase_url) 
        : _firebase_url(firebase_url) {}

    // Функция для начала работы (настройка Firebase)
    void begin() {
        // Настройка Firebase
        ssl.setInsecure();  // Устанавливаем небезопасное соединение (не рекомендуется для продакшн)
#if defined(ESP8266)
        ssl.setBufferSizes(1024, 1024);
#endif

        initializeApp(client, app, getAuth(noAuth));
        app.getApp<RealtimeDatabase>(database);
        database.url(_firebase_url);
        client.setAsyncResult(result);

        Serial.println("Firebase initialized.");
    }

    // Функция записи значения по ключу
    void writeData(const String& key, float value) {
        String path = "/triggers/harry/" + key + "/now";  // Формирование пути к ключу в базе данных
        bool status = database.set<number_t>(client, path, number_t(value, 2));
        if (status) {
            Serial.println("Data successfully written to Firebase!");
        } else {
            printError(client.lastError().code(), client.lastError().message());
        }
    }
    
    void writeData(const String& key, String value) {
        String path = "/triggers/harry/" + key + "/now";  // Формирование пути к ключу в базе данных
        bool status = database.set<String>(client, path, value);
        if (status) {
            Serial.println("Data successfully written to Firebase!");
        } else {
            printError(client.lastError().code(), client.lastError().message());
        }
    }

private:
    const char* _firebase_url;

    WiFiClientSecure ssl;
    DefaultNetwork network;
    AsyncClientClass client{ssl, getNetwork(network)};
    FirebaseApp app;
    RealtimeDatabase database;
    AsyncResult result;
    NoAuth noAuth;

    // Функция вывода ошибки
    void printError(int code, const String& msg) {
        Serial.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
    }
};
