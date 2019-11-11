#include <string>
#include <BLEDevice.h>
#include "BLEWrapper.h"

using namespace std;

string* ssid_pointer;
string* password_pointer;

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

void BLEWrapper::setup(string* ssid_pointer, string* password_pointer) {
    ::ssid_pointer = ssid_pointer;
    ::password_pointer = password_pointer;
    BLEUUID wifi_connection_service_uuid("66037b25-aa3b-41ee-a533-9b0da129f4d0");
    BLEUUID ssid_characteristic_uuid("9a1d34d4-e3ab-4978-bc0c-7a7ca0ab9ae7");
    BLEUUID password_characteristic_uuid("c56e90af-3b2d-4fe2-ad0b-dcc98207102a");

    // Create the BLE Server
	BLEDevice::init("Plantagotchi");
	BLEServer* server_pointer = BLEDevice::createServer();
	  
	// Create the BLE Service
	BLEService* wifi_connection_service = server_pointer->createService(wifi_connection_service_uuid);

	// Create the BLE Characteristics
	BLECharacteristic* ssid_characteristic  = wifi_connection_service->createCharacteristic(ssid_characteristic_uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic* password_characteristic = wifi_connection_service->createCharacteristic(password_characteristic_uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    ssid_characteristic->setCallbacks(new SSIDCharacteristicCallback());
    password_characteristic->setCallbacks(new PasswordCharacteristicCallback());

	wifi_connection_service->start();

	// Advertise the BLE Server
	BLEAdvertising* advertising_pointer = BLEDevice::getAdvertising();
	advertising_pointer->addServiceUUID(wifi_connection_service_uuid);
	advertising_pointer->setScanResponse(true);

	// Functions to help with iPhone connectivity issues
	advertising_pointer->setMinPreferred(0x06);  
 	advertising_pointer->setMinPreferred(0x12);
	
	BLEDevice::startAdvertising();
}