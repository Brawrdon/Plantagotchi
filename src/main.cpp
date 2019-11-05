#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

#define AMOUNT_OF_READINGS 15

float getTemperature(){
	return bme.readTemperature();
}

float getHumidity(){
	return bme.readHumidity();
}

float getAverage(float *data, int size) {
	float sum = 0.0, average;       
	
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}
	
	average = sum /  size;
	
	return average;
}

void setup() {
	Wire.begin(23, 22);
	Serial.begin(9600);
	
	if (!bme.begin()) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	} else {
		Serial.println("Welcçome to Plantagotchi!");
	}
}

void loop() {
	float temparatures[AMOUNT_OF_READINGS], humidities[AMOUNT_OF_READINGS];
	
	for (int i = 0; i < AMOUNT_OF_READINGS; i++)
	{
		temparatures[i] = getTemperature();
		humidities[i] = getHumidity();
		sleep(1);
	}

	float average_temparature = getAverage(temparatures, sizeof(temparatures));
	float average_humidity = getAverage(humidities, sizeof(humidities));
	
	Serial.println("");
	Serial.print("Average temperature = ");
	Serial.print(average_temparature);
	Serial.println(" *C");

	Serial.print("Average humidity = ");
	Serial.print(average_humidity);
	Serial.println("%");

	sleep(30);
}

