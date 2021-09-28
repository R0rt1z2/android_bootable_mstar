/*
 * Copyright (c) 2015, Google Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>

#if defined(CONFIG_ANDROID_AVB_ENABLE)
#include "../lib/libavb_ab_uboot/libavb_ab_uboot.h"

#include <image.h>
#include <android_image.h>
#include <MsUtility.h>

#define DEVICE_UNLOCK   0
#define DEVICE_LOCK     1
#define BOOT_GREEN      0
#define BOOT_YELLOW     1
#define BOOT_ORANGE     2

#define RSA2048_BYTES   (2048 / 8)
#define RSA4096_BYTES   (4096 / 8)

/* Block size in bytes of a SHA-256 digest. */
#define SHA256_BYTES 64

/* Block size in bytes of a SHA-512 digest. */
#define SHA512_BYTES 128

#define KEYMASTER_OPTEE_ACCESS_ADDR 0xb200585E

typedef enum {
    KM_VERIFIED_BOOT_VERIFIED = 0,    /* Full chain of trust extending from the bootloader to
                                       * verified partitions, including the bootloader, boot
                                       * partition, and all verified partitions*/
    KM_VERIFIED_BOOT_SELF_SIGNED = 1, /* The boot partition has been verified using the embedded
                                       * certificate, and the signature is valid. The bootloader
                                       * displays a warning and the fingerprint of the public
                                       * key before allowing the boot process to continue.*/
    KM_VERIFIED_BOOT_UNVERIFIED = 2,  /* The device may be freely modified. Device integrity is left
                                       * to the user to verify out-of-band. The bootloader
                                       * displays a warning to the user before allowing the boot
                                       * process to continue */
    KM_VERIFIED_BOOT_FAILED = 3,      /* The device failed verification. The bootloader displays a
                                       * warning and stops the boot process, so no keymaster
                                       * implementation should ever actually return this value,
                                       * since it should not run.  Included here only for
                                       * completeness. */
} keymaster_verified_boot_t;


// only for tee_binding to reference used.
typedef struct {
    uint32_t os_version;
    uint32_t os_patchlevel;
    uint32_t verified_boot_key_len;
    char* verified_boot_key;
    uint32_t verified_boot_state;
    uint32_t device_locked; // true=1, false=0
    uint32_t verified_boot_hash_len;
    char *verified_boot_hash;
} Keymaster_verified_boot_parameters;

typedef struct {
    uint32_t buffer_size;
    uint8_t id[8]; //(i.e. mboot\0, null terminated)
    uint8_t name[28]; //(i.e. verified-boot\0, null terminated)
    uint8_t data[1];
} TEE_data;
/**
 * tee_binding()
 */
static void tee_binding(
    uint32_t os_version,
    uint32_t os_patchlevel,
    uint32_t verified_boot_key_len,
    uint8_t* verified_boot_key,
    uint32_t verified_boot_state,
    uint32_t device_locked,
    uint32_t verified_boot_hash_len,
    char* verified_boot_hash)
{
    uint32_t keymaster_params_size = 0;
    keymaster_params_size = sizeof(uint32_t)*6 + verified_boot_key_len + verified_boot_hash_len;
    uint8_t *keymaster_params_raw_data = malloc(keymaster_params_size);
    memset(keymaster_params_raw_data, 0, keymaster_params_size);
    uint32_t raw_offset = 0;

    //printf("os_version=%d\n", os_version);
    memcpy(keymaster_params_raw_data, &os_version, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    //printf("os_patchlevel=%d\n", os_patchlevel);
    memcpy(keymaster_params_raw_data + raw_offset, &os_patchlevel, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    //printf("verified_boot_key_len=%d\n", verified_boot_key_len);
    memcpy(keymaster_params_raw_data + raw_offset, &verified_boot_key_len, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    // add verified_boot_key
    memcpy(keymaster_params_raw_data + raw_offset, verified_boot_key, verified_boot_key_len);
    raw_offset += verified_boot_key_len;

    uint32_t temp_verified_boot_state = verified_boot_state;
    //printf("state=%d\n", temp_verified_boot_state);
    memcpy(keymaster_params_raw_data + raw_offset, &temp_verified_boot_state, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    //printf("device_locked=%d\n", device_locked);
    memcpy(keymaster_params_raw_data + raw_offset, &device_locked, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    //printf("verified_boot_hash_len=%d\n", verified_boot_hash_len);
    memcpy(keymaster_params_raw_data + raw_offset, &verified_boot_hash_len, sizeof(uint32_t));
    raw_offset += sizeof(uint32_t);

    // add verified_boot_hash
    memcpy(keymaster_params_raw_data + raw_offset, verified_boot_hash, verified_boot_hash_len);

    TEE_data *TEEdata = NULL;
    uint32_t tee_data_size = sizeof(*TEEdata) + keymaster_params_size;
    TEEdata = malloc(tee_data_size);
    memset(TEEdata, 0, tee_data_size);
    TEEdata->buffer_size = 4 + 36 + keymaster_params_size;
    strncpy(TEEdata->id, "mboot\0", 5);
    strncpy(TEEdata->name, "verified-boot\0", 14);
    memcpy(TEEdata->data, keymaster_params_raw_data, keymaster_params_size);

    flush_cache(TEEdata, tee_data_size);

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
    struct smc_param tee_bind_smc_params;
    tee_bind_smc_params.a0 = KEYMASTER_OPTEE_ACCESS_ADDR;
    tee_bind_smc_params.a1 = TEEdata;
    tee_smc_call(&tee_bind_smc_params);
#endif
}

void env_add_boot_state(unsigned int state)
{
    char newstr[40] = {0};
    sprintf(newstr, "androidboot.verifiedbootstate=%s", state == 0 ? "green" : state == 1 ? "yellow" : state == 2 ? "orange" : "red");
    env_add_bootargs("androidboot.verifiedbootstate=", newstr);
}

int get_data_from_bootargs(const char* bootargs, const char *prefix, int prefix_len, char* out_buf, int out_buf_len)
{
    char temp_prefix[128] = {0};
    char string_value[128] = {0};
    char* ptr = NULL;
    ptr = strstr(bootargs, prefix);
    if( ptr == NULL )
    {
        printf("%s:%d Can't find prefix:%s\n", __FUNCTION__, __LINE__, prefix);
        return -1;
    }
    strncpy(temp_prefix, prefix, prefix_len);
    strcat(temp_prefix, "%s");
    sscanf(ptr, temp_prefix, string_value);
    strncpy(out_buf, string_value, out_buf_len);
    return 0;
}

int get_vbmeta_digest(AvbSlotVerifyData *avb_data, char* vbmeta_hash_value, uint32_t* verified_boot_hash_len)
{
    char vbmeta_hash_alg[16] = {0};
    int ret = 0;

    ret = get_data_from_bootargs(avb_data->cmdline, "androidboot.vbmeta.hash_alg=", strlen("androidboot.vbmeta.hash_alg="), vbmeta_hash_alg, sizeof(vbmeta_hash_alg));
    if(ret != 0)
    {
        printf("%s:%d Get SHA algorithm fail\n", __FUNCTION__, __LINE__);
        return -1;
    }
    else
    {
        if(strcmp(vbmeta_hash_alg, "sha256")==0)
            *verified_boot_hash_len = SHA256_BYTES;
        else if(strcmp(vbmeta_hash_alg, "sha512")==0)
            *verified_boot_hash_len = SHA512_BYTES;
        else
        {
            printf("%s:%d Unsupport SHA algorithm\n", __FUNCTION__, __LINE__);
            return -1;
        }
        ret = get_data_from_bootargs(avb_data->cmdline, "androidboot.vbmeta.digest=", strlen("androidboot.vbmeta.digest="), vbmeta_hash_value, *verified_boot_hash_len);
        if(ret != 0)
        {
            printf("%s:%d Get digest fail\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }
    return 0;
}

#ifdef CONFIG_ANDROID_AVB_ENABLE

#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
void check_android_boot_state(AvbSlotVerifyData *avb_data, AvbABFlowResult avb_ab_flow_result, unsigned int os_version)
{
    AvbABOps* ab_ops;
    bool is_device_state_unlocked = false;
    uint32_t verified_boot_hash_len = SHA256_BYTES;
    char vbmeta_hash_value[SHA512_BYTES] = {0};
    int ret = 0;

    ab_ops = avb_get_ab_ops();
    ab_ops->ops->read_is_device_unlocked(ab_ops->ops, &is_device_state_unlocked);

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B
    if(is_device_state_unlocked
        && (avb_ab_flow_result == AVB_AB_FLOW_RESULT_OK
            || avb_ab_flow_result == AVB_AB_FLOW_RESULT_OK_WITH_VERIFICATION_ERROR))
    {// orange state.
        //PanelLogo(1);
        ret = get_vbmeta_digest(avb_data, vbmeta_hash_value, &verified_boot_hash_len);
        env_add_boot_state(BOOT_ORANGE);
        tee_binding(os_version, 0, avb_data->vbmeta_images->public_key_len, avb_data->vbmeta_images->public_key_data, BOOT_ORANGE,
                    DEVICE_UNLOCK, verified_boot_hash_len, vbmeta_hash_value);
        printf("\nBoot state is: Orange !\n\n");
        return;
    }
#endif

    //green state.
    if(is_device_state_unlocked == false
        && avb_ab_flow_result == AVB_AB_FLOW_RESULT_OK)
    {
        ret = get_vbmeta_digest(avb_data, vbmeta_hash_value, &verified_boot_hash_len);
        env_add_boot_state(BOOT_GREEN);
        tee_binding(os_version, 0, avb_data->vbmeta_images->public_key_len, avb_data->vbmeta_images->public_key_data, BOOT_GREEN,
                    DEVICE_LOCK, verified_boot_hash_len, vbmeta_hash_value);
        printf("Boot state is: Green !\n\n");
        return;
    }
    else
    {
        printf("\n%s:%d: Android Verify Boot 2.0 fail, AvbABFlowResult: %d\n\n",__FILE__,__LINE__,avb_ab_flow_result);
    }
    //halted for red state.
    //PanelLogo(0);
    printf("\nBoot state is: Red, System halted !\n\n");
    while (1);
}
#else
void check_android_boot_state(AvbSlotVerifyData *avb_data, AvbSlotVerifyResult avb_slot_verify_result, unsigned int os_version)
{
    AvbABOps* ab_ops;
    bool is_device_state_unlocked = false;
    uint32_t verified_boot_hash_len = SHA256_BYTES;
    char vbmeta_hash_value[SHA512_BYTES] = {0};
    int ret = 0;

    ab_ops = avb_get_ab_ops();
    ab_ops->ops->read_is_device_unlocked(ab_ops->ops, &is_device_state_unlocked);

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B
    if(is_device_state_unlocked
        && (avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_OK
            || avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_VERIFICATION
            || avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_ROLLBACK_INDEX
            || avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_PUBLIC_KEY_REJECTED))
    {// orange state.
        //PanelLogo(1);
        ret = get_vbmeta_digest(avb_data, vbmeta_hash_value, &verified_boot_hash_len);
        env_add_boot_state(BOOT_ORANGE);
        tee_binding(os_version, 0, avb_data->vbmeta_images->public_key_len, avb_data->vbmeta_images->public_key_data, BOOT_ORANGE,
                    DEVICE_UNLOCK, verified_boot_hash_len, vbmeta_hash_value);
        printf("\nBoot state is: Orange !\n\n");
        return;
    }
#endif

    //green state.
    if(is_device_state_unlocked == false
        && avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_OK)
    {
        ret = get_vbmeta_digest(avb_data, vbmeta_hash_value, &verified_boot_hash_len);
        env_add_boot_state(BOOT_GREEN);
        tee_binding(os_version, 0, avb_data->vbmeta_images->public_key_len, avb_data->vbmeta_images->public_key_data, BOOT_GREEN,
                    DEVICE_LOCK, verified_boot_hash_len, vbmeta_hash_value);
        printf("Boot state is: Green !\n\n");
        return;
    }
    else
    {
        printf("\n%s:%d: Android Verify Boot 2.0 fail, AvbSlotVerifyResult: %d\n\n",__FILE__,__LINE__,avb_slot_verify_result);
    }
    //halted for red state.
    //PanelLogo(0);
    printf("\nBoot state is: Red, System halted !\n\n");
    while (1);
}

int update_stored_rollback_index(AvbOps* ops, AvbSlotVerifyData *avb_data)
{
    int ret = 0, n;
    bool is_device_state_unlocked = false;
    AvbIOResult io_ret;

    is_device_state_unlocked = ops->read_is_device_unlocked(ops, &is_device_state_unlocked);

    // if device state is unlock, should not to update rollback index.
    if(is_device_state_unlocked)
    {
        goto out;
    }

    for (n = 0; n < AVB_MAX_NUMBER_OF_ROLLBACK_INDEX_LOCATIONS; n++) {
        uint64_t rollback_index_value = 0;

        if (avb_data != NULL) {
            rollback_index_value = avb_data->rollback_indexes[n];
        }

        if (rollback_index_value != 0) {
            uint64_t current_rollback_index_value;
            io_ret = ops->read_rollback_index(ops, n, &current_rollback_index_value);
            if (io_ret == AVB_IO_RESULT_ERROR_OOM) {
                ret = -1;
                goto out;
            } else if (io_ret != AVB_IO_RESULT_OK) {
                printf("%s: Error getting rollback index for slot.\n",__FUNCTION__);
                ret = -2;
                goto out;
            }
            if (current_rollback_index_value != rollback_index_value) {
                io_ret = ops->write_rollback_index(ops, n, rollback_index_value);
                if (io_ret == AVB_IO_RESULT_ERROR_OOM) {
                    ret = -3;
                    goto out;
                } else if (io_ret != AVB_IO_RESULT_OK) {
                    printf("%s: Error setting stored rollback index.\n",__FUNCTION__);
                    ret = -4;
                    goto out;
                }
            }
        }
    }

out:
    if (ret) {
        printf("%s: Fail, Error code: %d.\n",__FUNCTION__,ret);
    }
    return ret;
}

#endif
#endif //CC_SUPPORT_AVB

#endif //defined(CC_SUPPORT_AVB)||defined(CONFIG_ANDROID_SIGNATURE)

