#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

const int pan_channel  = 1;
const int tilt_channel = 2;
const int roll_channel = 4;

int main() {

    int file;

    char *filename = "/dev/i2c-1";
    if((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open i2c bus");
        exit(1);
    }

    int arduino_addr = 0x04;

    if(ioctl(file, I2C_SLAVE, arduino_addr) < 0) {
        printf("Failed to talk to Arduino");
        exit(1);
    }

    int pan_value  = 1023;
    int tilt_value = 1023;
    int roll_value = 1023;

    char line[100];


    while(1) {
        fgets(line, sizeof(line), stdin);
        sscanf(line, "%d %d %d", &pan_value, &tilt_value, &roll_value);

        uint16_t buf[3] = {0};
        buf[0] = pan_value;
        buf[1] = tilt_value;
        buf[2] = roll_value;

        if(write(file, buf, 6) != 6) {
            fprintf(stderr, "Error writing\n");
            close(file);
            exit(EXIT_FAILURE);
        }
        printf("Write Success\n");
    }
    return 0;
}


