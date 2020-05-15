/*!*****************************************************************************
 * @file         IMutex.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license This file is part of "se-scope-target". It is release under a commercial 
 *          license. Refer to the license PDF file received together with this code 
 *          for the exact licensing terms. 
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Supplies an Interface which has to be implemented to be able to synchronise
 *               tasks. Implementing this Interface, enables to run the scope as a multi task
 *               application on various operating systems such as RTOS.
 *
 ******************************************************************************/

#ifndef IMUTEX_H
#define IMUTEX_H

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IMutexStruct* IMutexHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IMutexStruct{
    SeScopeGenericReference handle;

    /**
     * Implement lock for the specific operating system mutex lock operation.
     * Locks the mutex
     * @param self
     * @return
     */
    bool (*lock)(IMutexHandle self, uint32_t timeout);

    /**
     * Implement unlock for the specific operating system mutex unlock operation.
     * Unlocks the mutex
     * @param self
     */
    void (*unlock)(IMutexHandle self);

} IMutex;

#endif
