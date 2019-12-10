//
//  ContentView.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 10/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//

import SwiftUI
import Foundation
import CoreBluetooth


struct Reading: Identifiable, Codable{
    let id: String
    let initiatedByUser: String?
    let temperature: Int
    let humidity: Int
    let lightLevel: Int
    let soilMoistureLevel: Int
    let withinTemperatureRange: Bool
    let withinHumidityRange: Bool
    let withinLightLevelRange: Bool
}

class ApiConnection: ObservableObject {
    
    @Published var readings = [Reading(id: "10", initiatedByUser: "1812", temperature: 10, humidity: 10, lightLevel: 10, soilMoistureLevel: 10, withinTemperatureRange: true, withinHumidityRange: true, withinLightLevelRange: true)]
    
    init() {
        
    }
    
    public func getReadings() {
        if let url = URL(string: "http://localhost:5000/readings/2109698364?date=2019-12-10") {
            URLSession.shared.dataTask(with: url) { data, response, error in
                if let data = data {
                    do {
                        let res = try JSONDecoder().decode([Reading].self, from: data)
                        DispatchQueue.main.async {
                            self.readings = res;
                        }
                        
                    } catch let error {
                        print(error)
                    }
                }
            }.resume()
        }
    }
}



struct ContentView: View {
    @ObservedObject var apiConnection = ApiConnection();
    
    
    init() {
        UITableView.appearance().tableFooterView = UIView();
        apiConnection.getReadings();
    }
    
    
    var body: some View {
        NavigationView {
            List(self.apiConnection.readings) { reading in
                NavigationLink(destination: ReadingDetailedView(reading: reading)) { ReadingRow(id: reading.id)
                }
            }
            .navigationBarTitle(Text("Readings"))
            .navigationBarItems(trailing:
                    Button("Update Readings") {
                        self.apiConnection.getReadings();
                    }
                )
        }
    }
}


struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}


struct ReadingRow: View {
    let id : String;
    
    var body: some View {
        HStack {
            Text(id)
            Spacer()
        }
    }
}


struct ReadingRow_Previews: PreviewProvider {
    static var previews: some View {
        ReadingRow(id: "100")
    }
}
