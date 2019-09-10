#ifndef PI_SBUS
#define PI_SBUS

#include <iostream>

class PiSBus {
    public:
        PiSBus(std::string port);
        int Begin();
        void Read();
        void DisplayData();
        int Write();
        int InsertDataIntoChannel(int channel, uint16_t value); 
        ~PiSBus();

    private:
       std::string _port;
       int _file;
       uint16_t _channel_values[18];
       uint8_t _sbus_data[25];
};

#endif
