#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHTPIN 26
#define DHTTYPE DHT11

const char* ssid = "Excalibur"; 
const char* password = "kingarthur"; 
const char* serverUrl = "http://192.168.1.2:5000/sensor/data";

DHT dht(DHTPIN, DHTTYPE);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi is Connected!");
}

float getTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read temperature!");
    return 0.0;
  }
  return temp;
}

float getHumidity() {
  float hum = dht.readHumidity();
  if (isnan(hum)) {
    Serial.println("Failed to read humidity!");
    return 0.0;
  }
  return hum;
}

void sendDataToServer(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String data = "{"
                  "\"humidity\": " + String(humidity, 2) + ","
                  "\"temperature\": " + String(temperature, 2) + ","
                  "\"timestamp\": " + String(millis()) +
                  "}";

    int httpCode = http.POST(data);

    Serial.println(data);
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Data successfully sent");
      } else {
        Serial.println("Unexpected HTTP code: " + String(httpCode));
      }
    } else {
      Serial.println("Error on sending POST: " + String(http.errorToString(httpCode).c_str()));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected, unable to send data");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  dht.begin();
  connectToWiFi();
}

void loop() {
  float temperature = getTemperature();
  float humidity = getHumidity();

  sendDataToServer(temperature, humidity);

  delay(3000);
}
