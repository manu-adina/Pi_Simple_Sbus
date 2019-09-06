#ifndef PI_SBUS
#define PI_SBUS

class PiSBus {
    public:
        PiSBus(std::string port);
        int uint16_t Begin();
        void Read();

    private:
       std::string _port;
       int _file;
       uint16_t _channel_values[18];
       uint8_t _sbus_data[25];
};

#endif
