/*
  * Copyright (C) 2015 - 2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
*/
#ifdef SUPPORT_UBOOT
#include <common.h>
#include <config.h>
#if !defined(UFBL_PROJ_ABC) && !defined(UFBL_PROJ_ABC) && !defined(UFBL_PROJ_ABC) \
    && !defined(UFBL_PROJ_SOPHIA) && !defined(UFBL_PROJ_ABC) && !defined(UFBL_PLAT_MTK_T31) \
    && !defined(UFBL_PROJ_MONTANA)
#include <vsprintf.h>
#endif
#include <linux/string.h>
#if defined(UFBL_PROJ_ABC) || defined(UFBL_PROJ_ABC) || defined(UFBL_PROJ_ABC) \
    && !defined(UFBL_PROJ_SOPHIA) && !defined(UFBL_PROJ_ABC) && !defined(UFBL_PLAT_MTK_T31) \
    && !defined(UFBL_PROJ_MONTANA)
#include <stdio.h>
#endif
#endif

/*!
 * @file idme.h
 * @brief This file contains functions for interacting with variables
 *  in the userstore partition
 *
 */

#ifndef __IDME_H__
#define __IDME_H__

#if defined(UFBL_FASTBOOT_RESPONSE) || defined(CONFIG_IDME_FASTBOOT_RESPONSE)
int fastboot_idme(char *response, const char *cmd);
#else
int fastboot_idme(const char *cmd);
#endif

/**
 * @brief Macro definition for fastboot_info and fastboot_fail, which have
 *        different parameter on different platform
 */
#if defined(UFBL_PROJ_HOYA)
#define UFBL_FASTBOOT_FAIL(response,buf) \
	fastboot_info(buf,response)
#define UFBL_FASTBOOT_INFO(response,buf) \
	fastboot_info(buf,response)
#else
#define UFBL_FASTBOOT_FAIL(response,buf) \
	fastboot_fail(response,buf)
#define UFBL_FASTBOOT_INFO(response,buf) \
	fastboot_info(response,buf)
#endif

/**
 * @brief Report values of or modify IDME variables. cmdline is 'idme xyz abc'
 *
 * @parmas  flag is deprecated and not used. Its here for compatibility with uboot
 *         [OUT] response is optional output for fastboot response string.
 *         [IN] argc is number of values in argument list argv, at least one
 *              argument is expected, argc <= 0 results into error
 *         [IN] argv is expected to be "idme" to be compatible with
 *              all previous platforms or bootloader commandlines.
 * @return  int value of 0 for success and -1 for error
 */
#ifdef UFBL_FASTBOOT_RESPONSE
int do_idme(char *response, int flag, int argc, char * const argv[]);
#else
int do_idme(int flag, int argc, char * const argv[]);
#endif

#define MAGIC_NUMBER_OFFSET 0
#define IDME_MAGIC_NUMBER "beefdeed"
#define IDME_BASE_ADDR 8        /* magic number and size takes 8 bytes */
#define MAX_IDME_VALUE 2560
#define IDME_ALIGN_SIZE 4

/* base address of idme location on user partition */
#if !defined(CONFIG_USERDATA_IDME_ADDR)
#define CONFIG_USERDATA_IDME_ADDR 0x00
#endif
#define CONFIG_MMC_BLOCK_SIZE 512

#if !defined(IDME_NUM_OF_EMMC_BLOCKS)
#ifdef SUPPORT_UBOOT
#define IDME_NUM_OF_EMMC_BLOCKS 10
#else
#define IDME_NUM_OF_EMMC_BLOCKS 30
#endif
#endif
/* total size of 5120 bytes - 10 NAND native blocks*/
#define CONFIG_IDME_SIZE (CONFIG_MMC_BLOCK_SIZE * IDME_NUM_OF_EMMC_BLOCKS)
#if !defined(CONFIG_IDME_PARTITION_NUM)
#define CONFIG_IDME_PARTITION_NUM 2
#endif

//idme size limit
#define IDME_MAX_NAME_LEN 16
#define IDME_MAX_IDME_ITEM_SIZE CONFIG_IDME_SIZE
#define IDME_MAX_PRINT_SIZE 40
#if !defined(IDME_MAX_DT_PRINT_SIZE)
#define IDME_MAX_DT_PRINT_SIZE 1024
#endif
#define IDME_ATAG_SIZE 2048

/*
Define idme atag address, which is same as OMAP platform.
Because the addresses for CORE, MEM and CMDLINE are the same
*/
#define ATAG_IDME 0x54410010

#define IDME_VER_TAB_LEN	4
#include "idme_v2_0.h"

#if !defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PROJ_ABC)
#include <stdlib.h>
#endif

#define IDME_DEFAULT_VERSION	IDME_VERSION_2P1

enum {
	IDME_BOOTMODE_NORMAL = 1,
	IDME_BOOTMODE_DIAG = 2,
	IDME_BOOTMODE_RECOVERY = 3,
	IDME_BOOTMODE_EMERGENCY = 4,
	IDME_BOOTMODE_POWERSAVE = 5,
	IDME_BOOTMODE_FASTBOOT = 6,
	IDME_BOOTMODE_TRANSITION = 7,
	IDME_BOOTMODE_WSWDL = 8,
	/* After rebooting into shipping software from diag software, display logo and enter standby mode.*/
	IDME_BOOTMODE_STANDBY_LOGO_POST_SHIPPING_SW_SWITCH = 9,
	IDME_BOOTMODE_MAX
};

#ifndef MIN
#define MIN(x,y) ((x) > (y)? (y):(x))
#endif

#ifndef countof
#define countof(a) (sizeof(a) / sizeof((a)[0]))
#endif

// flags bitmask
#define FOS_FLAGS_NONE                  (0x00000000ull)
#define FOS_FLAGS_ADB_ON                (0x00000001ull)
#define FOS_FLAGS_ADB_ROOT              (0x00000002ull)
#define FOS_FLAGS_CONSOLE_ON            (0x00000004ull)
#define FOS_FLAGS_RAMDUMP_ON            (0x00000008ull)
#define FOS_FLAGS_VERBOSITY_ON          (0x00000010ull)
#define FOS_FLAGS_ADB_AUTH_DISABLE      (0x00000020ull)
#define FOS_FLAGS_FORCE_DM_VERITY       (0x00000040ull) /* Deprecated - DO NOT USE */
#define FOS_FLAGS_DM_VERITY_OFF         (0x00000080ull)
#define FOS_FLAGS_BOOT_DEXOPT           (0x00000100ull)
#define FOS_FLAGS_MAX                   (0x00000100ull)

#define FOS_FLAGS_STR_MAX_LEN           (32)
#define FOS_FLAGS_TO_STR(FLAG) #FLAG

#if !defined(SUPPORT_KERNELFLINGER)
static char* idme_flags_table[] = {
	FOS_FLAGS_TO_STR(FOS_FLAGS_NONE),
	FOS_FLAGS_TO_STR(FOS_FLAGS_ADB_ON),
	FOS_FLAGS_TO_STR(FOS_FLAGS_ADB_ROOT),
	FOS_FLAGS_TO_STR(FOS_FLAGS_CONSOLE_ON),
	FOS_FLAGS_TO_STR(FOS_FLAGS_RAMDUMP_ON),
	FOS_FLAGS_TO_STR(FOS_FLAGS_VERBOSITY_ON),
	FOS_FLAGS_TO_STR(FOS_FLAGS_ADB_AUTH_DISABLE),
	FOS_FLAGS_TO_STR(FOS_FLAGS_FORCE_DM_VERITY), /* Deprecated - DO NOT USE */
	FOS_FLAGS_TO_STR(FOS_FLAGS_DM_VERITY_OFF),
	FOS_FLAGS_TO_STR(FOS_FLAGS_BOOT_DEXOPT),
};
#endif

#if !defined(SUPPORT_KERNELFLINGER) && \
	!defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC)
// Decode a flags bitmask to symbolic representation
// Return value:
//  zero indicates success
//  non zero indicates failure, with return value being the buffer size needed
static inline int fos_flags_to_str(unsigned long long flags, char* buf, unsigned int bufsize) {
	unsigned int size_needed = 0;
	unsigned int s = 0; // size used so far of buf
	unsigned int pos = 1;

	if(!buf)
		bufsize = 0;

	do {
		if(flags & 0x1 || (!flags && !--pos)) {
			size_needed += snprintf(buf+s, bufsize-s, "%s%s", idme_flags_table[pos], (flags<=1)?"":"|");
			s = MIN(size_needed, bufsize);
		}
		pos++;
	} while( (flags >>= 1) && (pos < countof(idme_flags_table)));

	if(flags) // handle unknown flag bits
		size_needed += snprintf(buf+s, bufsize-s,"!UNKNOWN FLAGS 0x%llx!", flags<<(pos-1));

	return (size_needed <= bufsize)? 0 : size_needed + 1;
}
#endif

struct idme_desc {
	char name[IDME_MAX_NAME_LEN];
	unsigned int size;
	unsigned int exportable;
	unsigned int permission;
};

struct item_t {
	struct idme_desc desc;
	unsigned char data[0];
};

struct idme_t {
	char magic[8];
	char version[4];
	unsigned int items_num;

	unsigned char item_data[0];
};

struct idme_ver_t {
	char version[4];
	enum version_type {
		IDME_VER_1P2 = 0,
		IDME_VER_2P0,
		IDME_VER_2P1,
		IDME_VER_3P0
	} ver_type;
};

/* Defines the initial values along with the IDME tag */
struct idme_init_values {
	struct idme_desc desc;
	void *value;
};

int idme_initialize(void);
void idme_fastboot_register(void);
int idme_atag_initialize(void *atag_buffer, unsigned int size);
int idme_device_tree_initialize(void *fdt);
int idme_check_magic_number(struct idme_t *pidme);
int idme_update_var_ex(const char *name, const char *value, unsigned int length);
int idme_clean(void);
void idme_boot_info(void);
int idme_boot_mode(void);
/* For external access to idme fields */
int idme_get_var_external(const char *name, char *buf, unsigned int length);
/* platform dependent */
int idme_platform_read(unsigned char *pbuf);
int idme_platform_write(const unsigned char *pbuf);
const char *idme_board_id(void);

#endif /* __IDME_H__ */

