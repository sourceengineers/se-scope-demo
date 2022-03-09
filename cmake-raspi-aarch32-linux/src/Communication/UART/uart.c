//
// Created by anselm on 11.05.21.
// Adapted from: https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart
// opens the file /dev/serial0 on the raspberry pi with a baudrate of 9600,  Transmits some Bytes,
// waits 3 seconds, prints the bytes received during this time, and closes the port after.
//

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART


int open_uart(char* uart_path){
    int uart0_filestream = open(uart_path, O_RDWR | O_NOCTTY | O_NDELAY);
    return uart0_filestream;
}

void setOptions(int uart0_filestream){
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void uart_transmit(int uart0_filestream, char* msg, int len){
    if (uart0_filestream != -1)
    {
        write(uart0_filestream, msg, len);		//Filestream, bytes to write, number of bytes to write
    }
}


int uart_rxBytes(int uart0_filestream, char* rx_buffer, int rx_buffer_length){
    int rx_length = -1;
    if (uart0_filestream != -1)
    {
        rx_length = read(uart0_filestream, (void*)rx_buffer, rx_buffer_length);
    }
    else{
        printf("E: uart0_filestream error: %d\n\r", uart0_filestream);
    }
    return rx_length;
}

void closeUart(int uart0_filestream){
    close(uart0_filestream);
}


