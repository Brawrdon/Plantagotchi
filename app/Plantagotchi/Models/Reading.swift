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
