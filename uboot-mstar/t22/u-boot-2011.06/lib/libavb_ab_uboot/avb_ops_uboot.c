
#include <common.h>
#include <jffs2/jffs2.h>
#include <mmc.h>
#include <linux/mtd/mtd.h>
#include <MsMmc.h>
#include <MsRawIO.h>
#include <android_image.h>
#include <key.h>

#include "avb_ops_uboot.h"
#include "../libavb_ab/libavb_ab.h"
#include "../libavb_ab_uboot/avb_rpmb.h"

#define BOOT_DEVICE_MAJOR (179)

AvbABOps avbAB_ops_user;
AvbOps avb_ops_user;

UBOOT_RPMB_RAW_DATA g_uboot_rpmb_raw_data;
bool g_support_rpmb = false;

#define WITH_PADDING_SIZE(size, page)    (((size)+(page)-1) & (~((page)-1)))

#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
void env_add_boot_slot_suffix(char* slot_suffix)
{
    const char boot_slot_prefix[] = "androidboot.slot_suffix=";
    char boot_slot_args[64] = {0};

    strcpy(boot_slot_args,boot_slot_prefix);
    strcat(boot_slot_args,slot_suffix);
    env_add_bootargs(boot_slot_prefix, boot_slot_args);
}

void env_add_boot_skip_initramfs(void)
{
    const char boot_skip_initramfs[] = "skip_initramfs";
    env_add_bootargs(boot_skip_initramfs, boot_skip_initramfs);
}
#endif

static size_t avb_read_emmc(u64 offset, void* addr, size_t cnt)
{
    int r;
    struct mmc *mmc;

    mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
    if (!mmc)
    {
        puts("AVB Read: Have no MMC device.\n");
        return 0;
    }

    if (mmc_init(mmc))
    {
        puts("AVB Read: MMC init failed.\n");
        return 0;
    }

    //r = mmc->block_dev.byte_read(CONFIG_SYS_MMC_ENV_DEV, offset, cnt, addr);
    if (r != cnt)
    {
        puts("AVB Read: MMC Read error.\n");
        return 0;
    }

    return cnt;
}

static size_t avb_write_emmc(u64 offset, void* addr, size_t cnt)
{
    int r;
    struct mmc *mmc;

    mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
    if (!mmc)
    {
        puts("AVB write: Have no MMC device.\n");
        return 0;
    }

    if (mmc_init(mmc))
    {
        puts("AVB write: MMC init failed.\n");
        return 0;
    }

    //r = mmc->block_dev.byte_write(CONFIG_SYS_MMC_ENV_DEV, offset, cnt, addr);
    if (r != cnt)
    {
        puts("AVB write: MMC write error.\n");
        return 0;
    }

    return cnt;
}

size_t avb_get_part_number(const char* partition, int *partNum)
{
    block_dev_desc_t *mmc_dev;
    struct mmc *mmc = NULL;
    u32 curr_partnum = 1;
    int curr_device = 0;
    disk_partition_t dpt;

    mmc = find_mmc_device(curr_device);

    if (!mmc) {
        printf("no mmc device at slot %x\n", curr_device);
        return -1;
    }

    if(!mmc->has_init){
        printf("Do mmc init first!\n");
        mmc_init(mmc);
        mmc->has_init = 1;
    }

    mmc_dev = mmc_get_dev(curr_device);
    if ((mmc_dev == NULL) ||
            (mmc_dev->type == DEV_TYPE_UNKNOWN))
    {
        printf("no mmc device found!\n");
        return 1;
    }

    for(;;)
    {
        if(get_partition_info_emmc(mmc_dev, curr_partnum, &dpt))
            break;
        if(!strcmp(partition, (const char *)dpt.name)){
            *partNum = curr_partnum;
            return 0;
        }
        curr_partnum++;
    }
    return 1;
}

static AvbIOResult read_from_partition(AvbOps* ops,
                                       const char* partition,
                                       int64_t offset,
                                       size_t num_bytes,
                                       void* buffer,
                                       size_t* out_num_read) {
    struct mtd_device *dev;
    struct part_info *partInfo;
    uint8_t partNum;
    AvbIOResult ret;
    unsigned int u32Size=0;
    uint64_t image_size;
    uint32_t partition_size;
    int ret1;

    raw_io_config_push();
    ret1 = raw_io_config(FLASH_RAW_DATA_TARGET, partition, UNNECESSARY_VOLUME);

    if(ret1 != 0)
    {
        printf("raw_io_config setting fail!\n");
        return AVB_IO_RESULT_ERROR_IO;
    }

    ops->get_size_of_partition(ops, partition, &image_size);
    partition_size = (uint32_t)image_size;

    if(offset < partition_size)
    {
        // If offset is negative, interprets its absolute value as the number of bytes from the end of the partition.
        if(offset < 0)
        {
            offset += partition_size;
        }

        if((offset + num_bytes) > partition_size)
        {
            num_bytes = partition_size - offset;
        }

        ret = raw_read((unsigned int)buffer, offset, num_bytes);
        if(ret != 0)
        {
            printf("%s:%d:%s: Error, raw_read %s fail!\n",__FILE__,__LINE__,__FUNCTION__,partition);
            return AVB_IO_RESULT_ERROR_IO;
        }
    }
    else
    {
        printf("%s:%d:%s: Error, Read offset is out of partition size.\n",__FILE__,__LINE__,__FUNCTION__);
        ret = AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
    }

    // aaron skip check
    *out_num_read = num_bytes;
    raw_io_config_pop();
    return AVB_IO_RESULT_OK;

    /*
    // aaron skip
    if ((mtdparts_init() == 0) && (find_dev_and_part(partition, &dev, &partNum, &partInfo) == 0))
    {
        // If offset is negative, interprets its absolute value as the number of bytes from the end of the partition.
        if(offset < 0)
        {
            offset += partInfo->size;
        }

        if(offset < partInfo->size)
        {
            if((offset + num_bytes) > partInfo->size)
            {
                num_bytes = partInfo->size - offset;
            }

            *out_num_read = avb_read_emmc(partInfo->offset+offset, buffer, num_bytes);
            if(num_bytes != *out_num_read)
            {
                printf("%s:%d:%s: Error, Read emmc fail.\n",__FILE__,__LINE__,__FUNCTION__);
                ret = AVB_IO_RESULT_ERROR_IO;
            }
            else
            {
                ret = AVB_IO_RESULT_OK;
            }
        }
        else
        {
            printf("%s:%d:%s: Error, Read offset is out of partition size.\n",__FILE__,__LINE__,__FUNCTION__);
            ret = AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
        }
    }
    else
    {
        printf("%s:%d:%s: Error, there no partition named %s.\n",__FILE__,__LINE__,__FUNCTION__,partition);
        ret = AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }

    return ret;*/
}

static AvbIOResult write_to_partition(AvbOps* ops,
                                      const char* partition,
                                      int64_t offset,
                                      size_t num_bytes,
                                      const void* buffer) {
    struct mtd_device *dev;
    struct part_info *partInfo;
    uint8_t partNum;
    size_t write_bytes;
    AvbIOResult ret;
    uint64_t image_size;
    uint32_t partition_size;
    int ret1 = 0;

    raw_io_config_push();
    ret1 = raw_io_config(FLASH_RAW_DATA_TARGET, partition, UNNECESSARY_VOLUME);

    if(ret1 != 0)
    {
        printf("raw_io_config setting fail!\n");
        return AVB_IO_RESULT_ERROR_IO;
    }

    ops->get_size_of_partition(ops, partition, &image_size);
    partition_size = (uint32_t)image_size;

    if(offset < partition_size)
    {
        // If offset is negative, interprets its absolute value as the number of bytes from the end of the partition.
        if(offset < 0)
        {
            offset += partition_size;
        }

        if((offset + num_bytes) > partition_size)
        {
            num_bytes = partition_size - offset;
        }

        ret = raw_write((unsigned int)buffer, offset, num_bytes);
        if(ret != 0)
        {
            printf("%s:%d:%s: Error, raw_write %s fail!\n",__FILE__,__LINE__,__FUNCTION__,partition);
            return AVB_IO_RESULT_ERROR_IO;
        }
    }
    else
    {
        printf("%s:%d:%s: Error, Write offset is out of partition size.\n",__FILE__,__LINE__,__FUNCTION__);
        ret = AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
    }

    raw_io_config_pop();
    return AVB_IO_RESULT_OK;

/*
    // aaron skip
    if ((mtdparts_init() == 0) && (find_dev_and_part(partition, &dev, &partNum, &partInfo) == 0))
    {
        if(offset < partInfo->size)
        {
            if((offset + num_bytes) > partInfo->size)
            {
                num_bytes = partInfo->size - offset;
            }

            write_bytes = avb_write_emmc(partInfo->offset+offset, buffer, num_bytes);
            if(num_bytes != write_bytes)
            {
                printf("%s:%d:%s: Error, Write emmc fail.\n",__FILE__,__LINE__,__FUNCTION__);
                ret = AVB_IO_RESULT_ERROR_IO;
            }
            else
            {
                ret = AVB_IO_RESULT_OK;
            }
        }
        else
        {
            printf("%s:%d:%s: Error, Write offset is out of partition size.\n",__FILE__,__LINE__,__FUNCTION__);
            ret = AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
        }
    }
    else
    {
        printf("%s:%d:%s: Error, there no partition named %s.\n",__FILE__,__LINE__,__FUNCTION__,partition);
        ret = AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }

    return ret;
*/
}

static AvbIOResult validate_vbmeta_public_key(AvbOps* ops,
                                          const uint8_t* public_key_data,
                                          size_t public_key_length,
                                          const uint8_t* public_key_metadata,
                                          size_t public_key_metadata_length,
                                          bool* out_is_trusted)
{
    uint8_t avb_public_key0[] = {CONFIG_ANDROID_SIGNATURE_PUBKEY};

    if(public_key_metadata != NULL || public_key_metadata_length != 0)
    {
        printf("%s: Error: public key metadata is not supported now.\n",__FUNCTION__);
        *out_is_trusted = false;
        return AVB_IO_RESULT_ERROR_IO;
    }

    if(sizeof(avb_public_key0) != public_key_length)
    {
        printf("%s: Warning: vbmeta public key is mismatch by key length.\n",__FUNCTION__);
        *out_is_trusted = false;
    }
    else if(memcmp(public_key_data, avb_public_key0, public_key_length) == 0)
    {
        *out_is_trusted = true;
    }
    else
    {
        printf("%s: Warning: vbmeta public key is mismatch by key value.\n",__FUNCTION__);
        *out_is_trusted = false;
    }

    return AVB_IO_RESULT_OK;
}

#if !defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH) || !defined(CONFIG_AVB_RPMB_ENABLE)
static void avb_load_info_from_rpmb(void)
{
    return;
}

static AvbIOResult read_rollback_index(AvbOps* ops,
                                   size_t rollback_index_location,
                                   uint64_t* out_rollback_index)
{
    *out_rollback_index = 0;
    return AVB_IO_RESULT_OK;
}

static AvbIOResult write_rollback_index(AvbOps* ops, uint64_t *new_rollback_index)
{
    return AVB_IO_RESULT_OK;
}

int read_device_state(uint16_t *devicestate)
{
    char *dev_state = getenv("devicestate");

    if(dev_state != NULL && strcmp(dev_state, "unlock") == 0)
    {
        *devicestate = 0;
    }
    else
    {
        *devicestate = 1;
    }
    return 0;
}

int write_device_state(const uint16_t devicestate)
{
    char str_state[16] = {0};
    // 0 is unlock, 1 is lock
    if( devicestate == 0 )
        strcpy(str_state, "unlock");
    else
        strcpy(str_state, "lock");

    setenv("devicestate", str_state);
    saveenv();
    return 0;
}
#else
static void avb_load_info_from_rpmb(void)
{
    uint32_t ret = 1;
    uint8_t block_data[RPMB_BLOCK_SIZE] = {0};

    ret = ubootRpmbReadData(block_data);
    if(ret != 0)
    {
        printf("%s:%s:%d Fail to read rpmb data\n", __FILE__, __FUNCTION__, __LINE__);
        g_support_rpmb = false;
        return;
    }

    eMMC_RPMB_DATA *rpmb_data_ptr = &block_data;
    memset(&g_uboot_rpmb_raw_data, 0, sizeof(g_uboot_rpmb_raw_data));
    // avb only uses 128 bytes in block_data
    memcpy(&g_uboot_rpmb_raw_data, rpmb_data_ptr->u8_data, 128);

    // data need to reverse to match UBOOT_RPMB_RAW_DATA structure
    _ReverseEndian(&g_uboot_rpmb_raw_data, 128);
    g_support_rpmb = true;
}

static AvbIOResult read_rollback_index(AvbOps* ops,
                                   size_t rollback_index_location,
                                   uint64_t* out_rollback_index)
{

    // wait for optee function ready
    if ( g_support_rpmb == true )
    {
        *out_rollback_index = g_uboot_rpmb_raw_data.rollback_index[rollback_index_location];
    }
    else
        return AVB_IO_RESULT_ERROR_IO;

/*
    // temp use env
    char *rollback_index_ptr = getenv("rollback_index_location");
    if( rollback_index_ptr != NULL )
        *out_rollback_index = simple_strtoul (rollback_index_ptr, NULL, 10);
    else
        return AVB_IO_RESULT_ERROR_IO;
*/
    printf("%s done, Location: %d, Index: 0x%llx\n", __FUNCTION__, rollback_index_location, *out_rollback_index);

    return AVB_IO_RESULT_OK;
}

static AvbIOResult write_rollback_index(AvbOps* ops,
                                    size_t rollback_index_location,
                                    uint64_t rollback_index)
{
    int32_t ret = 0;
    uint8_t block_data[RPMB_BLOCK_SIZE] = {0};
    UBOOT_RPMB_RAW_DATA new_uboot_raw_data;
    memset(&new_uboot_raw_data, 0, sizeof(new_uboot_raw_data));

    // this stage will not change device_state
    new_uboot_raw_data.device_state = g_uboot_rpmb_raw_data.device_state;
    new_uboot_raw_data.index_counts = 0; // not use for now
    memcpy(new_uboot_raw_data.rollback_index, g_uboot_rpmb_raw_data.rollback_index, sizeof(new_uboot_raw_data.rollback_index));

    new_uboot_raw_data.rollback_index[rollback_index_location] = rollback_index;

    ret = ubootRpmbWriteData(block_data, new_uboot_raw_data);
    if( ret != 0 )
    {
        printf("%s:%d:%s: Error, AVB write rpmb data fail.\n",__FILE__,__LINE__,__FUNCTION__);
        return AVB_IO_RESULT_ERROR_IO;
    }

    // every time update partial index, all rpmb data needs to updata to global var.
    avb_load_info_from_rpmb();

    if( g_uboot_rpmb_raw_data.rollback_index[rollback_index_location] != rollback_index )
    {
        printf("%s:%d:%s: Error, AVB updates rollback_index fail.\n",__FILE__,__LINE__,__FUNCTION__);
        return AVB_IO_RESULT_ERROR_IO;
    }

    printf("%s done, Location: %d, Index: 0x%llx\n", __FUNCTION__, rollback_index_location, rollback_index);

    return AVB_IO_RESULT_OK;
}

int read_device_state(uint16_t *devicestate)
{
    avb_load_info_from_rpmb();
    // wait for optee function ready
    if ( g_support_rpmb == true )
    {
        *devicestate = g_uboot_rpmb_raw_data.device_state;
    }
    else
        return -1;

    return 0;
}

int write_device_state(const uint16_t devicestate)
{
    int32_t ret = 0;
    uint8_t block_data[RPMB_BLOCK_SIZE] = {0};
    UBOOT_RPMB_RAW_DATA new_uboot_raw_data;
    memset(&new_uboot_raw_data, 0, sizeof(new_uboot_raw_data));

    // init first
    avb_load_info_from_rpmb();

    // update devicestate
    new_uboot_raw_data.device_state = devicestate;

    new_uboot_raw_data.index_counts = 0; // not use for now
    // this stage will not change rollback_index
    memcpy(new_uboot_raw_data.rollback_index, g_uboot_rpmb_raw_data.rollback_index, sizeof(new_uboot_raw_data.rollback_index));

    ret = ubootRpmbWriteData(block_data, new_uboot_raw_data);
    if( ret != 0 )
    {
        printf("%s:%d:%s: Error, AVB write rpmb data fail.\n",__FILE__,__LINE__,__FUNCTION__);
        return -1;
    }

    // every time update data, all rpmb data needs to updata to global var.
    avb_load_info_from_rpmb();

    if( g_uboot_rpmb_raw_data.device_state != devicestate )
    {
        printf("%s:%d:%s: Error, AVB updates device_state fail.\n",__FILE__,__LINE__,__FUNCTION__);
        return -1;
    }

    return 0;
}
#endif // #if !defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH) || !defined(CONFIG_AVB_RPMB_ENABLE)

static AvbIOResult read_is_device_unlocked(AvbOps* ops, bool* out_is_unlocked)
{
    uint16_t devicestate = 0;
    read_device_state(&devicestate);

    if(devicestate == 0)
    {
        *out_is_unlocked = true;
    }
    else
    {
        *out_is_unlocked = false;
    }

    return AVB_IO_RESULT_OK;
}

static AvbIOResult get_unique_guid_for_partition(AvbOps* ops,
                                             const char* partition,
                                             char* guid_buf,
                                             size_t guid_buf_size)
{
    struct mtd_device *dev;
    struct part_info *partInfo;
    int partNum;
    char path_tmp[64] = {0};
    int copy_size = 0;

    if(avb_get_part_number(partition, &partNum) == 0)
    {
        sprintf(path_tmp,"%d:%d", BOOT_DEVICE_MAJOR, partNum);
        copy_size = (guid_buf_size < strlen(path_tmp))? guid_buf_size : strlen(path_tmp);
        strncpy(guid_buf, path_tmp, copy_size);
        guid_buf[copy_size] = '\0';
        return AVB_IO_RESULT_OK;
    }
    else
    {
        printf("%s: Error: there is no %s partition.\n",__FUNCTION__, partition);
        return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }
}

static AvbIOResult get_size_of_partition(AvbOps* ops,
                                         const char* partition,
                                         uint64_t* out_size_in_bytes) {
    struct mtd_device *dev;
    struct part_info *partInfo;
    uint64_t* partSize = 0;
    uint64_t* partOffset = 0;
    uint8_t partNum;
    AvbIOResult ret;

    if (get_mmc_part_size_and_offset(partition, (unsigned int*)partSize, (unsigned int*)partOffset) == 0)
    {
        *out_size_in_bytes = (*partSize)*512;
        ret = AVB_IO_RESULT_OK;
    }
    else
    {
        ret = AVB_IO_RESULT_ERROR_IO;
    }
    return ret;
}

static void avb_load_boot_image(char *partition_name, const uint32_t boot_addr, const int boot_img_size, const AvbSlotVerifyData *avb_data)
{
    char cmd_buf[CMD_BUF] = {0};
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
    strncat(partition_name, avb_data->ab_suffix, 2);
    #endif
    memset(cmd_buf, 0, CMD_BUF);
    sprintf(cmd_buf, "mmc read.p 0x%x %s 0x%x", boot_addr, partition_name, boot_img_size);

    #if defined(CONFIG_MMC)
    if(run_command(cmd_buf, 0)!=0)
    {
        printf("%s:%s:%d Read partition:%s fails\n", __FILE__, __FUNCTION__, __LINE__, partition_name);
        avb_abort();
    }
    #else
    // for now, only support emmc
    printf("%s:%s:%d Read partition:%s fails\n", __FILE__, __FUNCTION__, __LINE__, partition_name);
    avb_abort();
    #endif
}

void avb_ops_uboot_init(void)
{
    static bool already_initial = false;
    AvbABOps* ab_ops;

    if(already_initial == true) {
        return;
    } else {
        already_initial = true;
    }

    memset(&avbAB_ops_user,0,sizeof(AvbABOps));
    memset(&avb_ops_user,0,sizeof(AvbOps));

    //1. initial ab_ops.
    ab_ops = &avbAB_ops_user;
    //2. initial members of ab_ops.
    ab_ops->ops = &avb_ops_user;
    ab_ops->read_ab_metadata = avb_ab_data_read;
    ab_ops->write_ab_metadata = avb_ab_data_write;
    //3. initial members of ab_ops->ops.
    ab_ops->ops->ab_ops = ab_ops;
    ab_ops->ops->read_from_partition = read_from_partition;
    ab_ops->ops->write_to_partition = write_to_partition;
    ab_ops->ops->validate_vbmeta_public_key = validate_vbmeta_public_key;
    ab_ops->ops->read_rollback_index = read_rollback_index;
    ab_ops->ops->write_rollback_index = write_rollback_index;
    ab_ops->ops->read_is_device_unlocked = read_is_device_unlocked;
    ab_ops->ops->get_unique_guid_for_partition = get_unique_guid_for_partition;
    ab_ops->ops->get_size_of_partition = get_size_of_partition;
}

AvbABOps* avb_get_ab_ops(void)
{
    return (&avbAB_ops_user);
}

AvbOps* avb_get_ops(void)
{
    return (&avb_ops_user);
}

bool avb_is_device_unlocked(void)
{
    AvbABOps* ab_ops;
    bool is_device_state_unlocked = false;

    ab_ops = avb_get_ab_ops();
    if(ab_ops != NULL && ab_ops->ops != NULL && ab_ops->ops->read_is_device_unlocked != NULL)
    {
        ab_ops->ops->read_is_device_unlocked(ab_ops->ops, &is_device_state_unlocked);
    }

    return is_device_state_unlocked;
}

void avb_verify_boot_start(char *partition_name, uint32_t boot_addr)
{
    // android boot
    struct andr_img_hdr *boot_hdr;
    uint8_t *boot_img_addr = NULL, *kernel_addr = NULL, *ramdisk_addr = NULL;
    int boot_img_size = 0;
    char *boot_partition_name = "boot";
    char *local_args[3];
    char local_args_temp[3][16];
    char env_temp[64];

    int i = 0;
    // android verify boot 2.0
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
    AvbABFlowResult avb_ab_flow_result;
    #else
    AvbSlotVerifyResult avb_slot_verify_result;
    #endif
    AvbSlotVerifyData *avb_data = NULL;

    char *avb_ab_partitions[] = { "boot",
    #ifdef CC_SUPPORT_DTO
                                  "dtbo",
    #endif
                                  NULL};

    printf("\n###### Android Verify Boot 2.0 ######\n\n");

    avb_ops_uboot_init();

    // load device_state and rollback_index
    avb_load_info_from_rpmb();

    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        avb_ab_flow_result = avb_ab_flow( avb_get_ab_ops(),
                                          avb_ab_partitions,
                                          avb_is_device_unlocked(),
                                          AVB_HASHTREE_ERROR_MODE_RESTART,
                                          &avb_data);
        if(avb_ab_flow_result == AVB_AB_FLOW_RESULT_OK
            || avb_ab_flow_result == AVB_AB_FLOW_RESULT_OK_WITH_VERIFICATION_ERROR)
    #else
    if(strcmp(partition_name,"recovery") == 0)
    {
        for(i=0; avb_ab_partitions[i] != NULL; i++)
        {
            if(strcmp(avb_ab_partitions[i], "boot") == 0)
            {
                avb_ab_partitions[i] = "recovery";
                break;
            }
        }
    }
    avb_slot_verify_result = avb_slot_verify(avb_get_ops(),
                                            avb_ab_partitions,
                                            "",
                                            avb_is_device_unlocked(),
                                            AVB_HASHTREE_ERROR_MODE_RESTART,
                                            &avb_data);
    if( avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_OK
       ||  ( (avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_VERIFICATION
       || avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_ROLLBACK_INDEX
       || avb_slot_verify_result == AVB_SLOT_VERIFY_RESULT_ERROR_PUBLIC_KEY_REJECTED)
       && avb_is_device_unlocked()) )
    #endif
    {
        #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        extern void env_add_boot_skip_initramfs(void);
        extern void env_add_boot_slot_suffix(char* slot_suffix);

        printf("\nSystem will boot from slot: %s\n",avb_data->ab_suffix);

        if (strcmp(avb_data->ab_suffix, "_b") == 0)
        {
            //extern unsigned int avb_slot_number;
            //avb_slot_number = 1;
        }

        // append command line.
        if(strcmp(partition_name,"boot") == 0)
        {
            env_add_boot_skip_initramfs();
        }
        env_add_boot_slot_suffix(avb_data->ab_suffix);
        #else
        extern int update_stored_rollback_index(AvbOps* ops, AvbSlotVerifyData *avb_data);
        #endif

        // this need to add to bootargs
        printf("\n## AVB2.0 append extra bootargs:\n%s\n\n",avb_data->cmdline);
        env_add_bootargs(NULL, avb_data->cmdline);

        #ifndef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        boot_partition_name = partition_name;
        #endif

        //get boot image loaded address.
        for(i=0; i < avb_data->num_loaded_partitions; i++)
        {
            if(strcmp(boot_partition_name, avb_data->loaded_partitions[i].partition_name) == 0)
            {
                boot_img_addr = avb_data->loaded_partitions[i].data;
                boot_img_size = avb_data->loaded_partitions[i].data_size;
                break;
            }
        }

        if(boot_img_addr)
        {
            boot_hdr = (struct andr_img_hdr *)boot_img_addr;
            kernel_addr = boot_img_addr + WITH_PADDING_SIZE(sizeof(struct andr_img_hdr), boot_hdr->page_size);
            ramdisk_addr = kernel_addr + WITH_PADDING_SIZE(boot_hdr->kernel_size, boot_hdr->page_size);

            // config ramdisk.
            sprintf(env_temp, "0x%x", ramdisk_addr);
            setenv("initrd", env_temp);
            sprintf(env_temp, "0x%x", ramdisk_addr + boot_hdr->ramdisk_size);
            setenv("initrd_end", env_temp);
        }

        #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        check_android_boot_state(avb_data, avb_ab_flow_result, (boot_hdr == NULL ? 0 : boot_hdr->os_version));
        #else
        check_android_boot_state(avb_data, avb_slot_verify_result, (boot_hdr == NULL ? 0 : boot_hdr->os_version));
        update_stored_rollback_index(avb_get_ops(),avb_data);
        #endif

        avb_load_boot_image(partition_name, boot_addr, boot_img_size, avb_data);
    }
    else
    {
        //red state.
        #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        check_android_boot_state(avb_data, avb_ab_flow_result, 0);
        #else
        check_android_boot_state(avb_data, avb_slot_verify_result, 0);
        #endif
    }
}
