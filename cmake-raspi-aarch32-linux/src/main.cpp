#include <iostream>
#include <Server.h>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Serial port must be specified" << std::endl;
        exit(-1);
    }

    std::string serial(argv[1]);
    Server server(serial);
    std::cout << "Starting the server " << std::endl;
    server.start();
}
