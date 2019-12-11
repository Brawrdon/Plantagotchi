//
//  ConfigureView.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 11/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//

import SwiftUI

struct SettingsView: View {
    var body: some View {
        NavigationView {
            List {
                NavigationLink(destination: ConnectView()) {Text("Connect")}
                Text("Configure")
            }
            .navigationBarTitle("Settings")
        }
    }
}

struct ConnectView: View {
    @State var name = "";
    var body: some View {
        VStack {
        
            Spacer()
        }
        .padding()
        .navigationBarTitle("Connect", displayMode: .inline)
    }
}


struct SettingsView_Previews: PreviewProvider {
    static var previews: some View {
        SettingsView()
    }
}
