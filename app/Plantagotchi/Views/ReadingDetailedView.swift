//
//  ReadingView.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 10/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//

import SwiftUI

struct ReadingDetailedView: View {
    var reading : Reading;
    
    var body: some View {
        List {
            ReadingPropertyView(name: "Initiated By", data: reading.initiatedByUser ?? "N/A")
            ReadingPropertyView(name: "Temperature", data: String(reading.temperature))
            ReadingPropertyView(name: "Humidity", data: String(reading.humidity))
            ReadingPropertyView(name: "Light Level", data: String(reading.lightLevel))
            ReadingPropertyView(name: "Soil Moisture Level", data: String(reading.soilMoistureLevel))
        }
        .navigationBarTitle(Text(reading.id))
    }
}

struct ReadingDetailedView_Previews: PreviewProvider {
    static var previews: some View {
        ReadingDetailedView(reading: Reading(id: "10", initiatedByUser: "1812", temperature: 10, humidity: 10, lightLevel: 10, soilMoistureLevel: 10, withinTemperatureRange: true, withinHumidityRange: true, withinLightLevelRange: true))
    }
}
