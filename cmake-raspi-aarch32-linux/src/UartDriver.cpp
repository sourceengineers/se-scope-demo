#include <cstring>
#include <UartDriver.h>
#include <ostream>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int UartDriver::start(const std::string& path){
    fs = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (fs < 0) {
        return fs;
    }

    struct termios options;
    tcgetattr(fs, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    // Use the driver in a non blocking way
    // This will help to clean up the interface
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;
    tcflush(fs, TCIFLUSH);
    tcsetattr(fs, TCSANOW, &options);

    return 0;
}

void UartDriver::transmit(const std::vector<uint8_t>& data) const {
    for (uint8_t b: data) {
        write(fs, &b, 1);
    }
}

std::vector<uint8_t> UartDriver::receive() const {
    uint8_t byte;
    std::vector<uint8_t> bytes;
    while (read(fs, &byte, 1) > 0) {
        bytes.push_back(byte);
    }

    return bytes;
}

void UartDriver::stop() {
    close(fs);
}
