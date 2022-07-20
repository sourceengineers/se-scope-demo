#include "UartDriver.h"
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "Scope/Communication/ITransceiver.h"
#include "Scope/Builders/ScopeFramedStack.h"
#include <stdlib.h>

typedef struct __UartHandlerPrivateData {
    size_t inputSize;
    uint8_t* rxBuffer;
    uint32_t rxCount;
    uint8_t* txBuffer;
    int txCount;
    size_t outputSize;
    bool txIsDone;
    uint32_t bytesToSend;
		ITransceiverHandle transceiver;
} PrivateData;
static PrivateData me;

extern ScopeFramedStackHandle scopeStack;

void UartDriver_init() {
		// Make the buffers slightly bigger than needed, just to be sure
    me.inputSize = (uint32_t) 255;
    me.outputSize = (uint32_t) 255;
    me.rxBuffer = malloc(me.inputSize * sizeof(uint8_t));
    me.rxCount = 0;
    me.bytesToSend = 0;
    me.txBuffer = malloc(me.outputSize * sizeof(uint8_t));
    me.txCount = 0;
    me.txIsDone = true;
		me.transceiver = ScopeFramedStack_getTranscevier(scopeStack);
	
    LL_USART_EnableIT_RXNE(USART2);
    LL_USART_EnableIT_ERROR(USART2);
}

void UartDriver_run(void) {
    /* Check if the uart output buffer in the scope is empty or not. If yes, send more data */
    uint32_t dataPending = me.transceiver->outputSize(me.transceiver);
    if (me.txIsDone && dataPending > 0) {
        // Only send as many bytes as there are left to be sent
        me.bytesToSend = dataPending >= me.outputSize ? me.outputSize : dataPending;

        // Copy from scope to output buffer
				me.transceiver->get(me.transceiver, me.txBuffer, me.bytesToSend);
        me.txIsDone = false;
        me.txCount = 0;
        LL_USART_EnableIT_TXE(USART2);
    }

    if (me.rxCount > 0) {

				// Fast copy rx data into temporary buffer. 
        LL_USART_DisableIT_RXNE(USART2);
        size_t copyCount = me.rxCount;
        uint8_t tmpBuffer[copyCount];
        memcpy(tmpBuffer, me.rxBuffer, copyCount);
        me.rxCount = 0;
        LL_USART_EnableIT_RXNE(USART2);
				
				// Send copied rx data into scope
				me.transceiver->put(me.transceiver, tmpBuffer, copyCount);
    }
}

void transmit_data(ITransceiverHandle self) {

}

void UART_CharReception_Callback(void) {
    uint8_t byte = LL_USART_ReceiveData8(USART2);

    if(me.rxCount < me.inputSize){
        me.rxBuffer[me.rxCount] = byte;
        me.rxCount++;
    }
}

void UART_TXEmpty_Callback(void) {
    if(me.txCount < me.bytesToSend){
        uint8_t cRxedChar = me.txBuffer[me.txCount];
        LL_USART_TransmitData8(USART2, cRxedChar);
        me.txCount++;
    } else {
        me.txIsDone = true;
        LL_USART_DisableIT_TXE(USART2);
        LL_USART_EnableIT_RXNE(USART2);
    }
}

void UART_CharTransmitComplete_Callback(void) {
}

void UART_Error_Callback(void) {
}

