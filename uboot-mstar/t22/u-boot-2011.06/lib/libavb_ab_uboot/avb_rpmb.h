#include "eMMC_RPMB.h"
#include "libavb.h"

#define RPMB_NONCE_SIZE (16)
#define RPMB_DATA_SIZE (256)

#ifndef SHA256_HASH_SIZE
#define SHA256_HASH_SIZE (32)
#endif

#define RPMB_BLOCK_SIZE (512)
#define RPMB_EMMC_CID_SIZE (16)
#define RPMB_NONCE_SIZE (16)

#define RPMB_UBOOT_MAX_SIZE (128)
#define AVB_ROLLBACK_INDEX_MAX_COUNT AVB_MAX_NUMBER_OF_ROLLBACK_INDEX_LOCATIONS

typedef struct RPMB_DATAFRAM {
    unsigned short type;
    unsigned short result;
    unsigned short blkCnt;
    unsigned short addr;
    uint32_t writeCnt;
    uint8_t nonce[RPMB_NONCE_SIZE];
    uint8_t data[RPMB_DATA_SIZE];
    uint8_t mac[SHA256_HASH_SIZE];
    uint8_t stuff[196];
} RPMB_DATAFRAM;

typedef enum {
    REQ_SET_KEY = 0x1,
    REQ_GET_CNT = 0x2,
    REQ_WR_DATA = 0x3,
    REQ_RD_DATA = 0x4,
    REQ_RD_RESULT = 0x5,
} RPMB_REQ;

typedef enum {
    RESP_SET_KEY = 0x100,
    RESP_GET_CNT = 0x200,
    RESP_WR_DATA = 0x300,
    RESP_RD_DATA = 0x400,
} RPMB_RESP;

typedef enum {
    RESULT_OK = 0x0,
    RESULT_NG = 0x1,
    RESULT_ATH_NG = 0x2,
    RESULT_CNT_NG = 0x3,
    RESULT_ADR_NG = 0x4,
    RESULT_WR_NG = 0x5,
    RESULT_RD_NG = 0x6,
    RESULT_NO_KEY = 0x7,
    RESULT_TRYLOCK_FAIL = 0x8,
    RESULT_DECRYPT_FAIL = 0x80,
} RPMB_RESULT;

typedef enum {
    RPMB_WV_GENERATION = 0x0,
    RPMB_PR_GENERATION = 0x1,
    RPMB_AVB_GENERATION_1 = 0x2,
    RPMB_AVB_GENERATION_2 = 0x3,
} RPMB_ADDR;

typedef struct{
    uint16_t device_state; // 0 == unlock, 1 == lock
    uint16_t index_counts;
    uint64_t rollback_index[AVB_ROLLBACK_INDEX_MAX_COUNT];
} UBOOT_RPMB_RAW_DATA;

typedef struct{
    uint32_t uboot_nonce_magic;
    uint8_t nonce[RPMB_NONCE_SIZE];
} UBOOT_READ_NONCE;

typedef struct{
    uint32_t uboot_read_magic;
    uint8_t cid[RPMB_EMMC_CID_SIZE];
    uint8_t block_data[RPMB_BLOCK_SIZE];
} UBOOT_READ_RPMB;

typedef struct{
    uint32_t uboot_write_magic;
    uint32_t writer_counter;
    uint8_t cid[RPMB_EMMC_CID_SIZE];
    uint8_t block_data[RPMB_BLOCK_SIZE];
    UBOOT_RPMB_RAW_DATA uboot_raw_data; // raw data from uboot
} UBOOT_WRITE_RPMB;

uint32_t ubootRpmbReadData(uint8_t *data);
uint32_t ubootRpmbWriteData(uint8_t *block_data, UBOOT_RPMB_RAW_DATA uboot_raw_data);
void _ReverseEndian(void *data, size_t size);