//
// Created by anselm on 17.05.21.
//

#ifndef SE_SCOPE_LINUX_UARTCOMMUNICATOR_H
#define SE_SCOPE_LINUX_UARTCOMMUNICATOR_H

#include <string>
#include <vector>

class UartDriver {

public:
    int start(const std::string& path);
    void transmit(const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> receive() const;
    void stop();
private:
    int fs;
};

#endif //SE_SCOPE_LINUX_UARTCOMMUNICATOR_H
