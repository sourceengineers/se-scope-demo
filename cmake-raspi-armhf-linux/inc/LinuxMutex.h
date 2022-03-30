/*!*****************************************************************************
 * @file         LinuxMutex.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implements an example of how to implement the IMutex interface
 *               for any linux system
 *
 ******************************************************************************/

#ifndef LINUXMUTEX_H
#define LINUXMUTEX_H

#include <se-lib-c/osal/IMutex.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __LinuxMutexPrivateData* LinuxMutexHandle;

/******************************************************************************
 Public functions
******************************************************************************/

LinuxMutexHandle LinuxMutex_create();

IMutexHandle LinuxMutex_getIMutex(LinuxMutexHandle self);

void LinuxMutex_destroy(LinuxMutexHandle self);

#endif
