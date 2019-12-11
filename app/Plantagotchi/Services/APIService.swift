//
//  APIService.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 11/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//
import SwiftUI
import Foundation
import CoreBluetooth

class APIService : NSObject, ObservableObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    @Published var readings = [Reading]();
    private var centralManager: CBCentralManager!
    private var peripheral : CBPeripheral!
    
    private let wifiService = CBUUID.init(string: "66037b25-aa3b-41ee-a533-9b0da129f4d0")
    private let ssidCharactersitc = CBUUID.init(string: "9a1d34d4-e3ab-4978-bc0c-7a7ca0ab9ae7")
    private let passwordCharactersitc =  CBUUID.init(string: "c56e90af-3b2d-4fe2-ad0b-dcc98207102a")
    
    private let sensorService =  CBUUID.init(string: "2c915297-4a46-4827-b7b4-781fa8ace4e1")
    
    private let minTemp = CBUUID.init(string: "3ff81d36-b257-4fa9-89fe-151ce5e04401")
    private let maxTemp = CBUUID.init(string: "97e7c4d3-f76d-41df-9b13-27c7733d0570")
    private let minHumidity =  CBUUID.init(string: "4d2d7e33-7fdb-44d6-b94d-621eaddc8844")
    private let maxHumidty = CBUUID.init(string: "2ec69ce1-2ae5-44d3-9367-d0708300d6c1")

    override init() {
        super.init();
        centralManager = CBCentralManager(delegate: self, queue: DispatchQueue.main)
    }
    
    func getReadings() {
        if let url = URL(string: "http://localhost:5000/readings/2109698364?date=2019-12-10") {
            URLSession.shared.dataTask(with: url) { data, response, error in
                if let data = data {
                    do {
                        let readingsResponse = try JSONDecoder().decode([Reading].self, from: data)
                        DispatchQueue.main.async {
                            self.readings = readingsResponse;
                        }
                        
                    } catch let error {
                        print(error);
                    }
                }
            }.resume();
        }
    }
    
    public func scanForBluetooth() {
        if(centralManager.state == .poweredOn) {
            centralManager.scanForPeripherals(withServices: [self.wifiService,])
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        
        // We've found it so stop scan
        self.centralManager.stopScan()
        
        // Copy the peripheral instance
        self.peripheral = peripheral
        self.peripheral.delegate = self
        
        // Connect!
        self.centralManager.connect(self.peripheral, options: nil)
        
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        if peripheral == self.peripheral {
            print("Connected to Plantpot")
            peripheral.discoverServices([wifiService, sensorService])
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let services = peripheral.services {
            for service in services {
                if service.uuid == wifiService {
                    print("Wifi Service")
                    //Now kick off discovery of characteristics
                    peripheral.discoverCharacteristics([ssidCharactersitc, passwordCharactersitc], for: service)
                } else if service.uuid == sensorService {
                    print("Sensor")
                    //Now kick off discovery of characteristics
                    peripheral.discoverCharacteristics([minTemp, maxTemp, minHumidity, maxHumidty], for: service)
                }
            }
        }
    }
    
    // Handling discovery of characteristics
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
       var myInt = 77
       var myIntData = Data(bytes: &myInt,
                            count: MemoryLayout.size(ofValue: myInt))
        
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                if characteristic.uuid == ssidCharactersitc {
                    peripheral.writeValue(Data("".utf8), for: characteristic, type: .withResponse)
                } else if characteristic.uuid == passwordCharactersitc {
                    peripheral.writeValue(Data("".utf8), for: characteristic, type: .withResponse)
                }else if characteristic.uuid == minTemp {
                    peripheral.writeValue(myIntData, for: characteristic, type: .withResponse)
                }else if characteristic.uuid == maxTemp {
                    peripheral.writeValue(myIntData, for: characteristic, type: .withResponse)
                }else if characteristic.uuid == minHumidity {
                    peripheral.writeValue(myIntData, for: characteristic, type: .withResponse)
                }else if characteristic.uuid == maxHumidty {
                    peripheral.writeValue(myIntData, for: characteristic, type: .withResponse)
                }
            }
        }
    }
    

    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        
    }
}

