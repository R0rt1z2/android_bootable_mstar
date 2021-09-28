/*
 * Copyright (C) 2018 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#ifndef AMZN_MTK_IMPL_UNLOCK_H
#define AMZN_MTK_IMPL_UNLOCK_H

void cmd_oem_relock(const char *arg, void *data, unsigned sz);

// Use the below call to publish the unlock_code and unlock status.
// We need to own the unlock code buffer and unlock status buffer,
// since we need to change the unlock code version during relock, and change the
// unlock status after flashing valid unlock signature or relock.
// Here we call this function to pulish we owned buffer to fastboot varibles.
void amzn_fastboot_publish_unlock_vars(void);

// Check the unlock status and update the we-owned unlock status buffer,
// which eventually will be published to "unlock_status" in
// amzn_fastboot_publish_unlock_vars.
// return 1 means device is unlocked,
// return 0 means device is NOT unlocked
int amzn_check_unlock_status(void);
void amzn_flash_unlock_signature(const unsigned char *sig,
                                 unsigned int sig_len);

#if defined(UFBL_FEATURE_TEMP_UNLOCK)
#include <amzn_temp_unlock.h>
int amzn_flash_temp_unlock_code_or_cert_maybe(const unsigned char *arg,
                                              void *data, unsigned sz);
#define AMZN_TEMP_UNLOCK_HMAC_BUFFER_SIZE ((TEMP_UNLOCK_HMAC_HASH_SIZE)*(CFG_AMZN_TEMP_UNLOCK_REBOOT_CNT))
struct boot_tag_temp_unlock {
    unsigned int temp_unlock_reboot_cnt;
    unsigned char temp_unlock_hmac[AMZN_TEMP_UNLOCK_HMAC_BUFFER_SIZE];
} __attribute__ ((aligned(4)));

void amzn_save_temp_unlock_data(struct boot_tag_temp_unlock *data_input);
int amzn_get_temp_unlock_codes(unsigned char **codes, unsigned int *reboot_cnt);
int amzn_get_temp_unlock_root_pubkey(unsigned char **key, unsigned int *key_len);
#endif // defined(UFBL_FEATURE_TEMP_UNLOCK)

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#define RANDOM_BYTES_SIZE (ONETIME_UNLOCK_CODE_LEN + 3) / 4 * 3
#define ENTROPY_LEN (24)
int amzn_flash_onetime_unlock_code_or_cert_maybe(const unsigned char *arg,
                                              void *data, unsigned sz);

#endif // defined(UFBL_FEATURE_ONETIME_UNLOCK)
#endif
