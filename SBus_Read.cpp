#include "PiSBus.h"
#include <unistd.h> // usleep()

int main() {

    PiSBus sbus("lol"); // Need to find the serial output
    sbus.Begin();

    while(1) {
        sbus.Read();
        sbus.DisplayData();
        usleep(100000);
    }

    return 0;
}
