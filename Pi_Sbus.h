#ifndef PI_SBUS
#define PI_SBUS

class PiSBus {
    public:
        PiSBus(std::string port);
        uint8_t sbus_data[25];
        int uint16_t Begin();

    private:
       std::string _port;
       int _file;
       uint16_t _channel_values[18];

};

#endif
