/*!*****************************************************************************
 * @file         ITransceiver.h
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
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Supplies an Interface to access the communication interfaces
 *
 ******************************************************************************/

#ifndef ITRANSCEIVER_H_
#define ITRANSCEIVER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ITransceiverStruct* ITransceiverHandle;

/**
 * Prototype of function that will be used as a callback
 */
typedef void(*TransmitCallback)(ITransceiverHandle self);

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ITransceiverStruct{
    SeScopeGenericReference handle;

    /**
     * Read data from the output stream
     * @param self
     * @param data
     * @param length
     * @return
     */
    void (* get)(ITransceiverHandle self,  uint8_t* data, size_t length);

    /**
     * Put data into the input stream so it can be interpreted
     * @param self
     * @param data
     * @param length
     */
    void (* put)(ITransceiverHandle self, uint8_t* data, size_t length);

    /**
     * Reset the input handler
     * @param self
     */
    void (* resetInput)(ITransceiverHandle self);

    /**
     * Reset the output handler
     * @param self
     */
    void (* resetOutput)(ITransceiverHandle self);

    /**
     * Bytes pending in the output buffer
     * @param self
     * @return
     */
    size_t (* outputSize)(ITransceiverHandle self);

} ITransceiver;

#endif
