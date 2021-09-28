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

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <MsCmdTable.h>
#include <MsCoreRegister.h>
#include <MsCustomerRegister.h>

#include <MsInit.h>
#include <ShareType.h>
#include <drvGPIO.h>
#include <drvIRQ.h>
#include <MsOS.h>
#include <MsDevice.h>
#include <common.h>
#include <command.h>
#include <drvSERFLASH.h>
#include <drvBDMA.h>
#include <drvSYS.h>
#include <drvPM.h>
#include <mmc.h>
#include <MsDebug.h>
#include <CusCB.h>
#include <drvWDT.h>
#include <miu/MsDrvMiu.h>
#include <MsSystem.h>
#include <MsSysUtility.h>
#include <drvSEM.h>
#include <asm/system.h>

#if defined (CONFIG_USB_PREINIT)
#include <usb.h>
#endif
#if(ENABLE_URSA_8==1)
#include <drvMSPI.h>
#endif

#if defined (CONFIG_USB_PREINIT)
void init_usb(void)
{
    usb_preinit(0);
#if defined(ENABLE_SECOND_EHC)
    usb_preinit(1);
#endif
#if defined(ENABLE_THIRD_EHC)
    usb_preinit(2);
#endif
}
#endif


extern int msI2C_init(void);
extern void msKeypad_Init(void);
#if(ENABLE_IR)
    extern void msIR_Initialize(U8 irclk_mhz);
#elif(ENABLE_DYNAMIC_IR)
    extern void drv_IR_SW_Initialize(void);
#else
#endif

extern MS_BOOL MDrv_MMIO_GetBASE(MS_U32 *u32Baseaddr, MS_U32 *u32Basesize, MS_U32 u32Module);
extern MS_BOOL MDrv_MMIO_Init(void);
#ifdef STATIC_RELIABLE_TEST
extern int do_test_reliable_write(void);
#endif

unsigned long MS_RIU_MAP = CONFIG_SYS_RIU_BASE;
unsigned long gOpteeFailFlag = 0;

// Internal Data
static int gMBootBakFlag     = 0;
static MS_BOOL gDataCfgFlg   = FALSE;

void msFlash_ActiveFlash_Set_HW_WP(BOOLEAN bEnable)
{
    if(bEnable)
    {
#if(PIN_FLASH_WP0)
        mdrv_gpio_set_low( PIN_FLASH_WP0);
#endif
        udelay (500);
    }
    else
    {
#if(PIN_FLASH_WP0)
        mdrv_gpio_set_high( PIN_FLASH_WP0);
#endif
        udelay (500);
    }
}
#if defined(CONFIG_TIMER_TEST)
void TimerISR(MS_U32 u32Tick)
{
    static unsigned int clk = 0;

    if(clk!=0)
    {
        printf("TIME COUNT %d\n", clk);
    }
    clk++;

    u32Tick=u32Tick;
}

void MDrv_Timer_ISR_Register(void)
{
    MS_U32 check_timer_status;
    check_timer_status = MsOS_CreateTimer( (TimerCb)TimerISR,
                                                  0,
                                                  1000,
                                                  TRUE,
                                                  "Check timer");
}
#endif

void Load_SBoot_Msg(void)
{
    MS_U16 reg_val;

    reg_val = (*(volatile U16*)(CONFIG_SYS_RIU_BASE + (0x103380<<1)));
    gMBootBakFlag = ((reg_val >> 8)&0xFF) == 1 ? 1 : 0;

    // Data Load complete flag
    gDataCfgFlg = TRUE;
}



#if defined(__ARM__)

void MstarSysInit(void)
{
    uint u32Tmp;
    /*set up Mstar IRQ handler*/
    extern void mhal_fiq_merge(void);
    extern void MAsm_CPU_TimerStart(void);
    extern void dcache_init(void);
    extern void l2_cache_init(void);

    gOpteeFailFlag = (*(volatile U32*)(CONFIG_SYS_RIU_BASE+(0x103382<<1))==0xEEFF)?1:0;
    if(gOpteeFailFlag != 0)
    {
        UBOOT_ERROR("Optee OS is not running!!!\n");
    }
    Load_SBoot_Msg();

#if !defined(CONFIG_MSTAR_NS_UBOOT) && !defined(CONFIG_OPTEE_TEELOADER)
    mhal_fiq_merge();
#else
    if(gOpteeFailFlag != 0)
    {
        mhal_fiq_merge();
    }
#endif
    MsOS_Init();

#if defined(CONFIG_MSTAR_MUSTANG) && defined(CONFIG_ARMv7_OPTEE)
    uint32_t reg;
    icache_disable();
    dcache_disable();
    reg = get_cr();
    set_cr(reg & ~1);
    asm("isb");
    asm("dsb");
    //flush_cache_all();
    v7_dma_inv_range(0x0,0x40000000);
    dcache_init();
#elif (defined(CONFIG_MSTAR_MUNICH) || defined (CONFIG_MSTAR_MACAN) || defined(CONFIG_MSTAR_MUSTANG) || defined(CONFIG_MSTAR_M3822)) || defined (CONFIG_MSTAR_MAZDA)
        #ifdef CONFIG_SYS_DCACHE
        dcache_init();
        #endif
    flush_cache_all();
#else
    flush_cache_all();

        #ifdef CONFIG_SYS_DCACHE
        dcache_init();
        #endif

#endif

#ifndef CONFIG_L2_OFF
    l2_cache_init();
#endif
#if defined(CONFIG_TIMER_TEST)
    MDrv_Timer_ISR_Register();
#endif
    MsOS_CPU_EnableInterrupt();
    MAsm_CPU_TimerStart();

    MDrv_MMIO_Init();
    MDrv_MMIO_GetBASE((MS_U32 *)&MS_RIU_MAP, (MS_U32 *)&u32Tmp, MS_MODULE_PM);

#if(ENABLE_IR)
    msIR_Initialize(12);
#elif(ENABLE_DYNAMIC_IR)
    drv_IR_SW_Initialize();
#else
#endif

    MDrv_SEM_Init();

    MDrv_SYS_Init();

#if defined(UFO_SYS_TEE_INFO_SET_MBOOT) && \
    (defined(CONFIG_MSTAR_CURRY) || defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K7U))
#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
    if(gOpteeFailFlag == 0)
        MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_OPTEE);
    else
        MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NOTEE);
#elif defined(CONFIG_R2_BRINGUP)
    MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NUTTX);
#else
    MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NOTEE);
#endif
#endif /* UFO_SYS_TEE_INFO_SET_MBOOT */

    MDrv_WDT_Init(0);
    MsDrv_MiuInit();

#if (ENABLE_MODULE_ENV_IN_SERIAL==1)
#if (ENABLE_MODULE_SPI == 1)
    puts ("SPI:  ");
    {
        extern int spi_init (void);
        spi_init();     /* go init the SPI */
    }

    #if (CONFIG_MSTAR_BD_MST028B_10AFX_EAGLE||CONFIG_MSTAR_BD_MST038B_10AHT_EAGLE ||CONFIG_MSTAR_BD_MST049B_10AQV_NIKE || CONFIG_MSTAR_BD_MST049B_10AQV_NIKE_U)
    MDrv_SERFLASH_SetWPInfo(TRUE);
    #else
    ms_Flash_SetHWWP_CB pCB = msFlash_ActiveFlash_Set_HW_WP;
    MDrv_SERFLASH_SetFlashWPCallBack(pCB);
    FlashSetHWWPCB = FlashSetHWWPCB;
    McuChipSelectCB = McuChipSelectCB;
    #endif
#endif
#else
    #if (ENABLE_MODULE_SPI==1)
        MDrv_SERFLASH_Init();
    #endif
#endif

#if(ENABLE_URSA_8==1)
    MDrv_MSPI_Init_Ext(0x0);
#endif

#if (ENABLE_MODULE_GPIO == 1)
    mdrv_gpio_init();
#endif

#if (ENABLE_MODULE_BDMA == 1)
    MDrv_BDMA_Init(CONFIG_SYS_MIU_INTERVAL);
#endif

#if defined (CONFIG_USB_PREINIT)
        init_usb();
#endif
    #ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN
    run_command("init_raw_io" , 0);
    run_command("config_raw_io" , 0);
    #endif
    CusCallback();
}

#else


void MstarSysInit(void)
{
    uint u32Tmp;
    extern MS_BOOL MDrv_MIU_SetIOMapBase(void);

    Load_SBoot_Msg();
    MsOS_Init();
    MDrv_MMIO_Init();
    MDrv_MMIO_GetBASE((MS_U32 *)&MS_RIU_MAP, (MS_U32 *)&u32Tmp, MS_MODULE_PM);

#if(ENABLE_IR)
    msIR_Initialize(12);
#elif(ENABLE_DYNAMIC_IR)
    drv_IR_SW_Initialize();
#else
#endif

    MDrv_SEM_Init();
    MDrv_MIU_SetIOMapBase();
    MsOS_CPU_EnableInterrupt();
    MDrv_WDT_Init(0);
    MsDrv_MiuInit();
    MDrv_SYS_Init();

    gOpteeFailFlag = (*(volatile U32*)(MS_RIU_MAP+(0x103382<<1))==0xEEFF)?1:0;
    if(gOpteeFailFlag != 0)
    {
        UBOOT_ERROR("Optee OS is not running!!!\n");
    }
#if defined(UFO_SYS_TEE_INFO_SET_MBOOT) \
    && (defined(CONFIG_MSTAR_CURRY) || defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K7U))
#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
    if(gOpteeFailFlag == 0)
        MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_OPTEE);
    else
        MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NOTEE);
#elif defined(CONFIG_R2_BRINGUP)
    MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NUTTX);
#else
    MDrv_SYS_SetTEEInfo(SYS_TEEINFO_OSTYPE_NOTEE);
#endif
#endif /* UFO_SYS_TEE_INFO_SET_MBOOT */

#if (ENABLE_MODULE_SPI == 1)
#if (ENABLE_MODULE_ENV_IN_SERIAL==1)
    puts ("SPI:  ");
    {
        extern int spi_init (void);
        spi_init();     /* go init the SPI */
    }
    ms_Flash_SetHWWP_CB pCB = msFlash_ActiveFlash_Set_HW_WP;
    MDrv_SERFLASH_SetFlashWPCallBack(pCB);
#else
    MDrv_SERFLASH_Init();
#endif
#endif

#if (ENABLE_MODULE_BDMA == 1)
    MDrv_BDMA_Init(CONFIG_SYS_MIU_INTERVAL);
#endif

#if (ENABLE_MODULE_GPIO == 1)
    mdrv_gpio_init();
#endif

#if defined (CONFIG_USB_PREINIT)
    init_usb();
#endif
#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN
    run_command("init_raw_io" , 0);
    run_command("config_raw_io" , 0);
#endif
    CusCallback();
}


#endif

void MstarInitByFun(void)
{

//for Customer keypad init to set in board_r
#ifdef CONFIG_KEYPAD
    msKeypad_Init();
#endif

#if(CONFIG_MSTARI2C == 1)
    msI2C_init();
#endif

    char *pMiu0Size=NULL;
    char *pMiu1Size=NULL;
    char *pMiu2Size=NULL;
    pMiu0Size=MsDrv_GetMIUSize(0); //miu0
    pMiu1Size=MsDrv_GetMIUSize(1); //miu1
    pMiu2Size=MsDrv_GetMIUSize(2); //miu2
    if(pMiu0Size!=NULL)
    {
         printf("DRAM_SIZE1=%s MB \n",pMiu0Size);
    }
    if(pMiu0Size!=NULL)
    {
         printf("DRAM_SIZE2=%s MB \n",pMiu1Size);
    }
    if(pMiu0Size!=NULL)
    {
         printf("DRAM_SIZE3=%s MB \n",pMiu2Size);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @Functino Name :  MstarInit
/// @brief         :  This function is specially for device init via cmd.
///////////////////////////////////////////////////////////////////////////////////////////////////

int MstarDrvInit(void)
{
    int reval=0;

    // SRAM used by sboot & TSP. now switch for TSP in case DMX load TSP FW fail in multiprocess enviroment.
    WriteRegBit(0x101EB4, 1<<5, FALSE);  //set 0x101E_B4[5] = 0

#if CONFIG_MINIUBOOT
    #if (CONFIG_APP_IN_SPI)
    run_command("spi ptinit", 0);
    #endif
#else
    ST_CMD_RECORED *pCmd=NULL;
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m -------------------MstarDrvInit----------------------------\033[0m\n");
    UBOOT_DEBUG("\n \033[0;35m ===========================================================\033[0m\n");

    Table_Init();

    Core_Register_MsInit();
    Customer_Register_MsInit();

    pCmd=getFirstCmd();
    if(pCmd!=NULL)
    {
        while(1)
        {

            if(pCmd->stage == STAGE_MSINIT)
            {
                UBOOT_BOOTTIME("[AT][MB][%s][%lu]_start\n",pCmd->cmd, MsSystemGetBootTime());
                run_command(pCmd->cmd,  pCmd->flag);
                UBOOT_BOOTTIME("[AT][MB][%s][%lu]_end\n",pCmd->cmd, MsSystemGetBootTime());
            }

            pCmd=getNextCmd(pCmd);
            if(pCmd==NULL)
            {
                break;
            }
        }
    }
    else
    {
        printf("There are no any commands in table\n");
    }


    MstarInitByFun();
#endif

#ifdef STATIC_RELIABLE_TEST
    char buf[16]={0};
    char * reliable_test_ctl = getenv("reliable_test_ctl");

    if(reliable_test_ctl != NULL)
    {
        if((int)simple_strtol(reliable_test_ctl, NULL, 10)==1)
        {
            do_test_reliable_write();

            //set reliable_test_ctl=0
            sprintf(buf, "%d", 0);
            setenv("reliable_test_ctl", buf);
            saveenv();
        }
    }
#endif

    return reval;
}


int MsApi_GetMBootBak(void)
{
    if (gDataCfgFlg != TRUE)
    {
        printf("\033[31;1mError : gDataCfgFlg is Not Inited !\033[0m\n");
        return 0;
    }
#if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
    return gMBootBakFlag;
#else
    return 0;
#endif
}
