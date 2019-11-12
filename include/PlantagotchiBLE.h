namespace PlantagotchiBLE {
    void setup(std::string* ssid_pointer, std::string* password_pointer, float* min_temperature_pointer, float* max_temperature_pointer);
    void setWifiStatus(bool wifi_status);
    bool getSensorsConfigured();
    void setSensorsConfigured(bool sensors_configured);
};