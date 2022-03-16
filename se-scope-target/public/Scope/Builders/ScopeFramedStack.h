/*!*****************************************************************************
 * @file         ScopeFramedStack.h
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Builds a Framed Json stack.
 *
 ******************************************************************************/

#ifndef SCOPEFRAMEDSTACK_H_
#define SCOPEFRAMEDSTACK_H_

#include "Scope/Control/AnnounceStorage.h"
#include <se-lib-c/osal/IMutex.h>
#include "Scope/Communication/ITransceiver.h"
#include "Scope/Communication/MessagePriorities.h"
#include <se-lib-c/stream/IByteStream.h>
#include <se-lib-c/osal/IMutex.h>
/******************************************************************************
 Configuration interface for the ScopeFramedStack
******************************************************************************/

/**
 * Configuration struct for the scope
 */
typedef struct __ScopeFramedStackConfig {
    size_t sizeOfChannels;                  // Size of the channels
    size_t  amountOfChannels;
    TransmitCallback callback;          // Callback to the UsartTransmit method defined by the user
    uint32_t* timestamp;                    // Timestamp as base for the scope. This has to be incremented by the user.
    size_t  addressesInAddressAnnouncer;    // Amount of addresses that can be stored to be transmitted
    float timebase;                         // Timebase of the system
} ScopeFramedStackConfig;

/**
 * If the scope is run in a mutli threaded context, two mutexes have to be provided to allow the scope to
 * protect itself from race conditions
 */
typedef struct __ScopeFramedStackMutex {
    IMutexHandle configMutex;
    IMutexHandle dataMutex;
    IMutexHandle logBufferMutex;
} ScopeFramedStackMutex;


/**
 * Settings for the logger of the scope. If it is empty,
 * it will not be logging
 */
typedef struct __ScopeFramedStackLogOptions {
	IByteStreamHandle logByteStream;
} ScopeFramedStackLogOptions;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeFramedStackPrivateData* ScopeFramedStackHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 *
 * @param config
 * @return
 */
ScopeFramedStackHandle ScopeFramedStack_create(
		ScopeFramedStackConfig scopeConfig,
		ScopeFramedStackLogOptions logConfig,
        Message_Priorities priorities);

/**
 * Constructor. Creates a static instance of the stack and scope. If the stack is created with this function, it can be used in a
 * multithreaded environment.
 * @param config
 * @param mutexes
 */
ScopeFramedStackHandle ScopeFramedStack_createThreadSafe(
		ScopeFramedStackConfig scopeConfig,
		ScopeFramedStackMutex mutexes,
		ScopeFramedStackLogOptions logConfig,
        Message_Priorities priorities);

/**
 * Runs the JsonUart stack
 * @param self
 */
void ScopeFramedStack_run(ScopeFramedStackHandle self);

/**
 * Runs the scope in a thread safe environment.
 * @param self
 */
void ScopeFramedStack_runThreadScope(ScopeFramedStackHandle self);

/**
 * Runs the stack in a thread safe environment.
 * @param self
 */
void ScopeFramedStack_runThreadStack(ScopeFramedStackHandle self);

/**
 * Returns the handle to the announcement storage.
 * @return
 * @param self
 */
AnnounceStorageHandle ScopeFramedStack_getAnnounceStorage(ScopeFramedStackHandle self);

/**
 * Returns the transceiver interface
 * @return
 * @param self
 */
ITransceiver* ScopeFramedStack_getTranscevier(ScopeFramedStackHandle self);

/**
 * Deconstructor
 * @param self
 */
void ScopeFramedStack_destroy(ScopeFramedStackHandle self);
#endif //SCOPEFRAMEDSTACK_H_
