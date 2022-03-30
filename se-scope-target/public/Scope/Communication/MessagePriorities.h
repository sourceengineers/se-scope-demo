/*!*****************************************************************************
 * @file         MessagePriorities.h
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
 * @brief        Supplies an Interface to access the communication interfaces
 *
 ******************************************************************************/

#ifndef SE_SCOPE_MESSAGEPRIORITIES_H
#define SE_SCOPE_MESSAGEPRIORITIES_H

typedef enum __Message_Priority {
    HIGH = 2,
    MEDIUM = 3,
    LOW = 4,
} Message_Priority;

typedef struct __Message_Priorities {
    Message_Priority data;
    Message_Priority log;
    Message_Priority stream;
} Message_Priorities;

#endif //SE_SCOPE_MESSAGEPRIORITIES_H
