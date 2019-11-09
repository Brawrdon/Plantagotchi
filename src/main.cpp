#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "BH1750.h"
#include "Ticker.h"

Adafruit_BME280 bme;
MPU6050 accelgyro;
BH1750 lightMeter;

#define AMOUNT_OF_READINGS 	50
#define MIN_TEMPERATURE 30.00
#define MIN_HUMIDITY 40.00

bool has_moved;
volatile bool detect_movement = true;
volatile bool allow_reading_sensors = true;
volatile bool is_reading_sensors = true;


void setAllowReadingSensors();
void setDetectMovement();
Ticker allow_reading_sensors_timer(setAllowReadingSensors, 30000);
Ticker read_after_movement_timer(setAllowReadingSensors, 10000);

float getTemperature(){
	return bme.readTemperature();
}

float getHumidity(){
	return bme.readHumidity();
}

float getLightLevel(){
	return lightMeter.readLightLevel();
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
	float temparatures[AMOUNT_OF_READINGS], humidities[AMOUNT_OF_READINGS], light_levels[AMOUNT_OF_READINGS];

	for (int i = 0; i < AMOUNT_OF_READINGS; i++)
	{
		Serial.print(".");
		temparatures[i] = getTemperature();
		humidities[i] = getHumidity();
		light_levels[i] = getLightLevel();
		delay(100);
	}

	float average_temparature = getAverage(temparatures, AMOUNT_OF_READINGS);
	float average_humidity = getAverage(humidities, AMOUNT_OF_READINGS);
	float average_light_level = getAverage(light_levels, AMOUNT_OF_READINGS);

	Serial.print("\nAverage temperature = ");
	Serial.print(average_temparature);
	Serial.print(" *C\t - \t");
	Serial.println(average_temparature < MIN_TEMPERATURE ? "Temperature is too low!" : "Temperature is good!");


	Serial.print("Average humidity = ");
	Serial.print(average_humidity);
	Serial.print("%\t - \t");
	Serial.println(average_humidity < MIN_HUMIDITY ? "Humidity is too low!" : "Humidity is good!");

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
	read_after_movement_timer.stop();
	allow_reading_sensors_timer.start();
	
}

void setAllowReadingSensors() {
	allow_reading_sensors = true;
}

void connectToSensors() {
	bool is_bme280_connected = false;
	bool is_gy521_connected = false;
	bool is_gy30_connected = false;


	while (!is_bme280_connected || !is_gy521_connected || !is_gy30_connected)
	{
		delay(500);

		// Setup and Initialise BME280 (Temperature & Humidity)
		is_bme280_connected = bme.begin();
		Serial.println(is_bme280_connected ? "\nBME280 connection successful." : "\nBME280 connection failed.");

		//Setup and Initialise GY-521 / MPU6050 (Gyroscope)
		is_gy521_connected = accelgyro.testConnection();
		Serial.println(is_gy521_connected ? "GY-521 connection successful." : "GY-521 connection failed.");

		//Setup and Initialise GY-30 / BH1750 (Light Sensor)
		is_gy30_connected = lightMeter.begin();
		Serial.println(is_gy30_connected ? "GY-30 connection successful." : "GY-30 connection failed.");

	}

	// Initialise GY-521 / MPU6050 (Gyroscope)
	accelgyro.initialize();
	accelgyro.setMotionDetectionThreshold(2);
	accelgyro.setMotionDetectionDuration(5);
}

void setup() {

	Serial.begin(9600);
  	Wire.begin(23, 22);

	connectToSensors();

	Serial.println("\nWelcome to Plantagotchi!");
}

void loop() {
	allow_reading_sensors_timer.update();
	read_after_movement_timer.update();
	if(detect_movement && !is_reading_sensors) {

		has_moved = accelgyro.getXPosMotionDetected() || accelgyro.getXNegMotionDetected() || accelgyro.getYPosMotionDetected() 
				|| accelgyro.getYNegMotionDetected() || accelgyro.getZPosMotionDetected() || accelgyro.getZNegMotionDetected();

		if(has_moved) {
			Serial.println("\nForcing read from sensors as movement has been detected.");
			allow_reading_sensors_timer.stop();
			read_after_movement_timer.start();
			detect_movement = false;
		}
	}

	if(allow_reading_sensors) {
		readSensors();
	}
}