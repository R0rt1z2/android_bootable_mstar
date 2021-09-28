/*
  * Copyright (C) 2011-2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
*/
#if defined(SUPPORT_UBOOT) || defined(SUPPORT_BOLT)

#ifdef SUPPORT_UBOOT
#include <common.h>
#include <config.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#endif

#ifdef SUPPORT_BOLT
#include <platform/bcm_platform.h>
#endif

#include "ufbl_debug.h"

#else // no uboot or bolt

#include <stdlib.h>
#include <string.h>

#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC)
#include <debug.h>
#include <platform/mmc_core.h>
#else /* UFBL_TESTS UFBL_PROJ_ABC */
#include <stdio.h>
#include "ufbl_debug.h"
#endif /* UFBL_TESTS UFBL_PROJ_ABC */
#endif /* defined(SUPPORT_UBOOT) || defined(SUPPORT_BOLT) */

#include "idme.h"

#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
#include "fastboot.h"
#endif

/* Local variables */
#if defined(UFBL_PROJ_ABC)
/* ABC: the address is expected to be 64 aligned */
static unsigned char gidme_buff[CONFIG_IDME_SIZE] __aligned(64);
#else
/**
 * According to the definition of idme_t, the buffer need to be 32 bytes aligned at least,
 * we make it 64 bytes aligned here to make it compatible for DMA.
 */
static unsigned char gidme_buff[CONFIG_IDME_SIZE] __attribute__((aligned(64)));
#endif
static struct idme_t *pidme_data = NULL;

/* Local function prototype */
static int idme_null_func(void);

static int (*idme_init_var)(void *data);
static int (*idme_get_var)(const char *name, char *buf, unsigned int length, void *data);
static int (*idme_update_var)(const char *name, const char *value, unsigned int length, void *data);
#ifdef UFBL_FASTBOOT_RESPONSE
static int (*idme_print_var)(char *response, void *data);
#else
static int (*idme_print_var)(void *data);
#endif
static int (*idme_init_atag)(void *atag_buffer, void *data, unsigned int size);
static int (*idme_init_dev_tree)(void *fdt, void *data);
#ifdef IDME_UPDATE_TABLE
static int (*idme_update_table)(void *data);
#endif
static struct idme_desc* (*idme_get_item_desc)(void *data, char *item_name);
static int idme_read(unsigned char *pbuf);
static int idme_write(const unsigned char *pbuf);


/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')
/* defining atoi if stdlib is not there */
#if  !defined(__STDLIB_H)
#if  !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PROJ_SOPHIA) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(UFBL_PLAT_MTK_T31) && \
	!defined(UFBL_PROJ_MONTANA)

static int atoi(const char *s)
{
	int i = 0;
	int sign = 0;

	if (s[0] == '-') {
		sign = 1;
		s++;
	}
	while (*s && is_digit(*s))
		i = i*10 + *((s)++) - '0';

	if (sign)
		i = -i;

	return i;
}
#endif
static unsigned int atoui(const char *s)
{
	int i = 0;

	while (*s && is_digit(*s))
		i = i*10 + *((s)++) - '0';

	return i;
}
#endif
/* List all the versions of idme in turn */
static struct idme_ver_t idme_ver_table[IDME_VER_TAB_LEN] =
{
	{ "1.2", IDME_VER_1P2},
	{ "2.0", IDME_VER_2P0},
	{ "2.1", IDME_VER_2P1},
	{ "3.0", IDME_VER_3P0},
};

/* read entire idme */
static int idme_read(unsigned char *pbuf)
{
	return idme_platform_read(pbuf);
}


static int idme_write( const unsigned char *pbuf)
{
	return idme_platform_write(pbuf);
}

int idme_clean(void)
{
	int ret = 0;
	int data_presence  = idme_check_magic_number(pidme_data);

	if (data_presence) {
		if (data_presence == -2)
		dprintf(CRITICAL,
		"Error Idme data not present: magic number error");
		return -1;
	}

	memset(pidme_data, 0x00, CONFIG_IDME_SIZE);
	if(idme_write((const unsigned char *)pidme_data)) {
		dprintf(CRITICAL, "Error IDME: Failure in idme write");
		ret = -1;
	}
	pidme_data = NULL;
	return ret;
}

static int idme_null_func(void)
{
	/* Set all the functions to NULL */
	idme_init_var = NULL;
	idme_get_var = NULL;
	idme_update_var = NULL;
	idme_print_var = NULL;
	idme_init_atag = NULL;
	idme_get_item_desc = NULL;
#ifdef IDME_UPDATE_TABLE
	idme_update_table = NULL;
#endif
	return 0;
}

int idme_device_tree_initialize(void *fdt)
{
	if (idme_init_dev_tree) {
		return idme_init_dev_tree(fdt, pidme_data);
	} else {
		return 0;
	}
}

int idme_atag_initialize(void *atag_buffer, unsigned int size)
{
	if (NULL == atag_buffer)
		return -1;

	if (NULL == idme_init_atag)
		return -1;

	return idme_init_atag(atag_buffer, (void *)pidme_data, size);
}
/**
*@brief check magic number in idme data
*
*checks for the magic number in idme blob
*if the magic number is not present returns erro code silently
*the caller of this function should perform the check on return
*code and take appropriate action
*@param pointer to idme data blob
*@return 0 on success, error code on failure
*/
int idme_check_magic_number(struct idme_t *pidme)
{
	if(pidme == NULL) {
		dprintf(CRITICAL, "Error, the pointer of pidme_data is NULL.\n");
		return -1;
	}

	if (strncmp(pidme->magic, IDME_MAGIC_NUMBER, strlen(IDME_MAGIC_NUMBER))){
		return -2;
	}
	else
		return 0;
}

int idme_update_var_ex(const char *name, const char *value, unsigned int length)
{
	if (NULL == name) {
		dprintf(CRITICAL,
			"Error IDME: idme_update_var_ex: name not found");
		return -1;
	}

	if (NULL == value) {
		dprintf(CRITICAL,
			"Error IDME: idme_update_var_ex: value is NULL");
		return -1;
	}

	if (idme_update_var) {
		if (idme_update_var(name, value, length, pidme_data)) {
			dprintf (CRITICAL,
			"Error IDME: updating field %s failed", name);
			return -1;
		}
		if (idme_write((const unsigned char *)pidme_data)) {
			dprintf(CRITICAL,
			"Error IDME: idme_update_var_ex: failed idme_write");
			return -1;
		}
		return 0;
	}
	else {
		dprintf(CRITICAL,
			"Error IDME: idme_update_var is not implemented");
		return -1;
	}
}
/**
* @brief increments the bootcount
*
*Reads the IDME data bootcount from the stored IDME data, increments it by 1 and
*stores it back. Prints the value of increased value of bootcount in debugging logs
*This function is not idempotent. Must be called from bootloader only once.
*/
void idme_boot_info(void)
{
	/* bootcount is 8 bytes on most of the devices hence allocating
	9 bytes (8 chars + 1 null terminator) to read/write bootcount are enough
	FIXME: if the bootcount increases more than 8 bytes*/
	char boot_count_str[9];
	unsigned int value = 0;

	memset(boot_count_str, 0, sizeof(boot_count_str));

	/* get the bootcount */
	if (idme_get_var) {
		if((idme_get_var("bootcount",
		boot_count_str, sizeof(boot_count_str), pidme_data) == 0)) {
		value = atoui(boot_count_str);
		if (value == 99999999) {
		/* bootcount is 8 bytes long (for now) on most of the
		devices. magic number 99999999 will serve as MAX for the
		bootcount. FIXME: if the bootcount changes to more than 8 B */
			dprintf(CRITICAL, "bootcount reached MAX (%u)",value);
		} else {
			value++;
			snprintf(boot_count_str, sizeof(boot_count_str), "%d",
						value);
			if (idme_update_var_ex("bootcount", boot_count_str,
					IDME_MAX_IDME_ITEM_SIZE)) {
				dprintf(CRITICAL,
					"Error IDME: bootcount update failed");
			} else {
				dprintf(INFO, "bootcount = %u", value);
			}
		}
		}
	} else
		dprintf(CRITICAL, "Error IDME: idme_get_var is not implemented");

}
/**
* @brief read the bootmode
*
*Reads the bootmode from the stored idme data expect positive integer value
*in stored idme bootmode. If bootmode is negative logs error and returns -1.
*
*@param none
*@return positive bootmode integer value on success and -1 on failure.
*/
int idme_boot_mode(void)
{
	char bootmode[6];
	int value = -1;

	memset(bootmode, 0, sizeof(bootmode));

	if (idme_get_var) {
		if (idme_get_var("bootmode", bootmode,
				sizeof(bootmode), pidme_data) == 0) {
			value = atoi(bootmode);
			if (value < 0) {
				dprintf(CRITICAL,
				"Error IDME: bootmode negative (%d)", value);
				return -1;
			}
			dprintf(INFO, "Boot mode is %d\n", value);
			return value;
		} else
			dprintf(CRITICAL,"Error IDME: reading bootmode failed");
	} else
		dprintf(CRITICAL,"Error IDME: idme_get_var is not implemented");

	return -1;
}

static char board_id[16];
const char* idme_board_id(void)
{
	if (idme_get_var)
		if (idme_get_var("board_id", board_id,
				sizeof(board_id), pidme_data) == 0) {
			dprintf(INFO, "Board ID is %s\n", board_id);
			return board_id;
		}

	return NULL;
}

/* Please note: there is a tokenizing glitch here, in the case where 	*/
/* no arguments are passed in ("fastboot oem idme"). Argc is  		*/
/* incorrectly advanced.						*/
#ifdef UFBL_FASTBOOT_RESPONSE
int fastboot_idme(char *response, const char *cmd)
#else
int fastboot_idme(const char *cmd)
#endif
{
	char *token;
	char ver[32], value[32];
	char *idme[3]  = {"idme", NULL, NULL};
	int i=2;
	idme[1]=ver;
	idme[2]=value;
	dprintf(INFO, "The token is:  \"%s\"\n", cmd);
	token = strtok((char *)cmd,(const char *)" ");
	dprintf(INFO, "Tokenized string is:  \"%s\"\n", token);
	snprintf(ver, sizeof(ver), "%s", (const char *)token);
	while (token != NULL){
		token = strtok(NULL," ");
		if(token != NULL){
			snprintf(value, sizeof(value), "%s", token);
			i++;
		}else{
			break;
		}
	}
#ifdef UFBL_FASTBOOT_RESPONSE
	return do_idme(response, 0, i, idme);
#else
	return do_idme(0, i, idme);
#endif
}

#ifdef UFBL_FASTBOOT_RESPONSE
static void idme_log_failure(char *response, const char *msg)
#else
static void idme_log_failure(const char *msg)
#endif
{
#if !defined(UFBL_TESTS) && \
        !defined(UFBL_PROJ_ABC) && \
        !defined(SUPPORT_BOLT) && \
        !defined(UFBL_PLATFORM_IMX) && \
        !defined(UFBL_PROJ_ABC)
#ifdef UFBL_FASTBOOT_RESPONSE
	UFBL_FASTBOOT_FAIL(response,msg);
#else
        fastboot_fail(msg);
#endif
#endif
	dprintf(CRITICAL, "%s", msg);
}

/**
 * @brief Report values of or modify IDME variables. cmdline is 'idme xyz abc'
 *
 * @parmas  flag is deprecated and not used. Its here for compatibility with uboot
 *          [OUT] response is optional output for fastboot response string.
 *          [IN] argc is number of values in argument list argv, at least one
 *               argument is expected, argc <= 0 results into error
 *          [IN] argv is expected to be "idme" to be compatible with
 *               all previous platforms or bootloader commandlines.
 * @return  int value of 0 for success and -1 for error
 * */
#ifdef UFBL_FASTBOOT_RESPONSE
int do_idme(char *response, int flag, int argc, char * const argv[])
#else
int do_idme(int flag, int argc, char * const argv[])
#endif
{
	int i = 0;

	/* Get rid of compilation warnings due to flag not used*/
	(void) flag;

	if (argc <= 0) {
		dprintf(CRITICAL, "IDME: unexpected input failing!\n");
		return -1;
	}
	for (i = 0; i < argc; i++) {
		dprintf(INFO, "arg %d is %s argc %d\n", i, argv[i], argc);
	}

	if (argc == 1 || strncmp(argv[1], "?", 1) == 0) {
		if (argc == 3) {
			char buf[1024];
			int len = 0;

			memset(buf, 0, 1024);

			len = snprintf(buf, sizeof(buf), "%s: ", argv[2]);
			if (len >= sizeof(buf))
				len = sizeof(buf) - 1;

			if (idme_get_var) {
				if(idme_get_var(argv[2], buf+len,
					sizeof(buf)-len, pidme_data)) {
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
#ifdef UFBL_FASTBOOT_RESPONSE
					UFBL_FASTBOOT_FAIL(response,buf);
#else
					fastboot_fail(buf);
#endif
#endif
					dprintf(CRITICAL,
				"Error IDME: failure to process cmd %s", buf);
					return -1;
				} else {
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
#ifdef UFBL_FASTBOOT_RESPONSE
					UFBL_FASTBOOT_INFO(response,buf);
#else
					fastboot_info(buf);
#endif
#endif
					/*print on terminal */
					dprintf(CRITICAL,"%s\n",buf);
					return 0;
				}
			} else {
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
#ifdef UFBL_FASTBOOT_RESPONSE
				UFBL_FASTBOOT_FAIL(response,buf);
#else
				fastboot_fail(buf);
#endif
#endif
				dprintf(CRITICAL,
				"Error IDME: idme_get_var not implemented");
				return -1;
			}
		} else if(idme_print_var) {
			dprintf(INFO, "idme_print_var\n");

#ifdef UFBL_FASTBOOT_RESPONSE
			return idme_print_var(response, pidme_data);
#else
			return idme_print_var(pidme_data);
#endif

		} else {
			dprintf(INFO, "idme error return\n");
			return -1;
		}
	}
	else if (argc <= 3) {
		char *value = NULL;
		int len = 0;
		int ret = -1;

		dprintf(INFO, "idme argc <= 3\n");
		if (argc == 3) {
			if(strncmp(argv[2], "clean", strlen("clean")) == 0) {
				value = "0";
				dprintf(INFO, "clean '%s'\n", argv[1]);
				len = 1;
			} else if(strncmp(argv[1], "version", strlen("version")) == 0) {
				for(i = 0; i < IDME_VER_TAB_LEN; i++) {
					if (strncmp(idme_ver_table[i].version, argv[2],
							strlen(argv[2])) == 0) {
						value = argv[2];
						ret = 1;
						break;
					}
				}
				if(ret == -1) {
					dprintf(CRITICAL, "Error version, try again!\n");
					return ret;
				}
				memcpy(pidme_data->version, value, strlen(value));
				dprintf(INFO, "Modifying version to '%s'\n", argv[2]);
				idme_write((const unsigned char *)pidme_data);
				return 0;
			} else {
				value = argv[2];
				dprintf(INFO, "setting '%s' to '%s'\n", argv[1], argv[2]);
				len = strlen(value);
			}
		}
		else if (argc == 2) {
			if (strncmp(argv[1], "clean", strlen("clean")) == 0) {
				if(idme_clean()) {

				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"Error IDME: Failure in clean");
				return -1;
				}
				return 0;
			}
			else if (strncmp(argv[1], "init", strlen("init")) == 0) {
				if (idme_initialize()) {
				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"Error IDME: Failure in Init");
				return -1;
				}
				return 0;
			}
			else {
				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"Error IDME: Unknown Command");
				return -1;
			}
		}

		if (idme_update_var) {
			if (idme_update_var(argv[1], value, len, pidme_data)) {

				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"IDME: Failure in update var");
				return -1;
			}

			if (idme_write((const unsigned char *)pidme_data)) {

				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"IDME: Failure in idme write");
				return -1;
			}
			return 0;
		} else {
				idme_log_failure(
#ifdef UFBL_FASTBOOT_RESPONSE
						response,
#endif
						"IDME: failure");
			return -1;
		}
	}

	return 0;
}

/* idme_get_var_external
 *
 * Use this function from areas outside of idme who wish to access
 * idme fields without having access to the pidme_data struct
 */
int idme_get_var_external(const char *name, char *buf, unsigned int length)
{
	if(!idme_get_var)
		return -2; // Something different from idme_get_var
	return idme_get_var(name, buf, length, pidme_data);
}

#ifndef UFBL_FASTBOOT_RESPONSE
void cmd_idme(const char *arg, void *data, unsigned sz)
{
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
	char response[64] = "idme done";
#endif

	if( 0 == fastboot_idme(arg)) {
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
		fastboot_info(response);
		fastboot_okay("");
#endif
	} else {
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
		fastboot_fail("idme fail");
#endif
	}
}
#endif

int idme_initialize(void)
{
	int iTmp;
	unsigned int i = 0;

	dprintf(CRITICAL, "idme_initialize\n");
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC)
	if (unlikely(pidme_data != NULL)) {
#else
	if (pidme_data != NULL) {
#endif
		dprintf(INFO, "idme: pidme_data is not NULL, initialization already completed.\n");
		return 0;
	}

	memset(gidme_buff, 0x00, CONFIG_IDME_SIZE);

	/* load the idme data from boot area
	   Please make sure the MMC driver has been initialized before calling this function */
#if !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC)
	if (unlikely(idme_read(gidme_buff) != 0)) {
#else
	if (idme_read(gidme_buff) != 0) {
#endif
		dprintf(CRITICAL, "Error, failed to read idme from boot area.\n");
		return -1;
	}
	pidme_data = (struct idme_t*)&gidme_buff[0];

	iTmp = idme_check_magic_number(pidme_data);

	if (-2 == iTmp) {
		dprintf(CRITICAL, "WARNING: Failed to find IDME in boot area. Generating default idme ...\n");
		memcpy(pidme_data->magic, IDME_MAGIC_NUMBER, strlen(IDME_MAGIC_NUMBER));
		memcpy(pidme_data->version, IDME_DEFAULT_VERSION, strlen(IDME_DEFAULT_VERSION));
	}

	for ( ; i < sizeof(idme_ver_table)/sizeof(struct idme_ver_t); i++ ) {
		if (strncmp(pidme_data->version, idme_ver_table[i].version, strlen(idme_ver_table[i].version)) == 0) {
			if (idme_ver_table[i].ver_type == IDME_VER_2P0 ||
					idme_ver_table[i].ver_type == IDME_VER_2P1) {
				dprintf(INFO, "Idme version is 2.x and set related function to V2.x\n");
				idme_init_var = idme_init_var_v2p0;
				idme_get_var = idme_get_var_v2p0;
				idme_update_var = idme_update_var_v2p0;
				idme_print_var = idme_print_var_v2p0;
				idme_init_atag = idme_init_atag_v2p0;
				idme_get_item_desc = idme_get_item_desc_v2p0;
				idme_init_dev_tree = idme_init_dev_tree_v2p1;
#ifdef IDME_UPDATE_TABLE
				idme_update_table = idme_update_table_v2p0;
#endif
			}
			else if (idme_ver_table[i].ver_type < IDME_VER_2P0) {
				dprintf(INFO, "Warning, version %s is an old version\n", pidme_data->version);
				idme_null_func();
			}
			else {
				//TODO
				dprintf(INFO, "Warning, we will implement a newer version %s here\n", pidme_data->version);
				idme_null_func();
			}
			break;
		}
	}

	/* Generating default idme ... */
	if ((-2 == iTmp) && (idme_init_var)) {
		if (idme_init_var((void*)pidme_data)) {
			dprintf(CRITICAL, "Error IDME: failure in init");
			return -1;
		}
		if (idme_write((const unsigned char *)pidme_data)) {
			dprintf(CRITICAL, "Error IDME: failure in idme write");
			return -1;
		}
	}
#ifdef IDME_UPDATE_TABLE
	/* This function is non-functional unless IDME_UPDATE_TABLE is defined */
	else if (idme_update_table && (0 == idme_update_table((void*)pidme_data))) {
		if (idme_write((const unsigned char *)pidme_data)) {
			dprintf(CRITICAL, "Error IDME: failure in idme write");
			return -1;
		}
	}
#endif

	return 0;
}

#ifndef UFBL_FASTBOOT_RESPONSE
void idme_fastboot_register(void)
{
#if defined(UFBL_PROJ_MT8516) || \
	defined(UFBL_PROJ_MT8183_ECHO) || \
	defined(UFBL_PROJ_ABC) || \
        defined(UFBL_PROJ_ABC) || \
        defined(UFBL_PROJ_KARA) || \
	defined(UFBL_PROJ_abc123) || \
	defined(UFBL_PROJ_ABC) || \
	defined(UFBL_PROJ_TRONA)
	fastboot_register("oem idme", cmd_idme, TRUE, FALSE);
#elif !defined(UFBL_TESTS) && \
	!defined(UFBL_PROJ_ABC) && \
	!defined(SUPPORT_BOLT) && \
	!defined(UFBL_PLATFORM_IMX) && \
	!defined(UFBL_PROJ_ABC)
	fastboot_register("oem idme", cmd_idme, 0);
#endif
}
#endif //UFBL_FASTBOOT_RESPONSE

#ifdef SUPPORT_UBOOT
/***************************************************/
int do_idme_uboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef UFBL_FASTBOOT_RESPONSE
	char response[FASTBOOT_RESPONSE_LEN];
	return do_idme(response, flag, argc, argv);
#else
	return do_idme(flag, argc, argv);
#endif
}

#ifndef CONFIG_DISABLE_CMD_IDME
U_BOOT_CMD(
	   idme, 3, 0, do_idme_uboot,
	   "idme support",
	   "<var> <value>  :  set a variable value\n"
	   "     ?              :  print out known values\n"
	   "     clean          :  reset idme data to default values\n"
	   );
#endif
#endif
