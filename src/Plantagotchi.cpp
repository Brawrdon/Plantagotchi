#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "BH1750.h"
#include "Ticker.h"
#include "PlantagotchiBLE.h"

using namespace std;

Adafruit_BME280 bme;
BH1750 light_meter;

#define VIBRATION_PIN 15
#define AMOUNT_OF_READINGS 50

bool has_moved = false;
bool detect_movement = true;
bool allow_reading_sensors = true;
bool is_reading_sensors = true;

string ssid = "";
string password = "";

uint8_t min_temperature;
uint8_t max_temperature;
uint8_t min_humidity;
uint8_t max_humidity;


void setAllowReadingSensors();
Ticker allow_reading_sensors_timer(setAllowReadingSensors, 30000);

float getTemperature(){
	return bme.readTemperature();
}

float getHumidity(){
	return bme.readHumidity();
}

float getLightLevel(){
	return light_meter.readLightLevel();
}

float getAverage(float *data, int size) {
	float sum = 0.0, average;       
	
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}
	
	average = sum /  size;
	
	return average;
}

void readTemperature() {
	Serial.print("\nReading");
	float temperatures[AMOUNT_OF_READINGS], humidities[AMOUNT_OF_READINGS], light_levels[AMOUNT_OF_READINGS];

	for (int i = 0; i < AMOUNT_OF_READINGS; i++)
	{
		Serial.print(".");
        temperatures[i] = getTemperature();
		humidities[i] = getHumidity();
		light_levels[i] = getLightLevel();
		delay(100);
	}

	float average_temperature = getAverage(temperatures, AMOUNT_OF_READINGS);
	float average_humidity = getAverage(humidities, AMOUNT_OF_READINGS);
	float average_light_level = getAverage(light_levels, AMOUNT_OF_READINGS);

	Serial.print("\nAverage temperature = ");
	Serial.print(average_temperature);
	Serial.print(" *C\t - \t");
	Serial.println(average_temperature < min_temperature ? "Temperature is too low!" : "Temperature is good!");


	Serial.print("Average humidity = ");
	Serial.print(average_humidity);
	Serial.print("%\t - \t");
	Serial.println(average_humidity < min_temperature ? "Humidity is too low!" : "Humidity is good!");

	Serial.print("Average light level = ");
	Serial.print(average_light_level);
	Serial.println("lux");
}

void readSensors() {
	is_reading_sensors = true;
	readTemperature();
	is_reading_sensors = false;
	allow_reading_sensors = false;
	detect_movement = true;
	allow_reading_sensors_timer.start();
}

void setAllowReadingSensors() {
	allow_reading_sensors = true;
}

void connectToSensors() {
	Serial.println("\n== Connecting to sensors ==\n");

	bool is_bme280_connected = false;
	bool is_gy30_connected = false;

	while (!is_bme280_connected || !is_gy30_connected)
	{
		// Setup and Initialise BME280 (Temperature & Humidity Sensor)
		is_bme280_connected = bme.begin();
		Serial.println(is_bme280_connected ? "BME280 connection successful." : "BME280 connection failed.");

		//Setup and Initialise KY-002  (Vibration Sensor)
		pinMode(VIBRATION_PIN, INPUT);

		//Setup and Initialise GY-30 / BH1750 (Light Sensor)
		is_gy30_connected = light_meter.begin();
		Serial.println(is_gy30_connected ? "GY-30 connection successful." : "GY-30 connection failed.");
	}

	Serial.println("\nWaiting for confirmation that sensors min/max values have been set.");
	while (!PlantagotchiBLE::getSensorsConfigured())
	{
	}

	Serial.println("\nMin/max values set.");
    Serial.print("Temperature: ");
    Serial.print(min_temperature);
    Serial.print(" - ");
    Serial.print(max_temperature);
    Serial.println(" *c");

    Serial.print("Humidity: ");
    Serial.print(min_humidity);
    Serial.print(" - ");
    Serial.print(max_humidity);
    Serial.println(" % ");
    Serial.println("\n===========\n");
}
//rmsj9491
void connectToWifi() {
	int attempts = 0;
	Serial.println("\n== Connecting to WiFi ==\n");
	while(WiFi.status() != WL_CONNECTED) {
		if (PlantagotchiBLE::getWifiConfigured()) {
			attempts++;
			
			WiFi.begin(ssid.c_str(), password.c_str());

			if(WiFi.status() != WL_CONNECTED) {
			    Serial.print("-");
				if(attempts == 3) {
				    PlantagotchiBLE::resetWifiConfigured();
					attempts = 0;
					Serial.println("\n ** WiFi connection failed.");
				}
				delay(2000);
			}
		}
	}

	Serial.println("\nWiFI connection successful.");
	Serial.println("\n===========\n");
}

void setup() {
	Serial.begin(115200);
  	Wire.begin(23, 22);

    PlantagotchiBLE::setup(&ssid, &password, &min_temperature, &max_temperature, &min_humidity, &max_humidity);
	connectToSensors();
	connectToWifi();

	Serial.println("\nWelcome to Plantagotchi!\n");
}

void loop() {
	allow_reading_sensors_timer.update();

	if (digitalRead(VIBRATION_PIN) == LOW) {
		has_moved = true;
	} else {
		has_moved = false;
	}
	
	if((detect_movement && !is_reading_sensors) && has_moved) {
			Serial.println("\nForcing read from sensors as movement has been detected.");
			allow_reading_sensors_timer.stop();
			allow_reading_sensors = true;
			detect_movement = false;
	}

	if(allow_reading_sensors) {
		readSensors();
	}
}