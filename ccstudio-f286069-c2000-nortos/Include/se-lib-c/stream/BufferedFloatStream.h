/*!*****************************************************************************
 * @file         BufferedFloatStream.h
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
 * @brief        Implementation of the float stream interface.
 *               The buffered float stream uses a ring buffer as underlying data
 *               structure
 *
 ******************************************************************************/

#ifndef FLOATSTREAM_H_
#define FLOATSTREAM_H_

#include <se-lib-c/definition/SeLibCTypes.h>
#include <se-lib-c/stream/IFloatStream.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedFloatStreamPrivateData* BufferedFloatStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param capacity size of the buffer
 * @return
 */
BufferedFloatStreamHandle BufferedFloatStream_create(size_t capacity);

/**
 * Returns the stream interface
 * @param self
 * @return
 */
IFloatStreamHandle BufferedFloatStream_getIFloatStream(BufferedFloatStreamHandle self);

/**
 * Deconstructor
 * @param self
 */
void BufferedFloatStream_destroy(BufferedFloatStreamHandle self);

#endif
