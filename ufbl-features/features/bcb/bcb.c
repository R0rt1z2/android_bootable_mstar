/*
 * Copyright (C) 2016 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#if !defined(SUPPORT_UBOOT) && !defined(SUPPORT_MTK_PRELOADER)
# include <string.h>
# include <debug.h>
#else
# include "ufbl_debug.h"
#endif
#include "bcb.h"
#include "bcb_platform.h"
#define UFBL_BCB_BOOTCTRL_INTERNAL
#include "bootctrl.h"

#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef cpu_to_le32
# define cpu_to_le32(x)         ((uint32_t)(x))
#endif

#ifdef UFBL_TESTS
# define VISIBLE_FOR_TESTING
#else
# define VISIBLE_FOR_TESTING    static
#endif

#define BCB_MAX_SLOT_PRIORITY 15
#define BCB_MAX_RETRY_LIMIT   3
#define BCB_DEFAULT_ACTIVE_SLOT_PRIORITY BCB_MAX_SLOT_PRIORITY
#define BCB_DEFAULT_OTHER_SLOT_PRIORITY (BCB_MAX_SLOT_PRIORITY - 1)

static const char *SLOT_SUFFIXES[] = { "_a", "_b" };
static const int MAX_SLOT_SUFFIXES = sizeof(SLOT_SUFFIXES) / sizeof(SLOT_SUFFIXES[0]);

static bool retry(bool (*op)(void *buf, size_t offset, size_t size), void *buf, size_t size)
{
    int retry = BCB_MAX_RETRY_LIMIT;
    while (retry--) {
        if (op(buf, VENDOR_SPACE_OFFSET_IN_MISC, size)) {
            return true;
        }
    }
    return false;
}

static uint32_t CRC32(const uint8_t* buf, size_t size)
{
    static uint32_t crc_table[256] = {0};
    uint32_t i, j, ret;

    if (!crc_table[1]) {
        for (i = 0; i < 256; ++i) {
            uint32_t crc = i;
            for (j = 0; j < 8; ++j) {
                uint32_t mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
            crc_table[i] = crc;
        }
    }

    ret = -1;
    for (i = 0; i < size; ++i) {
        ret = (ret >> 8) ^ crc_table[(ret ^ buf[i]) & 0xFF];
    }

    return ~ret;
}

VISIBLE_FOR_TESTING uint32_t bootloader_control_crc32(const void *bc)
{
    return cpu_to_le32(CRC32(bc, offsetof(struct bootloader_control, crc32_le)));
}

VISIBLE_FOR_TESTING void create_default_metadata(struct bootloader_control *bc, int default_active_slot)
{
    int i;

    memset(bc, 0, sizeof(*bc));

    strcpy(bc->slot_suffix, SLOT_SUFFIXES[default_active_slot]);
    bc->magic = BOOT_CTRL_MAGIC;
    bc->version = BOOT_CTRL_VERSION;
    bc->nb_slot = MAX_SLOT_SUFFIXES;
    bc->recovery_tries_remaining = 0;

    for (i = 0; i < MAX_SLOT_SUFFIXES; i++) {
        struct slot_metadata *slot = &bc->slot_info[i];
        if (default_active_slot == i) {
            slot->priority = BCB_DEFAULT_ACTIVE_SLOT_PRIORITY;
        } else {
            slot->priority = BCB_DEFAULT_OTHER_SLOT_PRIORITY;
        }
        slot->tries_remaining = BCB_MAX_RETRY_LIMIT;
        slot->successful_boot = 0;
        slot->verity_corrupted = 0;
    }
    bc->crc32_le = bootloader_control_crc32(bc);
}

// example output:
// banner 5F 61 00 00 42 43 41 42 01 02 00 00 3F 00 3E 00 00 00 00 00 00 00 00 00 00 00 00 00 5A 0F D7 C0
VISIBLE_FOR_TESTING void hex_dump_bootloader_control(int log_level, const char *banner,
                                                     const struct bootloader_control *bc)
{
    const unsigned char *p = (const unsigned char *)bc;
    size_t i;

    dprintf(log_level, "%s", banner);
    for (i = 0; i < sizeof(*bc); i++, p++) {
        dprintf(log_level, " %02X", *p);
    }
    dprintf(log_level, "\n");
}

VISIBLE_FOR_TESTING void print_bootloader_control(int log_level, const struct bootloader_control *bc)
{
    int i;

    dprintf(log_level, "struct bootloader_control\n");
    dprintf(log_level, "  .magic: 0x%x\n", bc->magic);
    dprintf(log_level, "  .version: %d\n", bc->version);
    dprintf(log_level, "  .nb_slot: %d\n", bc->nb_slot);
    dprintf(log_level, "  .recovery_tries_remaining: %d\n", bc->recovery_tries_remaining);
    dprintf(log_level, "  .crc32_le: %x\n", bc->crc32_le);
    for (i = 0; i < MAX_SLOT_SUFFIXES; i++) {
        const struct slot_metadata *slot = &bc->slot_info[i];
        dprintf(log_level, "  .slot_info[%d].priority: %d\n", i, slot->priority);
        dprintf(log_level, "  .slot_info[%d].tries_remaining: %d\n", i, slot->tries_remaining);
        dprintf(log_level, "  .slot_info[%d].successful_boot: %d\n", i, slot->successful_boot);
        dprintf(log_level, "  .slot_info[%d].verity_corrupted: %d\n", i, slot->verity_corrupted);
    }
}

/*
 * @return negative integer on error, or active slot number equal to or greater than 0.
 */
VISIBLE_FOR_TESTING int get_active_slot(const struct bootloader_control *bc)
{
    int i, active_slot = -1, max_priority = 0;

    if (bc->magic != BOOT_CTRL_MAGIC) return -1;
    if (bc->crc32_le != bootloader_control_crc32(bc)) return -1;

    for (i = 0; i < MAX_SLOT_SUFFIXES; i++) {
        const struct slot_metadata *slot = &bc->slot_info[i];
        if (slot->priority > max_priority) {
            active_slot = i;
            max_priority = slot->priority;
        }
    }

    return active_slot;
}

VISIBLE_FOR_TESTING int fill_info(void *ctx, struct bootloader_control *bc, int active_slot)
{
    struct ufbl_bcb *out = ctx;
    int i;

    memset(out, 0, sizeof(*out));
    if (MAX_SLOT_SUFFIXES > bc->nb_slot) {
        out->slot_count = bc->nb_slot;
    } else {
        out->slot_count = MAX_SLOT_SUFFIXES;
    }
    out->current_slot = active_slot;
    for (i = 0; i < bc->nb_slot; i++) {
        out->slot_info[i].tries_remaining = bc->slot_info[i].tries_remaining;
        out->slot_info[i].successful = bc->slot_info[i].successful_boot;
        out->slot_info[i].unbootable = (bc->slot_info[i].priority == 0 && bc->slot_info[i].successful_boot == 0);
        out->slot_info[i].suffix = SLOT_SUFFIXES[i][1];
    }

    return active_slot;
}

VISIBLE_FOR_TESTING int decrement_retry(void *ctx, struct bootloader_control *bc, int active_slot)
{
    struct slot_metadata *slot = &bc->slot_info[active_slot];

    hex_dump_bootloader_control(INFO, "read:", bc);
    print_bootloader_control(CRITICAL, bc);
    if (slot->successful_boot == 0) {
        if (slot->tries_remaining > 0) {
            slot->tries_remaining--;
        } else {
            // run out of retry count, mark as invalid (successful_boot = priority = 0)
            slot->priority = 0;
        }

        active_slot = get_active_slot(bc);
        if (active_slot >= 0) {
            hex_dump_bootloader_control(CRITICAL, "write bcb:", bc);
            if (retry(bcb_platform_write, bc, sizeof(*bc))) {
                if (retry(bcb_platform_read, bc, sizeof(*bc))) {
                    hex_dump_bootloader_control(SPEW, "read back bcb:", bc);
                }
            }
        }
    }
    return active_slot;
}

ufbl_bcb_slot_policy_t UFBL_BCB_DECREMENT_RETRY_POLICY = decrement_retry;

VISIBLE_FOR_TESTING int is_valid_slot(int slot)
{
    return slot >= 0 && slot < MAX_SLOT_SUFFIXES;
}

VISIBLE_FOR_TESTING int read_bcb_and_apply_policy(ufbl_bcb_slot_policy_t policy, void *context)
{
    struct bootloader_control bc;
    int active_slot;

    if (!retry(bcb_platform_read, &bc, sizeof(bc))) {
        return -1;
    }

    active_slot = get_active_slot(&bc);
    if (is_valid_slot(active_slot) && policy != NULL) {
        active_slot = policy(context, &bc, active_slot);
    }
    return active_slot;
}

const char *ufbl_bcb_get_active_slot_suffix(ufbl_bcb_slot_policy_t policy)
{
    int active_slot = read_bcb_and_apply_policy(policy, NULL);
    if (is_valid_slot(active_slot)) {
        return SLOT_SUFFIXES[active_slot];
    }
    return NULL;
}

int ufbl_bcb_set_active_slot(int slot)
{
    if (is_valid_slot(slot)) {
        struct bootloader_control bc;
        create_default_metadata(&bc, slot);
        if (retry(bcb_platform_write, &bc, sizeof(bc))) {
            return 0;
        }
    }
    return -1;
}

int ufbl_bcb_get_info(struct ufbl_bcb *out)
{
    if (!out) {
        return -1;
    }
    if (is_valid_slot(read_bcb_and_apply_policy(fill_info, out))) {
        return 0;
    }
    return -1;
}
