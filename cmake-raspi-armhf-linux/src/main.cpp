#include <iostream>
#include <Server.h>

int main(int argc, char** argv) {

    Server server;
    std::cout << "Starting the server " << std::endl;
    server.start();
}
