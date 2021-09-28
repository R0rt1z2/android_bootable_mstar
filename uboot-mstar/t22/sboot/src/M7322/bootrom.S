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


#ifndef _BOOT_INC_
#include "boot.inc"
#endif

#ifdef CONFIG_ONEBIN_ENABLE
#include "drvLoadTbl.h"
#endif

    .text

BOOT_MESSAGE_AC_ON: .short  '\r', '\n', 'A', 'C', '_', 'O', 'N', '\r', '\n', 0x00
BOOT_MESSAGE_DC_ON: .short  '\r', '\n', 'D', 'C', '_', 'O', 'N', '\r', '\n', 0x00

    .align      4

//-------------------------------------------------------------------------------------------------
// BootRom_Entry:
// @param  None
// @return None
// @note   Entry point of Boot code
//-------------------------------------------------------------------------------------------------
    .globl      BootRom_Entry
BootRom_Entry:

    DBGREG      0x0001

#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN
    bl          BootRom_PLLRecovery
#endif /*CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN*/

#ifndef CONFIG_MSTAR_TITANIA_BD_FPGA

    //Configure PAD top for MUX and GPIO (e.g. DDR power)
#if (CONFIG_ONEBIN_ENABLE == 1)
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =MIUBuffer
    ldr         r2, =PADPRE
    bl          drv_LoadTbl
    ldr         r0, =MIUBuffer
#else
    ldr         r0, =padInitTbl_PreInit
#endif
    add         r0, r0, #10
    bl          MDrv_WriteRegMaskTbl

#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN

#ifdef CONFIG_MSTAR_VID_ENABLE
    //Initial Voltage Identification
    bl          BootRom_InitVid
#endif  /*CONFIG_MSTAR_VID_ENABLE*/

    //Initial releated settings of PLL
    bl          BootRom_InitPll

    //Initial related settings of Clock
    bl          BootRom_InitClock

    DBGREG      0x0002

#endif  /*CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN*/
#endif  /*CONFIG_MSTAR_TITANIA_BD_FPGA*/

    //Initial related settings of UART
    bl          BootRom_InitPeripheral

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
    bl          BootRom_SaveHash1Version
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    //Enable Dynamic Voltage / Frequency Selection (DVFS)
    bl          BootRom_InitDvfs
#endif

    // Set CPU L4 clock MUX_1 to 24MHz
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101800 << 1))
    ldr         r0, =0x00
    strb        r0, [r1, #(0x82 << 1)]
    
    // Set CPU WROR setting
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x313A00 << 1))
    ldr         r1, [r0, #(0x00 << 1)]
    bic         r1, r1, #(0x01 << 0)
    str         r1, [r0, #(0x00 << 1)]  
    
#if !defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && \
    !defined(CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51)

    //Initial related settings of MIU
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   //Disable Divisor Latch Access
    lsr         r0, r0, #14
    ldr         r1, =0x0003
    cmp         r0, r1
    beq         BootRom_Str_AcFlow

#ifdef CONFIG_MSTAR_STR_ENABLE

    bl          BootRom_InitMiu_STR

#ifdef CONFIG_MSTAR_SELF_STR_TEST_ENABLE
    bl          Miu0_BistTest_Str
    bl          Miu1_BistTest_Str

Self_Str_Test_Level:
    b           Self_Str_Test_Level
#endif

#else
    DPUTK_STR   BOOT_MESSAGE_DC_ON
    b           BootRom_Str_InitMiu
#endif /* CONFIG_MSTAR_STR_ENABLE */

    b           BootRom_Str_Exit

BootRom_Str_AcFlow:
    DPUTK_STR   BOOT_MESSAGE_AC_ON

BootRom_Str_InitMiu:

    bl          BootRom_InitMiu
BootRom_Str_Exit:

#endif
    bl          BootRom_SetL4Clock

#ifdef CONFIG_MSTAR_EFUSE_DDR_ATOP
    //Initial DDR ATOP Trimming Control Setting
    bl          BootRom_InitDdrAtopTrimming
#endif

#ifdef CONFIG_TEE_LOADER
    DBGREG      0xDEE1
    bl          MDrv_SEAL_Init
    DBGREG      0xDEE2
    ldr         r0, =(_ld_TEE_LDR_load_start - CONFIG_MIU0_BUSADDR) @//Set the star address of Secure Range
    ldr         r1, =0x0
    ldr         r2, =(_ld_TEE_LDR_load_end   - CONFIG_MIU0_BUSADDR) @//Set the end address of Secure Range
    add         r2, #0x10000                                        @//Secure range end=_ld_TEE_LDR_load_end+bss session(64KB)
    movw        r6, #0xfff                                          @//4K align
    add         r2, r6
    mov         r2, r2, LSR#12
    mov         r2, r2, LSL#12
    ldr         r3, =0x0
    ldr         r7, =0x3                                            @//SEAL_ATTR_SECURE_RW (0x03),
    str         r7, [sp, #0]
    bl          MDrv_SEAL_SetSecureRange
#endif

//================================================================================================
#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
    // jump back to rom code during USB upgrade procedure
    RREG        r2, BOOT_INFO_REGISTER_BOOT_STAGE
    cmp         r2, #CONFIG_BOOT_STAGE_PC_UPGRADE_1
    bne         BootRom_InitMiu_End


    WREG        BOOT_INFO_REGISTER_PC_UPGRADE_DRAM_ADDR_LOW, ((CONFIG_UBOOT_LOADADDR - 0x400) & 0xFFFF)
    WREG        BOOT_INFO_REGISTER_PC_UPGRADE_DRAM_ADDR_HIGH, (((CONFIG_UBOOT_LOADADDR - 0x400) >> 16) & 0xFFFF)
    ldr         r0, =_CONFIG_HASH1_END
    sub         r0, r0, #4
    ldr         r1, [r0]
    ldr         r0, =BOOT_INFO_REGISTER_PC_UPGRADE_DRAM_SIZE_LOW
    str         r1, [r0]
    lsr         r1, r1, #16
    ldr         r0, =BOOT_INFO_REGISTER_PC_UPGRADE_DRAM_SIZE_HIGH
    str         r1, [r0]

    RREG        r0, BOOT_INFO_REGISTER_PC_UPGRADE_ADDR_HIGH
    RREG        r1, BOOT_INFO_REGISTER_PC_UPGRADE_ADDR_LOW
    lsl         r0, r0, #16
    orr         r0, r0, r1
    mov         pc, r0

BootRom_InitMiu_End:
#endif

//================================================================================================
    //Initial related settings of cache
    bl          BootRom_InitCache

//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)

    bl          BootRom_SwitchIMI

#if defined(CONFIG_TEE_LOADER)
    ldr         r0, =_ld_TEE_LDR_load_start
    ldr         r1, =_ld_TEE_LDR_size
#else
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram_size
#endif
    bl          drvNAND_Load2ndBL

#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    bl          BootRom_SwitchIMI

    #ifdef CONFIG_DOUBLE_MBOOT
    ldr         r0, =_ld_bootram1_start
    ldr         r1, =_ld_bootram1_size
    #elif defined(CONFIG_TEE_LOADER)
    ldr         r0, =_ld_TEE_LDR_load_start
    ldr         r1, =_ld_TEE_LDR_size
    #else
    ldr         r0, =CONFIG_BOOTRAM_LOADADDR
    ldr         r1, =_ld_bootram_size
    #endif

#ifdef CONFIG_BOOTRAM_STORE_IN_MBOOT
    ldr         r2, =0x1002     //bootram store in mboot.bin's 0x400 offset(0x200400)
    bl          eMMC_ReadData_MIU
#else
    ldr         r2, =CONFIG_EMMC_BOOTRAM_ADDRESS
    mov         r2, r2, lsr #9
    bl          eMMC_LoadImages
#endif

#else
    //Copy image of bootram to DRAM
    bl          BootRom_LoadBootRam

#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
    #ifdef CONFIG_DOUBLE_MBOOT
        ldr         r0, =_ld_bootram1_start                        //Set the starting address of HASH for Authentication
        ldr         r1, =(_ld_bootram1_size - 0x400)               //Set the HASH size for Authentication
        ldr         r2, =(_ld_bootram1_end - 0x100)                //Set the starting address of SIG for Authentication
    #elif defined(CONFIG_TEE_LOADER)
        ldr         r0, =_ld_TEE_LDR_load_start                    //Set the starting address of HASH for Authentication
        ldr         r1, =(_ld_TEE_LDR_size - 0x400)                //Set the HASH size for Authentication
        ldr         r2, =(_ld_TEE_LDR_load_end - 0x100)            //Set the starting address of SIG for Authentication
    #else
        ldr         r0, =CONFIG_BOOTRAM_LOADADDR                   //Set the starting address of HASH for Authentication
        ldr         r1, =(_ld_bootram_size - 0x400)                //Set the HASH size for Authentication
        ldr         r2, =(_ld_bootram_end - 0x100)                 //Set the starting address of SIG for Authentication
    #endif
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRom_AuthenticationFail
#endif


#if 1 // move to here @20181227 @M7322
//Configure PAD top for MUX and GPIO (e.g. DDR power)
#ifdef CONFIG_ONEBIN_ENABLE
    bl          Get_Packge_CFG @put result to r1
    ldr         r0, =RamBuffer
    ldr         r2, =PAD
    bl          drv_LoadTbl
    ldr         r0, =RamBuffer
#else
    ldr         r0, =padInitTbl
#endif
    add         r0, r0, #10
    bl          MDrv_WriteRegMaskTbl

    //Configure Analog GPIO
    ldr         r0, =analogPadInitTbl
    add         r0, r0, #10
    bl          MDrv_WriteRegMaskTbl
#endif

//================================================================================================
#ifdef CONFIG_MSTAR_STR_ENABLE
    bl          BootRom_StrEntry
#endif

//================================================================================================
#if defined(CONFIG_ENCRYPTED_TEE_LOADER)
    DBGREG      0xDEE3
    ldr         r0, =(_ld_TEE_LDR_load_start - CONFIG_MIU0_BUSADDR) @//Set the starting address for AES Decrypt
    ldr         r1, =(_ld_TEE_LDR_size - 0x400)                     @//Set the size for AES Decrypt
    ldr         r2, = 0x0                                           @//Don't care
    ldr         r3, = 0xA100                                        @//AESDMA_ENGINE_CBC
    ldr         r7, = 0x100                                         @//E_AESDMA_UNIFORMKEY0 = 0x100,
    str         r7, [sp, #0]
    bl          MDrv_AESDMA_Decrypt
    ldr         r1, =(_ld_TEE_LDR_load_start)
    ldr         r2, [r1]
    ldr         r3, =0x3052444c                                     @//Check LDR0 tag
    cmp         r2, r3
    bne         BootRom_DecryptFail
#endif
    DBGREG      0x0003

#if defined(CONFIG_TEE_LOADER) && defined( CONFIG_PROGRAM_EMMC_RPMB_KEY) 
    /* Use CONFIG_PROGRAM_EMMC_RPMB_KEY to distinguish OPTEE and R2TEE */

    # Anti-rollbacok
    DBGREG      0xDEE4

    @// Check if RPMB Key was programmed
    ldr         r0, =0x0000                          @// Pass NULL for eFuse Key
    bl          eMMC_RPMB_Check_Program_Key

    @// Copy rpmb_fs_partition to DRAM Buffer
    ldr         r0, =CONFIG_UBOOT_LOADADDR           @//Temporarily use UBOOT_LOADADDR as DRAM buf
    ldr         r1, =0x100
    ldr         r2, =0x0
    ldr         r3, =0x0
    bl          eMMC_RPMB_Read_data

    @// Load HASH2_VER_REF
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r2, [r0]                             @//teeloader header in rpmb_fs_partition
    ldr         r1, [r0, #112]                       @//teeloader_version in rpmb_fs_partition
    cmn         r2, #1                               @//detect empty flash
    cmneq       r1, #1                               @//if ((header == 0xFFFFFFFF) && (ver_ref == 0xFFFFFFFF))
    beq         EmptyRPMB                            @//    EmptyRPMB
    cmp         r2, #0                               @//else if((header == 0x0) && (ver_ref == 0x0))
    cmp         r1, #0                               @//    EmptyRPMB
    beq         EmptyRPMB

    @// Clear DRAM Buf
    ldr         r10, =CONFIG_UBOOT_LOADADDR
    ldr         r11, =CONFIG_UBOOT_LOADADDR + 0x100
    movw        r12, #:lower16:0xDEADBEEF
    movt        r12, #:upper16:0xDEADBEEF

    MEMSET32    RPMB_FS                              @//Clear RPMB_FS on DRAM buffer

    @// Load HASH2_VER_EXT
    ldr         r2, =(_ld_TEE_LDR_load_start)
    ldr         r3, [r2, #4]                         @//teeloader_version in loaded teeloader

    @// Fail if HASH2_VER_REF > HASH2_VER_EXT
    cmp         r1, r3
    bhi         BootRom_AuthenticationFail

EmptyRPMB:
    nop
#endif

    @//Jump to the entry of BOOTRAM
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH) && defined(CONFIG_DOUBLE_MBOOT)
    ldr         pc, =BOOTRAM_Entry_Hash
#elif defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    ldr         r0, =teeloader_entry
		bx          r0
#elif defined(CONFIG_TEE_LOADER)
    ldr         r1, =TEE_loader
    ldr         r0, [r1,#24]    //_ld_TEE_LDR_arg_start
    ldr         r1, =_ld_bootram_start
    str         r1, [r0, #0]    //ree_entry
    ldr         r1, =_ld_bootram_size
    str         r1, [r0, #4]    //ree_size
    mov         r1, #0
#if defined(CONFIG_VERIFY_REE_LOADER)
    orr         r1,r1,#1
#endif
#if defined(CONFIG_ENCRYPTED_REE_LOADER)
    orr         r1,r1,#2
#endif
#if defined(CONFIG_VERIFY_CUSTOMER_KEYBANK)
    orr         r1,r1,#8
#endif
#if defined(CONFIG_ENCRYPTED_CUSTOMER_KEYBANK)
    orr         r1,r1,#16
#endif
    orr         r1,r1,#32    //bEnableCKB
    str         r1, [r0, #8]    //ree_opt
    ldr         r1, =0x400
    str         r1, [r0, #12]    //CKB_Offset
    ldr         r1, =CONFIG_CUSTOMER_KEYBANK_LOADADDR
    str         r1, [r0, #16]    //CKB_address
#if defined(CONFIG_R2TEE_TEELOADER)
    mov         r1, #1120
#elif defined(CONFIG_OPTEE_TEELOADER)
    mov         r1, #8192
#endif
    str         r1, [r0, #20]    //CKB_size
    ldr         r3, =TEE_loader
    add         r3,r3,#0x100
    ldr         r4, =ree_entry
    bx          r3
#else
    ldr         pc, =BOOTRAM_Entry
#endif

BootRom_DummyLoop:
    b           BootRom_DummyLoop

//-------------------------------------------------------------------------------------------------
// BootRom_SwitchIMI
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
BootRom_SwitchIMI:
    ldr         r0, =(MS_BUS_RIU_ADDR + (0x100B00 << 1))
    ldr         r1, [r0, #(0xc8 << 1)]
    orr         r1, r1, #(0x01 << 7)
    str         r1, [r0, #(0xc8 << 1)]

#ifndef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
    //----------------------------------------------------------
    // switch memory to EMI from usb rom code
    //----------------------------------------------------------
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x100B00 << 1))
    ldr         r0, =0x01
    strb        r0, [r1, #(0x18 << 1)]      @//IMI/EMI clock_mux sel : 0x100b_18 bit [0], 1: EMI,  0: IMI (default)

    DELAYUS     100                         //wait clk_miu_usb stable

    ldr         r0, =(MS_BUS_RIU_ADDR + (0x101E00 << 1))
    ldr         r1, [r0, #(0x5a << 1)]
    bic         r1, r1, #(0x01 << 8)
    str         r1, [r0, #(0x5a << 1)]      //IMI/EMI MIU_MUX sel : 0x101e_5a bit [8], 0: EMI,  1: IMI (default)
#endif

    bx          lr

//-------------------------------------------------------------------------------------------------
// BootRom_PLLRecovery
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
BootRom_PLLRecovery:
    WREG        (MS_BUS_RIU_ADDR + (0x101E26 << 1)), 0x0200     //set ejtag padmux
    WREG        (MS_BUS_RIU_ADDR + (0x101882 << 1)), 0x0404     //Set L3 switch to xtali
    WREG        (MS_BUS_RIU_ADDR + (0x110C72 << 1)), 0x0101     //MIU_256BUS_PLL power down
    WREG        (MS_BUS_RIU_ADDR + (0x110C62 << 1)), 0x0101     //MIU_128BUS_PLL power down

    bx          lr

//================================================================================================
#if !defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) && \
    !defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
//-------------------------------------------------------------------------------------------------
// BootRom_LoadBootRam
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
BootRom_LoadBootRam:

    ldr         r10, =_ld_bootrom_end
    ldr         r11, =_ld_bootram_start
    ldr         r12, =_ld_bootram_end
    ldr         r9, =_ld_bootram_start
    sub         r12, r12, r9

_CopyDataLoop:
    ldr         r9, [r10], #4
    str         r9, [r11], #4
    subs        r12, r12, #4
    bne         _CopyDataLoop

    bx          lr
#endif

//-------------------------------------------------------------------------------------------------
// BootRom_WriteUartCharactor
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .globl      BootRom_WriteUartCharactor
BootRom_WriteUartCharactor:
    ldr         r12, =(MS_BUS_RIU_ADDR + (0x100900 << 1))
    strb        r0, [r12, #(0x80 << 1)]     //REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100980 << 1))
BootRom_WriteUartCharactor_Loop:
    ldr         r0, [r12, #(0x94 << 1)]     //RREG        r0, (MS_BUS_RIU_ADDR + (0x100994 << 1))
    and         r0, r0, #0x0060
    cmp         r0, #0x0060
    bne         BootRom_WriteUartCharactor_Loop

    bx          lr

//-------------------------------------------------------------------------------------------------
// BootRom_WriteUartString
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .globl      BootRom_WriteUartString
BootRom_WriteUartString:

    ldrb        r11, [r0], #2
    and         r11, r11, #0xff
    cmp         r11, #0x00
    beq         BootRom_WriteUartString_Exit

    ldr         r12, =(MS_BUS_RIU_ADDR + (0x100900 << 1))
    str         r11, [r12, #(0x80 << 1)]     //REG_C2M     r0, (MS_BUS_RIU_ADDR + (0x100980 << 1))
BootRom_WriteUartString_Loop:
    ldr         r11, [r12, #(0x94 << 1)]     //RREG        r0, (MS_BUS_RIU_ADDR + (0x100994 << 1))
    and         r11, r11, #0x60
    cmp         r11, #0x60
    bne         BootRom_WriteUartString_Loop
    b           BootRom_WriteUartString

BootRom_WriteUartString_Exit:

    bx          lr

//-------------------------------------------------------------------------------------------------
// BootRom_SetL4Clock
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
BootRom_SetL4Clock:

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101800 << 1))
    ldr         r0, =0x01
    strb        r0, [r1, #(0x84 << 1)]   //Select MIU2x Clock

    ldr         r1, =(MS_BUS_RIU_ADDR + (0x101800 << 1))
    ldr         r0, =0x80
    strb        r0, [r1, #(0x82 << 1)]   //Select MIU2x Clock

    bx          lr

//-------------------------------------------------------------------------------------------------
// BootRom_StrEntry
// @param  None
// @return None
// @note   None
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MSTAR_STR_ENABLE
    .global     BootRom_StrEntry
BootRom_StrEntry:

    mov         r8, lr

.if (MSTAR_STR_SBOOT_RESUME == 1)
    bl          read_wkup_pmu
    bl          put_reg_val

    is_slept
    bne         1f

    set_wakeup
    b           2f

1:
    eor         r0, r0, r0
    bl          write_wkup_pmu

2:
    is_wakeup
    bne         3f
    PUTK        'W'
    PUTK        'K'
    PUTK        'P'
    bl          read_wkup_pmu

    bic         r0, r0, #WAKEUP_FLAG_MASK
    mov         r0, r0, lsl #16             // wake up info addr
    ldmia       r0, {r1-r2}
    ldr         r0, =MSTAR_SLEEP_MAGIC
    cmp         r1, r0
    bne         3f

    PUTK        'J'
    cmp         r2, #0
    beq         3f

    PUTK        'M'
    mov         pc, r2                      // jump to wakup addr
3:
.endif

    mov         lr, r8
    bx          lr

#endif

//================================================================================================
#ifdef CONFIG_MSTAR_STR_ENABLE
#if 0   //calculate CRC for STR debug
MEM_REGION1_ST      = 0x00200000
MEM_REGION1_SZ      = 0x09F00000
//MEM_REGION2_ST   = 0x10E00000    //MIU0
//MEM_REGION2_SZ   = 0x0C200000    //MIU0

MEM_REGION2_ST      = 0x01800000    //MIU1
MEM_REGION2_SZ      = 0x0E800000    //MIU1

//Input: r0-region address   r1-resion size
//Output: r0-CRC value
BDMABank = 0x1F201200
BDMACRC_Calc_MIU:
    ldr         r2, =BDMABank

    mov         r3, #0
    str         r3, [r2, #(0x0c<<2)]
    str         r3, [r2, #(0x0d<<2)]
// mov         r3, #0x0340
// str         r3, [r2, #(0x02<<2)]

    str         r0, [r2, #(0x04<<2)]
    str         r0, [r2, #(0x06<<2)]
    lsr         r0, r0, #16
    str         r0, [r2, #(0x05<<2)]
    str         r0, [r2, #(0x07<<2)]

    str         r1, [r2, #(0x08<<2)]
    lsr         r1, r1, #16
    str         r1, [r2, #(0x09<<2)]

    ldr         r3, =0x1DB7
    str         r3, [r2, #(0x0a<<2)]
    ldr         r3, =0x04c1
    str         r3, [r2, #(0x0b<<2)]

    ldr         r3, [r2]
    orr         r3, r3, #1
    str         r3, [r2]

BDMACRC_Calc_agn:
    ldr         r3, [r2, #(0x01<<2)]
    tst         r3, #0x08
    beq         BDMACRC_Calc_agn

    ldr         r3, [r2, #(0x0c<<2)]
    ldr         r0, [r2, #(0x0d<<2)]
    orr         r0, r3, r0, LSL #16

    mov         pc, lr

 SBootMemCRCCheck:
    mov         r8, lr
    PUTK        'S'
    PUTK        'B'
    PUTK        'O'
    PUTK        'O'
    PUTK        'T'
    PUTK        ' '
    PUTK        'C'
    PUTK        'R'
    PUTK        'C'
    PUTK        ':'

    PUTK        '\n'
    PUTK        '\r'

    PUTK        '1'
    PUTK        '.'
    ldr         r0, =MEM_REGION1_ST
    ldr         r1, =MEM_REGION1_SZ
    WREG        (MS_BUS_RIU_ADDR + (0x100904 << 1)), 0x0340
    bl          BDMACRC_Calc_MIU
    bl          put_reg_val
    PUTK        '\n'
    PUTK        '\r'

    PUTK        '2'
    PUTK        '.'
    ldr         r0, =MEM_REGION2_ST
    ldr         r1, =MEM_REGION2_SZ
    WREG        (MS_BUS_RIU_ADDR + (0x100904 << 1)), 0x0341
    bl          BDMACRC_Calc_MIU
    bl          put_reg_val
    PUTK        '\n'
    PUTK        '\r'
    mov         pc, r8
    .ltorg
#endif

.if (MSTAR_STR_SBOOT_RESUME == 1)
read_wkup_pmu:
    ldr         r2, =PMU_WAKEUP_ADDR_REGL
    ldr         r0, [r2]

    ldr         r2, =PMU_WAKEUP_ADDR_REGH
    ldr         r1, [r2]

    and         r0, r0, #PMU_WAKEUP_ADDR_LMASK
    and         r1, r1, #PMU_WAKEUP_ADDR_HMASK
    orr         r0, r0, r1
    mov         pc, lr
    .ltorg

write_wkup_pmu:
    ldr         r3, =PMU_WAKEUP_ADDR_REGL
    ldr         r1, [r3]
    bic         r1, #PMU_WAKEUP_ADDR_LMASK
    and         r2, r0, #PMU_WAKEUP_ADDR_LMASK
    orr         r1, r1, r2
    str         r1, [r3]

    ldr         r3, =PMU_WAKEUP_ADDR_REGH
    ldr         r1, [r3]
    bic         r1, #PMU_WAKEUP_ADDR_HMASK
    and         r2, r0, #PMU_WAKEUP_ADDR_HMASK
    orr         r1, r1, r2
    str         r1, [r3]
    mov         pc, lr
    .ltorg
.endif

text_tbl: .short '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

// r9-r12 destroyed
put_reg_val:
    mov         r9, #32
    mov         r11, #0x0F
40:
    sub         r9, #4
    and         r10, r11, r0, LSR r9
    ldr         r12, =text_tbl
    add         r12, r12, r10, lsl #1
    ldr         r10,  [r12]
    mov         r12, #0x1F000000
    orr         r12, r12, #0x00200000
    orr         r12, r12, #0x1300
    strb        r10,[r12]
    cmp         r9, #0
    bgt         40b

    mov         r10, #'-'
    strb        r10,[r12]

    mov         pc, lr
    .ltorg
#endif

#ifdef CONFIG_TEE_LOADER
BootRom_DecryptFail:
    DPUTK       '\n'
    DPUTK       '\r'
    DPUTK       'D'
    DPUTK       'E'
    DPUTK       'C'
    DPUTK       'R'
    DPUTK       'Y'
    DPUTK       'P'
    DPUTK       'T'
    DPUTK       '_'
    DPUTK       'F'
    DPUTK       'A'
    DPUTK       'I'
    DPUTK       'L'
    b BootRom_FailHandler
#endif

#ifdef CONFIG_MSTAR_AUTHEN_ENABLE
BootRom_AuthenticationFail:
    DPUTK       '\n'
    DPUTK       '\r'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'F'
    DPUTK       'A'
    DPUTK       'I'
    DPUTK       'L'

#ifdef CONFIG_TEE_LOADER
BootRom_FailHandler:
    ldr         r10, =(_ld_TEE_LDR_load_start)
    ldr         r11, =(_ld_TEE_LDR_load_end  )
    movw        r12, #:lower16:0xDEADBEEF
    movt        r12, #:upper16:0xDEADBEEF
    MEMSET32    TEELoader
#endif

BootRom_AuthenticationFailLoop:
    b           BootRom_AuthenticationFailLoop
#endif

#ifdef CONFIG_MSTAR_EFUSE_DDR_ATOP
//-------------------------------------------------------------------------------------------------
// Initial DDR ATOP Trimming Control Setting
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitDdrAtopTrimming
BootRom_InitDdrAtopTrimming:

    push        {lr}

//Initial DDR ATOP Trimming Control Setting
#if !defined(CONFIG_MIU0_DRAM_NONE)
    ldr         r0, =0x0148
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x151600 << 1))
    bl          BootRom_InitDdrAtopTrimmingControl
#endif //CONFIG_MIU0_DRAM_NONE

    pop         {lr}
    bx          lr


//-------------------------------------------------------------------------------------------------
// BootRom_InitDdrAtopTrimmingControl
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .global     BootRom_InitDdrAtopTrimmingControl
BootRom_InitDdrAtopTrimmingControl:

    mov         r11, lr

    //Get Trim Value of DDR_ATOP in eFuse
    ldr         r5, =(MS_BUS_RIU_ADDR + (0x002000 << 1))
    str         r0, [r5, #(0x50 << 1)]
    orr         r0, r0, #0x2000
    str         r0, [r5, #(0x50 << 1)]

BootRom_ReadEfuse_DdrAtopLoop_0:

    ldr         r0, [r5, #(0x50 << 1)]
    and         r0, r0, #(0x0001 << 13)
    cmp         r0, #0
    bne         BootRom_ReadEfuse_DdrAtopLoop_0

    ldr         r6, [r5, #(0x58 << 1)]
    ldr         r7, [r5, #(0x5a << 1)]

    mov         r7, r7, lsl #16
    orr         r0, r6, r7

    bl          BootRom_DdrDrivingControl

    mov         lr, r11
    bx          lr


//-------------------------------------------------------------------------------------------------
// BootRom_DdrDrivingControl
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
    .global     BootRom_DdrDrivingControl
BootRom_DdrDrivingControl:
BootRom_DdrDrivingControl_DRVP:
    mov         r10, lr
    //MIU_drvp
    and         r4, r0, #(0x01 << 3)
    cmp         r4, #0
    beq         BootRom_DdrDrivingControl_DRVN

    //eFuse IO[2:0] = Driving Strength
    and         r4, r0, #0x07

    //clkp: {0x1516_55[5:0]}
    ldr         r2, [r1, #(0xaa << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xaa << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xaa << 1)]

    //clkn: {0x1516_55[13:8]}
    ldr         r2, [r1, #(0xaa << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xaa << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xaa << 1)]

    //cs: {0x1516_56[5:0]}
    ldr         r2, [r1, #(0xac << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xac << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xac << 1)]

    //cmd: {0x1516_56[13:8]}
    ldr         r2, [r1, #(0xac << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xac << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xac << 1)]

    //dqsp: {0x1516_57[5:0]}
    ldr         r2, [r1, #(0xae << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xae << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xae << 1)]

    //dqsn: {0x1516_57[13:8]}
    ldr         r2, [r1, #(0xae << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xae << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xae << 1)]

    //dq: {0x1516_58[5:0]}
    ldr         r2, [r1, #(0xb0 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xb0 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xb0 << 1)]

    //dqh: {0x1516_58[13:8]}
    ldr         r2, [r1, #(0xb0 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xb0 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xb0 << 1)]
    
    //dqsph: {0x1516_59[5:0]}
    ldr         r2, [r1, #(0xb2 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xb2 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xb2 << 1)]

    //dqsnh: {0x1516_59[13:8]}
    ldr         r2, [r1, #(0xb2 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xb2 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xb2 << 1)]

BootRom_DdrDrivingControl_DRVN:

    //MIU_drvn
    and         r4, r0, #(0x01 << 7)
    cmp         r4, #0
    beq         BootRom_DdrDrivingControl_Exit

    //eFuse IO[2:0] = Driving Strength
    lsr         r4, r0, #0x4
    and         r4, r4, #0x7

    //clkp: {0x1516_50[5:0]}
    ldr         r2, [r1, #(0xa0 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa0 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xa0 << 1)]

    //clkn: {0x1516_50[13:8]}
    ldr         r2, [r1, #(0xa0 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa0 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xa0 << 1)]

    //cs: {0x1516_51[5:0]}
    ldr         r2, [r1, #(0xa2 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa2 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xa2 << 1)]

    //cmd: {0x1516_51[13:8]}
    ldr         r2, [r1, #(0xa2 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa2 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xa2 << 1)]

    //dqsp: {0x1516_52[5:0]}
    ldr         r2, [r1, #(0xa4 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa4 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xa4 << 1)]

    //dqsn: {0x1516_52[13:8]}
    ldr         r2, [r1, #(0xa4 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa4 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xa4 << 1)]

    //dq: {0x1516_53[5:0]}
    ldr         r2, [r1, #(0xa6 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa6 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xa6 << 1)]

    //dqh: {0x1516_53[13:8]}
    ldr         r2, [r1, #(0xa6 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa6 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xa6 << 1)]

    //dqsph: {0x1516_54[5:0]}
    ldr         r2, [r1, #(0xa8 << 1)]
    and         r2, r2, #(0x3f << 0)
    lsr         r2, r2, #0

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa8 << 1)]
    bic         r5, r5, #(0x3f << 0)
    and         r6, r2, #0x3f
    lsl         r6, r6, #0
    orr         r5, r5, r6
    str         r5, [r1, #(0xa8 << 1)]
    
    //dqsnh: {0x1516_54[13:8]}
    ldr         r2, [r1, #(0xa8 << 1)]
    and         r2, r2, #(0x3f << 8)
    lsr         r2, r2, #8

    bl          BootRom_DdrAtopTrimmingDataProcess

    ldr         r5, [r1, #(0xa8 << 1)]
    bic         r5, r5, #(0x3f << 8)
    and         r6, r2, #0x3f
    lsl         r6, r6, #8
    orr         r5, r5, r6
    str         r5, [r1, #(0xa8 << 1)]

BootRom_DdrDrivingControl_DRVCAL_OFFSET:
    
    and         r4, r0, #(0x01 << 16)
    cmp         r4, #0
    beq         BootRom_DdrDrivingControl_Exit

    lsr         r4, r0, #0x8
    and         r4, r4, #0xff
    
    //dqsnh: {0x1516_61[15:8]}
    ldr         r2, [r1, #(0xc2 << 1)]
    bic         r2, r2, #(0xff << 8)
    lsl         r4, r4, #8
    orr         r2, r2, r4
    str         r2, [r1, #(0xc2 << 1)]

BootRom_DdrDrivingControl_Exit:
    mov         lr, r10
    bx          lr

//-------------------------------------------------------------------------------------------------
// BootRom_DdrAtopTrimmingDataProcess
// @param  r2: DDR ATOP value, r4: efuse value
// @return None
// @note r3,r4,r5 used
//-------------------------------------------------------------------------------------------------
    .global     BootRom_DdrAtopTrimmingDataProcess
BootRom_DdrAtopTrimmingDataProcess:

    //Check Driving Strength Group ([+4], [0, -4]
    and         r5, r4, #(0x01 << 2)
    cmp         r5, #0
    beq         BootRom_DdrAtopTrimmingDataProcess_1_1

BootRom_DdrAtopTrimmingDataProcess_Round_1_0:

    //Driving Strength Group [+4]
    and         r5, r4, #0x03

    //If Driving Strength was 2, That is a Fail Case
    cmp         r5, #0x2
    beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    //If Driving Strength was 3, That is a Fail Case
    cmp         r5, #0x3
    beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    //Add Driving Strength in eFuse
    add         r2, r2, #0x04

    cmp         r2, #0x3f
    ble         BootRom_InitDdrAtopTrimmingDataProcessExit

    ldr         r2, =0x3f
    b           BootRom_InitDdrAtopTrimmingDataProcessExit

BootRom_DdrAtopTrimmingDataProcess_1_1:

    //Driving Strength Group [0, -4]
    and         r5, r4, #0x03

    //If Driving Strength was 2, That is a Fail Case
    cmp         r5, #0x2
    beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    //If Driving Strength was 3, That is a Fail Case
    cmp         r5, #0x3
    beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    cmp         r5, #0x0
    beq         BootRom_InitDdrAtopTrimmingDataProcessExit

    //lsl       r5, #1
    cmp         r2, r5
    blt         BootRom_DdrAtopTrimmingDataProcess_1_2

    //Subtract Driving Strength in eFuse
    sub         r2, r2, #0x04
    b           BootRom_InitDdrAtopTrimmingDataProcessExit

BootRom_DdrAtopTrimmingDataProcess_1_2:

    //If The Default Value is less than The Value of Driving Strength, Force to 0
    ldr         r2, =0x00

BootRom_InitDdrAtopTrimmingDataProcessExit:

    bx          lr

#endif

//-------------------------------------------------------------------------------------------------
// Miu_DramSizeCheck
// @param
// @return None
// @note
//-------------------------------------------------------------------------------------------------
#ifdef ADAPTIVE
Miu_DramSizeCheck:

    //Disable Boundry Check
    ldr         r2, =0x0
    str         r2, [r0, #(0xDE << 1)]

    ldr         r2, =0xA0000000
    ldr         r3, =0x0
    str         r3, [r2]

Miu_DramSize128M:
    ldr         r4, =0xA8000000
    ldr         r3, =0x7
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize256M:
    ldr         r4, =0xB0000000
    ldr         r3, =0x8
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize512M:
    ldr         r4, =0xC0000000
    ldr         r3, =0x9
    str         r3, [r4]
    ldr         r5, [r2]
    cmp         r3, r5
    beq         Miu_DramSizeSet

Miu_DramSize1024M:
    ldr         r3, =0xA

Miu_DramSizeSet:

    ldr         r2, [r1, #(0xd2 << 1)]
    bic         r2, #0xf000
    orr         r2, r3, lsl #12
    str         r2, [r1, #(0xd2 << 1)]

    //Enable Boundry Check
    ldr         r2, =0x3
    str         r2, [r0, #(0xDE << 1)]

    bx          lr
#endif
