#include "HarryServer.h"
#include "ConfigStorage.h"

// Инициализация экземпляра веб-сервера на порту 80
ESP8266WebServer HarryServer::server(80);
std::vector<HasHTMLStat*> HarryServer::statProviders;

// Метод для запуска и настройки сервера
void HarryServer::begin() {
    // Настройка маршрутов
    server.on("/", handleHome);
    server.on("/stats", handleStats);
    server.on("/settings", handleSettings);

    // Запуск сервера
    server.begin();
    Serial.println("HTTP server started");
}

void HarryServer::addStatProvider(HasHTMLStat* provider) {
    statProviders.push_back(provider);
}

// Метод для обработки запросов
void HarryServer::handleClient() {
    server.handleClient();
}

void HarryServer::handleStringConfigParam(const String& paramName, void (*setFunc)(String)) {
    if (server.hasArg(paramName)) {
        setFunc(server.arg(paramName));
    }
}

void HarryServer::handleIntConfigParam(const String& paramName, void (*setFunc)(int)) {
    if (server.hasArg(paramName)) {
        int value = server.arg(paramName).toInt();
        if (value >= 0) {
            setFunc(value);
        }
    }
}

// Функция для отрисовки страницы с общим стилем и меню
String HarryServer::renderPage(const String& body) {
    String page = "<html><head>";
    page += "<style>";
    page += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f2f2f2; margin: 0; padding: 0; }";
    page += "header { background-color: #4CAF50; color: white; padding: 10px 0; margin-bottom: 20px; }";
    page += "nav { margin: 10px 0; }";
    page += "nav ul { list-style-type: none; padding: 0; }";
    page += "nav ul li { display: inline; margin-right: 15px; }";
    page += "nav ul li a { text-decoration: none; color: white; font-size: 1.2em; }";
    page += "nav ul li a:hover { text-decoration: underline; }";
    page += "main { width: 90%; max-width: 800px; margin: 0 auto; padding: 20px; background-color: white; border-radius: 8px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }"; // Центрируем main и ограничиваем ширину
    page += "input[type='text'], input[type='number'] { width: 100%; padding: 12px; margin: 10px 0; font-size: 1.1em; border: 1px solid #ccc; border-radius: 5px; }"; // Увеличенные инпуты, марджины и скругления
    page += "button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 1.2em; margin: 10px 2px; border-radius: 5px; cursor: pointer; }";
    page += "button:hover { background-color: #45a049; }";
    page += "</style>";
    page += "</head><body>";

    page += "<header>";
    page += "<h1>Device Dashboard</h1>";
    page += "<nav>";
    page += "<ul>";
    page += "<li><a href='/'>Home</a></li>";
    page += "<li><a href='/settings'>Settings</a></li>";
    page += "<li><a href='/stats'>Stats</a></li>";
    page += "<li><a href='https://pippers-home.web.app/' target='_blank'>Dashboard</a></li>";
    page += "</ul>";
    page += "</nav>";
    page += "</header>";

    page += "<main>";  // Начало main, который будет центрирован, но контент внутри не центрируется
    page += body;
    page += "</main>";

    page += "</body></html>";

    return page;
}

// Обработчик для страницы /
void HarryServer::handleHome() {
    String body = "<h2>Welcome to the Harry Sensor</h2>";

    server.send(200, "text/html", renderPage(body));
}

// Обработчик для страницы /stats
void HarryServer::handleStats() {
    String body = "<h2>Statistics</h2>";
    
    for (auto provider : statProviders) {
        body += provider->getHTMLStat();
    }

    server.send(200, "text/html", renderPage(body));
}

void HarryServer::handleSettings() {
    String body = "<h2>Settings</h2>";
    body += "<form action='/settings' method='POST' onsubmit='handleSubmit()'>";

    body += "<table style='width:100%; text-align:left; border-collapse:collapse;'>";

    // WiFi параметры
    body += "<tr><td>WiFi Network 1:</td><td><input type='text' name='wifi1_network' value='" + ConfigStorage::getWifi1Network() + "'></td></tr>";
    body += "<tr><td>WiFi Password 1:</td><td><input type='text' name='wifi1_password' value='" + ConfigStorage::getWifi1Password() + "'></td></tr>";
    body += "<tr><td>WiFi Network 2:</td><td><input type='text' name='wifi2_network' value='" + ConfigStorage::getWifi2Network() + "'></td></tr>";
    body += "<tr><td>WiFi Password 2:</td><td><input type='text'name='wifi2_password' value='" + ConfigStorage::getWifi2Password() + "'></td></tr>";

    // Firebase параметры
    body += "<tr><td>Firebase Sync Time:</td><td><input type='number' name='firebase_sync_time' value='" + String(ConfigStorage::getFirebaseSyncTime()) + "'></td></tr>";

    // CO2 параметры
    body += "<tr><td>CO2 Sensor Poll Time:</td><td><input type='number' name='co2_sensor_poll_time' value='" + String(ConfigStorage::getCO2SensorPollTime()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor CO2 Range 0:</td><td><input type='number' name='co2_sensor_co2_range_0' value='" + String(ConfigStorage::getCO2SensorCO2Range0()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor CO2 Range 1:</td><td><input type='number' name='co2_sensor_co2_range_1' value='" + String(ConfigStorage::getCO2SensorCO2Range1()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor CO2 Raw Range 0:</td><td><input type='number' name='co2_sensor_co2_raw_range_0' value='" + String(ConfigStorage::getCO2SensorCO2RawRange0()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor CO2 Raw Range 1:</td><td><input type='number' name='co2_sensor_co2_raw_range_1' value='" + String(ConfigStorage::getCO2SensorCO2RawRange1()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor Temperature Range 0:</td><td><input type='number' name='co2_sensor_temperature_range_0' value='" + String(ConfigStorage::getCO2SensorTemperatureRange0()) + "'></td></tr>";
    body += "<tr><td>CO2 Sensor Temperature Range 1:</td><td><input type='number' name='co2_sensor_temperature_range_1' value='" + String(ConfigStorage::getCO2SensorTemperatureRange1()) + "'></td></tr>";

    // Параметры подсветки дисплея
    body += "<tr><td>Display Brightness (0-255):</td><td><input type='number' name='tft_backlight' value='" + String(ConfigStorage::getTFTBacklight()) + "'></td></tr>";

    body += "</table>";

    // Кнопка "Save"
    body += "<br><button type='submit' style='background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; font-size: 1.2em; border-radius: 5px; cursor: pointer;'>Update</button>";
    body += "</form>";

    body += "<script>";
    body += "function handleSubmit() {";
    body += "  setTimeout(function() {";
    body += "    window.location.reload();";
    body += "  }, 500);"; // Задержка для уверенности, что данные сохранены
    body += "}";
    body += "</script>";

    // Если запрос POST, сохраняем новые значения в EEPROM
    if (server.method() == HTTP_POST) {
        HarryServer::handleStringConfigParam("wifi1_network", ConfigStorage::setWifi1Network);
        HarryServer::handleStringConfigParam("wifi1_password", ConfigStorage::setWifi1Password);
        HarryServer::handleStringConfigParam("wifi2_network", ConfigStorage::setWifi2Network);
        HarryServer::handleStringConfigParam("wifi2_password", ConfigStorage::setWifi2Password);
        HarryServer::handleIntConfigParam("firebase_sync_time", ConfigStorage::setFirebaseSyncTime);
        HarryServer::handleIntConfigParam("co2_sensor_poll_time", ConfigStorage::setCO2SensorPollTime);
        HarryServer::handleIntConfigParam("co2_sensor_co2_range_0", ConfigStorage::setCO2SensorCO2Range0);
        HarryServer::handleIntConfigParam("co2_sensor_co2_range_1", ConfigStorage::setCO2SensorCO2Range1);
        HarryServer::handleIntConfigParam("co2_sensor_co2_raw_range_0", ConfigStorage::setCO2SensorCO2RawRange0);
        HarryServer::handleIntConfigParam("co2_sensor_co2_raw_range_1", ConfigStorage::setCO2SensorCO2RawRange1);
        HarryServer::handleIntConfigParam("co2_sensor_temperature_range_0", ConfigStorage::setCO2SensorTemperatureRange0);
        HarryServer::handleIntConfigParam("co2_sensor_temperature_range_1", ConfigStorage::setCO2SensorTemperatureRange1);

        // Сохранение яркости подсветки дисплея
        HarryServer::handleIntConfigParam("tft_backlight", ConfigStorage::setTFTBacklight);

        // Перенаправляем обратно на /settings
        server.sendHeader("Location", "/settings");
        server.send(303);
        return;
    }

    // Отправляем HTML-ответ
    server.send(200, "text/html", renderPage(body));
}
