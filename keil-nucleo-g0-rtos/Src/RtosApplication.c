#include "RtosApplication.h"
#include <FreeRTOS.h>
#include <task.h>
#include <UartDriver.h>
#include <arm_math.h>
#include <stdbool.h>
#include "Scope/Builders/ScopeFramedStack.h"

extern ScopeFramedStackHandle scopeStack;

void scopeApplication(void* args){
	
	static uint32_t tmp_timestamp = 0;
	
		while(true){
			timestamp = tmp_timestamp++;
			
			ScopeFramedStack_runThreadScope(scopeStack);
			vTaskDelay(pdMS_TO_TICKS(1));
		}
}


void dataApplication(void* args){
	
		while(true){
			float t_in_s = timestamp / 1000.0f;

			sinus = 2 * sinf(2 * PI * frequency * t_in_s);
			cosinus = 2 * cosf(2 * PI * frequency * t_in_s);
			leistung = sinus * cosinus;
			sum = sinus + cosinus + leistung;
			flipflop = flipflop * -1;
			
			if(timestamp % 10000 == 0){
				toggle = 1 - toggle;
			}
			
			vTaskDelay(pdMS_TO_TICKS(1));
		}
}

void stackApplication(void* args){
    while(true){
			  UartDriver_run();
				ScopeFramedStack_runThreadStack(scopeStack);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
