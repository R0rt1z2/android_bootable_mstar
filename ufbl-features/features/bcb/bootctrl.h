/*
 * Copyright (C) 2016 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

/*
 * Use bootable/recovery/boot_control IBootControl instead of an amazon HAL,
 * portion of the code is taken from:
 *
 * recovery/bootloader_message/include/bootloader_message/bootloader_message.h
 *
 * specifically the struct bootloader_message_ab for bootloader use.
 *
 * misc partition:
 *   AOSP bootloader message                    0 - 2*1024
 *   vendor space bootloader control       2*1024 - 4*1024
 *     sizeof(struct bootloader_control)
 *
 * CAUTION: THE HAL BOOTCTRL HEADER MUST BE IN SYNC WITH THE UBOOT BOOTCTRL HEADER
 */

#ifndef _UFBL_BOOTCTRL_H_
#define _UFBL_BOOTCTRL_H_

#ifndef UFBL_BCB_BOOTCTRL_INTERNAL
#error "You should not need to include bootctrl.h, consider adding your use cases in ufbl bcb feature!"
#endif

#ifndef SUPPORT_UBOOT
#include <stdint.h>
#endif

#define VENDOR_SPACE_OFFSET_IN_MISC (2 * 1024)

#define BOOT_CTRL_MAGIC   0x42414342 /* Bootloader Control AB */
#define BOOT_CTRL_VERSION 1

struct slot_metadata {
    // Slot priority with 15 meaning highest priority, 1 lowest
    // priority and 0 the slot is unbootable.
    uint8_t priority : 4;
    // Number of times left attempting to boot this slot.
    uint8_t tries_remaining : 3;
    // 1 if this slot has booted successfully, 0 otherwise.
    uint8_t successful_boot : 1;
    // 1 if this slot is corrupted from a dm-verity corruption, 0
    // otherwise.
    uint8_t verity_corrupted : 1;
    // Reserved for further use.
    uint8_t reserved : 7;
} __attribute__((packed));

/* Bootloader Control AB
 *
 * This struct can be used to manage A/B metadata. It is designed to
 * be put in the 'slot_suffix' field of the 'bootloader_message'
 * structure described above. It is encouraged to use the
 * 'bootloader_control' structure to store the A/B metadata, but not
 * mandatory.
 */
struct bootloader_control {
    // NUL terminated active slot suffix.
    char slot_suffix[4];
    // Bootloader Control AB magic number (see BOOT_CTRL_MAGIC).
    uint32_t magic;
    // Version of struct being used (see BOOT_CTRL_VERSION).
    uint8_t version;
    // Number of slots being managed.
    uint8_t nb_slot : 3;
    // Number of times left attempting to boot recovery.
    uint8_t recovery_tries_remaining : 3;
    // Ensure 4-bytes alignment for slot_info field.
    uint8_t reserved0[2];
    // Per-slot information.  Up to 4 slots.
    struct slot_metadata slot_info[4];
    // Reserved for further use.
    uint8_t reserved1[8];
    // CRC32 of all 28 bytes preceding this field (little endian
    // format).
    uint32_t crc32_le;
} __attribute__((packed));


#endif /* _UFBL_BOOTCTRL_H_ */
