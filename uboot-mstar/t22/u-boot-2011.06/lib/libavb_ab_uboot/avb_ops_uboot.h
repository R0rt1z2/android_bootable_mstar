
#ifndef AVB_OPS_UBOOT_H_
#define AVB_OPS_UBOOT_H_

#include "../libavb_ab/libavb_ab.h"

#if(ENABLE_MMC==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_MMC
#elif (ENABLE_NAND_FLASH==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_NAND
#else
#error
#endif

void avb_ops_uboot_init(void);
AvbABOps* avb_get_ab_ops(void);
AvbOps* avb_get_ops(void);
size_t avb_get_part_number(const char* partition, int *partNum);
void avb_verify_boot_start(char *partition_name, uint32_t boot_addr);
int read_device_state(uint16_t *devicestate);
int write_device_state(const uint16_t devicestate);

#endif