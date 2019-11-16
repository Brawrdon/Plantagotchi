#include <Arduino.h>
#include "Adafruit_BME280.h"
#include "BH1750.h"
#include "Ticker.h"
#include "Sensors.h"
#include "Connectivity.h"
#include "HTTP.h"


using namespace std;
using namespace Plantagotchi;


#define VIBRATION_PIN 15
#define AMOUNT_OF_READINGS 100


Adafruit_BME280 bme;
BH1750 light_meter;

int8_t Plantagotchi::min_temperature = 15;
int8_t Plantagotchi::max_temperature = 25;
int8_t Plantagotchi::min_humidity = 30;
int8_t Plantagotchi::max_humidity = 40;

bool Plantagotchi::has_moved = false;
bool Plantagotchi::detect_movement = true;
bool Plantagotchi::allow_reading_sensors = true;
bool Plantagotchi::is_reading_sensors = true;


int32_t getAverage(int32_t *array_pointer, uint8_t array_size) {
    int32_t sum = 0;
    int32_t average = 0, q1, q3;
    int16_t last_index = array_size - 1;
    int16_t data_set_size = array_size;
    bool outliers = false;

    sort(array_pointer, array_pointer + array_size);

    q1 = array_pointer[last_index / 4];
    q3 = array_pointer[(last_index / 4) * 3];

    int32_t interquartileRange = q3 - q1;
    int32_t low_outlier_threshold = q1 - (1.5 * interquartileRange);
    int32_t high_outlier_threshold = q3 + (1.5 * interquartileRange);

    Serial.print("\n> Outliers: ");

    for (int8_t i = 0; i < array_size; i++) {
        if(array_pointer[i] >= low_outlier_threshold && array_pointer[i] <= high_outlier_threshold) {
            sum += array_pointer[i];
        } else {
            outliers = true;
            data_set_size--;
            Serial.print(array_pointer[i]);
            Serial.print(" ");
        }
    }

    if (!outliers) {
        Serial.print("N/A");
    }

    Serial.println("");
    if (data_set_size != 0) {
        average = sum / data_set_size;
    }

    return average;
}

int32_t getSensorData(Sensor sensor) {
    int32_t data;
    switch (sensor) {
        case Temperature:
            data = bme.readTemperature();
            break;
        case Humiditiy :
            data = bme.readHumidity();
            break;
        case Light :
            data = light_meter.readLightLevel();
            break;
        default:
            data = 0;
            break;
    }
    return data;
}

void getSensorInformation(String *sensor_type, String *unit_of_measurement, Sensor sensor) {
    switch (sensor) {
        case Temperature:
            *sensor_type = "Temperature";
            *unit_of_measurement = "*c";
            break;
        case Humiditiy:
            *sensor_type = "Humidity";
            *unit_of_measurement = "%";
            break;
        case Light:
            *sensor_type = "Light Level";
            *unit_of_measurement = "lux";
            break;
        default:
            *sensor_type = "Invalid";
            *unit_of_measurement = "N/A";
            break;
    }
}

void displaySensorInformation(Sensor sensor) {
    String sensor_type;
    String unit_of_measurement;

    getSensorInformation(&sensor_type, &unit_of_measurement, sensor);

    Serial.print(sensor_type);
    Serial.print(" (");
    Serial.print(unit_of_measurement);
    Serial.print(")");
}

void displaySensorData(int32_t *array_pointer, uint8_t array_size, Sensor sensor) {
    displaySensorInformation(sensor);
    Serial.print(" : ");

    for (int8_t i = 0; i < array_size ; ++i) {
        Serial.print(array_pointer[i]);
        Serial.print(" ");
    }

    Serial.println("\n");
}

void checkAverageToThreshold(int32_t average, Sensor sensor) {
    displaySensorInformation(sensor);

    Serial.print("> Average: ");
    Serial.println(average);

    uint8_t min_threshold;
    uint8_t max_threshold;

    switch (sensor) {
        case Temperature:
            min_threshold = min_temperature;
            max_threshold = max_temperature;
            break;
        case Humiditiy:
            min_threshold = min_humidity;
            max_threshold = max_humidity;
            break;
        default:
            min_threshold = 0;
            max_threshold = 0;
            break;
    }
    
    Serial.print("> Within range");
    if(min_threshold != 0 && max_threshold != 0 ) {
        Serial.print(" (");
        Serial.print(min_threshold);
        Serial.print(" - ");
        Serial.print(max_threshold);
        Serial.print("): ");
        Serial.println(average >= min_threshold && average <= max_threshold ? "Yes" : "No");
    } else {
        Serial.println(": No range set");
    }

    Serial.println("");

}

void Plantagotchi::connectToSensors() {
    Serial.println("\n== [Connecting to sensors] ==\n");

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

        if(!is_bme280_connected || !is_gy30_connected) {
            Serial.println("Trying again in 5 seconds.");
            delay(5000);
        }
    }

    Serial.println("\n\n== [Setting sensor ranges] ==\n");
    Serial.println("Waiting for sensor ranges to be set via BLE.");

	while (!getSensorsConfigured())
	{
	}

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
}

void Plantagotchi::checkForMovement() {
    if (digitalRead(VIBRATION_PIN) == LOW) {
        has_moved = true;
    } else {
        has_moved = false;
    }
}

void Plantagotchi::readSensors(uint8_t * ticker_pointer) {
    Serial.println("\n## [Reading sensor data] ##\n");
    Ticker* ticker_pointer_cast = (Ticker*) ticker_pointer;

    is_reading_sensors = true;

    int32_t temperatures[AMOUNT_OF_READINGS], humidities[AMOUNT_OF_READINGS], light_levels[AMOUNT_OF_READINGS];

    for (int8_t i = 0; i < AMOUNT_OF_READINGS; i++)
    {
        temperatures[i] = getSensorData(Temperature);
        humidities[i] = getSensorData(Humiditiy);
        light_levels[i] = getSensorData(Light);

        delay(100);
    }

    Serial.println("# [Sensor data] #\n");
    displaySensorData(temperatures, AMOUNT_OF_READINGS, Temperature);
    displaySensorData(humidities, AMOUNT_OF_READINGS, Humiditiy);
    displaySensorData(light_levels, AMOUNT_OF_READINGS, Light);

    Serial.println("\n# [Sensor data averages] #\n");
    int32_t temperature_average = getAverage(temperatures, AMOUNT_OF_READINGS);
    int32_t humidity_average = getAverage(humidities, AMOUNT_OF_READINGS);
    int32_t light_level_average = getAverage(light_levels, AMOUNT_OF_READINGS);

    checkAverageToThreshold(temperature_average, Temperature);
    checkAverageToThreshold(humidity_average, Humiditiy);
    checkAverageToThreshold(light_level_average, Light);

    sendReadings(temperature_average, humidity_average, light_level_average);

    is_reading_sensors = false;
    allow_reading_sensors = false;
    detect_movement = true;
    ticker_pointer_cast->start();
}






