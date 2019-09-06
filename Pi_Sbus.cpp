#include "PiSBus.h"

#include <errno.h>      // For errors
#include <fcntl.h>      // For open / close file.
#include <sys/ioctl.h>  // For manipulating io.
#include <string.h>
#include <asm-generic/termbits.h> // For setting up serial params and having a non-default freq.

#define BAUDRATE 100000

PiSBus::PiSBus(std::string port) {
    _port = port;
}

/** Setting up serial communication with the right parameters for the SBus communication **/
int PiSBus::Begin() {
    _file = open(_port.c_str(), O_RDWR);
    if(_file < 0) {
        cerr << "Error " << errno << " opening " << port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    //Set custom baudrate (100k Hz) on Linux.
    struct termios2 tio;
    if(ioctl(_fd, TCGETS2, &tio)) { // Gets current serial port settings.
        std::cerr << "Failed to get serial settings." << std::endl;
        return -1;
    }

    tio.c_cflag &= ~CBAUD; // To remove the current baud rate.
    tio.c_cflag |= BOTHER; // To allow custom baud rate using int input.
    tio.c_cflag |= CST0PB; // To have 2 stop bits for the SBus frame.
    tio.c_cflag |= PARENB; // Enable parity bit.
    tio.c_ispeed = BAUDRATE;
    tio.c_ospeed = BAUDRATE;

    if(ioctl(_file, TCSETS2, &tio)) {
        std::cerr << "Failed to set the serial params." << std::endl;
    }

    return 0;
}

void PiSBus::Read() {
    int bytes_read;

    while(1) {
        bytes_read = read(_file, &_sbus_data, sizeof(_sbus_data));

        //0x0F is the start header for the Sbus protocol.
        if(_sbus_data[0] == 0x0F && _sbus_data[24] == 0x00) {
            break;
        }
    }

    //0x07FF is to ensure the bits have approriate value
    _channel_values[0]  = (uint16_t)((_sbus_data[1]       | _sbus_data[2]  << 8) & 0x07FF);
    _channel_values[1]  = (uint16_t)((_sbus_data[2]  >> 3 | _sbus_data[3]  << 5) & 0x07FF);
    _channel_values[2]  = (uint16_t)((_sbus_data[3]  >> 6 | _sbus_data[4]  << 2 | _sbus_data[5] << 10) & 0x07FF);
    _channel_values[3]  = (uint16_t)((_sbus_data[5]  >> 1 | _sbus_data[6]  << 7) & 0x07FF);
    _channel_values[4]  = (uint16_t)((_sbus_data[6]  >> 4 | _sbus_data[7]  << 4) & 0x07FF);
    _channel_values[5]  = (uint16_t)((_sbus_data[7]  >> 7 | _sbus_data[8]  << 1 | _sbus_data[9] << 9) & 0x07FF);
    _channel_values[6]  = (uint16_t)((_sbus_data[9]  >> 2 | _sbus_data[10] << 6) & 0x07FF);
    _channel_values[7]  = (uint16_t)((_sbus_data[10] >> 5 | _sbus_data[11] << 3) & 0x07FF);
    _channel_values[8]  = (uint16_t)((_sbus_data[12]      | _sbus_data[13] << 8) & 0x07FF);
    _channel_values[9]  = (uint16_t)((_sbus_data[13] >> 3 | _sbus_data[14] << 5) & 0x07FF);
    _channel_values[10] = (uint16_t)((_sbus_data[14] >> 6 | _sbus_data[15] << 2 | _sbus_data[16] << 10) & 0x07FF);
    _channel_values[11] = (uint16_t)((_sbus_data[16] >> 1 | _sbus_data[17] << 7) & 0x07FF);
    _channel_values[12] = (uint16_t)((_sbus_data[17] >> 4 | _sbus_data[18] << 4) & 0x07FF);
    _channel_values[13] = (uint16_t)((_sbus_data[18] >> 7 | _sbus_data[19] << 1 | _sbus_data[20] << 9) & 0x07FF);
    _channel_values[14] = (uint16_t)((_sbus_data[20] >> 2 | _sbus_data[21] << 6) & 0x07FF);
    _channel_values[15] = (uint16_t)((_sbus_data[21] >> 5 | _sbus_data[22] << 3) & 0x07FF);
}

PiSBus::~PiSBus() {
    close(_file);
}

void PiSBus::DisplayData() {
    std::cout << "Read data" << _channel_values[0] << " : " << _channel_values[1] << " : " << _channel_values[3] << std::endl;
}
