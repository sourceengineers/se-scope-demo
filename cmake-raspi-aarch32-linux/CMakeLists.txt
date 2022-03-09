cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 14)

# Set the Project name
set(PROJECT_NAME Se-Scope-Server)

project(${PROJECT_NAME})

################################################################################
# Add Sources
################################################################################

# se-scope-server Sources
set(SE-SCOPE-SERVER-SOURCES src/Server.cpp src/LinuxMutex.c)

# Communication Sources
set(COMMUNICATION-SOURCES src/Communication/UART/uart.c src/Communication/Communicator.cpp src/Communication/UartCommunicator.cpp)

add_executable(Se-Scope-Server ${SE-SCOPE-SERVER-SOURCES} ${ADC-SOURCES} ${COMMUNICATION-SOURCES})

target_include_directories(Se-Scope-Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../se-scope-target/public/ inc)

################################################################################
# Required Libraries
################################################################################
target_link_libraries(${PROJECT_NAME} pthread)

