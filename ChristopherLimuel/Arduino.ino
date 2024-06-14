#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define PIN_DHT 32
#define TYPE_DHT DHT11

const char* wifi_ssid = "Excalibur";
const char* wifi_pass = "kingarthur";
const char* api_endpoint = "http://192.168.1.2:5000/sensor/data";

DHT dht_sensor(PIN_DHT, TYPE_DHT);

void connectWiFi() {
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

float readTemp() {
  float temperature = dht_sensor.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Temperature read failed!");
    return 0.0;
  }
  return temperature;
}

float readHum() {
  float humidity = dht_sensor.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Humidity read failed!");
    return 0.0;
  }
  return humidity;
}

void uploadData(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http_client;
    http_client.begin(api_endpoint);
    http_client.addHeader("Content-Type", "application/json");

    String json_data = "{"
                       "\"humidity\": " + String(humidity, 2) + ","
                       "\"temperature\": " + String(temperature, 2) + ","
                       "\"timestamp\": " + String(millis()) +
                       "}";

    int http_response = http_client.POST(json_data);

    Serial.println(json_data);
    if (http_response > 0) {
      if (http_response == HTTP_CODE_OK) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Unexpected HTTP response: " + String(http_response));
      }
    } else {
      Serial.println("POST request failed: " + String(http_client.errorToString(http_response).c_str()));
    }

    http_client.end();
  } else {
    Serial.println("WiFi not connected, data upload failed");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  dht_sensor.begin();
  connectWiFi();
}

void loop() {
  float temperature = readTemp();
  float humidity = readHum();

  uploadData(temperature, humidity);

  delay(5000);
}