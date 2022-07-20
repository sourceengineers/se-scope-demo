/*!****************************************************************************************************************************************
 * @file         LinuxMutex.cpp
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <LinuxMutex.h>
#include <mutex>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __LinuxMutexPrivateData{

    IMutex mutex;
    std::mutex linuxMutex;

} LinuxMutexPrivateData;

bool lock(IMutexHandle imutex, uint32_t timeout);
void unlock(IMutexHandle imutex);

/******************************************************************************
 Private functions
******************************************************************************/
bool lock(IMutexHandle imutex, uint32_t timeout){
    auto self = (LinuxMutexHandle) imutex->handle;
    return self->linuxMutex.try_lock();
}

void unlock(IMutexHandle imutex){
    auto self = (LinuxMutexHandle) imutex->handle;
    self->linuxMutex.unlock();
}
/******************************************************************************
 Public functions
******************************************************************************/

LinuxMutexHandle LinuxMutex_create(){

    auto self = (LinuxMutexHandle) malloc(sizeof(LinuxMutexPrivateData));

    self->mutex.handle = self;
    self->mutex.lock = &lock;
    self->mutex.unlock = &unlock;

    return self;
}

IMutexHandle LinuxMutex_getIMutex(LinuxMutexHandle self){
    return &self->mutex;
}

void LinuxMutex_destroy(LinuxMutexHandle self){
    free(self);
    self = NULL;
}
