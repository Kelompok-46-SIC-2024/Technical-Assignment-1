#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHT_PIN 25
#define DHT_TYPE DHT11

const char* wifi_ssid = "abc";
const char* wifi_password = "12121212";
const char* endpoint = "http://192.168.141.206:5000/sensor/data";

DHT sensor(DHT_PIN, DHT_TYPE);

void setupWiFi() {
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

float readTemperature() {
  float temperature = sensor.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Temperature reading failed!");
    return 0.0;
  }
  return temperature;
}

float readHumidity() {
  float humidity = sensor.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Humidity reading failed!");
    return 0.0;
  }
  return humidity;
}

void sendToServer(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(endpoint);
    http.addHeader("Content-Type", "application/json");

    String payload = "{"
                     "\"humidity\": " + String(humidity, 2) + ","
                     "\"temperature\": " + String(temperature, 2) + ","
                     "\"timestamp\": " + String(millis()) +
                     "}";

    int httpCode = http.POST(payload);

    Serial.println(payload);
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Unexpected HTTP response code: " + String(httpCode));
      }
    } else {
      Serial.println("Failed to send POST request: " + String(http.errorToString(httpCode).c_str()));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected, cannot send data");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  sensor.begin();
  setupWiFi();
}

void loop() {
  float temperature = readTemperature();
  float humidity = readHumidity();

  sendToServer(temperature, humidity);

  delay(2000);
}