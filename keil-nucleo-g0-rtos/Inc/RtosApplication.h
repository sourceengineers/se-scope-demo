
#include <stdint.h>

#ifndef RTOSAPPLICATION
#define RTOSAPPLICATION

/******************************************************************************
 External variables
******************************************************************************/

extern const float frequency;

extern float sinus;
extern float cosinus;
extern uint32_t timestamp;
extern float leistung;
extern float sum;
extern int flipflop;
extern uint8_t toggle;

/******************************************************************************
 Public functions
******************************************************************************/

void scopeApplication(void* args);

void stackApplication(void* args);

void dataApplication(void* args);

#endif
