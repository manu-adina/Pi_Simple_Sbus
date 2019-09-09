#include "PiSBus.h"
#include <unistd.h> // usleep()

int main() {

    PiSBus sbus("/dev/ttyUSB0"); // Need to find the serial output
    sbus.Begin();

    while(1) {
        sbus.Read();
        sbus.DisplayData();
        usleep(10000);
    }

    return 0;
}
