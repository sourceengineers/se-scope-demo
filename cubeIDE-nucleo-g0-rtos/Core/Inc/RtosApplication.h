#ifndef RTOSAPPLICATION
#define RTOSAPPLICATION

#include <stdint.h>

#include "Scope/Builders/ScopeFramedStack.h"
#include "se-lib-c/stream/IByteStream.h"

/******************************************************************************
 External variables
******************************************************************************/

extern float sinus;
extern float cosinus;
extern uint32_t timestamp;
extern float leistung;
extern float sum;
const extern float frequency;
extern int flipflop;
extern uint8_t toggle;

extern IByteStreamHandle logger;
extern ScopeFramedStackHandle scopeStack;

/******************************************************************************
 Public functions
******************************************************************************/

void stackApplication(void* args);

void dataApplication(void* args);

void logApplication(void *args);

#endif
