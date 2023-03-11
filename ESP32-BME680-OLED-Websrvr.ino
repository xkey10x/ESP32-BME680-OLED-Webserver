#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <Adafruit_GFX.h>
#include <SparkFun_Qwiic_Micro_OLED_Arduino_Library.h>
#include <WiFi.h>
#include <WebServer.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48
#define OLED_RESET 4

Adafruit_BME680 bme;

SFE_MicroOLED screen(OLED_RESET);

const char* ssid = "YourSSID";
const char* password = "YourPassword";

WebServer server(80);

void setup() {
  Serial.begin(115200);
  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  
  screen.begin();
  screen.clear(ALL);
  screen.display();

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  server.on("/", handleRoot);
  
  server.begin();
  
  Serial.println("Web server started.");
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float gas = bme.readGas();
  
  String html = "<html><body><h1>BME680 Sensor Readings</h1><p>Temperature: ";
  html += String(temperature);
  html += " &deg;C</p><p>Humidity: ";
  html += String(humidity);
  html += " %</p><p>Pressure: ";
  html += String(pressure);
  html += " hPa</p><p>Gas Resistance: ";
  html += String(gas);
  html += " kOhms</p></body></html>";
  
  server.handleClient();
  
  screen.clear(ALL);
  screen.setCursor(0, 0);
  screen.print("Temp: ");
  screen.print(temperature, 1);
  screen.println(" C");
  screen.print("Humidity: ");
  screen.print(humidity, 1);
  screen.println(" %");
  screen.print("Pressure: ");
  screen.print(pressure, 1);
  screen.println(" hPa");
  screen.print("Gas: ");
  screen.print(gas / 1000, 1);
  screen.println(" kOhms");
  screen.display();
  
  delay(5000);
}

void handleRoot() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float gas = bme.readGas();
  
  String html = "<html><body><h1>BME680 Sensor Readings</h1><p>Temperature: ";
  html += String(temperature);
  html += " &deg;C</p><p>Humidity: ";
  html += String(humidity);
  html += " %</p><p>Pressure: ";
  html += String(pressure);
  html += " hPa</p><p>Gas Resistance: ";
  html += String(gas);
  html += " kOhms</p></body></html>";
  
  server.send(200, "text/html", html);
}
