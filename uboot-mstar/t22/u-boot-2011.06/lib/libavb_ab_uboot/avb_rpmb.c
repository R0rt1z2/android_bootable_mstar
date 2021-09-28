/* ============================================================================
    Includes
============================================================================ */
#include "avb_rpmb.h"
#include "MsOS.h"

/* ============================================================================
    Constant definitions
============================================================================ */
#ifndef UINT_MAX
#define UINT_MAX 4294967295U
#endif

#define UBOOT_READ_RPMB_MAGIC 0x5552524d
#define UBOOT_WRITE_RPMB_MAGIC 0x5557524d
#define UBOOT_GET_NONCE_MAGIC 0x55474e4d

#define RPMB_OPTEE_ACCESS_ADDR 0xb200585F

#define STD_RPMB_OPTEE_ACCESS_ADDR 0x3200ff04
#define OPTEE_SMC_CALL_RETURN_FROM_RPC 0x32000003

#define GET_OPTEE_VERSION_NUMBER_SMC_CMD (0xb2000001)

/* ============================================================================
    Global Variable
============================================================================ */
uint32_t g_optee_version_number;
int is_std_init = 0;
uint32_t g_optee_shm_adr;

/* ============================================================================
    Internal Functions
============================================================================ */

struct smc_param {
    U32 a0;
    U32 a1;
    U32 a2;
    U32 a3;
    U32 a4;
    U32 a5;
    U32 a6;
    U32 a7;
};


static void init_std(void)
{
    if (is_std_init)
        return;

   is_std_init = 1;

    get_addr_from_mmap("E_MMAP_ID_OPTEE_MEM_SHM", &g_optee_shm_adr);
    g_optee_shm_adr += CONFIG_SYS_MIU0_BUS;

    struct smc_param get_optee_version_smc_params;
    memset(&get_optee_version_smc_params, 0, sizeof(get_optee_version_smc_params));
    get_optee_version_smc_params.a0 = GET_OPTEE_VERSION_NUMBER_SMC_CMD;

#if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
    tee_smc_call(&get_optee_version_smc_params);
#endif

   g_optee_version_number =  get_optee_version_smc_params.a0;
   printf("[%s] OPTEE version is %d\n", __func__, g_optee_version_number);
}

void _ReverseEndian(void *data, size_t size)
{
    uint32_t i;
    char tmp;
    char *swp = (char *)data;
    for (i = 0; i < (size / 2); ++i) {
        tmp = swp[i];
        swp[i] = swp[size - 1 - i];
        swp[size - 1 - i] = tmp;
    }
}

static inline void reg_pair_from_64(u32 *reg0, u32 *reg1, u64 val)
{
    *reg0 = val >> 32;
    *reg1 = val;
}

static int32_t ubootRpmbWriteCounter(void) {
    int32_t ret = 0;
    volatile U32 write_cnt = 0;

    if(eMMC_RPMB_IfKeyWritten() != 1)
    {
        printf("%s:%s:%d There is no key in emmc\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    ret = eMMC_RPMB_Get_Counter_for_cmd(&write_cnt);
    if( ret != eMMC_ST_SUCCESS )
    {
        printf("%s:%s:%d Fail to get rpmb write counter\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    return write_cnt;
}

static uint32_t ubootGetNonce(u8 *nonce)
{
    uint32_t ret = 0;
    UBOOT_READ_NONCE* uboot_read_nonce = NULL;
    uboot_read_nonce = (UBOOT_READ_NONCE*)malloc(sizeof(UBOOT_READ_NONCE));
    memset(uboot_read_nonce, 0, sizeof(UBOOT_READ_NONCE));

    uboot_read_nonce->uboot_nonce_magic = UBOOT_GET_NONCE_MAGIC;
    flush_cache(uboot_read_nonce, sizeof(UBOOT_READ_NONCE));

    struct smc_param rpmb_get_nonce_smc_params;
    memset(&rpmb_get_nonce_smc_params, 0, sizeof(rpmb_get_nonce_smc_params));

    init_std();


    if (g_optee_version_number == 1)
    {
        rpmb_get_nonce_smc_params.a0 = RPMB_OPTEE_ACCESS_ADDR;
        rpmb_get_nonce_smc_params.a1 = uboot_read_nonce;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        tee_smc_call(&rpmb_get_nonce_smc_params);
    #endif

        invalidate_dcache_range(rpmb_get_nonce_smc_params.a0, sizeof(rpmb_get_nonce_smc_params.a0));
        invalidate_dcache_range(rpmb_get_nonce_smc_params.a1, RPMB_NONCE_SIZE);

        ret = rpmb_get_nonce_smc_params.a0;
        memcpy(nonce, rpmb_get_nonce_smc_params.a1, RPMB_NONCE_SIZE);
    }
    else if (g_optee_version_number >= 2)
    {
        rpmb_get_nonce_smc_params.a0 = STD_RPMB_OPTEE_ACCESS_ADDR;
        rpmb_get_nonce_smc_params.a6 = uboot_read_nonce;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__); // this printf is for timing issue ???? without this, rpmb std call will fail
        tee_smc_call(&rpmb_get_nonce_smc_params);
    #endif

        rpmb_get_nonce_smc_params.a0 = OPTEE_SMC_CALL_RETURN_FROM_RPC;
        rpmb_get_nonce_smc_params.a6 = uboot_read_nonce;

        reg_pair_from_64(&(rpmb_get_nonce_smc_params.a1), &(rpmb_get_nonce_smc_params.a2), g_optee_shm_adr);
    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__);
        tee_smc_call(&rpmb_get_nonce_smc_params);
    #endif

        invalidate_dcache_range(rpmb_get_nonce_smc_params.a0, sizeof(rpmb_get_nonce_smc_params.a0));
        invalidate_dcache_range(rpmb_get_nonce_smc_params.a6, RPMB_NONCE_SIZE);

        ret = rpmb_get_nonce_smc_params.a0;
        memcpy(nonce, rpmb_get_nonce_smc_params.a6, RPMB_NONCE_SIZE);
    }
    else
    {
        printf("%s:%s:%d Incorrect OPTEE version number\n", __FILE__, __FUNCTION__, __LINE__);
    }

    free(uboot_read_nonce);
    return ret;
}

/* ============================================================================
    Exported Entry Points
============================================================================ */
uint32_t ubootRpmbReadData(uint8_t *block_data)
{
    int32_t ret = 0;
    uint8_t cid[RPMB_EMMC_CID_SIZE] = {0};
    uint8_t nonce[RPMB_NONCE_SIZE] = {0};

    ret = eMMC_GetCID(&cid);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to get emmc cid\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    ret = ubootGetNonce(nonce);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to get nonce\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    _ReverseEndian(nonce, RPMB_NONCE_SIZE);

    ret = eMMC_RPMB_Read_Blk(block_data, nonce, 0);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to read rpmb block\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    UBOOT_READ_RPMB rmpb_read_data;
    uint32_t rmpb_read_data_size = sizeof(rmpb_read_data);
    memset(rmpb_read_data.block_data, 0, RPMB_BLOCK_SIZE);
    rmpb_read_data.uboot_read_magic = UBOOT_READ_RPMB_MAGIC;
    memcpy(rmpb_read_data.cid, cid, RPMB_EMMC_CID_SIZE);
    memcpy(rmpb_read_data.block_data, block_data, RPMB_BLOCK_SIZE);

    flush_cache(&rmpb_read_data, rmpb_read_data_size);

    struct smc_param rpmb_read_smc_params;
    memset(&rpmb_read_smc_params, 0, sizeof(rpmb_read_smc_params));

    init_std();


    if (g_optee_version_number == 1)
    {
        rpmb_read_smc_params.a0 = RPMB_OPTEE_ACCESS_ADDR;
        rpmb_read_smc_params.a1 = &rmpb_read_data;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        tee_smc_call(&rpmb_read_smc_params);
    #endif
        invalidate_dcache_range(rpmb_read_smc_params.a0, sizeof(rpmb_read_smc_params.a0));
        ret = rpmb_read_smc_params.a0;
    }
    else if (g_optee_version_number >= 2)
    {
        rpmb_read_smc_params.a0 = STD_RPMB_OPTEE_ACCESS_ADDR;
        rpmb_read_smc_params.a6 = &rmpb_read_data;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__); // this printf is for timing issue ???? without this, rpmb std call will fail
        tee_smc_call(&rpmb_read_smc_params);
    #endif

        rpmb_read_smc_params.a0 = OPTEE_SMC_CALL_RETURN_FROM_RPC;
        rpmb_read_smc_params.a6 = &rmpb_read_data;
        reg_pair_from_64(&(rpmb_read_smc_params.a1), &(rpmb_read_smc_params.a2), g_optee_shm_adr);
    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__);
        tee_smc_call(&rpmb_read_smc_params);
    #endif
        invalidate_dcache_range(rpmb_read_smc_params.a0, sizeof(rpmb_read_smc_params.a0));
        ret = rpmb_read_smc_params.a0;
    }
    else
    {
        printf("%s:%s:%d Incorrect OPTEE version number\n", __FILE__, __FUNCTION__, __LINE__);
    }

    _ReverseEndian(block_data, RPMB_BLOCK_SIZE);
    return ret;
}

uint32_t ubootRpmbWriteData(uint8_t *block_data, UBOOT_RPMB_RAW_DATA uboot_raw_data)
{
    int32_t ret = 0;
    uint8_t cid[RPMB_EMMC_CID_SIZE] = {0};
    uint8_t nonce[RPMB_NONCE_SIZE] = {0};
    uint32_t write_cnt = 0;

    write_cnt = ubootRpmbWriteCounter();
    if(write_cnt < 0)
    {
        printf("%s:%s:%d Fail to get rpmb write counter\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    ret = eMMC_GetCID(&cid);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to get emmc cid\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    ret = ubootGetNonce(nonce);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to get nonce\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    _ReverseEndian(nonce, RPMB_NONCE_SIZE);

    ret = eMMC_RPMB_Read_Blk(block_data, nonce, 0);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to read rpmb block\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    UBOOT_WRITE_RPMB *rmpb_write_data = NULL;
    rmpb_write_data = (UBOOT_WRITE_RPMB *)malloc(sizeof(UBOOT_WRITE_RPMB));
    rmpb_write_data->uboot_write_magic = UBOOT_WRITE_RPMB_MAGIC;
    rmpb_write_data->writer_counter = write_cnt;
    memcpy(&rmpb_write_data->cid, cid, RPMB_EMMC_CID_SIZE);
    memcpy(&rmpb_write_data->block_data, block_data, RPMB_BLOCK_SIZE);
    memcpy(&rmpb_write_data->uboot_raw_data, &uboot_raw_data, sizeof(UBOOT_RPMB_RAW_DATA));

    flush_cache(rmpb_write_data, sizeof(UBOOT_WRITE_RPMB));

    struct smc_param rpmb_write_smc_params;
    memset(&rpmb_write_smc_params, 0, sizeof(rpmb_write_smc_params));

    init_std();


    if (g_optee_version_number == 1)
    {
        rpmb_write_smc_params.a0 = RPMB_OPTEE_ACCESS_ADDR;
        rpmb_write_smc_params.a1 = rmpb_write_data;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        tee_smc_call(&rpmb_write_smc_params);
    #endif

        invalidate_dcache_range(rpmb_write_smc_params.a0, RPMB_BLOCK_SIZE);
        invalidate_dcache_range(rpmb_write_smc_params.a1, RPMB_BLOCK_SIZE);

        ret = rpmb_write_smc_params.a0;
        if(ret != eMMC_ST_SUCCESS)
        {
            printf("%s:%s:%d Fail to get rpmb block(with MAC)\n", __FILE__, __FUNCTION__, __LINE__);
            free(rmpb_write_data);
            return 1;
        }

        memcpy(block_data, rpmb_write_smc_params.a1, RPMB_BLOCK_SIZE);
    }
    else if (g_optee_version_number >= 2)
    {
        rpmb_write_smc_params.a0 = STD_RPMB_OPTEE_ACCESS_ADDR;
        rpmb_write_smc_params.a6 = rmpb_write_data;

    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__); // this printf is for timing issue ???? without this, rpmb std call will fail
        tee_smc_call(&rpmb_write_smc_params);
    #endif

        rpmb_write_smc_params.a0 = OPTEE_SMC_CALL_RETURN_FROM_RPC;
        rpmb_write_smc_params.a6 = rmpb_write_data;
        reg_pair_from_64(&(rpmb_write_smc_params.a1), &(rpmb_write_smc_params.a2), g_optee_shm_adr);
    #if defined(CONFIG_OPTEE_TEELOADER) || defined(CONFIG_MSTAR_NS_UBOOT)
        printf("%s\n", __func__);
        tee_smc_call(&rpmb_write_smc_params);
    #endif
        invalidate_dcache_range(rpmb_write_smc_params.a0, RPMB_BLOCK_SIZE);
        invalidate_dcache_range(rpmb_write_smc_params.a6, RPMB_BLOCK_SIZE);

        ret = rpmb_write_smc_params.a0;
        if(ret != eMMC_ST_SUCCESS)
        {
            printf("%s:%s:%d Fail to get rpmb block(with MAC)\n", __FILE__, __FUNCTION__, __LINE__);
            free(rmpb_write_data);
            return 1;
        }

        memcpy(block_data, rpmb_write_smc_params.a6, RPMB_BLOCK_SIZE);
    }
    else
    {
        printf("%s:%s:%d Incorrect OPTEE version number\n", __FILE__, __FUNCTION__, __LINE__);
    }
    _ReverseEndian(block_data, RPMB_BLOCK_SIZE);
    ret = eMMC_RPMB_Write_Blk(block_data);
    if(ret != eMMC_ST_SUCCESS)
    {
        printf("%s:%s:%d Fail to write RPMB\n", __FILE__, __FUNCTION__, __LINE__);
        free(rmpb_write_data);
        return 1;
    }

    free(rmpb_write_data);
    return 0;
}
