#include "PiSBus.h"
#include <unistd.h> // usleep()

int main(int argc, char *argv[]) {

    if(argc != 3) {
        std::cerr << "Too many or too few arguements." << std::endl;
        return -1;
    }

    if(argv[1][0] != 'r' && argv[1][0] != 'w') {
        std::cerr << "Incorrect arguements." << std::endl;
        return -1;
    }

    std::string port = argv[2];

    PiSBus sbus(port); // Need to find the serial output
    sbus.Begin();

    //sbus.InsertDataIntoChannel(0, 500);
    //sbus.InsertDataIntoChannel(1, 500);
    //sbus.InsertDataIntoChannel(3, 500);

    switch(argv[1][0]) {
        case 'r':
            std::cout << "Reading" << std::endl;
            while(1) {
                if(sbus.Read() == -1) {
                    std::cerr << "Failed to read" << std::endl;
                    return -1;
                }
                sbus.DisplayData();
                usleep(10000);
            }
            break;

        case 'w':
            std::cout << "Writing" << std::endl;
            while(1) {
                if(sbus.Write() == -1) {
                    std::cerr << "Failed to write" << std::endl;
                }
                usleep(7000);
            }
            break;

        default:
            return -1;
            break;
    }
    return 0;
}
