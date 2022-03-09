/*!*****************************************************************************
 * @file         IMutex.h
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-lib-c.
 *
 *               se-lib-c is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-lib-c is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-lib-c.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Supplies an Interface which has to be implemented to be able to synchronise
 *               tasks. Implementing this Interface, enables to run the scope as a multi task
 *               application on various operating systems such as RTOS.
 *
 ******************************************************************************/

#ifndef IMUTEX_H
#define IMUTEX_H

#include <se-lib-c/definition/SeLibCTypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IMutexStruct* IMutexHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IMutexStruct{
    SeLibGenericReferece handle;

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

#endif //IMUTEX_H
