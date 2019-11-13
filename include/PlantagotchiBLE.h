namespace PlantagotchiBLE {
    void setup(std::string* ssid_pointer, std::string* password_pointer, uint8_t* min_temperature_pointer, uint8_t* max_temperature_pointer, uint8_t* min_humidity_pointer, uint8_t* max_humidity_pointer);
    bool getWifiConfigured();
    void resetWifiConfigured();
    bool getSensorsConfigured();
};