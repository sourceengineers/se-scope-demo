/*!****************************************************************************************************************************************
 * @file         Types.h
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
 * @brief        Defines the data types that can be used by the scope.
 *               It specifies the size of addresses by defines which can be controlled by setting
 *               the ARCH_SIZE_32. ARCH_SIZE_32=1 equals a 32 bit architecture and
 *               ARCH_SIZE_32=0, or not setting ARCH_SIZE_32 at all, defined a 64 bit architecture.
 *               This is necessary to be able to read from the correct address, and that the channels can be configured
 *               properly
 *
 *****************************************************************************************************************************************/

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * Enum representing the data types
 */
typedef enum  __DATA_TYPES {
    SE_INT8,
    SE_INT16,
    SE_INT32,
    SE_UINT8,
    SE_UINT16,
    SE_UINT32,
    SE_FLOAT
} DATA_TYPES ;

#ifdef SCOPE_TARGET_TI_SUPPORT
    typedef uint16_t uint8_t;
    typedef int16_t int8_t;
#endif

/* Choose the right integer length */
#ifndef SCOPE_TARGET_64BIT_SUPPORT
#define ADDRESS_DATA_TYPE uint32_t
#else
#define ADDRESS_DATA_TYPE uint64_t
#endif

#define SeScopeGenericReference void*

#endif
