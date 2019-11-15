#include <Arduino.h>
#include <Wire.h>
#include "Ticker.h"
#include "Sensors.h"
#include "Connectivity.h"

using namespace std;
using namespace Plantagotchi;


void setAllowReadingSensors();
Ticker allow_reading_sensors_timer(setAllowReadingSensors, 30000);

void setAllowReadingSensors() {
    allow_reading_sensors = true;
}

void setup() {
	Serial.begin(115200);
  	Wire.begin(23, 22);

    setupBLE(&min_temperature, &max_temperature, &min_humidity, &max_humidity);
	connectToSensors();
	setupWiFi();

	Serial.println("\n\n*** Welcome to Plantagotchi! ***\n");
}

void loop() {
	allow_reading_sensors_timer.update();

	checkForMovement();

	if((detect_movement && !is_reading_sensors) && has_moved) {
			allow_reading_sensors_timer.stop();
			allow_reading_sensors = true;
			detect_movement = false;
	}

	if(allow_reading_sensors) {
		readSensors((uint8_t*) &allow_reading_sensors_timer);
	}
}