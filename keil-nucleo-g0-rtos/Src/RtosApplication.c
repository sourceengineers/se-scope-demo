#include "RtosApplication.h"

#include <UartDriver.h>
#include "Scope/Builders/ScopeFramedStack.h"

#include <FreeRTOS.h>
#include <task.h>

#include <arm_math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern ScopeFramedStackHandle scopeStack;
extern IByteStreamHandle logger;

#define DATA_APPLICATION_TASK_PERIOD_MS  (10u)
#define STACK_APPLICATION_TASK_PERIOD_MS (10u * DATA_APPLICATION_TASK_PERIOD_MS)

/* Increase timestamp and calculate waveforms */
void dataApplication(void *args) {
	timestamp = 0u;
	toggle = 0;
	uint16_t toggleTime = 0u;
	
	while (true) {
		/* Timestamping */
		timestamp += DATA_APPLICATION_TASK_PERIOD_MS;
		
		float t_in_s = timestamp / 1000.0f;
		sinus = 2 * sinf(2 * PI * frequency * t_in_s);
		cosinus = 2 * cosf(2 * PI * frequency * t_in_s);
		leistung = sinus * cosinus;
		sum = sinus + cosinus + leistung;
		flipflop = flipflop * -1;

		toggleTime += DATA_APPLICATION_TASK_PERIOD_MS;
		if (toggleTime > 10000u) {
			toggleTime = 0u;
			toggle = 1 - toggle;
		}

		ScopeFramedStack_runThreadScope(scopeStack);
		vTaskDelay(pdMS_TO_TICKS(DATA_APPLICATION_TASK_PERIOD_MS));
	}
}

void logApplication(void *args) {
	while(true) {
		char msg[60];
		sprintf(msg, "%d", (int)timestamp);
		strcat(msg, ": Log Message\n\r");
		logger->write(logger->handle, (uint8_t *)msg, strlen(msg));
		vTaskDelay(1000);
	}
}

void stackApplication(void *args) {
	while (true) {
		UartDriver_run();
		ScopeFramedStack_runThreadStack(scopeStack);
		vTaskDelay(pdMS_TO_TICKS(STACK_APPLICATION_TASK_PERIOD_MS));
	}
}
