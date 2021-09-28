/*
 * Copyright (C) 2016 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#ifndef FEATURES_BCB_BCB_PLATFORM_H_
#define FEATURES_BCB_BCB_PLATFORM_H_

/*
 * Interfaces for platform to implement bootloader control read/write to storage.
 *
 * Must use misc partition same as platform/bootable/recovery/boot_control bootctrl.bcb module.
 * It's defined as:
 * ------------ misc partition ---------
 * bootloader message        0 -  2*1024 (for recovery command)
 * vendor space         2*1024 - 16*1024 (for vendor specific command)
 * wipe package        16*1024 - 64*1024 (for wipe/kill package, secure erase partitions in TARGET_RECOVERY_WIPE)
 * -------------------------------------
 *
 * @param buf pointer to input buffer start address.
 * @param offset offset in the misc partition to avoid overlapping non-vendor space data.
 * @param size length of the input buffer.
 * @return: true on success, false when error.
 */
bool bcb_platform_read(void *buf, size_t offset, size_t size);
bool bcb_platform_write(void *buf, size_t offset, size_t size);

#endif /* FEATURES_BCB_BCB_PLATFORM_H_ */
