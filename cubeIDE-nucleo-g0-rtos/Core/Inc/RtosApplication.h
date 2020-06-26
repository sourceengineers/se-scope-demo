
#include <stdint.h>

#ifndef RTOSAPPLICATION
#define RTOSAPPLICATION

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

/******************************************************************************
 Public functions
******************************************************************************/

void stackApplication(void* args);

void dataApplication(void* args);

void logApplication(void *args);

#endif
