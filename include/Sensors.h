namespace Plantagotchi {

    extern int8_t min_temperature, max_temperature, min_humidity, max_humidity;
    extern bool has_moved;
    extern bool detect_movement;
    extern bool allow_reading_sensors;
    extern bool is_reading_sensors;


    enum Sensor {
        Temperature = 0x01,
        Humiditiy = 0x02,
        Light = 0x04
    };


    void connectToSensors();

    void checkForMovement();

    void readSensors(uint8_t* ticker_pointer);
}