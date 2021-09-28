
#include <common.h>
#include <command.h>

#include "avb_ab_interface.h"
#include "avb_ops_uboot.h"
#include "../libavb_ab/libavb_ab.h"
#include "../libavb_ab_uboot/avb_rpmb.h"

static const char* slot_suffixes[2] = {"_a", "_b"};

static bool slot_is_bootable(AvbABSlotData* slot) {
  return slot->priority > 0 &&
         (slot->successful_boot || (slot->tries_remaining > 0));
}

int ab_getCurrentSlotIndex(void)
{
    AvbABOps* ab_ops;
    AvbABData ab_data;
    AvbIOResult io_ret;
    size_t bootSlot = -1;

    avb_ops_uboot_init();
    ab_ops = avb_get_ab_ops();

    io_ret = ab_ops->read_ab_metadata(ab_ops, &ab_data);
    if (io_ret != AVB_IO_RESULT_OK)
    {
        printf("%s:%d: I/O error while loading A/B metadata.\n",__FILE__,__LINE__);
        bootSlot = -1;
        return bootSlot;
    }

    if (slot_is_bootable(&ab_data.slots[0]) &&
        slot_is_bootable(&ab_data.slots[1]))
    {
        if (ab_data.slots[1].priority > ab_data.slots[0].priority)
        {
            bootSlot = 1;
        } else
        {
            bootSlot = 0;
        }
    }
    else if (slot_is_bootable(&ab_data.slots[0]))
    {
        bootSlot = 0;
    }
    else if (slot_is_bootable(&ab_data.slots[1]))
    {
        bootSlot = 1;
    }
    else
    {
        printf("%s:%d: There is no bootable slot.\n",__FILE__,__LINE__);
        bootSlot = -1;
    }

    return bootSlot;
}

int ab_getSlotRetryCount(int slotNumber)
{
    AvbABOps* ab_ops;
    AvbABData ab_data;
    AvbIOResult io_ret;

    avb_ops_uboot_init();
    ab_ops = avb_get_ab_ops();

    io_ret = ab_ops->read_ab_metadata(ab_ops, &ab_data);
    if (io_ret != AVB_IO_RESULT_OK)
    {
        printf("%s:%d: I/O error while loading A/B metadata.\n",__FILE__,__LINE__);
        return -1;
    }

    return ab_data.slots[slotNumber].tries_remaining;
}

int ab_getSlotSuccessful(int slotNumber)
{
    AvbABOps* ab_ops;
    AvbABData ab_data;
    AvbIOResult io_ret;

    avb_ops_uboot_init();
    ab_ops = avb_get_ab_ops();

    io_ret = ab_ops->read_ab_metadata(ab_ops, &ab_data);
    if (io_ret != AVB_IO_RESULT_OK)
    {
        printf("%s:%d: I/O error while loading A/B metadata.\n",__FILE__,__LINE__);
        return -1;
    }

    return ab_data.slots[slotNumber].successful_boot;
}

int ab_getSlotUnbootable(int slotNumber)
{
    AvbABOps* ab_ops;
    AvbABData ab_data;
    AvbIOResult io_ret;

    avb_ops_uboot_init();
    ab_ops = avb_get_ab_ops();

    io_ret = ab_ops->read_ab_metadata(ab_ops, &ab_data);
    if (io_ret != AVB_IO_RESULT_OK)
    {
        printf("%s:%d: I/O error while loading A/B metadata.\n",__FILE__,__LINE__);
        return -1;
    }

    return (!slot_is_bootable(&ab_data.slots[slotNumber]));
}

int ab_setActiveSlot(int slotNumber)
{
    avb_ops_uboot_init();
    if(avb_ab_mark_slot_active(avb_get_ab_ops(),slotNumber) == AVB_IO_RESULT_OK)
    {
        return 0;
    }
    else
    {
        printf("%s:%d: set active slot fail.\n",__FILE__,__LINE__);
        return -1;
    }
}

bool ab_appendSlotSuffix(char* out_data, const char* prefix_str)
{
    size_t slot_index;

    slot_index = ab_getCurrentSlotIndex();
    if(slot_index >=0)
    {
        strcpy(out_data,prefix_str);
        strcat(out_data,slot_suffixes[slot_index]);
        return true;
    }
    else
    {
        printf("%s:%d: There is no bootable slot.\n",__FILE__,__LINE__);
        return false;
    }
}

#if defined(CONFIG_AVB_DEBUG) && defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
static int avb_read_rpmb_raw_data(uint8_t *block_data, UBOOT_RPMB_RAW_DATA *uboot_raw_data)
{
    int ret = 0;
    ret = ubootRpmbReadData(block_data);
    if(ret != 0)
    {
        printf("%s:%s:%d Fail to read rpmb data\n", __FILE__, __FUNCTION__, __LINE__);
        return;
    }

    eMMC_RPMB_DATA *rpmb_data_ptr = block_data;

    memset(uboot_raw_data, 0, sizeof(uboot_raw_data));
    // avb only uses 128 bytes in block_data
    memcpy(uboot_raw_data, rpmb_data_ptr->u8_data, 128);

    // data need to reverse to match UBOOT_RPMB_RAW_DATA structure
    _ReverseEndian(uboot_raw_data, 128);
}

static int avb_setDeviceState(char *in_value)
{
    int i=0;
    uint16_t value = 0;
    uint8_t block_data[RPMB_BLOCK_SIZE] __attribute__((aligned(16))) = {0};
    UBOOT_RPMB_RAW_DATA uboot_raw_data;
    memset(&uboot_raw_data, 0, sizeof(uboot_raw_data));

    avb_read_rpmb_raw_data(&block_data, &uboot_raw_data);

    if( in_value != NULL )
        value = simple_strtoull(in_value, NULL, 10);
    else
        value = 0;

    if( value != 0 && value != 1 )
    {
        printf("%s: Device State:%d is not support.\n",__FUNCTION__, value);
        return -1;
    }

    uboot_raw_data.device_state = value;

    // not use for now, always set to 0
    uboot_raw_data.index_counts = 0;

    ubootRpmbWriteData(block_data, uboot_raw_data);

    printf("%s: Device State is set to %s.\n",__FUNCTION__, (value==0?"UNLOCK":"LOCK"));
    return 0;
}

static int avb_clearRollbackIndex(char *in_location, char *in_value)
{
    uint8_t block_data[RPMB_BLOCK_SIZE] __attribute__((aligned(16))) = {0};
    UBOOT_RPMB_RAW_DATA uboot_raw_data;
    memset(&uboot_raw_data, 0, sizeof(uboot_raw_data));

    avb_read_rpmb_raw_data(&block_data, &uboot_raw_data);

    int i = 0;
    int location = 0;
    uint64_t value = 0;

    if( in_location != NULL )
        location = simple_strtoull(in_location, NULL, 10);
    else
        location = -1;

    if( in_value != NULL )
        value = simple_strtoull(in_value, NULL, 10);
    else
        value = 0;

    for(i=0;i<AVB_MAX_NUMBER_OF_ROLLBACK_INDEX_LOCATIONS;i++)
    {
        if( location < 0)
        {
            uboot_raw_data.rollback_index[i] = value;
            printf("[%s] Update rollback_index[%d] to %llu\n", __FUNCTION__, i, value);
        }
        else
        {
            if( location == i )
            {
                uboot_raw_data.rollback_index[i] = value;
                printf("[%s] Update rollback_index[%d] to %llu\n", __FUNCTION__, i, value);
            }
        }
    }

    // not use for now, always set to 0
    uboot_raw_data.index_counts = 0;

    ubootRpmbWriteData(block_data, uboot_raw_data);

    printf("%s: Rollback index data has been set.\n",__FUNCTION__);
    return 0;
}

static int avb_dump_avb_rpmb_data(void)
{
    uint32_t ret = 1;
    uint8_t block_data[RPMB_BLOCK_SIZE] __attribute__((aligned(16))) = {0};
    UBOOT_RPMB_RAW_DATA uboot_rpmb_raw_data;
    int i = 0;

    avb_read_rpmb_raw_data(&block_data, &uboot_rpmb_raw_data);

    printf("uboot_rpmb_raw_data.device_state=%d\n", uboot_rpmb_raw_data.device_state);
    printf("uboot_rpmb_raw_data.index_counts=%d\n", uboot_rpmb_raw_data.index_counts);

    printf("%s: dump rollback_index:\n", __FUNCTION__);
    for( i=0; i< AVB_MAX_NUMBER_OF_ROLLBACK_INDEX_LOCATIONS; i++)
        printf("rollback_index[%d]:%d\n", i, uboot_rpmb_raw_data.rollback_index[i]);
    printf("\n");
    return 0;
}
#endif

int do_avbab(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    int i = 0, ret = 0;

    if(argc == 1 || strcmp(argv[1],"?") == 0) // for help information.
    {
        printf("%s\n", cmdtp->usage);
    }
    else
    {
        if(strcmp(argv[1],"enable-verity") == 0)
        {
            if(argc == 2)
            {
                AvbABOps* ab_ops;
                bool device_is_unlocked = false;
                avb_ops_uboot_init();
                ab_ops = avb_get_ab_ops();
                ab_ops->ops->read_is_device_unlocked(ab_ops->ops, &device_is_unlocked);
                if(device_is_unlocked == true)
                {
                    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
                    avb_set_verity(ab_ops->ops,"_a",true);
                    avb_set_verity(ab_ops->ops,"_b",true);
                    #else
                    avb_set_verity(ab_ops->ops,"",true);
                    #endif
                }
                else
                {
                    printf("%s: Error, device state is locked, don't allow to on/off dm-verity.\n",__FUNCTION__);
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
        else if(strcmp(argv[1],"disable-verity") == 0)
        {
            if(argc == 2)
            {
                AvbABOps* ab_ops;
                bool device_is_unlocked = false;
                avb_ops_uboot_init();
                ab_ops = avb_get_ab_ops();
                ab_ops->ops->read_is_device_unlocked(ab_ops->ops, &device_is_unlocked);
                if(device_is_unlocked == true)
                {
                    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
                    avb_set_verity(ab_ops->ops,"_a",false);
                    avb_set_verity(ab_ops->ops,"_b",false);
                    #else
                    avb_set_verity(ab_ops->ops,"",false);
                    #endif
                }
                else
                {
                    printf("%s: Error, device state is locked, don't allow to on/off dm-verity.\n",__FUNCTION__);
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
        else if(strcmp(argv[1],"set_active") == 0)
        {
            if(argc == 3)
            {
                if(strcmp(argv[2],"a") == 0 || strcmp(argv[2],"_a") == 0)
                {
                    if(ab_setActiveSlot(0))
                    {
                        ret = 1;
                    }
                }
                else if(strcmp(argv[2],"b") == 0 || strcmp(argv[2],"_b") == 0)
                {
                    if(ab_setActiveSlot(1))
                    {
                        ret = 1;
                    }
                }
                else
                {
                    printf("%s: Error, unknown parameter <%s> for sub command: %s\n",__FUNCTION__,argv[2],argv[1]);
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
    #endif
    #if defined(CONFIG_AVB_DEBUG) && defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
        else if(strcmp(argv[1],"set_device_state") == 0)
        {
            if(argc == 2)
            {
                if(avb_setDeviceState(NULL))
                {
                    ret = 1;
                }
            }
            else if(argc == 3)
            {
                if(avb_setDeviceState(argv[2]))
                {
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
        else if(strcmp(argv[1],"clear_rollback_index") == 0)
        {
            if(argc == 2)
            {
                if(avb_clearRollbackIndex(NULL, NULL))
                {
                    ret = 1;
                }
            }
            else if(argc == 3)
            {
                if(avb_clearRollbackIndex(NULL, argv[2]))
                {
                    ret = 1;
                }
            }
            else if(argc == 4)
            {
                if(avb_clearRollbackIndex(argv[2], argv[3]))
                {
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
        else if(strcmp(argv[1],"dump") == 0)
        {
            if(argc == 2)
            {
                if(avb_dump_avb_rpmb_data())
                {
                    ret = 1;
                }
            }
            else
            {
                printf("%s: Error, incorrect numbers of parameter for sub command: %s\n",__FUNCTION__,argv[1]);
                ret = 1;
            }
        }
    #endif
        else
        {
            printf("%s: Error, unknown sub command: %s\n",__FUNCTION__,argv[1]);
            ret = 1;
        }

        for(i=0; i<argc; i++)
        {
            printf("%s ",argv[i]);
        }
        if(ret){
            printf("Fail !!\n");
        } else {
            printf("Done !!\n");
        }
    }

    return ret;
}

U_BOOT_CMD(
    avbab, 4, 1, do_avbab,
    "\n\t  enable-verity     - AVB2.0 enable dm-verity."
    "\n\t  disable-verity    - AVB2.0 disable dm-verity."
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
    "\n\t  set_active [slot] - set slot a/b to active slot."
    #endif
    #if defined(CONFIG_AVB_DEBUG) && defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH)
    "\n\t  set_device_state [value]     - Set device_state to [value], lock = 1, unlock = 0, default value = 0."
    "\n\t  clear_rollback_index [location] [value]     - Set Rollback Index[location] to [value], default value = 0."
    "\n\t  dump              - Dump device state and rollback index."
    #endif
    "\n",
    NULL
    );