#include "PiSBus.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    PiSBus::PiSBus sbus;
    sbus.Begin();

    while(1) {
        sbus.Read();
        sbus.DisplayData();
        usleep(100000);
    }

    return 0;
}
