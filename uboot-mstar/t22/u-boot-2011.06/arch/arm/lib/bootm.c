/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <fdt.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <MsApiMiu.h>
#include "MsEnvironment.h"
#include <amzn_secure_boot.h>
#include <timestamp.h>
#include <MsDebug.h>
#include "drvUART.h"
#include "drvPM.h"

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
#include <asm/armv7.h>
#endif

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define CMD_BUF    128
#define ENV_RECOVER_BUFFER "recovery_fbaddr"
#define ENV_TV_OLED "OLED_TV"
#define URSA_MUTE_ENV_CFG_PREFIX  "URSA_MUTE"

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd);

# ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd);
# endif
static void setup_commandline_tag (bd_t *bd, char *commandline);

# ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start,
			      ulong initrd_end);
# endif
static void setup_end_tag (bd_t *bd);

static struct tag *params;
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void);
#if !defined(CONFIG_OF_LIBFDT) //TODO: change back for proper DTB work
static int bootm_linux_fdt(int machid, bootm_headers_t *images);
#endif

void arch_lmb_reserve(struct lmb *lmb)
{
	ulong sp;

	/*
	 * Booting a (Linux) kernel image
	 *
	 * Allocate space for command line and board info - the
	 * address should be as high as possible within the reach of
	 * the kernel (see CONFIG_SYS_BOOTMAPSZ settings), but in unused
	 * memory, which means far enough below the current stack
	 * pointer.
	 */
	sp = get_sp();
	debug("## Current stack ends at 0x%08lx ", sp);

	/* adjust sp by 1K to be safe */
	sp -= 1024;
	lmb_reserve(lmb, sp,
		    gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size - sp);
}

#if 0
// MStar start
#define BOOTTIME_SBOOT_STR "BOOTTIME_SBOOT"
#define BOOTTIME_UBOOT_STR "BOOTTIME_UBOOT"
#include <stdlib.h>
#include <malloc.h>
#include "MsEnvironment.h"

unsigned long G_MS_BOOTTIME_SBOOT=1; // global variable for storing the boot time used in sboot (ms)
unsigned long G_MS_BOOTTIME_UBOOT=1; // global variable for storing the boot time used in sboot (ms)

static void _boottime_set_to_env(void)
{
    //extern unsigned long G_MS_BOOTTIME_SBOOT; // global variable for storing the boot time used in sboot (ms)
    //extern unsigned long G_MS_BOOTTIME_UBOOT; // global variable for storing the boot time used in sboot (ms)
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    const char *strBootArg = "bootargs";
    char *strEnv = NULL;
    char *strPrevEnv = NULL;
    const int u32EnvSize = 32;

    {
        unsigned int PiuTick = *(volatile unsigned int *)(0x1f006090);
        PiuTick |= (*(volatile unsigned int *)(0x1f006094)) << 16;
        unsigned int PiuTime = PiuTick / 12000;
        G_MS_BOOTTIME_UBOOT = (unsigned long)PiuTime;
    }

    strEnv = malloc(u32EnvSize);
    if(strEnv  != NULL)
    {
        strPrevEnv = getenv((char*)strBootArg);
        if(strPrevEnv != NULL)
        {
            memset(strEnv , 0, u32EnvSize);
            snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_SBOOT_STR, G_MS_BOOTTIME_SBOOT);
            if(0 != set_bootargs_cfg((char*)strBootArg, strEnv, 1))
            {
                printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
            }

            //NOTE: getenv again because if has been updated
            strPrevEnv = getenv((char*)strBootArg);
            if(strPrevEnv != NULL)
            {
                memset(strEnv , 0,u32EnvSize);
                snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_UBOOT_STR, G_MS_BOOTTIME_UBOOT);
                if(0 != set_bootargs_cfg((char*)strBootArg, strEnv, 1))
                {
                    printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
                }
            }
        }

        free(strEnv );
    }
}
// MStar end
#endif
static void announce_and_cleanup(void)
{
    char* s = NULL;
    char* t = NULL;

    /* checkpoint for autotest boottime, plz dont remove it */
    unsigned int PiuTick = *(volatile unsigned int *)(0x1f006090);
    PiuTick |= (*(volatile unsigned int *)(0x1f006094)) << 16;
    unsigned int PiuTime = PiuTick / 12000;
    printf("[AT][MB][start kr][%u]\n", PiuTime);

	open_console_log();
	printf("\nStarting kernel ...\n\n");

    #ifdef STR_FOR_AGATE
	    *(volatile unsigned int*)(0x1f206700) = 0xdfdf;
	#else
	    extern void MsOS_DisableAllInterrupts(void);
        MsOS_DisableAllInterrupts();
    #endif

    s = getenv("sync_miuprot");
    t = getenv("KERNEL_PROTECT");

    if(!(strcmp(s, "2")))
    {
        MsApi_MiuProtect();
    }
    else if ((!(strcmp(s, "1"))) || (t != NULL))
    {
        MsApi_kernelProtect();
        s = getenv("kernelProtectBist");
        if (s != NULL)
        {
            MsApi_kernelProtectBist();
        }
    }

#ifdef CONFIG_USB_DEVICE
	{
		extern void udc_disconnect(void);
		udc_disconnect();
	}
#endif
	cleanup_before_linux();
}

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
static void do_nonsec_virt_switch(void)
{
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
	if (armv7_switch_nonsec() == 0)
#ifdef CONFIG_ARMV7_VIRT
	if (armv7_switch_hyp() == 0)
		debug("entered HYP mode\n");
#else
	debug("entered non-secure state\n");
#endif
#endif
}
#endif

/*
 *	AMP Nuttx flow
 *	OPTEE flow
 *	ARM FW flow
 */
#include <MsTrustZone.h>
#ifdef CONFIG_ARMv7_OPTEE
typedef void (*optee_entry)(int zero, int arch, uint params, unsigned long kernel_entry);
#endif

extern unsigned int optee_real_va_addr;
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
extern unsigned int armfw_real_va_addr;
#endif

#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
void ARM_FW_Enter_64(unsigned long cmd_ptr, ulong ramdisk_start, ulong ramdisk_len)
{
#if !defined(CONFIG_MSTAR_NS_UBOOT) && !defined(CONFIG_OPTEE_TEELOADER) //SMP ARMv8 optee flow with 64bit kernel
	mstar_boot_prameters_t bootp;
	unsigned long Fireware_entry = armfw_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x00023000;//0xC0000000
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	printf("\033[0;32;31m %s %d  ARMv8 Uboot for 64bit kernel Enter ARM FW\033[m\n",__func__,__LINE__);
	printf("\033[0;32;31m %s %d %x %x %x\033[m\n",__func__,__LINE__,ramdisk_start,ramdisk_len,cmd_ptr);

	bootp.BIN0_entry_point = optee_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x02100000;
	bootp.BIN0_RW64 = MODE_RW_32; //not use now
	bootp.BIN0_bootargs = 0;
	bootp.BIN1_entry_point = 0x20280000;
	bootp.BIN1_RW64 = MODE_RW_64;
	bootp.BIN1_r2 = cmd_ptr;
	bootp.BIN1_r3 = ramdisk_start;
	bootp.BIN1_r4 = ramdisk_len;
	bootp.BIN2_RW64 = MODE_RW_64;
	printf("\033[0;32;31m %s %d %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN0_entry_point,bootp.BIN1_entry_point);
	printf("\033[0;32;31m %s %d %llx %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN1_r2,bootp.BIN1_r3,bootp.BIN1_r4);
	cleanup_before_linux();
	__asm__ __volatile__(
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(&bootp)
		:"r0","r1"
	);
#elif defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)//SMP ARMv8 optee flow with 64bit kernel for NS-Uboot
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	printf("\033[0;32;31m %s %d  ARMv8 NS-Uboot for 64bit kernel\033[m\n",__func__,__LINE__);
	printf("\033[0;32;31m %s %d %x %x %x\033[m\n",__func__,__LINE__,ramdisk_start,ramdisk_len,cmd_ptr);

	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r3, [%0]  \n\t"
	"ldr  r4, [%1]  \n\t"
	"ldr  r2, [%2]  \n\t"
	"smc #0         \n\t"
	:
	: "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(&cmd_ptr)
	: "r0","r3","r4","r2"
	);
#endif
}

void ARM_FW_Enter_32(int machid,char* bi_boot_params)
{
	mstar_boot_prameters_t bootp;
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	unsigned long Fireware_entry = armfw_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x00023000;//0xC0000000
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	bootp.BIN0_entry_point = optee_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x02100000;
	bootp.BIN0_RW64 = MODE_RW_32; //not use now
	bootp.BIN0_bootargs = 0;
	bootp.BIN1_entry_point = 0x20208000;
	bootp.BIN1_RW64 = MODE_RW_32;
	printf("\033[0;32;31m %s %d %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN0_entry_point,bootp.BIN1_entry_point);
	bootp.BIN1_r0 = 0;
	bootp.BIN1_r1 = machid;
	bootp.BIN1_r2 = bi_boot_params;
	bootp.BIN2_RW64 = MODE_RW_32;
	cleanup_before_linux();
	__asm__ __volatile__(
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(&bootp)
		:"r0","r1"
	);
}
#endif

#if defined(UFBL_FEATURE_IDME)
static char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    if (!count)   // nothing to be done
        return NULL;

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
#endif

unsigned int gUrsaMuteState = 0;
extern int del_bootargs_cfg(char *delCfg,MS_BOOL bDontSaveEnv);
extern int wdt_intr_dump[4];
int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
	bd_t	*bd = gd->bd;
	char	*s;
	int	machid = bd->bi_arch_number;
	void	(*kernel_entry)(int zero, int arch, uint params);
	int ret=0;
	MS_PHYADDR addr=0;
	char buf[CMD_BUF]="\0";
  // To get optee status
  extern unsigned long gOpteeFailFlag;

#ifdef CONFIG_CMDLINE_TAG
	char *commandline = getenv ("bootargs");
#endif

	if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
		return 1;

	s = getenv ("machid");
	if (s) {
		machid = simple_strtoul (s, NULL, 16);
		printf ("Using machid 0x%x from environment\n", machid);
	}

	show_boot_progress (15);

#ifndef CONFIG_OF_LIBFDT //TODO: change back for proper DTB work
	if (images->ft_len)
		return bootm_linux_fdt(machid, images);
#endif

	kernel_entry = (void (*)(int, int, uint))images->ep;

	debug ("## Transferring control to Linux (at address %08lx) ...\n",
	       (ulong) kernel_entry);

//MStar start
    //_boottime_set_to_env();
#ifdef CONFIG_CMDLINE_TAG
    // delete root and androidboot.selinux conf from bootargs
    run_command("dc root=", 0);
    run_command("dc androidboot.selinux=", 0);

    commandline = getenv ("bootargs"); // NOTE: should get agaig, becuase the env has been updated
    UBOOT_DEBUG("bootarg=%s \n",commandline);

    /* remove params that should not in env variables, for security reason */
    int bNeedToSaveEnv = 0;
    char* new_commandline = str_replace(commandline, "androidboot.unlocked_kernel=true", " ");
    if (new_commandline) {
        setenv("bootargs", new_commandline);
        bNeedToSaveEnv = 1;
        commandline = getenv("bootargs");
        free(new_commandline);
    }
    new_commandline = str_replace(commandline, "androidboot.veritymode=disabled", " ");
    if (new_commandline) {
        setenv("bootargs", new_commandline);
        bNeedToSaveEnv = 1;
        commandline = getenv("bootargs");
        free(new_commandline);
    }
    new_commandline = str_replace(commandline, "androidboot.prod=0", " ");
    if (new_commandline) {
        setenv("bootargs", new_commandline);
        bNeedToSaveEnv = 1;
        commandline = getenv("bootargs");
        free(new_commandline);
    }
    new_commandline = str_replace(commandline, "androidboot.selinux=permissive", " ");
    if (new_commandline) {
        setenv("bootargs", new_commandline);
        bNeedToSaveEnv = 1;
        commandline = getenv("bootargs");
        free(new_commandline);
    }
    new_commandline = str_replace(commandline, "loglevel=7", " ");
    if (new_commandline) {
        setenv("bootargs", new_commandline);
        bNeedToSaveEnv = 1;
        commandline = getenv("bootargs");
        free(new_commandline);
    }

    memset(buf , 0, CMD_BUF);
#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    snprintf(buf, CMD_BUF,"%s=%s", ENV_TV_OLED, IsTVOled() ? "1" : "0");
#else
    snprintf(buf, CMD_BUF,"%s=%s", ENV_TV_OLED, "0");
#endif
    UBOOT_DEBUG("Add buf=[%s] into bootargs\n", buf);
    env_add_bootargs(ENV_TV_OLED, buf);
    commandline = getenv("bootargs");

#if defined(CONFIG_PNL_OLED_LG_SUPPORT)
    if (IsTVOled())
    {
        del_bootargs_cfg(URSA_MUTE_ENV_CFG_PREFIX, 0);
        memset(buf , 0, CMD_BUF);
        if (gUrsaMuteState)
            snprintf(buf, CMD_BUF,"%s=1", URSA_MUTE_ENV_CFG_PREFIX);
        else
            snprintf(buf, CMD_BUF,"%s=0", URSA_MUTE_ENV_CFG_PREFIX);

        UBOOT_DEBUG("Add buf=[%s] into bootargs\n", buf);
        env_add_bootargs(URSA_MUTE_ENV_CFG_PREFIX, buf);
        commandline = getenv("bootargs");
    }
#endif

    //Recovery Buffer
    memset(buf , 0, CMD_BUF);
    ret=get_map_addr_from_env(E_RECOVERY_BUFFER, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("[%s] No %s information\n", __FUNCTION__ ,"E_MMAP_ID_RECOVERY_BUFFER_ADR");
    }
    else
    {
        snprintf(buf, CMD_BUF,"%s=0x%llx", ENV_RECOVER_BUFFER, (MS_U64)addr);
        if(isBootToRecovery())
        {
            UBOOT_DEBUG("Add buf=[%s] into bootargs\n", buf);
            env_add_bootargs(ENV_RECOVER_BUFFER, buf);
            commandline = getenv("bootargs");
        }
        else
        {
            new_commandline = str_replace(commandline, buf, " ");
            if (new_commandline) {
                setenv("bootargs", new_commandline);
                bNeedToSaveEnv = 1;
                commandline = getenv("bootargs");
                free(new_commandline);
            }
        }
    }
    if (bNeedToSaveEnv)
        saveenv();
#endif
//MStar end
#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
	setup_start_tag (bd);
#ifdef CONFIG_SERIAL_TAG
	setup_serial_tag (&params);
#endif
#ifdef CONFIG_REVISION_TAG
	setup_revision_tag (&params);
#endif
#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
	setup_commandline_tag (bd, commandline);
#endif
#ifdef CONFIG_INITRD_TAG
	if (images->rd_start && images->rd_end)
		setup_initrd_tag (bd, images->rd_start, images->rd_end);
#endif
	setup_end_tag(bd);
#endif

	announce_and_cleanup();

#if defined (CONFIG_ARMv8_64BIT_KERNEL) //Kernel 64bit
    //there is no atags pointer within 64 bit kernel, so we need to pass bootargs to kernel
    char* __cmdline = bd->bi_boot_params;
    //cmd_ptr is used to save pointer of command line

    char* __bootmodestr = getenv ("bootmode");
    int ibootmode = atoi(__bootmodestr);
#if defined(UFBL_FEATURE_UNLOCK) && defined(UFBL_FEATURE_SECURE_BOOT)
    int iunlocked = amzn_device_is_unlocked();
#else
	int iunlocked = 0;
#endif
    unsigned long cmd_ptr = (unsigned long)bd->bi_boot_params;
    {
        char serialno[18] = {0};
        char *__ursa_version = NULL;
        char* __prod;
        char* __selinux;
        char* __console = "";
        char* __secure_cpu = "";
        char* __unlocked = "androidboot.unlocked_kernel=false";
        char* __rpmb_state = "";
        char* __dm_verity_mode = "";
        char __wdt_intr_dump[64] = {0};
        char* __recovery_mode = "";
	char * __quiescent = "androidboot.quiescent=0";
        char* __verifybootstate = "androidboot.verifiedbootstate=orange";
        int rootPartNum = 0;
        char rootParaBuffer[CONFIG_SYS_CBSIZE] = {0};
        char* __rootpart = "system";
        unsigned loglevel = 0;

        if ( target_is_production() ) {
            __prod = "androidboot.prod=1";
        } else {
            __prod = "androidboot.prod=0";
        }
        if (ibootmode == IDME_BOOTMODE_DIAG) {//boot into diag mode
            __selinux = "androidboot.selinux=permissive security=selinux";
        } else {
            __selinux = "androidboot.selinux=enforcing security=selinux";
        }
        if ( is_secure_cpu() ) {
            __secure_cpu = "androidboot.secure_cpu=1";
        } else {
            __secure_cpu = "androidboot.secure_cpu=0";
        }

        if (anti_rb_enabled()) {
            __rpmb_state = "androidboot.rpmb_state=1";
        } else {
            __rpmb_state = "androidboot.rpmb_state=0";
        }

        __ursa_version = getenv ("ursa_version");

#if defined(UFBL_FEATURE_IDME)
        idme_get_var_external("serial", serialno, sizeof(serialno)-1);

#if defined(UFBL_FEATURE_UNLOCK)

#if defined(CONFIG_MSTAR_BD_M7322_WINSTON)
        if ( !target_is_production() || iunlocked || ibootmode == IDME_BOOTMODE_DIAG ) {
#else
        if ( !target_is_production() || iunlocked ) {
#endif // CONFIG_MSTAR_BD_M7322_WINSTON

            __unlocked = "androidboot.unlocked_kernel=true";
        }
#endif // UFBL_FEATURE_UNLOCK

#if defined(CONFIG_MSTAR_BD_M7322_WINSTON)
        if (amzn_dm_verity_is_off(iunlocked) || isBootToRecovery() || ibootmode == IDME_BOOTMODE_DIAG ) {
#else
        if (amzn_dm_verity_is_off(iunlocked) || isBootToRecovery() ) {
#endif //CONFIG_MSTAR_BD_M7322_WINSTON
            __dm_verity_mode = "androidboot.veritymode=disabled";
        } else {
            __dm_verity_mode = "androidboot.veritymode=eio";
        }

#else
        sprintf((char *)serialno, "0123456789ABCDEF");
#endif

        if (isBootToRecovery()) {
                __recovery_mode = "recovery_mode=true";
        } else {
                __recovery_mode = "recovery_mode=false";
        }

	if (is_quiescent_mode()) {
		__quiescent = "androidboot.quiescent=1";
		/* set screen off flag */
		Write2Byte(BOOT_REASON_PM_ADDR_OFFSET, Read2Byte(BOOT_REASON_PM_ADDR_OFFSET) | PM_SPARE_SCREEN_STATE);
	}
        // for root partition
#ifdef UFBL_FEATURE_IDME
        if (IDME_BOOTMODE_DIAG == ibootmode)
           __rootpart = "dfs";
#endif
        if( get_mmc_part_number(__rootpart, &rootPartNum) == 0)
        {

#if defined(CONFIG_MSTAR_BD_M7322_WINSTON)
            if (amzn_dm_verity_is_off(iunlocked) || isBootToRecovery() || ibootmode == IDME_BOOTMODE_DIAG )
#else
            if (amzn_dm_verity_is_off(iunlocked) || isBootToRecovery() )
#endif //CONFIG_MSTAR_BD_M7322_WINSTON

                sprintf(rootParaBuffer, "root=/dev/mmcblk0p%d", rootPartNum);
            else
                sprintf(rootParaBuffer, "skip_initramfs rootwait ro init=/init root=/dev/dm-0 dm=\"system none ro,0 1 android-verity /dev/mmcblk0p%d \"", rootPartNum);
        }
        else
        {
            UBOOT_ERROR("Could not get root partition: %s!\n", __rootpart);
        }

        if ( (wdt_intr_dump[0] + wdt_intr_dump[1] + wdt_intr_dump[2] + wdt_intr_dump[3]) != 0)
            sprintf(__wdt_intr_dump, "WDT_INTR_DUMP=%04x:%04x:%04x:%04x",wdt_intr_dump[0],
                wdt_intr_dump[1], wdt_intr_dump[2], wdt_intr_dump[3]);

        // Turn on serial console here for
        // 1. non production device, or
        // 2. unlocked device, or
        // 3. in factory (diag) mode
        // Kernel adds more control in user build based on FOS_FLAGS_CONSOLE_ON
        if (!target_is_production() || iunlocked || (ibootmode == IDME_BOOTMODE_DIAG)) {
            __console = "console=ttyS0,115200 androidboot.console=ttyS0";
        }

#if defined(UFBL_FEATURE_IDME)
        // update loglevel
        {
            unsigned long fos_flags = 0;
            char fos_buf[16] = {0};
            int ret = 0;
            char *s;
            unsigned kernel_loglevel = 0;

            if ((s = getenv ("kernel_loglevel")) != NULL)
                kernel_loglevel = (int)simple_strtoul (s, NULL, 10);

            ret = idme_get_var_external("fos_flags", fos_buf, sizeof(fos_buf));
            if (ret >= 0)
                fos_flags = simple_strtoul(fos_buf, NULL, 16);

            if ((fos_flags & FOS_FLAGS_CONSOLE_ON) && (0 == kernel_loglevel)) {
                // force set loglevel to 7 if FOS_FLAGS_CONSOLE_ON is configured and loglevel is not set.
                // loglevel can still be tuned to other level using ac loglevel in mboot console.
                loglevel = 7;
            } else if (!(fos_flags & FOS_FLAGS_CONSOLE_ON)) {
                MDrv_UART_SetIOMapBase();
                ret = mdrv_uart_get_connection(E_UART_PORT0);
                if (ret==E_UART_PIU_UART0)
                {
                    printf("Disable UART0 driver \n");
                    mdrv_uart_connect(E_UART_PORT0, E_UART_OFF);
                }
                else
                {
                    printf("Fail to disable UART0 driver %d \n",ret);
                }
                // Still need to set kernel console log level to have last_kmsg work.
                loglevel = kernel_loglevel;
            }
            else
                loglevel = kernel_loglevel;
        }
#endif
        sprintf(__cmdline, "loglevel=%d androidboot.serialno=%s androidboot.ursa_version=%s %s %s %s %s %s %s %s %s %s %s mboot_build_time=%s-%s %s %s %s",
                loglevel, serialno, __ursa_version, __unlocked, __prod, __secure_cpu, __rpmb_state, __selinux, __dm_verity_mode,
                __console, __wdt_intr_dump, __quiescent, __recovery_mode, U_BOOT_DATE, U_BOOT_TIME, __verifybootstate, rootParaBuffer, commandline);
    }

#if defined(UFBL_FEATURE_IDME)
        char* dev_flags_str = NULL;
        unsigned long dev_flags = 0;
        if ((dev_flags_str = getenv("dev_flags")) != NULL)
                dev_flags = simple_strtoul(dev_flags_str, NULL, 16);

        if ((dev_flags & DEV_FLAGS_SELINUX_FORCE_ENFORCING) == DEV_FLAGS_SELINUX_FORCE_ENFORCING) {
                printf("force selinux enforcing mode\n");
                char* enforcing_cmdline = str_replace(__cmdline, "selinux=permissive", "selinux=enforcing");
                if (enforcing_cmdline) {
                        int length = strlen(enforcing_cmdline);
                        strncpy(__cmdline, enforcing_cmdline, length);
                        __cmdline[length] = '\0';
                        free(enforcing_cmdline);
                }
        } else if (((dev_flags & DEV_FLAGS_SELINUX_FORCE_PERMISSIVE) == DEV_FLAGS_SELINUX_FORCE_PERMISSIVE)
                && !is_lockdown()) {
                printf("force selinux permissive mode\n");
                char* permissive_cmdline = str_replace(__cmdline, "selinux=enforcing", "selinux=permissive");
                if (permissive_cmdline) {
                        int length = strlen(permissive_cmdline);
                        strncpy(__cmdline, permissive_cmdline, length);
                        __cmdline[length] = '\0';
                        free(permissive_cmdline);
                }
        }
#endif

    printf("set 64 bit mode \n");
    extern ulong ramdisk_start,ramdisk_len;

#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_SECARM_OPTEE)
  if(gOpteeFailFlag == 0)
  {
    ARM_FW_Enter_64(cmd_ptr,ramdisk_start,ramdisk_len);
	}
	else
	{
		//Write gOpteeFailFlag flag back to 0x103382 since this bank may be overwritten by other APP
		Write2Byte(0x103382,0xEEFF);
    __asm__ __volatile__(
    #if (CONFIG_GIC_PATCH == 1)
      "ldr     r3, =0x16001080\n\t"
      "ldr     r2, [r3]\n\t"
      "ldr     r4, =(0x3 << 30)\n\t"
      "orr     r2, r2, r4\n\t"
      "str     r2, [r3]\n\t"
      "ldr     r3, =0x16001000\n\t"
      "mov     r2, #3\n\t"
      "str     r2, [r3]\n\t"
      "ldr     r3, =0x16002000\n\t"
      "mov     r2, #3\n\t"
      "str     r2, [r3]\n\t"
      "ldr     r3, =0x16002004\n\t"
      "mov     r2, #0xf8\n\t"
      "str     r2, [r3]\n\t"
    #endif
      "ldr  r3, [%0]  \n\t"
      "ldr  r4, [%1]  \n\t"
      "ldr  r2, [%2]  \n\t"
      :
      : "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(&cmd_ptr)
      : "r3","r4","r2"
      );
      __asm__ __volatile__("smc #0": );
	}
#else //ARMv8 Normal boot flow with 64bit kernel
	__asm__ __volatile__(
#if (CONFIG_GIC_PATCH == 1)
	"ldr     r3, =0x16001080\n\t"
	"ldr     r2, [r3]\n\t"
	"ldr     r4, =(0x3 << 30)\n\t"
	"orr     r2, r2, r4\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16001000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002004\n\t"
	"mov     r2, #0xf8\n\t"
	"str     r2, [r3]\n\t"
#endif
	"ldr  r3, [%0]  \n\t"
	"ldr  r4, [%1]  \n\t"
	"ldr  r2, [%2]  \n\t"
	:
	: "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(&cmd_ptr)
	: "r3","r4","r2"
	);
	__asm__ __volatile__("smc #0": );
#endif

#else //Kernel 32bit
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT) //AMP Nuttx flow
	do_nonsec_virt_switch();
	kernel_entry(0, machid, bd->bi_boot_params);
#elif defined(CONFIG_ARMv7_OPTEE) && !defined(CONFIG_MSTAR_NS_UBOOT) && !defined(CONFIG_OPTEE_TEELOADER)//SMP ARMv8 optee flow with 32bit kernel
	optee_entry entry;
	entry = (optee_entry) optee_real_va_addr; //here have to modify by tee.bin
	entry(0, machid, bd->bi_boot_params,(unsigned long)kernel_entry); //r0 r1 r2 r3=kernel_entry
	asm volatile( //because optee return to mboot will entry sm_set_nsec_ret_vals so we have to move back
		"mov r3,r2\n\t"
		"mov r2,r1\n\t"
		"mov r1,r0\n\t"
		"ldr r0,=0x0\n\t"
		"mov pc,r3\n\t":::
	);
#elif defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_MSTAR_NS_UBOOT) && !defined(CONFIG_OPTEE_TEELOADER) //SMP ARMv8 optee flow with 32bit kernel
	ARM_FW_Enter_32(machid, bd->bi_boot_params);
#else // ARMv8 and ARMv7 Normal boot flow with 32bit kernel & NS_UBOOT
	kernel_entry(0, machid, bd->bi_boot_params);
	/* does not return */
#endif
#endif
	return 1;
}

#if !defined(CONFIG_OF_LIBFDT) //TODO: change back for proper DTB work
static int fixup_memory_node(void *blob)
{
	bd_t	*bd = gd->bd;
	int bank;
	u64 start[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	for (bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {
		start[bank] = bd->bi_dram[bank].start;
		size[bank] = bd->bi_dram[bank].size;
	}

	return fdt_fixup_memory_banks(blob, start, size, CONFIG_NR_DRAM_BANKS);
}

static fdt_add_all_memory_node(void *blob)
{
	char* s = NULL;
	int i = 0;
	u64 start[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];
	int bank = 0;
	char tmp_buf = {0};
	char nodep[100] = {0};		/* new node to add */
	int  nodeoffset;	/* node offset from libfdt */
	int  ret;		/* return value */
	int err = 0;
	s = getenv("MAP_TYPE");
    if( s == NULL)
        return -1;

	if( strcmp(s, "MI") == 0 )
    {
    	for(i=CONFIG_NR_DRAM_NODES; i > 0; i--)
    	{
	    	sprintf(tmp_buf, "MI_KERNEL_POOL%d_ADR", i);
	        s = getenv(tmp_buf);
	        if( s == NULL)
	            continue;

	        start[bank] = CONFIG_SYS_MIU0_BUS + simple_strtoull (s, NULL, 16);

	        sprintf(tmp_buf, "MI_KERNEL_POOL%d_LEN", i);
	        s = getenv(tmp_buf);
	        if( s == NULL)
	        	continue;

	        size[bank] = simple_strtoull (s, NULL, 16);

	        sprintf(nodep, "memory@%8X", start[bank]);
	        nodeoffset = fdt_path_offset (blob, "/");
			if (nodeoffset < 0) {
				/*
				 * Not found or something else bad happened.
				 */
				printf ("libfdt fdt_path_offset() returned %s\n",
					fdt_strerror(nodeoffset));
				return 1;
			}

			err = fdt_add_subnode(blob, 0, nodep);
			if (err < 0) {
				printf ("libfdt fdt_add_subnode(): %s\n",
					fdt_strerror(err));
				return 1;
			}

			fdt_add_new_memory_banks(blob, start, size, CONFIG_NR_DRAM_BANKS, nodep);
    	}
    }
}

static int bootm_linux_fdt(int machid, bootm_headers_t *images)
{
	ulong rd_len;
	void (*kernel_entry)(int zero, int dt_machid, void *dtblob);
	ulong of_size = images->ft_len;
	char **of_flat_tree = &images->ft_addr;
	ulong *initrd_start = &images->initrd_start;
	ulong *initrd_end = &images->initrd_end;
	struct lmb *lmb = &images->lmb;
	int ret;
	extern unsigned long gOpteeFailFlag;
	extern ulong ramdisk_start,ramdisk_len;


	kernel_entry = (void (*)(int, int, void *))images->ep;

	boot_fdt_add_mem_rsv_regions(lmb, *of_flat_tree);

	rd_len = images->rd_end - images->rd_start;

#if 0
	ret = boot_ramdisk_high(lmb, images->rd_start, rd_len,
				initrd_start, initrd_end);
	if (ret)
		return ret;
#endif

    #ifdef CONFIG_SUPPORT_DTO
    ret = boot_ufdt_overlay(of_flat_tree, &of_size);
    if (ret)
    {
        printf("## %s: Merge DTBO to main DTB fail!\n",__FUNCTION__);
        //return ret;
    }
    #endif

	ret = boot_relocate_fdt(lmb, of_flat_tree, &of_size);
	if (ret)
		return ret;

	debug("## Transferring control to Linux (at address %08lx) ...\n",
	       (ulong) kernel_entry);

	fdt_chosen(*of_flat_tree, 1);

#if 0
	fixup_memory_node(*of_flat_tree);
#endif

	fdt_add_all_memory_node(*of_flat_tree);

	fdt_initrd(*of_flat_tree, *initrd_start, *initrd_end, 1);

	announce_and_cleanup();

	// copy to CONFIG_DTB_ADDR
	memcpy(CONFIG_DTB_ADDR, *of_flat_tree, 0x100000);
	uint32_t new_dtb_addr = CONFIG_DTB_ADDR;
	//ramdisk_start = CONFIG_RAMDISK_ADDR;

#if defined (CONFIG_ARMv8_64BIT_KERNEL) //Kernel 64bit
    //there is no atags pointer within 64 bit kernel, so we need to pass bootargs to kernel
    printf("set 64 bit mode \n");
#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_SECARM_OPTEE)
  if(gOpteeFailFlag == 0)
  {
    ARM_FW_Enter_64(new_dtb_addr,ramdisk_start,ramdisk_len);
	}
	else
	{
		//Write gOpteeFailFlag flag back to 0x103382 since this bank may be overwritten by other APP
		Write2Byte(0x103382,0xEEFF);
	    __asm__ __volatile__(
	    #if (CONFIG_GIC_PATCH == 1)
	      "ldr     r3, =0x16001080\n\t"
	      "ldr     r2, [r3]\n\t"
	      "ldr     r4, =(0x3 << 30)\n\t"
	      "orr     r2, r2, r4\n\t"
	      "str     r2, [r3]\n\t"
	      "ldr     r3, =0x16001000\n\t"
	      "mov     r2, #3\n\t"
	      "str     r2, [r3]\n\t"
	      "ldr     r3, =0x16002000\n\t"
	      "mov     r2, #3\n\t"
	      "str     r2, [r3]\n\t"
	      "ldr     r3, =0x16002004\n\t"
	      "mov     r2, #0xf8\n\t"
	      "str     r2, [r3]\n\t"
	    #endif
	      "ldr  r3, [%0]  \n\t"
	      "ldr  r4, [%1]  \n\t"
	      "ldr  r2, [%2]  \n\t"
	      :
	      : "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(of_flat_tree)
	      : "r3","r4","r2"
	      );
	      __asm__ __volatile__("smc #0": );
	}
#else //ARMv8 Normal boot flow with 64bit kernel
	__asm__ __volatile__(
#if (CONFIG_GIC_PATCH == 1)
	"ldr     r3, =0x16001080\n\t"
	"ldr     r2, [r3]\n\t"
	"ldr     r4, =(0x3 << 30)\n\t"
	"orr     r2, r2, r4\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16001000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002004\n\t"
	"mov     r2, #0xf8\n\t"
	"str     r2, [r3]\n\t"
#endif
	"ldr  r3, [%0]  \n\t"
	"ldr  r4, [%1]  \n\t"
	"ldr  r2, [%2]  \n\t"
	:
	: "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(of_flat_tree)
	: "r3","r4","r2"
	);
	__asm__ __volatile__("smc #0": );
#endif
#else //Kernel 32bit
	kernel_entry(0, machid, *of_flat_tree);
#endif

	/* does not return */

	return 1;
}
#endif

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *) bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}


#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start;
		params->u.mem.size = bd->bi_dram[i].size;

		params = tag_next (params);
	}
}
#endif /* CONFIG_SETUP_MEMORY_TAGS */


static void setup_commandline_tag (bd_t *bd, char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


#ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start, ulong initrd_end)
{
	/* an ATAG_INITRD node tells the kernel where the compressed
	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
	 */
	params->hdr.tag = ATAG_INITRD2;
	params->hdr.size = tag_size (tag_initrd);

	params->u.initrd.start = initrd_start;
	params->u.initrd.size = initrd_end - initrd_start;

	params = tag_next (params);
}
#endif /* CONFIG_INITRD_TAG */

#ifdef CONFIG_SERIAL_TAG
void setup_serial_tag (struct tag **tmp)
{
	struct tag *params = *tmp;
	struct tag_serialnr serialnr;
	void get_board_serial(struct tag_serialnr *serialnr);

	get_board_serial(&serialnr);
	params->hdr.tag = ATAG_SERIAL;
	params->hdr.size = tag_size (tag_serialnr);
	params->u.serialnr.low = serialnr.low;
	params->u.serialnr.high= serialnr.high;
	params = tag_next (params);
	*tmp = params;
}
#endif

#ifdef CONFIG_REVISION_TAG
void setup_revision_tag(struct tag **in_params)
{
	u32 rev = 0;
	u32 get_board_rev(void);

	rev = get_board_rev();
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (tag_revision);
	params->u.revision.rev = rev;
	params = tag_next (params);
}
#endif  /* CONFIG_REVISION_TAG */

static void setup_end_tag (bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void)
{
	ulong ret;

	asm("mov %0, sp" : "=r"(ret) : );
	return ret;
}
