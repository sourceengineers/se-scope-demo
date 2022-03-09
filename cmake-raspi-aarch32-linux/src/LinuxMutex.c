/*!****************************************************************************************************************************************
 * @file         LinuxMutex.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "../inc/LinuxMutex.h"
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __LinuxMutexPrivateData{

    IMutex mutex;
    pthread_mutex_t linuxMutex;

} LinuxMutexPrivateData;

bool lock(IMutexHandle imutex, uint32_t timeout);
void unlock(IMutexHandle imutex);

/******************************************************************************
 Private functions
******************************************************************************/
bool lock(IMutexHandle imutex, uint32_t timeout){
    LinuxMutexHandle self = (LinuxMutexHandle) imutex->handle;
    bool ret = pthread_mutex_trylock(&self->linuxMutex) == 0 ? true : false;
    return ret;
}

void unlock(IMutexHandle imutex){
    LinuxMutexHandle self = (LinuxMutexHandle) imutex->handle;
    pthread_mutex_unlock(&self->linuxMutex);
}
/******************************************************************************
 Public functions
******************************************************************************/

LinuxMutexHandle LinuxMutex_create(){

    LinuxMutexHandle self = (LinuxMutexHandle) malloc(sizeof(LinuxMutexPrivateData));

    self->mutex.handle = self;
    self->mutex.lock = &lock;
    self->mutex.unlock = &unlock;
    pthread_mutex_init(&self->linuxMutex, NULL);

    return self;
}

IMutexHandle LinuxMutex_getIMutex(LinuxMutexHandle self){
    return &self->mutex;
}

void LinuxMutex_destroy(LinuxMutexHandle self){
    pthread_mutex_destroy(&self->linuxMutex);
    free(self);
    self = NULL;
}