/*
 * main.c
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

//includes
#include "DSP28x_Project.h"
#include <stdint.h>
#include "Scope/Communication/ITransceiver.h"
#include "Scope/Builders/ScopeFramedStack.h"
#include "se-lib-c/stream/BufferedByteStream.h"
#include "UartDriver.h"
#include <math.h>

//Timer variables
uint32_t timestamp_ms = 0;
uint32_t deltaTime_ms = 0;
uint32_t timerDelta;
uint32_t timerCounts;
uint32_t timerCounts_0 = 0xFFFFFFFF;
uint32_t timerMax = 0xFFFFFFFF;
uint32_t timer_cycleCount = 0;

uint32_t loopCount = 0;

//Define Scope values
float valueA = 0.0f;
uint16_t valueB = 0;

void transmit(ITransceiverHandle self) {
    uint32_t outputSize = self->outputSize(self->handle);
    for (int i = 0; i < outputSize; ++i) {
        uint16_t byte = 0;
        self->get(self->handle, &byte, 1);
        UartDriver_write(&byte, 1);
    }
}


int main(void)
{
    //Required setup
    InitSysCtrl();

    //Initialize the Sci
    UartDriver_setup();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    //initialize the timer
    InitCpuTimers();
    CpuTimer0Regs.TCR.all = 0x0000; //Set timer TSS bit to 0

    UartConfig uartConfig = {
      .baudrate = 115200,
      .charCount = EIGHT,
      .parityEnable = DISABLED,
      .parityOddEven = ODD,
      .stopbits = ONE
    };

    //initialize the uart
    UartDriver_init(uartConfig, 100, 300);

    IER = 0x100;                         // Enable CPU INT

    EINT;

    //Scope Setup
    ScopeFramedStackConfig config = {
            .addressesInAddressAnnouncer = 3,
            .callback = &transmit,
            .timestamp = &timestamp_ms,
            .amountOfChannels = 3,
            .timebase = 0.001f,
            .sizeOfChannels = 50
   };

    BufferedByteStreamHandle bufferedStream = BufferedByteStream_create(50);
    IByteStreamHandle stream = BufferedByteStream_getIByteStream(bufferedStream);

    ScopeFramedStackLogOptions logConfig = {
            .logByteStream = BufferedByteStream_getIByteStream(bufferedStream)
    };


    Message_Priorities priorities = {
         .data = HIGH,
         .log = MEDIUM,
         .stream = LOW,
    };

    ScopeFramedStackHandle scopeStack = ScopeFramedStack_create(config, logConfig, priorities);
    ITransceiverHandle transceiver = ScopeFramedStack_getTranscevier(scopeStack);

    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(scopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "A", &valueA, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "B", &valueB, SE_UINT16);

    //Start CPU timer
    CpuTimer0Regs.TCR.bit.TSS = 0;

    bool logPending = false;

    //Scope loop
    for(;;)
    {

        uint16_t byte;
        while(UartDriver_read(&byte, 1) > 0) {
            transceiver->put(transceiver->handle, &byte, 1);
        }

        //Get Timestamp (CPU freq = 90 MHz, 32Bit Timer, counting down)
        timerCounts = CpuTimer0Regs.TIM.all;

        if (timestamp_ms % 1000 > 500)
        {
            logPending = true;
        }

        if (timestamp_ms % 1000 < 500 && logPending == true)
        {
            stream->write(stream->handle, "test\n", 5);
            logPending = false;
        }

        if (timerCounts < timerCounts_0)
        {
            timerDelta = timerCounts_0 - timerCounts;
        }
        else
        {
            timerDelta = timerCounts_0 + (timerMax - timerCounts);
            timer_cycleCount++;
        }
        timerCounts_0 = timerCounts;

        deltaTime_ms = timerDelta / 90000;
        timestamp_ms = ((timerMax - timerCounts)/90000)+(timer_cycleCount * 47722);

        //Assign values for scope stack
        valueA = sinf(timestamp_ms);
        valueB += 10;

        ScopeFramedStack_run(scopeStack);

        loopCount++;
    }
}

