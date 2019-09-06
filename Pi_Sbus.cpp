#include "PiSbus.h"

#include <errno.h>      // For errors
#include <fcntl.h>      // For open / close file.
#include <sys/ioctl.h>  // For manipulating io.
#include <string.h>

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

bool PiSBus::Update() {
    
}

bool PiSBus::Send() {

}

PiSBus::~PiSBus() {

}
