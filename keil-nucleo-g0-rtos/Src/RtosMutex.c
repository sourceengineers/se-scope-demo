
#include "RtosMutex.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __RtosMutexPrivateData{

    IMutex mutex;
    SemaphoreHandle_t rtosMutex;

} RtosMutexPrivateData;

bool lock(IMutexHandle imutex, uint32_t timeout);
void unlock(IMutexHandle imutex);

/******************************************************************************
 Private functions
******************************************************************************/
bool lock(IMutexHandle imutex, uint32_t timeout){
    RtosMutexHandle self = (RtosMutexHandle) imutex->handle;
    bool ret = xSemaphoreTake(self->rtosMutex, pdMS_TO_TICKS(timeout)) == pdTRUE ? true : false;
    return ret;
}

void unlock(IMutexHandle imutex){
    RtosMutexHandle self = (RtosMutexHandle) imutex->handle;
    xSemaphoreGive(self->rtosMutex);
}
/******************************************************************************
 Public functions
******************************************************************************/

RtosMutexHandle RtosMutex_create(){

    RtosMutexHandle self = (RtosMutexHandle) malloc(sizeof(RtosMutexPrivateData));

    self->mutex.handle = self;
    self->mutex.lock = &lock;
    self->mutex.unlock = &unlock;
    self->rtosMutex = xSemaphoreCreateMutex();

    return self;
}

IMutexHandle RtosMutex_getIMutex(RtosMutexHandle self){
    return &self->mutex;
}

void RtosMutex_destroy(RtosMutexHandle self){
    vSemaphoreDelete(&self->rtosMutex);
    free(self);
    self = NULL;
}
