cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 14)

# Set the Project name
set(PROJECT_NAME Se-Scope-Server)

project(${PROJECT_NAME})

################################################################################
# Add Sources
################################################################################
# Communication Sources
add_executable(Se-Scope-Server src/main.cpp src/Server.cpp src/LinuxMutex.cpp src/TcpServer.cpp)
target_include_directories(Se-Scope-Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../se-scope-target/public/ inc)

################################################################################
# Required Libraries
################################################################################
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/../se-scope-target/lib/aarch32-linux/libse-scope.a
        ${CMAKE_CURRENT_SOURCE_DIR}/../se-scope-target/lib/aarch32-linux/libse-lib-c.a
        ${CMAKE_CURRENT_SOURCE_DIR}/../se-scope-target/lib/aarch32-linux/libprotobuf-nanopb.a
        pthread)

