#include <BLEDevice.h>
#include <Arduino.h>
#include "PlantagotchiBLE.h"

using namespace std;

string* ssid_pointer;
string* password_pointer;
float* min_temperature_pointer;
float* max_temperature_pointer;
float* min_humidity_pointer;
float* max_humidity_pointer;

bool wifi_connected = false;
bool sensors_configured = false;

float CharacteristicStringToFloat(BLECharacteristic* characteristic_pointer) {
	string characteristic_value = characteristic_pointer->getValue();
	
	char characteristic_value_chars[characteristic_value.length() + 1];
	strcpy(characteristic_value_chars, characteristic_value.c_str()); 

	String characteristic_value_arduino(characteristic_value_chars);

	return characteristic_value_arduino.toFloat();
}

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
        *min_temperature_pointer = CharacteristicStringToFloat(characteristic_pointer);    
    };
};


class MaxTemperatureCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        *max_temperature_pointer = CharacteristicStringToFloat(characteristic_pointer);    
	};
};

class MinHumidityCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        *min_humidity_pointer = CharacteristicStringToFloat(characteristic_pointer);    
    };
};


class MaxHumidityCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic_pointer) {
        *max_humidity_pointer = CharacteristicStringToFloat(characteristic_pointer);      
	};
};


void PlantagotchiBLE::setup(string* ssid_pointer, string* password_pointer, float* min_temperature_pointer, float* max_temperature_pointer, float* min_humidity_pointer, float* max_humidity_pointer) {
	::ssid_pointer = ssid_pointer;
	::password_pointer = password_pointer;
	::min_temperature_pointer = min_temperature_pointer;
	::max_temperature_pointer = max_temperature_pointer;
	::min_humidity_pointer = min_humidity_pointer;
	::max_humidity_pointer = max_humidity_pointer;

	BLEUUID wifi_connection_service_uuid("66037b25-aa3b-41ee-a533-9b0da129f4d0");

	BLEUUID ssid_characteristic_uuid("9a1d34d4-e3ab-4978-bc0c-7a7ca0ab9ae7");
	BLEUUID password_characteristic_uuid("c56e90af-3b2d-4fe2-ad0b-dcc98207102a");
	BLEUUID min_temperature_characteristic_uuid("3ff81d36-b257-4fa9-89fe-151ce5e04401");
	BLEUUID max_temperature_characteristic_uuid("97e7c4d3-f76d-41df-9b13-27c7733d0570");
	BLEUUID min_humidity_characteristic_uuid("4d2d7e33-7fdb-44d6-b94d-621eaddc8844");
	BLEUUID max_humidity_charactersitc_uuid("2ec69ce1-2ae5-44d3-9367-d0708300d6c1");

	// Create BLE Server
	BLEDevice::init("Plantagotchi");
	BLEServer* server_pointer = BLEDevice::createServer();
	  
	// Create BLE Service
	BLEService* wifi_connection_service = server_pointer->createService(wifi_connection_service_uuid);

	// Create BLE Characteristics
	BLECharacteristic* ssid_characteristic  = wifi_connection_service->createCharacteristic(ssid_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* password_characteristic = wifi_connection_service->createCharacteristic(password_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	
	BLECharacteristic* min_temperature_characteristic = wifi_connection_service->createCharacteristic(min_temperature_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* max_temperature_characteristic  = wifi_connection_service->createCharacteristic(max_temperature_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* min_humidity_characteristic = wifi_connection_service->createCharacteristic(min_humidity_characteristic_uuid, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* max_humidity_characteristic  = wifi_connection_service->createCharacteristic(max_humidity_charactersitc_uuid, BLECharacteristic::PROPERTY_WRITE);

	// Define BLE Descriptors
	BLEDescriptor* min_humidity_user_characteristic_descriptor = new BLEDescriptor((uint16_t) 0x2901);
	BLEDescriptor* max_humidity_user_characteristic_descriptor = new BLEDescriptor((uint16_t) 0x2901);

	min_humidity_user_characteristic_descriptor->setValue("Minimum Humidity");
	max_humidity_user_characteristic_descriptor->setValue("Maximum Humidity");

	min_humidity_characteristic->addDescriptor(min_humidity_user_characteristic_descriptor);
	max_humidity_characteristic->addDescriptor(max_humidity_user_characteristic_descriptor);

	// Define Characteristic callbacks
	ssid_characteristic->setCallbacks(new SSIDCharacteristicCallback());
	password_characteristic->setCallbacks(new PasswordCharacteristicCallback());
	min_temperature_characteristic->setCallbacks(new MinTemperatureCharacteristicCallback());
	max_temperature_characteristic->setCallbacks(new MaxTemperatureCharacteristicCallback());
	min_humidity_characteristic->setCallbacks(new MinHumidityCharacteristicCallback());
	max_humidity_characteristic->setCallbacks(new MaxHumidityCharacteristicCallback());

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