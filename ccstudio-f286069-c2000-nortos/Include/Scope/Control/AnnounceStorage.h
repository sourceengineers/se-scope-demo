/*!*****************************************************************************
 * @file         AnnounceStorage.h
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
 * @brief        Implements a storage module through which addresses can be named
 *               and transmitted.
 *               This allows the usage of the scope, even for non static addresses.
 *
 ******************************************************************************/

#ifndef ANNOUNCESTORAGE_H_
#define ANNOUNCESTORAGE_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_LENGTH_OF_ANNOUNCE_NAME 30 // Max length of an address name
/**
 * Definitions for the AnnounceStorage
 */
typedef struct{
    char name[MAX_LENGTH_OF_ANNOUNCE_NAME]; // Name of an address
    DATA_TYPES type; // Type of the address
    ADDRESS_DATA_TYPE address; // The address
} AddressDefinition;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __AnnounceStoragePrivateData* AnnounceStorageHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param maxAmountOfAddresses the maximal amount of addresses which can be stored in the storage
 * @param timeBase Sets the time base with which will be polled. This depends on how the scope is implemented
 * @return
 */
AnnounceStorageHandle AnnounceStorage_create(const size_t maxAmountOfConfiguredAddresses,
        const size_t maxAmountOfChannels, const float timeBase);

/**
 * Sets a new definition of a address. If the addressId is bigger than the maximum amount of elements,
 * the function will return without doing anything
 * @param self
 * @param name name of the address
 * @param address address
 * @param type type of the address
 * @param addressId position into which the address should be written
 */
bool AnnounceStorage_addAnnounceAddress(AnnounceStorageHandle self, const char* name, const void* address,
                                       const DATA_TYPES type);

/**
 * Returns the max amount of channels that can be configured in the system. This is used to let the host know
 * that no more than maxAmountOfChannels will be accepted.
 * @param self
 * @return
 */
size_t AnnounceStorage_getMaxAmountOfChannels(AnnounceStorageHandle self);

/**
 * Returns the timebase of the system
 * @param self
 * @return
 */
float AnnounceStorage_getTimeBase(AnnounceStorageHandle self);

/**
 * Get the version of the se scope
 * @param version
 */
char* AnnounceStorage_getVersion(AnnounceStorageHandle self);

/**
 * Updates the packObserver, triggering a pending event in the packer
 * @param self
 */
void AnnounceStorage_announce(AnnounceStorageHandle self);

/**
 * Maximal amount of addresses that can be stored in the storage
 * @param self
 * @return
 */
size_t AnnounceStorage_getMaxAmountOfAddresses(AnnounceStorageHandle self);

/**
 * Returns the address stored in id.
 * @param self
 * @param addressId
 * @return Yiels NULL if id > AnnounceStorage_getMaxAmountOfAddresses
 */
AddressDefinition* AnnounceStorage_getAddressToTransmit(AnnounceStorageHandle self, const uint32_t addressId);


/**
 * Returns the amount of configured addresses
 * @param self
 * @return
 */
size_t AnnounceStorage_getAmountOfConfiguredAddresses(AnnounceStorageHandle self);

/**
 * Deconstructor
 * @param self
 */
void AnnounceStorage_destroy(AnnounceStorageHandle self);

#endif
