#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define SENSOR_PIN 27
#define SENSOR_TYPE DHT11

const char* network_ssid = "Excalibur";
const char* network_password = "kingarthur";
const char* server_address = "http://192.168.1.2:5000/sensor/data";

DHT dhtSensor(SENSOR_PIN, SENSOR_TYPE);

void initializeWiFi() {
  WiFi.begin(network_ssid, network_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(800);
    Serial.print(".");
  }
  Serial.println("Connected to network!");
}

float fetchTemperature() {
  float temp = dhtSensor.readTemperature();
  if (isnan(temp)) {
    Serial.println("Temperature read error!");
    return 0.0;
  }
  return temp;
}

float fetchHumidity() {
  float humidity = dhtSensor.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Humidity read error!");
    return 0.0;
  }
  return humidity;
}

void transmitData(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient httpClient;
    httpClient.begin(server_address);
    httpClient.addHeader("Content-Type", "application/json");

    String jsonPayload = "{"
                         "\"humidity\": " + String(humidity, 2) + ","
                         "\"temperature\": " + String(temperature, 2) + ","
                         "\"timestamp\": " + String(millis()) +
                         "}";

    int httpCode = httpClient.POST(jsonPayload);

    Serial.println(jsonPayload);
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Data transmission successful");
      } else {
        Serial.println("Unexpected response code: " + String(httpCode));
      }
    } else {
      Serial.println("POST request error: " + String(httpClient.errorToString(httpCode).c_str()));
    }

    httpClient.end();
  } else {
    Serial.println("Network connection failed, unable to send data");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  dhtSensor.begin();
  initializeWiFi();
}

void loop() {
  float temperature = fetchTemperature();
  float humidity = fetchHumidity();

  transmitData(temperature, humidity);

  delay(4000); // Ubah delay sesuai kebutuhan
}
