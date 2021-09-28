#include <common.h>
#include <command.h>
#include <fdt.h>
#include <libfdt.h>
#include <asm/global_data.h>
#include <idme.h>
#include "MsSysUtility.h"
#include "image.h"
#include <drvGPIO.h>
#include "CusConfig.h"
#include <amzn_secure_boot.h>
#include <unlock_ltc.h>

#define AMZN_T12_CID 0x40
#define CID_OFFSET   0x1004

#if defined(AMZN_FTVE_DTB_SIGNING_ENABLED)
#define AMZN_DTB_LEN (0x0100000)
#define DTB_SIG_LEN (256)
int amzn_verify_dtb(void *dtb_buf)
{
	int ret = 0; // success
	struct fdt_header *fdt_ptr = (struct fdt_header *)dtb_buf;
	unsigned int dtb_size = fdt_totalsize(fdt_ptr);
	if (AMZN_DTB_LEN < dtb_size) {
		printf("%s: dtb size %d too large\n", __FUNCTION__, dtb_size);
		ret = 1;
		goto done;
	}
	unsigned int key_len = 0;
	const uint8_t *key = NULL;
	// use unlock signer for now.
	key = amzn_get_unlock_key(&key_len);
	if (!key || !key_len) {
		printf("%s: Failed to get key\n", __FUNCTION__);
		ret = 1;
		goto done;
	}
	const unsigned char* sig_buf = (const unsigned char*)dtb_buf+dtb_size;

	ret = amzn_verify_code_internal((const unsigned char*)dtb_buf, dtb_size,
		sig_buf, DTB_SIG_LEN, key, key_len);

done:
	return ret;
}
#endif

void ft_board_setup(void *blob, bd_t *bd)
{
	struct fdt_header *fdt_ptr = (struct fdt_header *)blob;

	unsigned int newsize = fdt_totalsize(fdt_ptr)
				+ CONFIG_IDME_SIZE;
	fdt_open_into(fdt_ptr, fdt_ptr, newsize);
	idme_device_tree_initialize(fdt_ptr);
	printf("IDME inserted into FDT\n");
	amzn_disable_vendor_verity_maybe(fdt_ptr);
}


void ft_uboot_log_setup(void *blob)
{
	struct fdt_header *fdt_ptr = (struct fdt_header *)blob;
	int ret=0, offset=0, err=0;

	char *temp = NULL;
	temp = (char*)(malloc(AMZN_LOG_SIZE+1));
	if (!temp) {
		printf("%s: memory allocation error\n", __FUNCTION__);
		return;
	}
	temp[0]=0;
	amzn_get_log(temp);
	int logsize = strlen(temp);
	if (logsize <= 0)  /* buffer is empty */
		goto done;

        unsigned int newsize = fdt_totalsize(fdt_ptr)
                                + logsize + 32;

	err = fdt_open_into(fdt_ptr, fdt_ptr, newsize);
	if (err != 0) {
		printf ("libfdt fdt_open_into(): %s\n",
			fdt_strerror(err));
	}

	ret = fdt_path_offset(fdt_ptr, "/ubootlog");
	if (ret & FDT_ERR_NOTFOUND) {
		/* Create the ubootlog root node in FDT */
		if ((ret = fdt_path_offset(fdt_ptr, "/")) < 0) {
			printf("%s: unable to find root offset\n", __FUNCTION__);
			goto done;
		}

		if((ret = fdt_add_subnode(fdt_ptr, ret, "ubootlog")) < 0) {
			printf("%s: unable to add ubootlog root node\n", __FUNCTION__);
			goto done;
		}

		/* Get the offset of the new ubootlog root node */
		if ((ret = fdt_path_offset(fdt_ptr, "/ubootlog")) < 0) {
			printf("%s: unable to find ubootlog root node offset\n", __FUNCTION__);
			goto done;
		}
		offset = ret;

		ret = fdt_setprop_string(fdt_ptr, offset, "value", temp);
		if (ret < 0) {
			printf("%s: unable to set ubootlog (value)\n", __FUNCTION__);
			goto done;
		}
	}

done:
	if (temp) {
		free(temp);
	}
	return;
}

int amzn_boot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
        void* bootimg_addr = 0x25000000;
        void* dtb_dst     = 0x23000000;
	ulong dtb_src;
	ulong dtb_len;

	if (argc < 2) {
		printf("Invalid arguments\n");
		return -1;
	}

	if (!strncmp(argv[1], "recovery", 7)) {
		if (run_command("mmc read.p 0x25000000 recovery 0x02000000", 0)
				< 0) {
			printf("Failed to load recovery image\n");
			return -1;
		}
	} else if (!strncmp(argv[1],"diag", 4)) {
		if (run_command("mmc read.p 0x25000000 dkernel 0x01400000", 0) < 0) {
			printf("Failed to load dkernel image\n");
			return -1;
		}

	} else if (!strncmp(argv[1], "transition", 9)) {
		run_command("mmc erase.p dkernel", 0);
		run_command("mmc erase.p dfs", 0);
		run_command("mmc erase.p diag_userdata", 0);

		run_command("mmc remove dkernel", 0);
		run_command("mmc remove dfs", 0);
		run_command("mmc remove diag_userdata", 0);

		run_command("mmc remove userdata", 0);
		run_command("mmc create userdata MAX", 0);

                // reset the bootmode to 1
                run_command("idme bootmode 1", 0);

#if defined(CONFIG_MSTAR_BD_M7322_abc123)
                // for abc123 only
                // draw a green square after transition finishes to let operator know

                run_command("initenv", 0);
                run_command("panel_init", 0);
                run_command("osd_create 100 100", 0);
                run_command("draw_rect 300 200 100 100 00ff00", 0);
                run_command("osd_flush", 0);

                // take a log to let operator know and halt
                printf("\n\nshipment mode operations finished\n\n");
                while (1)
                   mdelay(3000);
#endif

                // remove MPOOL to clean up env
                run_command("mmc erase.p MPOOL", 0);

                // take a log to let operator know and halt
                printf("\n\nshipment mode operations finished\n\n");
                while (1)
                   mdelay(3000);

	} else if (run_command("mmc read.p 0x25000000 boot 0x01400000", 0) < 0) {
		printf("Failed to load boot image\n");
		return -1;
	}

#if BOARD_KERNEL_SEPARATED_DT
	/* copy DTB from boot image. DTB is appended after kernel and ramdisk sections */
	if (-1 == android_image_get_dtb((struct andr_img_hdr *)bootimg_addr, &dtb_src, &dtb_len)) {
		printf("Failed to load DTB\n");
	} else {
		memcpy(dtb_dst, (void*)dtb_src, dtb_len);
	}
#else
	/* read DTB from partition */
	if (run_command("mmc read.p 0x23000000 dtb 0x0100000", 0) < 0) {
		printf("Failed to load DTB\n");
		return -1;
	}
#endif

#if defined(AMZN_FTVE_DTB_SIGNING_ENABLED)
	if (amzn_verify_dtb(dtb_dst)) {
		printf("Failed to verify DTB\n");
		return -1;
	}
#endif

	if (run_command("fdt addr 0x23000000", 0) < 0) {
		printf("Failed to initialize DTB\n");
		return -1;
	}

	if (run_command("fdt boardsetup", 0) < 0) {
		printf("Failed to setup DTB\n");
		return -1;
	}

	if (run_command("bootm 0x25000000", 0) < 0) {
		printf("Failed to boot image\n");
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	amzn_boot, 2, 0, amzn_boot,
	"Amazon customized-boot",
		  "normal	- Normal boot (default)\n"
	"amzn_boot recovery	- Boot into recovery mode\n"
	"amzn_boot diag		- Boot into diagnostics mode\n"
);

/*
 * read fos_flags from idme
 */
unsigned long get_fos_flags(void)
{
	unsigned long flags = 0;

	char fos_buf[16];
	int ret = 0;
	ret = idme_get_var_external("fos_flags", fos_buf, sizeof(fos_buf));

	if (ret < 0) {
		printf("get idme fos_flags Error\n");
		return 0;
	}
	flags = simple_strtoul(fos_buf, NULL, 16);

	printf("fos_flags=%x\n", flags);
	return flags;
}

/*
 * Checks whether dm-verity is disabled
 * For locked production device , always return false
 * For unlocked/engineering device, check amazon fos_flags
 *      if bit7 is set, return true
 *      if bit7 is clear, return false
 */
/* TODO: Update code to use FOS_FLAGS_DM_VERITY_OFF inside idme.h */
#define PLATFORM_FOS_FLAGS_DM_VERITY_OFF   (1 << 7)
int amzn_dm_verity_is_off(int unlock_status)
{
	int lock_state;

	lock_state = ((amzn_target_device_type() != AMZN_ENGINEERING_DEVICE)
		&& (unlock_status == 0));

	if (lock_state) {
		/* Locked device: dm-verity is on and cannot be off */
		return 0;
	} else if (get_fos_flags() & PLATFORM_FOS_FLAGS_DM_VERITY_OFF) {
		/*
		 * Unlocked/Engineering device with bit 7 set
		 * in fos_flags, dm-verity is off
		 */
		return 1;
	} else {
		/* dm-verity is on otherwise */
		return 0;
	}
}

int target_is_production()
{
#if UFBL_FEATURE_SECURE_BOOT
#include "amzn_secure_boot.h"
	if (AMZN_PRODUCTION_DEVICE == amzn_target_device_type()) {
		return 1;
	}
#endif

	return 0;
}
static MS_BOOL read_eFuse_authenciation(void)
{
	//Bank 0x20 - 0x04[1]   (16bit mode) //eFuse authenciation enable
	MS_U32 addr = 0x2008;
	MS_U8 val = ReadByte(addr);
	if(val & 0x02)
		return TRUE;
	else
		return FALSE;
}

static MS_U8 read_board_cid(void)
{
	/* read Customer ID */
	MS_U32 addr = 0x3806;
	MS_U8 val = ReadByte(addr);
	return val;
}

int is_secure_cpu()
{
	MS_U8 val = read_board_cid();

	if(TRUE == read_eFuse_authenciation())
	{
		/* secure CPU if customer ID is AMZN_T12_CID */
		if (val == AMZN_T12_CID)
			return 1;
		else
			return 0;
	}
	else
	        return 0;
}

#define SBOOT_VER_MARK   "YAMA"
#define VER_MARK_LEN     4
#define VER_LEN          2
#define MAX_SBOOT_SIZE 153600 //150K
#define PROD_LEN         4

sbvc_result sboot_version_check(uchar* sboot_buf, int sboot_len)
{
	int mark_loc = 0;
	int prod_match = 1;

	// don't check sboot for non-secure device
	if (is_secure_cpu() == 0)
		return SBVC_MATCH;

	if (sboot_buf == NULL)
	{
		return SBVC_INVALID_ARG;
	}

	// check sboot version mark
	while ( (mark_loc <= sboot_len - VER_MARK_LEN - VER_LEN - PROD_LEN) &&
		strncmp(sboot_buf+mark_loc, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc++;
	}

	if (mark_loc > sboot_len - VER_MARK_LEN - VER_LEN - PROD_LEN) {
		return SBVC_MARK_NOT_FOUND;
	}

	// sophia/abc123 HW can only flash sophia/abc123 build
	// greta/abc123 HW can only flash greta/abc123 build
#if defined(CONFIG_MSTAR_BD_M7322_MB170)|| defined(CONFIG_MSTAR_BD_M7322_MB175)
	prod_match = !strnicmp(sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN, "sophia", PROD_LEN) ||
			!strnicmp(sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN, "abc123", PROD_LEN);
#elif defined(CONFIG_MSTAR_BD_M7322_G22) || defined(CONFIG_MSTAR_BD_M7322_G22_abc123)
	prod_match = !strnicmp(sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN, "greta", PROD_LEN) ||
			!strnicmp(sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN, "abc123", PROD_LEN);
#elif defined(CONFIG_MSTAR_BD_M7322_abc123)
        prod_match = !strnicmp(sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN, "abc123", PROD_LEN);
#endif
	if (!prod_match) {
		printf("\n!!Device:%s, sboot:%.*s!!\n", amzn_target_device_name(), PROD_LEN,
			sboot_buf+mark_loc+VER_MARK_LEN+VER_LEN);
		return SBVC_MARK_NOT_FOUND;
	}

	// check CID
	MS_U8 sboot_cid = sboot_buf[CID_OFFSET];
	printf("sboot cid: %d\n", sboot_cid);

	if (sboot_cid != AMZN_T12_CID) {
		return SBVC_INVALID_CID;
	}

	uchar *sboot_dev = NULL;
	// allocate memory for sboot image on the device
	sboot_dev = (uchar*)calloc(1, MAX_SBOOT_SIZE);
	if (!sboot_dev) {
		return SBVC_NO_MEM;
	}

	char mmc_command[64] = {0};
	int sboot_len_dev = (sboot_len < MAX_SBOOT_SIZE) ? sboot_len : MAX_SBOOT_SIZE;
	snprintf(mmc_command, 64, "mmc read.boot 1 0x%08x, 0, 0x%08x",
		sboot_dev, sboot_len_dev);
	printf("\n%s\n", mmc_command);
	if (run_command(mmc_command, 0) < 0) {
		free(sboot_dev);
		return SBVC_READ_ERR;
	}

	// find the version mark
	int mark_loc_dev = 0;
	while ( (mark_loc_dev <= sboot_len_dev - VER_MARK_LEN - VER_LEN - PROD_LEN) &&
		strncmp(sboot_dev+mark_loc_dev, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc_dev++;
	}
	if (mark_loc_dev > sboot_len_dev - VER_MARK_LEN - VER_LEN - PROD_LEN) {
		free(sboot_dev);
		return SBVC_DEV_MARK_NOT_FOUND;
	}

	unsigned short sboot_ver, sboot_ver_dev;
	sboot_ver = *(unsigned short*)(sboot_buf+mark_loc+VER_MARK_LEN);
	sboot_ver_dev = *(unsigned short*)(sboot_dev+mark_loc_dev+VER_MARK_LEN);
	printf("\n%s: sboot img version: %d, dev version: %d\n",
		__FUNCTION__, sboot_ver, sboot_ver_dev);
	if (sboot_ver == sboot_ver_dev) {
		free(sboot_dev);
		return SBVC_SAME_VER;
	}
	free(sboot_dev);

	return SBVC_MATCH;
}

extern int isBootToRecovery(void);
/* If dm-verity is OFF by fos_flags, we need to remove the 'verity' for vendor parititon */
void amzn_disable_vendor_verity_maybe(void *fdt) {
	int offset = 0;
	int len = 0;
	const void* vendor_fsmgr_flags = NULL;
	const char* node_name = "fsmgr_flags";
	// under /sys/firmware/devicetree/base
	const char* node_dir_name = "/firmware/android/fstab/vendor";
	char* buffer = NULL;
	char* target_buffer = NULL;
	int unlocked = amzn_device_is_unlocked();
#if defined(CONFIG_MSTAR_BD_M7322_abc123)
	char* __bootmodestr = getenv ("bootmode");
	int ibootmode = atoi(__bootmodestr);
	if (ibootmode == IDME_BOOTMODE_DIAG) {
		printf("Treat device as unlockded in Diag mode, Diable verify for diag mode \n");
		unlocked = 1;
	}
#endif
	/* do nothing if dm-verity is ON */
	if (amzn_dm_verity_is_off(unlocked) == 0 && !isBootToRecovery()) {
		return;
	}

	offset = fdt_path_offset(fdt, node_dir_name);
	if (offset < 0) {
		printf("[DM-VERITY] Couldn't find vendor node, do nothing!\n");
		return;
	}

	vendor_fsmgr_flags = fdt_getprop(fdt, offset, node_name, &len);
	if (!vendor_fsmgr_flags || !len) {
		printf("[DM-VERITY] couldn't get node %s/%sm, do nothing!\n",
				node_dir_name, node_name);
		return;
	}

	buffer = malloc(len+1);
	if (!buffer) {
		printf("[DM-VERITY] allocate temp buffer failed, do nothing!\n");
		return;
	}
	memcpy(buffer, vendor_fsmgr_flags, len);
	buffer[len] = '\0';

	len = strlen(buffer);

	printf("[DM-VERITY] Found vendor fsmgr_flags value is [%s]\n", buffer);

	char *found_ptr = strstr(buffer, "verify");
	int len_verify = strlen("verify");
	int i = 0;
	if (found_ptr && (found_ptr == buffer || *(found_ptr - 1) == ',') &&
	    (found_ptr + len_verify == buffer + len || *(found_ptr + len_verify) == ',')) {
		printf("[DM-VERITY] found verify, erase it!\n");
		char *from = found_ptr + len_verify;
		char *to = found_ptr;
		while (from < buffer + len) {
			if (from == found_ptr + len_verify &&
			    *from == ',' &&
			    (found_ptr == buffer || *(found_ptr - 1) == ',')) {
				// skip coping the unnecessary ','
				++from;
			} else {
				*to = *from;
				++to;
				++from;
			}
		}
		*to = '\0';
		if (to > buffer && *(to-1) == ',') {  // remove the possible trailing ','
			*(to-1) = '\0';
		}
	} else {
		printf("[DM-VERITY] don't find verify, do nothing!\n");
		free(buffer);
		return;
	}
	printf("[DM-VERITY] change fsmgr_flags to [%s]\n", buffer);
	fdt_setprop(fdt, offset, "fsmgr_flags", buffer, strlen(buffer) + 1);
	free(buffer);
}
