/*!****************************************************************************************************************************************
 * @file         IByteStream.h
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Interface for an byte stream.
 *
 *****************************************************************************************************************************************/

#ifndef IBYTESTREAM_H_
#define IBYTESTREAM_H_

#include <se-lib-c/definition/SeLibCTypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IByteStreamStruct* IByteStreamHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IByteStreamStruct{
    SeLibGenericReferece handle;

    /**
     * Checks if data is pending
     * @param self
     * @return True if data is pending, False if not
     */
    bool (* byteIsReady)(IByteStreamHandle self);

    /**
     * Returns the oldest data point
     * @param self
     * @return
     */
    uint8_t (* readByte)(IByteStreamHandle self);

    /**
     * Returns the amount of data in the stream
     * @param self
     * @return
     */
    size_t (* length)(IByteStreamHandle self);

    /**
     * Reads data in the stream
     * @param self
     * @param data Array into which the data has to be written
     * @param length Max size of the data array
     */
    void (* read)(IByteStreamHandle self, uint8_t* data, const size_t length);

    /**
     * Write one single data point into the stream
     * @param self
     * @param data
     */
    bool (* writeByte)(IByteStreamHandle self, const uint8_t data);

    /**
     * Writes multiple data points into the stream
     * @param self
     * @param data Data that has to be written into the stream
     * @param length Amount of data that has to be written into the stream
     */
    bool (* write)(IByteStreamHandle self, const uint8_t* data, const size_t length);

    /**
     * Flushes the stream
     * @param self
     */
    void (* flush)(IByteStreamHandle self);

    /**
     * Returns the maximum amount of data that can be saved in the stream
     * @param self
     */
    size_t (* capacity)(IByteStreamHandle self);


    /**
     * Returns the amount of free data in tue buffer
     * @param self
     */
    size_t (* numOfFreeBytes)(IByteStreamHandle self);

    /**
     * Returns the amount of used data in tue buffer
     * @param self
     */
    size_t (* numOfUsedBytes)(IByteStreamHandle self);

} IByteStream;

#endif
