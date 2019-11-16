#include <Arduino.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include "HTTP.h"


void Plantagotchi::sendReadings(int32_t temperature_average, int32_t humidity_average, int32_t light_level) {
    HTTPClient http;

    http.begin("http://192.168.0.5:5000/reading" );
    http.addHeader("Content-Type", "application/json");

    const int capacity = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> payload;
    payload["temperature"] = temperature_average;
    payload["humidity"] = humidity_average;
    payload["lightLevel"] = light_level;

    char payloadString[capacity];

    serializeJson(payload, payloadString);

    int httpCode = http.POST(payloadString);

    if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
            String response = http.getString();
            Serial.println(response);
        } else {
            Serial.println(httpCode);
        }
    } else {
        Serial.println("Connection error.");
    }

    http.end();
}