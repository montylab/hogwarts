#ifndef HARRY_SERVER_H
#define HARRY_SERVER_H

#include <ESP8266WebServer.h>
#include "ConfigStorage.h"
#include "./HasHTMLStat.h"

class HarryServer {
public:
    static void begin();
    static void handleClient();
    static void handleHome();
    static void handleStats();
    static void handleSettings();

    static void addStatProvider(HasHTMLStat* provider);

private:
    static ESP8266WebServer server;
    static std::vector<HasHTMLStat*> statProviders;

    // Статические методы для обработки конфигурационных параметров
    static void handleStringConfigParam(const String& paramName, void (*setFunc)(String));
    static void handleIntConfigParam(const String& paramName, void (*setFunc)(int));

    // Метод для генерации HTML-страниц
    static String renderPage(const String& body);
};

#endif // HARRY_SERVER_H
