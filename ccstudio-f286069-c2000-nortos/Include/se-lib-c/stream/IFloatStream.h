/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
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
 * @brief        Interface for an float stream.
 *
 *****************************************************************************************************************************************/

#ifndef IFLOATSTREAM_H_
#define IFLOATSTREAM_H_

#include <se-lib-c/definition/SeLibCTypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IFloatStreamStruct* IFloatStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IFloatStreamStruct{
    SeLibGenericReferece handle;

    /**
     * Checks if data is pending
     * @param self
     * @return True if data is pending, False if not
     */
    bool (* dataIsReady)(IFloatStreamHandle self);

    /**
    * Returns the oldest data point
    * @param self
    * @return
    */
    float (* readData)(IFloatStreamHandle self);

    /**
     * Returns the amount of data in the stream
     * @param self
     * @return
     */
    size_t (* length)(IFloatStreamHandle self);

    /**
     * Reads data in the stream
     * @param self
     * @param data Array into which the data has to be written
     * @param length Max size of the data array
     */
    void (* read)(IFloatStreamHandle self, float* data, const size_t length);

    /**
     * Write one single data point into the stream
     * @param self
     * @param data
     */
    void (* writeData)(IFloatStreamHandle self, const float data);

    /**
     * Writes multiple data points into the stream
     * @param self
     * @param data Data that has to be written into the stream
     * @param length Amount of data that has to be written into the stream
     */
    void (* write)(IFloatStreamHandle self, const float* data, const size_t length);

    /**
     * Flushes the stream
     * @param self
     */
    void (* flush)(IFloatStreamHandle self);

    /**
     * Returns the maximum amount of data that can be safed in the stream
     * @param self
     */
    size_t (* capacity)(IFloatStreamHandle self);

} IFloatStream;

#endif
