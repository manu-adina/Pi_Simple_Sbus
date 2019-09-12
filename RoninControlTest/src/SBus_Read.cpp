#include "PiSBus.h"
#include <unistd.h>
#include <thread>
#include <cstdio>

void foo() {
    std::cout << "Hello" << std::endl;
}

int main(int argc, char *argv[]) {

    std::string port = argv[1];
    PiSBus sbus(port);
    sbus.Begin();

    char line[100];
    
    std::thread writing_to_bus(&PiSBus::Write, &sbus);
    //std::thread read_bus(&PiSBus::Read, &sbus);

    while(1) {
        fgets(line, sizeof(line), stdin);
        int input_channel;
        uint16_t input_value;
        sscanf(line, "%d %d", &input_channel, &input_value);
        sbus.InsertDataIntoChannel(input_channel, input_value);
        sbus.DisplayData();
    }

    writing_to_bus.join();
    //read_bus.join();
    return 0;
}
