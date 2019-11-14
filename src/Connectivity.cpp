#include <BLEDevice.h>
#include <WiFi.h>
#include "Connectivity.h"


using namespace std;
using namespace Plantagotchi;


string Plantagotchi::ssid = "";
string Plantagotchi::password = "";

int8_t* Plantagotchi::min_temperature_pointer;
int8_t* Plantagotchi::max_temperature_pointer;
int8_t* Plantagotchi::min_humidity_pointer;
int8_t* Plantagotchi::max_humidity_pointer;

bool Plantagotchi::ssid_set = false;
bool Plantagotchi::password_set = false;
bool Plantagotchi::min_temperature_set = false;
bool Plantagotchi::max_temperature_set = false;
bool Plantagotchi::min_humidity_set = false;
bool Plantagotchi::max_humidity_set = false;


void setReceivedUInt8(BLECharacteristic* characteristic_pointer, int8_t* data_pointer, bool* set_flag_pointer)  {
    int8_t received_data = (int8_t) *(characteristic_pointer->getData());
    *data_pointer = received_data;
    *set_flag_pointer = true;
};

class SSIDCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        ssid = characteristic_pointer->getValue();
        ssid_set = true;
    };
};

class PasswordCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        password = characteristic_pointer->getValue();
        password_set = true;
    };
};

class MinTemperatureCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        setReceivedUInt8(characteristic_pointer, min_temperature_pointer, &min_temperature_set);
    }
};

class MaxTemperatureCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        setReceivedUInt8(characteristic_pointer, max_temperature_pointer, &max_temperature_set);
	};
};

class MinHumidityCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        setReceivedUInt8(characteristic_pointer, min_humidity_pointer, &min_humidity_set);
    };
};


class MaxHumidityCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        setReceivedUInt8(characteristic_pointer, max_humidity_pointer, &max_humidity_set);
	};
};


void Plantagotchi::setupBLE(int8_t* min_temperature_pointer, int8_t* max_temperature_pointer, int8_t* min_humidity_pointer, int8_t* max_humidity_pointer) {
	::min_temperature_pointer = min_temperature_pointer;
	::max_temperature_pointer = max_temperature_pointer;
	::min_humidity_pointer = min_humidity_pointer;
	::max_humidity_pointer = max_humidity_pointer;


	// Define UUIDs
	BLEUUID wifi_connection_service_uuid("66037b25-aa3b-41ee-a533-9b0da129f4d0");
	BLEUUID sensor_configuration_service_uuid("2c915297-4a46-4827-b7b4-781fa8ace4e1");

	BLEUUID ssid_characteristic_uuid("9a1d34d4-e3ab-4978-bc0c-7a7ca0ab9ae7");
	BLEUUID password_characteristic_uuid("c56e90af-3b2d-4fe2-ad0b-dcc98207102a");
	BLEUUID min_temperature_characteristic_uuid("3ff81d36-b257-4fa9-89fe-151ce5e04401");
	BLEUUID max_temperature_characteristic_uuid("97e7c4d3-f76d-41df-9b13-27c7733d0570");
	BLEUUID min_humidity_characteristic_uuid("4d2d7e33-7fdb-44d6-b94d-621eaddc8844");
	BLEUUID max_humidity_characteristic_uuid("2ec69ce1-2ae5-44d3-9367-d0708300d6c1");


	// Create BLE Server
	BLEDevice::init("Plantagotchi");
	BLEServer* server_pointer = BLEDevice::createServer();
	  

	// Create BLE Services
	BLEService* wifi_connection_service = server_pointer->createService(wifi_connection_service_uuid);
    BLEService* sensor_configuration_service = server_pointer->createService(sensor_configuration_service_uuid);


	// Create BLE Characteristics
	BLECharacteristic* ssid_characteristic  = wifi_connection_service->createCharacteristic(ssid_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* password_characteristic = wifi_connection_service->createCharacteristic(password_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	
	BLECharacteristic* min_temperature_characteristic = sensor_configuration_service->createCharacteristic(min_temperature_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* max_temperature_characteristic  = sensor_configuration_service->createCharacteristic(max_temperature_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* min_humidity_characteristic =  sensor_configuration_service->createCharacteristic(min_humidity_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* max_humidity_characteristic =  sensor_configuration_service->createCharacteristic(max_humidity_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);


	// Define Characteristic callbacks
	ssid_characteristic->setCallbacks(new SSIDCharacteristicCallback());
	password_characteristic->setCallbacks(new PasswordCharacteristicCallback());
	min_temperature_characteristic->setCallbacks(new MinTemperatureCharacteristicCallback());
	max_temperature_characteristic->setCallbacks(new MaxTemperatureCharacteristicCallback());
	min_humidity_characteristic->setCallbacks(new MinHumidityCharacteristicCallback());
	max_humidity_characteristic->setCallbacks(new MaxHumidityCharacteristicCallback());


	// Start Services
	wifi_connection_service->start();
	sensor_configuration_service->start();


	// Advertise BLE Server
	BLEAdvertising* advertising_pointer = BLEDevice::getAdvertising();
	advertising_pointer->addServiceUUID(wifi_connection_service_uuid);
	advertising_pointer->setScanResponse(true);
	

	// Functions to help with iPhone connectivity issues
	advertising_pointer->setMinPreferred(0x06);  
 	advertising_pointer->setMinPreferred(0x12);


	BLEDevice::startAdvertising();
}

void Plantagotchi::setupWiFi() {
    int attempts = 0;

    Serial.println("\n== [Setting WiFi connection] ==\n");
    Serial.println("Waiting for SSID and password to be set via BLE.");

    while(WiFi.status() != WL_CONNECTED) {
        if (getWifiConfigured()) {

            attempts++;

            if(attempts == 1) {
                Serial.print("Connecting");
            }

            WiFi.begin(ssid.c_str(), password.c_str());

            if(WiFi.status() != WL_CONNECTED) {
                Serial.print(" .");
                if(attempts == 3) {
                    resetWifiConfigured();
                    attempts = 0;
                    Serial.println(" WiFi connection failed.");
                    Serial.println("Waiting for SSID and password to be resent.");
                }
                delay(3000);
            }
        }
    }

    Serial.println(" WiFI connection successful.");
}

bool Plantagotchi::getWifiConfigured() {
    return ssid_set && password_set;
}

void Plantagotchi::resetWifiConfigured() {
    ssid_set = false;
    password_set = false;
}

bool Plantagotchi::getSensorsConfigured() {
    return min_temperature_set && max_temperature_set && min_humidity_set && max_humidity_set;
}

