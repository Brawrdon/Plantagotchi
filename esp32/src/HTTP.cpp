#include <Arduino.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include "HTTP.h"


using namespace std;


#define ADDITIONAL_BYTES 40

uint64_t chip_id;

const String base_url = "http://192.168.0.5:5000/";
const String content_type = "Content-Type";
const String application_type = "application/json";


void sendRequest(String url, char payload[]) {
    Serial.print("URL: ");
    Serial.println(url);
    Serial.print("Method: ");
    Serial.println("POST");
    Serial.print("Payload: ");
    Serial.println(payload);
    Serial.print("Response: ");

    HTTPClient http;

    http.begin(url);
    http.addHeader(content_type, application_type);

    int httpCode = http.POST(payload);

    if(httpCode > 0) {
        const int responseCapacity = JSON_OBJECT_SIZE(1) + ADDITIONAL_BYTES;
        DynamicJsonDocument response_payload(responseCapacity);

        String response = http.getString();
        DeserializationError error = deserializeJson(response_payload, response);

        Serial.print(httpCode);
        Serial.print(" - ");
        if (!error) {
            String result = response_payload["result"];
            Serial.print(result);
        } else {
            Serial.print("Error deserialising JSON ");
            Serial.print("(");
            Serial.print(error.c_str());
            Serial.print(")");
        }
    }

    Serial.println("\n");


    http.end();
}

void Plantagotchi::registerDevice() {
    Serial.println("\n\n== [Registering device] ==\n");

    chip_id = ESP.getEfuseMac();
    const int requestCapacity = JSON_OBJECT_SIZE(2) + ADDITIONAL_BYTES;
    DynamicJsonDocument request(requestCapacity);

    request["serial"] = chip_id;
    request["connected"] = true;

    char payload[requestCapacity];
    serializeJson(request, payload);

    sendRequest( base_url + "devices", payload);
}

void Plantagotchi::sendReadings(int32_t temperature_average, int32_t humidity_average, int32_t light_level) {
    Serial.println("\n# [Sending readings] #\n");

    unsigned long chip_id_long = (unsigned long) chip_id;
    const String chip_id_string = String(chip_id_long);
    const String url = base_url + "readings/" +  chip_id_string;
    const int requestCapacity = JSON_OBJECT_SIZE(3) + ADDITIONAL_BYTES;

    StaticJsonDocument<requestCapacity> request;
    request["temperature"] = temperature_average;
    request["humidity"] = humidity_average;
    request["lightLevel"] = light_level;

    char payload[requestCapacity];
    serializeJson(request, payload);

    sendRequest(url, payload);
}