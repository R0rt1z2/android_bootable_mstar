/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include <ShareType.h>
#include <MsUtility.h>
#include <MsApiMiu.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
#include <MsSystem.h>
#include <drvPM.h>
#include <common.h>
#include <CusPM.h>
#include <MsApiPM.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <drvBDMA.h>
#include <miu/MsDrvMiu.h>
#include <str/MsDrvStr.h>
#include <MsStr.h>
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
#include <asm/armv7.h>
#endif

#if CONFIG_MSTAR_STR_ENABLE
#define BIT0 0x01
#define BIT3 0x08
#define MEM_REGION1_ST 0x00200000
#define MEM_REGION1_SZ 0x9F00000
#define MEM_REGION2_ST 0x10E00000
#define MEM_REGION2_SZ 0xC200000
#if (CONFIG_MIPS32 == 1)
#define BDMA_BANK 0xbf201200
#define MB_BANK 0xBF206600
#define PM_SLEEP 0xBF001C00
#define Check_STR_CRC 0xBF002090 // 0x1048
#else
#define BDMA_BANK 0x1f201200
#define MB_BANK 0x1F206600
#define PM_SLEEP 0x1F001C00
#define Check_STR_CRC 0x1F002090 // 0x1048
#endif

extern BOOLEAN get_poweroff_flag(void);

void HAL_MBOOT_Write2Byte( unsigned long u32Reg, unsigned short u16Val )
{
    *((volatile unsigned long *)u32Reg)=u16Val;
}
unsigned short HAL_MBOOT_Read2Byte( unsigned long u32Reg )
{
    return (unsigned short)(*(volatile unsigned long *)u32Reg);
}
unsigned long BDMACRCCalc(int miu, unsigned long u32Addr, unsigned long u32Len)
{
    if (miu == 0)
        return MDrv_BDMA_CRC32(u32Addr, u32Len, 0x04C11DB7, 0x0, E_BDMA_SRCDEV_MIU0, 0x0);
    else if (miu == 1)
        return MDrv_BDMA_CRC32(u32Addr, u32Len, 0x04C11DB7, 0x0, E_BDMA_SRCDEV_MIU1, 0x0);
    else
        return false;
}

void CRCCheck(void)
{
    char *pEnv = NULL;
    unsigned char crclevel = 0;
    char *startp = NULL, *endp = NULL;
    U16 STR_CRC_num = 0;
    int len = 0;
    unsigned long lx_mem_addr = 0, lx_mem_size = 0 , lx_mem2_addr = 0, lx_mem2_size = 0 , lx_mem3_addr = 0, lx_mem3_size = 0;
    unsigned long crc_result_1 = 0, crc_result_2 = 0, crc_result_3 = 0, pm_crc_1 = 0, pm_crc_2 = 0, pm_crc_3 = 0;
    int i;

    UBOOT_TRACE("IN\n");
    pEnv = getenv("str_crc");
    crclevel=simple_strtol(pEnv, NULL, 16);
    if (pEnv != NULL && crclevel == 3)
    {
	lx_mem_addr        = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc0 * 2) * 0x2000;
	lx_mem2_addr       = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc4 * 2) * 0x2000;
	lx_mem3_addr       = HAL_MBOOT_Read2Byte(0x1F200c00 + 0xc0 * 2) * 0x2000;
	lx_mem_size        = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc2 * 2) * 0x2000 - lx_mem_addr;
	lx_mem2_size       = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc6 * 2) * 0x2000 - lx_mem2_addr;
	lx_mem3_size       = HAL_MBOOT_Read2Byte(0x1F200c00 + 0xc2 * 2) * 0x2000 - lx_mem3_addr;

	HAL_MBOOT_Write2Byte(0x1F200C00 + 0xF0*2, HAL_MBOOT_Read2Byte(0x1F200C00 + 0xF0*2) | 0x4000);
	crc_result_1 = BDMACRCCalc(0,lx_mem_addr, lx_mem_size);
	crc_result_2 = BDMACRCCalc(0,lx_mem2_addr, lx_mem2_size);
	crc_result_3 = BDMACRCCalc(1,lx_mem3_addr, lx_mem3_size);

	printf("\033[31m########################## \n");
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem_addr, lx_mem_size, crc_result_1);
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem2_addr, lx_mem2_size, crc_result_2);
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem3_addr, lx_mem3_size, crc_result_3);
	printf("########################## \033[0m\n");
    }
    UBOOT_TRACE("OK\n");
}

#define MSTAR_SLEEP_MAGIC         0x4D535452

#if (CONFIG_MIPS32 == 1)
#define PMU_WAKEUP_ADDR_REGL     0xBF001CC8
#define PMU_WAKEUP_ADDR_REGH     0xBF001CC8
#else
#if defined(CONFIG_MSTAR_M3822) //this register must be aligned with kernel pm.h
#define PMU_WAKEUP_ADDR_REGL     0x1F000C00
#define PMU_WAKEUP_ADDR_REGH     0x1F000C00
#else
#define PMU_WAKEUP_ADDR_REGL     0x1F001D48
#define PMU_WAKEUP_ADDR_REGH     0x1F001D48
#endif
#endif
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00


#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2

#if CONFIG_MCU_MIPS32
#define MIU0_VIRTBASE1   0x80000000
#define MIU1_BASE_ADDR1  0x60000000
#define MIU1_BASE_ADDR2  0x70000000
#define MIU1_VIRTBASE1   0x00000000
#define MIU1_VIRTBASE2   0x20000000
#define _PHY_TO_VIRT(x) ((x<MIU1_BASE_ADDR1)?(x+MIU0_VIRTBASE1):    \
                          ((x<MIU1_BASE_ADDR2)?(x-MIU1_BASE_ADDR1+MIU1_VIRTBASE1):(x-MIU1_BASE_ADDR2+MIU1_VIRTBASE2)) )
#elif CONFIG_MCU_ARM
#define M512M (512*1024*1024)
#define _PHY_TO_VIRT(x) ( (((x)>=(CONFIG_MIU0_BUSADDR)&&(x)<(CONFIG_MIU0_BUSADDR+M512M)) \
                            ||((x)>=(CONFIG_MIU1_BUSADDR)&&(x)<(CONFIG_MIU1_BUSADDR+M512M)))? \
                            (x):(0) )
#else
#define _PHY_TO_VIRT(x) (x)
#endif
#define PHY_TO_VIRT(x) _PHY_TO_VIRT((unsigned long)(x))

unsigned long read_wkup_pmu(void)
{
    volatile unsigned long *preg=0;
    unsigned long val=0;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    val = ((*preg)&PMU_WAKEUP_ADDR_LMASK);
    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    val |= ((*preg)&PMU_WAKEUP_ADDR_HMASK);

    return val;
}
void write_wkup_pmu(unsigned long val)
{
    volatile unsigned long *preg=0;
    unsigned long oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_LMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_LMASK);
    (*preg) = oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_HMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_HMASK);
    (*preg) = oldval;

}

int _is_str_resume(void)
{
    unsigned long pmu_str_reg=0,*pwkupsave=0;
    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<WAKEUP_ADDR_SHIFT);
#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if((pmu_str_reg & WAKEUP_FLAG_MASK) == WAKEUP_FLAG_SLEPT && pwkupsave
    	&& pwkupsave[0]==MSTAR_SLEEP_MAGIC && pwkupsave[1])
    {
		return 1;
    }
    return 0;
}

int check_str_resume(void)
{
    if(!(MDrv_PM_PowerOnMode() == E_PM_POWERON_ACON) &&
        (_is_str_resume()==1))
    {
        CRCCheck();
        return 1;
    }

    return 0;
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

int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    unsigned long pmu_str_reg=0,*pwkupsave=0;
    char* s = NULL;


    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<16);

#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if(_is_str_resume())
    {
        s = getenv("KERNEL_PROTECT");
        if (s != NULL)
        {
            MsApi_kernelProtect();
        }
		check_str_resume();

        write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);
        printf("[AT][MB][jump to kernel][%lu]\n", MsSystemGetBootTime());
        MsDrv_StrSetGICInterface();
#if CONFIG_MCU_MIPS32
        {
            void (*resume)(void);
            resume = (void *)PHY_TO_VIRT(pwkupsave[1]);
            if(resume)
            resume();
        }
#elif CONFIG_MCU_ARM
    cleanup_before_linux();
#if CONFIG_ARMv8_64BIT_KERNEL

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	printf("\033[0;32;31m [Ian] %s %d ARMv8 Trustzone STR flow\033[m\n",__func__,__LINE__);

	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r1, =0x0\n\t"
	"ldr  r2, =0x0\n\t"
	"ldr  r3, =0x0\n\t"
	"ldr  r4, =0x0\n\t"
	"smc #0         \n\t"
	::
	: "r0","r3","r4","r2"
	);
#else
	__asm__ __volatile__(
		"ldr r0, [%0]\n"
		"smc #0\n"
		::"r"(&pwkupsave[1]):"r0");
#endif //CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE

#else

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)

		do_nonsec_virt_switch();
#endif
        __asm__ volatile (
            "ldr r1, %0\n"
            "ldr r1, [r1, #4]\n"
            "cpsid if, #0x13\n"
            //; Disable MMU
            "MRC   p15, 0, r0, c1, c0, 0\n"
            "BIC   r0, r0, #0x1\n"
            "MCR   p15, 0, r0, c1, c0, 0\n"
            //; jump to wakeup address
            "mov pc, r1\n"
            ::"m"(pwkupsave):"r0","r1","memory","cc");
#endif
#endif
    }
    else
    {
        write_wkup_pmu(0);
    }
    return 0;
}
#ifdef CONFIG_MSTAR_FAST_STR
int do_fast_str_backup(void)
{
    ST_FSTR_BACKUP *stFstrBackUp=NULL;
    stFstrBackUp=(ST_FSTR_BACKUP *)(CONFIG_SYS_MIU0_CACHE+0x1F0000);
    memset(stFstrBackUp, 0x0, sizeof(stFstrBackUp));
    MSDrv_FSTR_MiuBackup(&stFstrBackUp->stMiuInfo);
    //Miu information Back Up
    UBOOT_DEBUG("*****u32NuttxAddr=[0x%x] u32AESAddr [0x%x] \n",stFstrBackUp->u32NuttxAddr,stFstrBackUp->u32AESAddr);
    UBOOT_DEBUG("*****u32MiuPriority[0]=[0x%x] u32MiuPriority[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuPriority[0],stFstrBackUp->stMiuInfo.u32MiuPriority[1]);
    UBOOT_DEBUG("*****MIU 0 u32MiuSel[0]=[0x%x] u32MiuSel[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuSel[0][0],stFstrBackUp->stMiuInfo.u32MiuSel[0][1]);
    UBOOT_DEBUG("*****MIU 1 u32MiuSel[0]=[0x%x] u32MiuSel[1] [0x%x] \n",stFstrBackUp->stMiuInfo.u32MiuSel[1][0],stFstrBackUp->stMiuInfo.u32MiuSel[1][1]);
    return 0;
}
#endif

#endif
