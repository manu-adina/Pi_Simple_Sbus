#include <stdio.h>
#include <stdlib.h>
#include "SBUS.h"

const int pan_channel  = 1;
const int tilt_channel = 2;
const int roll_channel = 4;

int main() {

    int pan_value  = 1023;
    int tilt_value = 1023;
    int roll_value = 1023;

    SBUS s_bus(Serial1);


    while(1) {
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%d %d %d", &pan_value, &tilt_value, &roll_value);
        
        

}


