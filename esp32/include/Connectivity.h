namespace Plantagotchi {

    extern std::string ssid;
    extern std::string password;

    extern int8_t* min_temperature_pointer;
    extern int8_t* max_temperature_pointer;
    extern int8_t* min_humidity_pointer;
    extern int8_t* max_humidity_pointer;

    extern bool ssid_set;
    extern bool password_set;
    extern bool min_temperature_set;
    extern bool max_temperature_set;
    extern bool min_humidity_set;
    extern bool max_humidity_set;


    void setupBLE(int8_t* min_temperature_pointer, int8_t* max_temperature_pointer, int8_t* min_humidity_pointer, int8_t* max_humidity_pointer);

    void setupWiFi();

    bool getWifiConfigured();
    void resetWifiConfigured();

    bool getSensorsConfigured();
};