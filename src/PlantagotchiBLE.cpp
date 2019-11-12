#include <BLEDevice.h>
#include <Arduino.h>
#include "PlantagotchiBLE.h"

using namespace std;

string* ssid_pointer;
string* password_pointer;
float* min_temperature_pointer;
float* max_temperature_pointer;
	
bool wifi_connected = false;
bool sensors_configured = false;

class SSIDCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        *ssid_pointer = characteristic_pointer->getValue();
    };
};

class PasswordCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        *password_pointer = characteristic_pointer->getValue();
    };
};

class MinTemperatureCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
		string new_min_temperature = characteristic_pointer->getValue();
		
		char new_min_temperature_chars[new_min_temperature.length() + 1];
		strcpy(new_min_temperature_chars, new_min_temperature.c_str()); 

		String new_min_temperature_string(new_min_temperature_chars);

        *min_temperature_pointer = new_min_temperature_string.toFloat();
    };
};


class MaxTemperatureCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
		string new_max_temperature = characteristic_pointer->getValue();
		
		char new_max_temperature_chars[new_max_temperature.length() + 1];
		strcpy(new_max_temperature_chars, new_max_temperature.c_str()); 

		String new_max_temperature_string(new_max_temperature_chars);

        *max_temperature_pointer = new_max_temperature_string.toFloat();    
	};
};


void PlantagotchiBLE::setup(string* ssid_pointer, string* password_pointer, float* min_temperature_pointer, float* max_temperature_pointer) {
	::ssid_pointer = ssid_pointer;
	::password_pointer = password_pointer;
	::min_temperature_pointer = min_temperature_pointer;
	::max_temperature_pointer = max_temperature_pointer;

	BLEUUID wifi_connection_service_uuid("66037b25-aa3b-41ee-a533-9b0da129f4d0");
	BLEUUID ssid_characteristic_uuid("9a1d34d4-e3ab-4978-bc0c-7a7ca0ab9ae7");
	BLEUUID password_characteristic_uuid("c56e90af-3b2d-4fe2-ad0b-dcc98207102a");
	BLEUUID min_temperature_characteristic_uuid("3ff81d36-b257-4fa9-89fe-151ce5e04401");
	BLEUUID max_temperature_charactersitc_uuid("97e7c4d3-f76d-41df-9b13-27c7733d0570");

	// Create BLE Server
	BLEDevice::init("Plantagotchi");
	BLEServer* server_pointer = BLEDevice::createServer();
	  
	// Create BLE Service
	BLEService* wifi_connection_service = server_pointer->createService(wifi_connection_service_uuid);

	// Create BLE Characteristics
	BLECharacteristic* ssid_characteristic  = wifi_connection_service->createCharacteristic(ssid_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* password_characteristic = wifi_connection_service->createCharacteristic(password_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	
	BLECharacteristic* min_temperature_characteristic = wifi_connection_service->createCharacteristic(min_temperature_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* max_temperature_characteristic  = wifi_connection_service->createCharacteristic(max_temperature_charactersitc_uuid, BLECharacteristic::PROPERTY_WRITE);

	// Define Characteristic callbacks
	ssid_characteristic->setCallbacks(new SSIDCharacteristicCallback());
	password_characteristic->setCallbacks(new PasswordCharacteristicCallback());
	min_temperature_characteristic->setCallbacks(new MinTemperatureCharacteristicCallback());
	max_temperature_characteristic->setCallbacks(new MaxTemperatureCharacteristicCallback());

	wifi_connection_service->start();
	
	// Advertise BLE Server
	BLEAdvertising* advertising_pointer = BLEDevice::getAdvertising();
	advertising_pointer->addServiceUUID(wifi_connection_service_uuid);
	advertising_pointer->setScanResponse(true);
	
	// Functions to help with iPhone connectivity issues
	advertising_pointer->setMinPreferred(0x06);  
 	advertising_pointer->setMinPreferred(0x12);
	
	BLEDevice::startAdvertising();
}

void PlantagotchiBLE::setWifiStatus(bool wifi_status) {
	wifi_connected = wifi_status;
}

bool PlantagotchiBLE::getSensorsConfigured() {
	return sensors_configured;
}

void PlantagotchiBLE::setSensorsConfigured(bool sensors_configured) {
	::sensors_configured = sensors_configured;
}