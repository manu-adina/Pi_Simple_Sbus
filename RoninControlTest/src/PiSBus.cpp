#include "PiSBus.h"

#include <errno.h>      // For errors
#include <fcntl.h>      // For open / close file.
#include <sys/ioctl.h>  // For manipulating io.
#include <unistd.h>     // read()
#include <string.h>
#include <asm-generic/termbits.h> // For setting up serial params and having a non-default freq.

#define BAUDRATE 100000

PiSBus::PiSBus(std::string port) {
    _port = port;
}

/** Setting up serial communication with the right parameters for the SBus communication **/
int PiSBus::Begin() {
    _file = open(_port.c_str(), O_RDWR | O_NOCTTY);
    if(_file < 0) {
        std::cerr << "Error: " << errno << " opening " << _port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    //Set custom baudrate (100k Hz) on Linux.
    struct termios2 tio;
    if(ioctl(_file, TCGETS2, &tio)) { // Gets current serial port settings.
        std::cerr << "Failed to get serial settings." << std::endl;
        return -1;
    }

    tio.c_cflag &= ~CBAUD;   // To remove the current baud rate.
    tio.c_cflag |= BOTHER;   // To allow custom baud rate using int input.
    tio.c_cflag |= CSTOPB;   // To have 2 stop bits for the SBus frame.
    tio.c_cflag |= PARENB;   // Enable parity bit.
    tio.c_ispeed = BAUDRATE; // Set input Hz
    tio.c_ospeed = BAUDRATE; // Set output Hz

    //New settings maybe it will help. HELPED.
    tio.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OPOST);
    tio.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    tio.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    tio.c_cflag &= ~(CSIZE | PARODD | CBAUD);
    
    tio.c_cflag |= CS8;

    if(ioctl(_file, TCSETS2, &tio)) {
        std::cerr << "Failed to set the serial params. " << errno << " : " <<strerror(errno) << std::endl;
        return -1;
    }

    for(int i = 0; i < 16; i++) {
        _channel_values[i] = 1023;
    }

    return 0;
}

int PiSBus::Read() {
    int bytes_read;

    std::cout << "Commencing to read" << std::endl;

    while(1) {
        bytes_read = read(_file, &_sbus_data, sizeof(_sbus_data));
        if(bytes_read == -1) {
            std::cerr << "Unable to Read: " << errno << ": " << strerror(errno) << std::endl;
            return -1;
        }
        std::cout << "Number of bytes " << bytes_read << std::endl;

        // 0x0F is the start header for the Sbus protocol.
        if(_sbus_data[0] == 0x0F && _sbus_data[24] == 0x00) {
            break;
        }

        usleep(7000);
    }

    std::cout << "Finished Reading" << std::endl;

    // 0x07FF is to ensure that the data is 11 bits long, so it '0's the other 5 bits
    // Retreiving the data from the frame.
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

    return 0;
}

int PiSBus::InsertDataIntoChannel(int channel, uint16_t value) {
    if(value < 0 || value > 2047 || channel < 0 || channel > 15) {
        std::cerr << "Inapporiate channel or value" << std::endl;
        return -1;
    }

    _channel_values[channel] = value;
    return 1;
}

int PiSBus::Write() {
    uint8_t frame_to_send[25];
    
    frame_to_send[0]  = 0x0F; // Header byte.
    frame_to_send[1]  = (uint8_t)((_channel_values[0]  & 0x07FF));
    frame_to_send[2]  = (uint8_t)((_channel_values[0]  & 0x07FF) >> 8  | (_channel_values[1]  & 0x07FF) << 3);
    frame_to_send[3]  = (uint8_t)((_channel_values[1]  & 0x07FF) >> 5  | (_channel_values[2]  & 0x07FF) << 6);
    frame_to_send[4]  = (uint8_t)((_channel_values[2]  & 0x07FF) >> 2);
    frame_to_send[5]  = (uint8_t)((_channel_values[2]  & 0x07FF) >> 10 | (_channel_values[3]  & 0x07FF) << 1);
    frame_to_send[6]  = (uint8_t)((_channel_values[3]  & 0x07FF) >> 7  | (_channel_values[4]  & 0x07FF) << 4);
    frame_to_send[7]  = (uint8_t)((_channel_values[4]  & 0x07FF) >> 4  | (_channel_values[5]  & 0x07FF) << 7);
    frame_to_send[8]  = (uint8_t)((_channel_values[5]  & 0x07FF) >> 1);
    frame_to_send[9]  = (uint8_t)((_channel_values[5]  & 0x07FF) >> 9  | (_channel_values[6]  & 0x07FF) << 2);
    frame_to_send[10] = (uint8_t)((_channel_values[6]  & 0x07FF) >> 6  | (_channel_values[7]  & 0x07FF) << 5);
    frame_to_send[11] = (uint8_t)((_channel_values[7]  & 0x07FF) >> 3);
    frame_to_send[12] = (uint8_t)((_channel_values[8]  & 0x07FF));
    frame_to_send[13] = (uint8_t)((_channel_values[8]  & 0x07FF) >> 8  | (_channel_values[9]  & 0x07FF) << 3);
    frame_to_send[14] = (uint8_t)((_channel_values[9]  & 0x07FF) >> 5  | (_channel_values[10] & 0x07FF) << 6);
    frame_to_send[15] = (uint8_t)((_channel_values[10] & 0x07FF) >> 2);
    frame_to_send[16] = (uint8_t)((_channel_values[10] & 0x07FF) >> 10 | (_channel_values[11] & 0x07FF) << 1);
    frame_to_send[17] = (uint8_t)((_channel_values[11] & 0x07FF) >> 7  | (_channel_values[12] & 0x07FF) << 4); 
    frame_to_send[18] = (uint8_t)((_channel_values[12] & 0x07FF) >> 4  | (_channel_values[13] & 0x07FF) << 7);
    frame_to_send[19] = (uint8_t)((_channel_values[13] & 0x07FF) >> 1);
    frame_to_send[20] = (uint8_t)((_channel_values[13] & 0x07FF) >> 9  | (_channel_values[14] & 0x07FF) << 2); 
    frame_to_send[21] = (uint8_t)((_channel_values[14] & 0x07FF) >> 6  | (_channel_values[15] & 0x07FF) << 5);
    frame_to_send[22] = (uint8_t)((_channel_values[15] & 0x07FF) >> 3);
    frame_to_send[23] = 0x00; // Flags
    frame_to_send[24] = 0x00; // Footer

    if(sizeof(frame_to_send) != write(_file, frame_to_send, sizeof(frame_to_send))) {
        std::cerr << errno << "Failed to send: " << strerror(errno) << std::endl;
        return -1;
    }
    
    return 0;
}

PiSBus::~PiSBus() {
    close(_file);
}

// Displays the channel data.
void PiSBus::DisplayData() {
    std::cout << "Read data " << _channel_values[0] << " : " << _channel_values[1] << " : " << _channel_values[3] << std::endl;
}
