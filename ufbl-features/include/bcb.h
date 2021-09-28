/*
 * Copyright (C) 2016 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#ifndef __UFBL_BOOTLOADER_CONTROL_H
#define __UFBL_BOOTLOADER_CONTROL_H

struct bootloader_control;
typedef int (*ufbl_bcb_slot_policy_t)(void *context, struct bootloader_control *, int active_slot);

/**
 * Decrement retry count each boot in bootloader control block as described in
 * <a href="https://source.android.com/devices/bootloader/flashing-updating#a-b-updates">
 * Bootloader slotting flow</a>.
 */
extern ufbl_bcb_slot_policy_t UFBL_BCB_DECREMENT_RETRY_POLICY;

/**
 * Get the current active slot suffix "_a" or "_b", or NULL when error.
 *
 * Caller should handle error by hanging the system for debugging, or assign an active slot by
 * using ufbl_set_active_slot_suffix(slot).
 *
 * @param policy apply specific policy to get the next active slot.  This can be
 *               NULL to get the current active slot or use predefined polices.
 *               @see UFBL_BCB_DECREMENT_RETRY_POLICY.
 * @return slot suffix on success, NULL on error.
 * @see <a href="https://source.android.com/devices/bootloader/flashing-updating#a-b-updates">AB bootloader updates</a>
 */
const char *ufbl_bcb_get_active_slot_suffix(ufbl_bcb_slot_policy_t policy);


/**
 * Set active slot to slot, for implementing: fastboot set_active slot.
 *
 * @return 0 on success, -1 on error.
 */
int ufbl_bcb_set_active_slot(int slot);


#define UFBL_BCB_MAX_SLOT_INFO 4
struct ufbl_bcb {
    int slot_count;
    int current_slot;
    struct ufbl_slot_info {
        int tries_remaining;
        bool successful;
        bool unbootable;
        char suffix; // 'a' or 'b'
    } slot_info[UFBL_BCB_MAX_SLOT_INFO];
};

/**
 * Get bootloader control info for implementing: fastboot getvar.
 *
 * Examples of related fastboot variables:
 * <pre>
 *   has-slot:boot: yes
 *   has-slot:system: yes
 *   slot-count: 2
 *   slot-successful:a: yes
 *   slot-successful:b: no
 *   slot-unbootable:a: no
 *   slot-unbootable:b: yes
 *   slot-retry-count:a: 3
 *   slot-retry-count:b: 3
 *   current-slot: a
 * </pre>
 *
 * @param out fill struct ufbl_bcb in given buffer.
 * @return 0 on success, otherwise failure.
 * @see <a href="https://source.android.com/devices/tech/ota/ab/ab_implement#kernel-slot-arguments">ab implementation</a>
 * @see <a href="https://android.googlesource.com/platform/system/extras/+/refs/heads/master/tests/bootloader/bootloadertest.py">bootloadertest.py</a>
 */
int ufbl_bcb_get_info(struct ufbl_bcb *out);


#endif // __UFBL_BOOTLOADER_CONTROL_H

