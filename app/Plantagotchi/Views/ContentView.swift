//
//  ContentView.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 10/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//

import SwiftUI
import Foundation


struct ContentView: View {
    @ObservedObject var apiService = APIService();
    @State var showSettings = false;
    
    init() {
        UITableView.appearance().tableFooterView = UIView();
//        apiService.getReadings();
    }
    
    
    var body: some View {
        NavigationView {
            
            List([Reading(id: "10", initiatedByUser: "1812", temperature: 10, humidity: 10, lightLevel: 10, soilMoistureLevel: 10, withinTemperatureRange: true, withinHumidityRange: true, withinLightLevelRange: true)]) { reading in
                NavigationLink(destination: ReadingDetailedView(reading: reading)) {             Text(reading.id)
                }
            }
            .navigationBarTitle(Text("Readings"))
            .navigationBarItems(trailing:
                HStack {
                    Button(action: {self.apiService.getReadings()}) {
                        Text("Update")
                    }
                    Button(action: {self.apiService.scanForBluetooth()}) {
                        Text("Configure")
                        
                    }
                }
            )
        }.sheet(isPresented: $showSettings) {
            SettingsView()
        }
        
    }
}


struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
