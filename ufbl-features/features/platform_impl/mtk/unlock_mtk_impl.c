/*
 * Copyright (C) 2018 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#include <string.h>
#include <stdio.h>
#include <amzn_unlock.h>
#include <unlock_mtk_impl.h>
#include <platform/mt_typedefs.h>
#include <fastboot.h>

#if defined(UFBL_FEATURE_TEMP_UNLOCK)
#include <amzn_temp_unlock.h>
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#endif

extern ulong get_timer(ulong base);
extern u32 get_devinfo_with_index(u32 index);

enum {
    UNLOCK_CODE_STR_REP_LEN = (8 + 8 + 8 + 2),
    UNLOCK_CODE_VERSION_LEN = (8),
};

#define ___FEATURE_MTK_IMPL_JOIN(x, y) x##y
#define __FEATURE_MTK_IMPL_JOIN(x, y) ___FEATURE_MTK_IMPL_JOIN(x, y)

static char unlock_code_buffer[UNLOCK_CODE_STR_REP_LEN + 1] __attribute__((aligned(4)));
static char unlock_status_buffer[16] __attribute__((aligned(4)));  // "true" or "false"

static u32 rand_u32() {
    u32 rc = 0;
    do {
        rc = (u32)((u32)(get_timer(0))*49537+17);
    } while (rc == 0 || rc == 0x00000030);  // 0 or '0'
    return rc;
}
// The caller should make sure the buffer is long enough
static int make_unlock_code(char *buffer, u32 unlock_version)
{
    return sprintf(buffer, "0x%08x%08x%08x",
                   get_devinfo_with_index(13),
                   get_devinfo_with_index(12),
                   unlock_version);
}

// The caller should make sure the buffer is long enough
static int make_entropy(char *buffer)
{
    /**
     * Different SoC/Product may have different scheme to add entropy into PRNG.
     * For mtk, take get_timer, SoC ID (devinfo 13) and hardware ID (devinfo 12) as entropy
     * and each will contribute 8 bytes
     */
    return sprintf(buffer, "%08x%08x%08x",
                    get_timer(0),
                    get_devinfo_with_index(13),
                    get_devinfo_with_index(12));
}

static u32 get_version_may_update()
{
    char unlock_version[UNLOCK_CODE_VERSION_LEN] __attribute__((aligned(4)));
    if (idme_get_var_external("unlock_version", unlock_version,
                              sizeof(unlock_version))) {
        // no unlock_version field.
        return 0x00000000;
    }
    u32 version = *(u32 *)(unlock_version);
    if (version == 0 || version == 0x00000030) {  // 0 or '0'
        version = rand_u32();
        *(u32 *)(unlock_version) = version;
        if (idme_update_var_ex("unlock_version", unlock_version, UNLOCK_CODE_VERSION_LEN)) {
            return 0x00000000;
        } else {
            make_unlock_code(unlock_code_buffer, version);
        }
    }
    return version;
}

void cmd_oem_relock(const char *arg, void *data, unsigned sz)
{
    int update = (!arg || strncmp(" keep-version", arg, strlen(" keep-version")));

#if defined(UFBL_FEATURE_TEMP_UNLOCK)
    amzn_clear_temp_unlock_idme();
#endif

    if (update) {
        char unlock_version[UNLOCK_CODE_VERSION_LEN] __attribute__((aligned(4)));
        u32 new_generated_version = rand_u32();
        *(u32 *)(unlock_version) = new_generated_version;
        if (!idme_update_var_ex("unlock_version", unlock_version, UNLOCK_CODE_VERSION_LEN)) {
            make_unlock_code(unlock_code_buffer, new_generated_version);
        }
    }
    idme_update_var_ex("unlock_code", "", 0);
    strcpy(unlock_status_buffer, "false");
    fastboot_okay("");
}

int amzn_get_unlock_code(unsigned char *code, unsigned int *len)
{
    if (!code || !len || *len < (UNLOCK_CODE_STR_REP_LEN + 1))
        return -1;

    *len = (unsigned) make_unlock_code((char*) code, get_version_may_update());

    return 0;
}

const unsigned char *amzn_get_unlock_key(unsigned int *key_len)
{
    static const unsigned char unlock_key[] __attribute__((aligned(4))) = {
        __FEATURE_MTK_IMPL_JOIN(UFBL_UNLOCK_PUBK_,
                                UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT_UPCASE)
    };

    const int unlock_key_size = sizeof(unlock_key);

    if (!key_len)
        return NULL;

    *key_len = unlock_key_size;

    return unlock_key;
}

int amzn_check_unlock_status(void) {
    int rc = amzn_target_is_unlocked();
#if defined(UFBL_FEATURE_TEMP_UNLOCK)
    rc |= amzn_target_is_temp_unlocked();
#endif
#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
    rc |= amzn_target_is_onetime_unlocked();
#endif
    strcpy(unlock_status_buffer, rc ? "true" : "false");
    return rc;
}

void amzn_fastboot_publish_unlock_vars(void) {
    static int unlock_vars_published = 0;
    if (unlock_vars_published != 0) {
        return;
    }
    unlock_vars_published = 1;
    make_unlock_code(unlock_code_buffer, get_version_may_update());
    fastboot_publish("unlock_code", unlock_code_buffer);
    amzn_check_unlock_status();
    fastboot_publish("unlock_status", unlock_status_buffer);

#if defined(UFBL_FEATURE_TEMP_UNLOCK)
    // _base64_encode_internal needs an extra bytes to append '\0' there.
    unsigned int temp_unlock_len = BASE64_LEN(TEMP_UNLOCK_CODE_LEN) + 1;
    static const unsigned char temp_unlock_code[BASE64_LEN(TEMP_UNLOCK_CODE_LEN) + 4] __attribute__((aligned(4))) = {0};
    if (!amzn_get_temp_unlock_current_code(temp_unlock_code, &temp_unlock_len)) {
        fastboot_publish("tu_code", (char *)temp_unlock_code);
    }
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
    // _base64_encode_internal needs an extra bytes to append '\0' there.
    static const unsigned char one_tu_code[ONETIME_UNLOCK_CODE_LEN + 1] = {0};
    unsigned int one_tu_len = sizeof(one_tu_code);
    if (!amzn_get_one_tu_code(one_tu_code, &one_tu_len)) {
        fastboot_publish("otu_code", strdup((char *)one_tu_code));
    }
#endif
}

void amzn_flash_unlock_signature(const unsigned char *sig,
                                 unsigned int sig_len) {
    if (sig_len < SIGNED_UNLOCK_CODE_LEN) {
        fastboot_fail("signature length error, wrong signature file? do nothing!");
        return;
    }
    if (amzn_verify_unlock(sig, SIGNED_UNLOCK_CODE_LEN)) {
        fastboot_fail("unlock signature verify failed, do nothing!");
        return;
    }
    if (idme_update_var_ex("unlock_code", sig, SIGNED_UNLOCK_CODE_LEN)) {
        fastboot_fail("correct signature, but update idme failed");
        return;
    }
    amzn_check_unlock_status();
    fastboot_okay("");
}

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
int amzn_flash_onetime_unlock_code_or_cert_maybe(const unsigned char *arg,
                                              void *data, unsigned sz) {
    if (!strcmp(arg, "otucert")) {
        if (amzn_set_onetime_unlock_cert(data, sz)) {
            fastboot_fail("write otucert failed!");
        } else {
            fastboot_okay("");
        }
        // unlock status might changed, recheck;
        amzn_check_unlock_status();
        return 1;
    }

    if (!strcmp(arg, "otucode")) {
        if (amzn_set_onetime_unlock_code(data, sz)) {
            fastboot_fail("write otucode failed!");
        } else {
            fastboot_okay("");
        }
        // unlock status might changed, recheck;
        amzn_check_unlock_status();
        return 1;
    }
    return 0;
}

int amzn_get_one_tu_code(unsigned char *code, unsigned int *len)
{
    static unsigned char code_generated = 0;
    static unsigned char one_tu_code[ONETIME_UNLOCK_CODE_LEN + 1] = {0};
    static unsigned char entropy[ENTROPY_LEN];
    uint8_t random_bytes[RANDOM_BYTES_SIZE] = {0};
    unsigned int out_len = sizeof(one_tu_code);

    if (!code || !len || *len < ONETIME_UNLOCK_CODE_LEN)
        return -1;

    /* If code is already generated skip generating new one */
    if (code_generated)
        goto skip;

    /* Generate entropy */
    make_entropy(entropy);

    /* Generate random bytes */
    if (amzn_get_onetime_random_number(entropy, ENTROPY_LEN, random_bytes,
        sizeof(random_bytes))) {
        fastboot_fail("random number generation failed");
        return -1;
    }

    /**
     * amzn_get_onetime_random_number will return a binary string which
     * cannot be returned via fastboot so use base64 to encode it.
     */
    if (amzn_onetime_unlock_b64_encode(random_bytes, sizeof(random_bytes),
        one_tu_code, &out_len)) {
        fastboot_fail("%s: onetime unlock code encode failed");
        return -1;
    }

    code_generated = 1;
skip:
    memcpy(code, one_tu_code, ONETIME_UNLOCK_CODE_LEN);
    *len = ONETIME_UNLOCK_CODE_LEN;
    return 0;
}

int amzn_get_onetime_unlock_root_pubkey(const unsigned char **key,
    unsigned int *key_len)
{
    static const unsigned char onetime_unlock_key[] __attribute__((aligned(4))) = {
        __FEATURE_MTK_IMPL_JOIN(UFBL_ONETIME_UNLOCK_PUBK_,
                                UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT_UPCASE)
    };
    const int onetime_unlock_key_size = sizeof(onetime_unlock_key);

    if (!key || !key_len)
        return -1;

    *key_len = onetime_unlock_key_size;
    *key = onetime_unlock_key;
    return 0;
}
#endif


#if defined(UFBL_FEATURE_TEMP_UNLOCK)
int amzn_flash_temp_unlock_code_or_cert_maybe(const unsigned char *arg,
                                              void *data, unsigned sz) {
    if (!strcmp(arg, "tucert")) {
        if (amzn_set_temp_unlock_idme_cert(data, sz)) {
            fastboot_fail("write tucert failed!");
        } else {
            fastboot_okay("");
        }
        // unlock status might changed, recheck;
        amzn_check_unlock_status();
        return 1;
    }

    if (!strcmp(arg, "tucode")) {
        if (amzn_set_temp_unlock_idme_code(data, sz)) {
            fastboot_fail("write tucode failed!");
        } else {
            fastboot_okay("");
        }
        // unlock status might changed, recheck;
        amzn_check_unlock_status();
        return 1;
    }
    return 0;
}

static struct boot_tag_temp_unlock temp_unlock_data __attribute__ ((aligned(4)));
void amzn_save_temp_unlock_data(struct boot_tag_temp_unlock *data_input) {
    if (!data_input) return;
    memcpy(&temp_unlock_data, data_input, sizeof(temp_unlock_data));
}

#include <platform/boot_mode.h>
int amzn_get_temp_unlock_codes(unsigned char **codes, unsigned int *reboot_cnt)
{
    if (!codes || !reboot_cnt || !g_boot_arg->rpmb_state) {
        return -1;
    }
    /**
     * reboot_cnt get from preloader should never exceed CFG_TEMP_UNLOCK_REBOOT_COUNT,
     * clear reboot_cnt and codes in case invalid value is passed in.
     */
    if (temp_unlock_data.temp_unlock_reboot_cnt > CFG_AMZN_TEMP_UNLOCK_REBOOT_CNT) {
        *reboot_cnt = 0;
        *codes = NULL;
        return -1;
    }
    *reboot_cnt = temp_unlock_data.temp_unlock_reboot_cnt;
    *codes = temp_unlock_data.temp_unlock_hmac;
    return 0;
}

int amzn_get_temp_unlock_root_pubkey(unsigned char **key, unsigned int *key_len)
{
    static const unsigned char temp_unlock_key[] __attribute__((aligned(4))) = {
        __FEATURE_MTK_IMPL_JOIN(UFBL_TEMP_UNLOCK_PUBK_,
                                UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT_UPCASE)
    };
    const int temp_unlock_key_size = sizeof(temp_unlock_key);

    if (!key || !key_len)
        return -1;

    *key_len = temp_unlock_key_size;
    *key = temp_unlock_key;
    return 0;
}
#endif  // UFBL_FEATURE_TEMP_UNLOCK
