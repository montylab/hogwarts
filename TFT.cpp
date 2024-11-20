#include <SPI.h>
#include <TFT_ILI9163C.h>
#include "./CO2Sensor.h"
#include "./WiFiHandler.h"
#include "./ConfigStorage.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define ORANGE  0xFD20
#define WHITE   0xFFFF
#define BROWN   0x8000
#define MAROON  0x7800

#define PADDING_X 5
#define PADDING_Y 5

class DisplayModule {
public:
    DisplayModule(int csPin, int dcPin, int rstPin, int blightPin)
        : tft(csPin, dcPin, rstPin), lastCO2(-1), lastTemperature(-999), lastUpdateTime(0), updateInterval(1000), backlightPin(255) {
        backlightPin = blightPin;
        SPI.begin();
        SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));  // 8 МГц, MSB первым, режим 0
    }

    void begin(CO2Sensor* sensor, WiFiHandler* wifi) {
        this->sensor = sensor;
        this->wifi = wifi;

        tft.begin();

        pinMode(backlightPin, OUTPUT);  // Настраиваем пин для управления подсветкой как выход
        setBacklight(128);  // Включаем подсветку по умолчанию

        tft.setRotation(2);  // Поворачиваем дисплей (если нужно)
        tft.fillScreen(BLACK);  // Очищаем экран
        tft.setTextSize(1);  // Уменьшаем размер текста
        tft.setTextColor(WHITE);  // Цвет текста по умолчанию
    }

    void updateDisplay() {
        if (millis() - lastUpdateTime < updateInterval) {
            return;
        }

        lastUpdateTime = millis();
        
        setBacklight(ConfigStorage::getTFTBacklight());

        // Получаем данные с датчика CO2
        float temperature = sensor->getLastTemperature();
        int co2 = sensor->getLastCO2();

        // Обновляем блок CO2 и температуры только если значения изменились
        if (co2 != lastCO2 || temperature != lastTemperature) {
            updateCO2AndTemperature(co2, temperature);
            lastCO2 = co2;
            lastTemperature = temperature;
        }

        // Обновляем Wi-Fi информацию
        updateWiFi();

        // Обновляем IP адрес
        updateIP();

        // Обновляем аптайм
        updateUptime();
    }

    // Установка уровня яркости подсветки (от 0 до 255)
    void setBacklight(uint8_t brightness) {
        analogWrite(backlightPin, brightness);
    }


private:
    TFT_ILI9163C tft;
    CO2Sensor* sensor;
    WiFiHandler* wifi;
    int backlightPin;
    int lastCO2;
    float lastTemperature;
    unsigned long lastUpdateTime;
    const unsigned long updateInterval;

    // Метод для обновления CO2 и температуры в одном блоке
    void updateCO2AndTemperature(int co2, float temperature) {
        int blockWidth = tft.width() - 2 * PADDING_X;
        int blockHeight = 60;  // Высота блока 60 пикселей
        int blockX = PADDING_X;
        int blockY = PADDING_Y;

        // Определение цвета фона в зависимости от значения CO2
        uint16_t bgColor;
        if (co2 < 600) {
            bgColor = GREEN;
        } else if (co2 < 800) {
            bgColor = YELLOW;
        } else if (co2 < 1000) {
            bgColor = ORANGE;
        } else if (co2 < 1200) {
            bgColor = RED;
        } else {
            bgColor = MAROON;
        }

        // Стираем старый текст, отрисовывая его цветом фона
        eraseText(lastCO2, lastTemperature, bgColor, blockX, blockY, blockWidth, blockHeight);

        // Рисуем новый фон блока
        tft.fillRect(blockX, blockY, blockWidth, blockHeight, bgColor);

        // Отображаем значение CO2 и температуру, центрируя текст
        tft.setTextColor(BLACK);
        tft.setTextSize(2);

        // CO2 строка
        String co2Text = String(co2) + " ppm";
        int16_t co2X = getCenteredX(co2Text.length() * 12);  // 12 пикселей на символ
        int16_t co2Y = blockY + 10;  // Центрирование по Y

        tft.setCursor(co2X, co2Y);
        tft.print(co2Text);

        // Температурная строка
        String tempText = String(temperature, 1) + " C";
        int16_t tempX = getCenteredX(tempText.length() * 12);
        int16_t tempY = blockY + 35;

        tft.setCursor(tempX, tempY);
        tft.print(tempText);
    }

    // Метод для стирания старого текста, выводя его цветом фона
    void eraseText(int co2, float temperature, uint16_t bgColor, int blockX, int blockY, int blockWidth, int blockHeight) {
        tft.setTextColor(bgColor);
        tft.setTextSize(2);

        // Стираем CO2 строку
        String co2Text = String(co2) + " ppm";
        int16_t co2X = getCenteredX(co2Text.length() * 12);  // 12 пикселей на символ
        int16_t co2Y = blockY + 10;

        tft.setCursor(co2X, co2Y);
        tft.print(co2Text);

        // Стираем температурную строку
        String tempText = String(temperature, 1) + " C";
        int16_t tempX = getCenteredX(tempText.length() * 12);
        int16_t tempY = blockY + 35;

        tft.setCursor(tempX, tempY);
        tft.print(tempText);
    }

    // Метод для получения координаты X для центрирования текста
    int16_t getCenteredX(int textWidth) {
        return (tft.width() - textWidth) / 2;
    }

    // Метод для обновления Wi-Fi информации
    void updateWiFi() {
        static String lastStatus = "";
        String currentStatus = wifi->getStatusString().substring(0, 10);

        if (currentStatus != lastStatus) {
            int startW = PADDING_X;
            int startH = 96;

            // Стираем старый статус Wi-Fi цветом фона
            tft.fillRect(startW, startH, tft.width() - 2 * PADDING_X, 15, BLACK);
            tft.setCursor(startW, startH);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);

            // Отображаем текущий статус Wi-Fi
            tft.print("WiFi: ");
            tft.print(currentStatus);

            if (wifi->getStatus() == WiFiHandler::CONNECTED) {
                int strength = wifi->getSignalStrength();
                tft.print(" ");
                tft.print(strength);
                tft.println("/3");
            } 

            lastStatus = currentStatus;
        }
    }

    // Метод для обновления IP адреса
    void updateIP() {
        static String lastIP = "";
        String currentIP = wifi->getIPAddress();

        if (currentIP != lastIP) {
            int startW = PADDING_X;
            int startH = 108;

            // Стираем старый IP адрес цветом фона
            tft.fillRect(startW, startH, tft.width() - 2 * PADDING_X, 10, BLACK);
            tft.setCursor(startW, startH);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);

            // Отображаем текущий IP адрес
            tft.print("IP: ");
            tft.println(currentIP);

            lastIP = currentIP;
        }
    }

    // Метод для обновления аптайма
    void updateUptime() {
        static unsigned long lastUptimeSeconds = -1;
        unsigned long uptimeMillis = millis();
        unsigned long uptimeSeconds = uptimeMillis / 1000;

        if (uptimeSeconds != lastUptimeSeconds) {
            int startW = PADDING_X;
            int startH = 120;

            tft.fillRect(startW, startH, tft.width() - 2 * PADDING_X, 15, BLACK);  // Очищаем область для аптайма
            tft.setCursor(startW, startH);
            tft.setTextSize(1);
            tft.setTextColor(WHITE);

            // Рассчитываем аптайм в днях, часах, минутах и секундах
            int uptimeDays = uptimeSeconds / 86400;
            int uptimeHours = (uptimeSeconds % 86400) / 3600;
            int uptimeMinutes = (uptimeSeconds % 3600) / 60;
            int uptimeRemainingSeconds = uptimeSeconds % 60;

            // Отображаем аптайм
            tft.print("Uptime: ");
            if (uptimeDays > 0) {
                tft.print(uptimeDays);
                tft.print("d ");
            }
            if (uptimeHours > 0) {
                tft.print(uptimeHours);
                tft.print("h:");
            }
            if (uptimeMinutes < 10) {
                tft.print("0");
            }
            tft.print(uptimeMinutes);
            tft.print(":");
            if (uptimeRemainingSeconds < 10) {
                tft.print("0");
            }
            tft.println(uptimeRemainingSeconds);

            lastUptimeSeconds = uptimeSeconds;
        }
    }
};


