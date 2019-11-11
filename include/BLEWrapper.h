#include <string>

class BLEWrapper {
    public:
        static void setup(std::string* ssid, std::string* password);
        static void setWifiStatus(std::string wifi_status);
};