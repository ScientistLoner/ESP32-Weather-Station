/*
 * ESP32 Weather Station
 * Датчики: AHT20 (температура/влажность) + BMP280 (давление)
 * Дисплей: OLED 128x64 (SSD1306)
 */

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Настройки OLED-дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Объекты датчиков
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  
  // Инициализация OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    for(;;);
  }
  
  // Инициализация датчиков
  if (!aht.begin()) {
    Serial.println("AHT20 not found!");
  }
  
  if (!bmp.begin(0x77)) {
    Serial.println("BMP280 not found!");
  }
  
  // Настройка BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
  
  // Настройка дисплея
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  // Чтение данных с AHT20
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  // Чтение данных с BMP280
  float pressure = bmp.readPressure() / 100.0F; // Па -> гПа
  float altitude = bmp.readAltitude(1013.25);   // Расчет высоты
  
  // Вывод на OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.print("Temp: "); display.print(temp.temperature); display.println(" C");
  display.print("Hum:  "); display.print(humidity.relative_humidity); display.println(" %");
  display.print("Pres: "); display.print(pressure); display.println(" hPa");
  display.print("Alt:  "); display.print(altitude); display.println(" m");
  
  display.display();
  
  // Вывод в Serial
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println(" %");
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
  Serial.println("-----------------------");
  
  delay(2000);
}