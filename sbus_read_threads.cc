#include "PiSBus.h"
#include <unistd.h>
#include <thread>

int main(int argc, char *argv[]) {

    std::string port = argv[2];
    PiSBus sbus(port);
    sbus.Begin();
    
    std::thread write_to_ch(sbus.Write());
    std::thread read_ch(sbus.Read());

    

    return 0;
}
