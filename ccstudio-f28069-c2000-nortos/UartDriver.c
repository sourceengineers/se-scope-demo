/*
 * UartDriver.c
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

#include "UartDriver.h"
#include <string.h>
#include <stdlib.h>
#include "DSP28x_Project.h"

#define SYSCLKOUT 90000000
#define SERIAL_FIFO_SIZE 4

//
// Function Prototypes
//
__interrupt void sciRxFifoIsr(void);
__interrupt void sciTxFifoIsr(void);
uint16_t txFifoFree();
void txFifoClear();
bool txIsReady();
bool rxFifoOverflowOccured();
void rxFifoClear();
uint16_t rxFifoUsed();

typedef struct {
    uint16_t* buffer;
    size_t read;
    size_t write;
    size_t max;
} Buffer;

void Buffer_init(Buffer* buffer, size_t length);
size_t Buffer_length(Buffer* buffer);
size_t Buffer_remaining(Buffer* buffer);
int32_t Buffer_put(Buffer* buffer, uint16_t byte);
int32 Buffer_get(Buffer* buffer, uint16_t* byte);


void Buffer_init(Buffer* buffer, size_t length) {
    buffer->buffer = malloc(sizeof(uint16_t) * length);
    buffer->max = length;
    buffer->write = 0;
    buffer->read = 0;

    for (int i = 0; i < buffer->max; ++i) {
        buffer->buffer[i] = 0;
    }
}

size_t Buffer_length(Buffer* buffer) {
    int length = buffer->write - buffer->read;
    length = length < 0 ? buffer->max + length : length;
    length = length % buffer->max;
    return length;
}

size_t Buffer_remaining(Buffer* buffer) {
    return buffer->max - Buffer_length(buffer);
}

int32_t Buffer_put(Buffer* buffer, uint16_t byte) {

    if (Buffer_remaining(buffer) <= 0) {
        return 0;
    }

    buffer->buffer[buffer->write] = byte;
    buffer->write += 1;
    buffer->write = buffer->write % buffer->max;

    return 1;
}

int32 Buffer_get(Buffer* buffer, uint16_t* byte) {

    if (Buffer_length(buffer) <= 0) {
        return 0;
    }

    *byte = buffer->buffer[buffer->read];
    buffer->read += 1;
    buffer->read = buffer->read % buffer->max;

    return 1;
}


typedef struct __UartHandlerPrivateData {
    Buffer input;
    Buffer output;
} PrivateData;

static PrivateData me = {};

void UartDriver_init(UartConfig config, size_t inputBufferSize, size_t outputBufferSize) {

    Buffer_init(&me.input, inputBufferSize);
    Buffer_init(&me.output, outputBufferSize);

    // Configure the uart according to user inputs
    ScibRegs.SCICCR.bit.STOPBITS = config.stopbits;
    ScibRegs.SCICCR.bit.PARITYENA = config.parityEnable;
    ScibRegs.SCICCR.bit.PARITY = config.parityOddEven;
    ScibRegs.SCICCR.bit.SCICHAR = config.charCount;

    // Enable rx and tx interrupts
    ScibRegs.SCICTL2.bit.TXINTENA = 1;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;

    // Clean this up. Sysclockspeed divided by reset default of low speed peripheral clock prescaler
    uint32_t clockFrequency = SYSCLKOUT / SysCtrlRegs.LOSPCP.bit.LSPCLK / 2;
    uint16_t brr = clockFrequency / (config.baudrate * 8) - 1;
    ScibRegs.SCIHBAUD = brr >> 8;
    ScibRegs.SCILBAUD = brr & 0xFF;

    // TX FIFO configuration
    ScibRegs.SCIFFTX.bit.SCIRST = 1;        // Reset TX and RX Channels
    ScibRegs.SCIFFTX.bit.SCIFFENA = 1;      // Enable TX FIFO
    ScibRegs.SCIFFTX.bit.TXFFIL = 2;        // Interrupt trigger level. Trigger when only one word is left in the FIFO
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;  // Relinquish TX FIFO from Reset

    ScibRegs.SCIFFRX.bit.RXFFIENA = 1;      // Enable RX FIFO interrupt
    ScibRegs.SCIFFRX.bit.RXFFIL = 1;        // Interrupt trigger level. Triggen when ever one byte has been received.
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;   // Relinquish RX FIFO from Reset

    ScibRegs.SCICTL1.bit.SWRESET = 1;       // Relinquish SCI from Reset
    ScibRegs.SCICTL1.bit.TXENA = 1;         // Enable Rx
    ScibRegs.SCICTL1.bit.RXENA = 1;         // Enable Tx


    // Register the interrupt functions
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIRXINTB = &sciRxFifoIsr;
    PieVectTable.SCITXINTB = &sciTxFifoIsr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;     // PIE Group 9, INT3
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;     // PIE Group 9, INT4
}


void UartDriver_setup() {
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0; // Enable pull-up for GPIO15 (SCIRXDB)
    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0; // Enable pull-up for GPIO58 (SCITXDB)

    //
    // Set qualification for selected pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    //
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)

    //
    // Configure SCI-B pins using GPIO regs
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    //

    //
    // Configure GPIO15 for SCIRXDB operation
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;

    //
    // Configure GPIO58 for SCITXDB operation
    //
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2;

    EDIS;
}


__interrupt void sciTxFifoIsr(void)
{
    ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
    ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;  // Clear SCI Interrupt flag
    PieCtrlRegs.PIEACK.all |= 0x100;      // Issue PIE ACK
    uint16_t bytesToWriteInStream = Buffer_length(&me.output);

    if (bytesToWriteInStream == 0) {
        return;
    }

    // Enable the interrupt as in this case, some bytes will have been pushed into the fifo
    // If the freeBytesInFifo check aborts, the interrupt needs to be started again to send the remaining bytes
    ScibRegs.SCIFFTX.bit.TXFFIENA = 1;

    uint16_t freeBytesInFifo = txFifoFree();

    if (freeBytesInFifo == 0) {
        return;
    }

    // Check how many bytes we can write into the fifo. Use at max the amount of free space in the fifo
    uint16_t bytesToWrite = bytesToWriteInStream < freeBytesInFifo ? bytesToWriteInStream : freeBytesInFifo;

    for (int i = 0; i < bytesToWrite; ++i) {
        uint16_t byte;
        if (Buffer_get(&me.output, &byte)) {
            ScibRegs.SCITXBUF = byte;
        }
    }
}

__interrupt void sciRxFifoIsr(void)
{
    while(rxFifoUsed() > 0) {
        uint16_t data = ScibRegs.SCIRXBUF.all;  // Read data
        if ((data >> 8) > 0) {
            // TODO: Receive error. Do something with it
        }

        Buffer_put(&me.input, data & 0xFF);
    }

    PieCtrlRegs.PIEACK.all |= 0x100;       // Issue PIE ack
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag
}


int UartDriver_readBytesPending() {
    return Buffer_length(&me.input);
}

int UartDriver_read(uint16_t* buffer, size_t length) {

    ScibRegs.SCIFFRX.bit.RXFFIENA = 0;

    uint16_t bytesToReadInStream = Buffer_length(&me.input);
    uint16_t bytesToRead = bytesToReadInStream < length ? bytesToReadInStream : length;

    for (int i = 0; i < bytesToRead; ++i) {
        uint16_t byte;
        if (Buffer_get(&me.input, &byte)) {
            buffer[i] = byte;
        }
    }

    PieCtrlRegs.PIEACK.all |= 0x100;       // Issue PIE ack
    ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag

    return bytesToRead;
}

int UartDriver_write(uint16_t* buffer, size_t length) {

    uint16_t outputBufferFreeBytes = Buffer_remaining(&me.output);
    uint16_t bytesToSend = length < outputBufferFreeBytes ? length : outputBufferFreeBytes;

    if (length > outputBufferFreeBytes) {
        return -1;
    }

    for (int i = 0; i < bytesToSend; ++i) {
        Buffer_put(&me.output, buffer[i]);
    }

    // Write the first byte into the stream to start the sending if its not already in progress
    if (Buffer_length(&me.output) == bytesToSend && txIsReady()) {
        uint16_t firstByte;
        if (Buffer_get(&me.output, &firstByte)) {
            ScibRegs.SCITXBUF = firstByte;
        }
    }

    // Start the interrupts
    ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;
    ScibRegs.SCIFFTX.bit.TXFFIENA = 1;
    PieCtrlRegs.PIEACK.all |= 0x100;

    return bytesToSend;
}

bool UartDriver_txReady() {
    return Buffer_length(&me.output) == 0 && txFifoFree() == 4 && txIsReady();
}

uint16_t txFifoFree() {
    return 4 - ScibRegs.SCIFFTX.bit.TXFFST;
}

void txFifoClear() {
    ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;
}

bool txIsReady() {
    return ScibRegs.SCICTL2.bit.TXRDY == 1;
}

bool rxFifoOverflowOccured() {
    return ScibRegs.SCIFFRX.bit.RXFFOVF == 1;
}

void rxFifoClear() {
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
}

uint16_t rxFifoUsed() {
    return ScibRegs.SCIFFRX.bit.RXFFST;
}
