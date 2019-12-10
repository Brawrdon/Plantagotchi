//
//  ReadingInfoRow.swift
//  Plantagotchi
//
//  Created by Brandon Okeke on 10/12/2019.
//  Copyright © 2019 Brandon Okeke. All rights reserved.
//

import SwiftUI

struct ReadingPropertyView: View {
    var name: String
    var data: String

    var body: some View {
        HStack {
            Text(name + ":")
            Text(data)
            Spacer()
        }
    }
}

struct ReadingPropertyView_Previews: PreviewProvider {
    static var previews: some View {
        ReadingPropertyView(name: "Hello", data: "Hello")
    }
}
