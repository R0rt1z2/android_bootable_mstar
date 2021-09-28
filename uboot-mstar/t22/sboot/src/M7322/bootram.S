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

#if defined(CONFIG_SBOOT_TO_KERNEL) && defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14010000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20208000
#define LINUX_KERNEL_ENTRY          0x20208000
#define LINUX_KERNEL_SIZE           0x00800000
#elif defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14020000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20400000
#define LINUX_KERNEL_ENTRY          0x20408000
#define LINUX_KERNEL_SIZE           0x00200000
#elif defined(CONFIG_SBOOT_TO_KERNEL)
#define LINUX_KERNEL_FLASH_ADDRESS  0x14010000
#define LINUX_KERNEL_DRAM_ADDRESS   0x20208000
#define LINUX_KERNEL_ENTRY          0x20208000
#define LINUX_KERNEL_SIZE           0x00800000
#endif

#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .sect ".ldr.teepostvector.text"
    .global vector
#endif
//-------------------------------------------------------------------------------------------------
// vector:
//-------------------------------------------------------------------------------------------------
vector:
    //Synchronous EL1t
    .align  7
    .word 0x14000000
    //IRQ EL3t
    .align  7
    .word 0x14000000
    //FIQ EL3t
    .align  7
    .word 0x14000000
    //Error EL3t
    .align  7
    .word 0x14000000

    //Synchronous EL3h
    .align  7
    .word 0x14000000
    //IRQ EL3h
    .align  7
    .word 0x14000000
    //FIQ EL3h
    .align  7
    .word 0x14000000
    //Error EL3h
    .align  7
    .word 0x14000000

    //Synchronous 64-bit EL2
    .align  7
    .word 0x14000000
    //IRQ 64-bit EL2
    .align  7
    .word 0x14000000
    //FIQ 64-bit EL2
    .align  7
    .word 0x14000000
    //Error 64-bit EL2
    .align  7
    .word 0x14000000

    //Synchronous 32-bit EL2
    .align  7
    //switch to 64 bit mode
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
.word 0xd28ce094 //mov x20, #0x6704
    .word 0xf2a3e414 //movk    x20, #0x1f20, lsl #16
    .word 0xf2c00014 //movk    x20, #0x0, lsl #32
    .word 0xf2e00014 //movk    x20, #0x0, lsl #48
    .word 0xb9400295 //ldr     w21, [x20]
    .word 0x529ddff4 //mov     w20, #0xeeff                    // #61183
    .word 0x6b15029f //cmp     w20, w21
	.word 0x54000160 //b.eq    48 <sboot_str_jump_64bit_kernel>
    .word 0xd29ff985 	//mov	x5, #0xffcc                	// #65484
    .word 0xeb05001f 	//cmp	x0, x5
    .word 0x54000121 	//b.ne	2c <FirmwarePath2>
    .word 0xd51e4021 	//msr	elr_el3, x1
    .word 0xd53e4001 	//mrs	x1, spsr_el3
    .word 0x9278dc21 	//and	x1, x1, #0xffffffffffffff00
    .word 0xb27c0021 	//orr	x1, x1, #0x10
    .word 0xb2400421 	//orr	x1, x1, #0x3
    .word 0xd2808821 	//mov	x1, #0x441                 	// #1089
    .word 0xd51e1101 	//msr	scr_el3, x1
    .word 0xd69f03e0 	//eret
    .word 0xd51e4021 //msr  elr_el3, x1
    .word 0xd53e4001 //mrs  x1, spsr_el3
    .word 0x9278dc21 //and  x1, x1, #0xffffffffffffff00
    .word 0xb27d0021 //orr  x1, x1, #0x8
    .word 0xb27e0021 //orr  x1, x1, #0x4
    .word 0xb2400021 //orr  x1, x1, #0x1
    .word 0xd51e4001 //msr  spsr_el3, x1
    .word 0xd2808801 //mov  x1, #0x440    // #1088
    .word 0xd51e1101 //msr  scr_el3, x1
    .word 0xd69f03e0 //eret
#endif
    //<sboot_str_jump_64bit_kernel>:
    .word 0xd2808820 //mov x0, #0x441
    .word 0xd51e1100 //msr scr_el3, x0
    .word 0xd2a4051e //mov x30, #0x20280000
    .word 0xd51e403e //msr elr_el3, x30
    .word 0xd53e4001 //mrs x1, spsr_el3
    .word 0x927be821 //and x1, x1, #0xffffffffffffffe0
    .word 0xb2400021 //orr x1, x1, #0x1
    .word 0xb27d0021 //orr x1, x1, #0x8
    .word 0xd51e4001 //msr spsr_el3, x1
    .word 0xd69f03e0 //eret
    //IRQ 32-bit EL2
    .align  7
    .word 0x14000000
    //FIQ 32-bit EL2
    .align  7
    .word 0x14000000
    //Error 32-bit EL2
    .align  7
    .word 0x14000000
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .text
#endif

//-------------------------------------------------------------------------------------------------
// BOOTRAM_Entry:
// @param  None
// @return None
// @note   Entry point of Boot code
//-------------------------------------------------------------------------------------------------
    .globl      BOOTRAM_Entry
BOOTRAM_Entry:

#if !defined(CONFIG_DOUBLE_MBOOT)
    DBGREG      0x0030
#endif


#if !defined(CONFIG_OPTEE_TEELOADER)
    ldr         r0, =vector                 //Starting Address of Linux Kernel in SPI Flash
    ldr         r1, =0x20000000             //Dest Address of Linux Kernel in DRAM
    ldr         r2, =0x80000                //Image Size of Linux Kernel

_CopyImageLoop1:

    ldr         r3, [r0], #0x04
    str         r3, [r1], #0x04
    subs        r2, r2, #0x04
    ldr         r3, =0x0007FFFF
    and         r4, r2, r3
    cmp         r4, #0
    bgt         _CopyImageLoop1
    PUTK        '#'
    cmp         r2, #0
    bgt         _CopyImageLoop1

    PUTK       '_'
#endif

#if 0
#if defined(CONFIG_ARMv8_32BIT_KERNEL) && !defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    //[PATCH] enter monitor mode to enable SMP bit and return to _ExceptionReturn
    ldr         r3, =_ExceptionReturn
    .arch_extension sec
    smc         #0
_ExceptionReturn:
#endif
#endif

#if 0 // move to bootrom @20181227 @M7322
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

#ifdef CONFIG_MSTAR_EMMC_LDO_POWER_OFF
    bl          Bootram_PowerSaving
#endif

#ifdef CONFIG_MSTAR_NS_UBOOT
   ldr         r0, =0x20080000
   ldr         r1, =_gMultiCoreTable
   str         r1,[r0]
   ldr         r1, =0x0
   str         r1,[r0,#4]
#endif

#if !defined(CONFIG_OPTEE_TEELOADER)
//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) || \
    defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)

    ldr         r0, =Bootram_SecondaryCore
    ldr         r1, =0xFFFF
    mov         r2, r0, lsr #16
    and         r2, r2, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH

    and         r2, r0, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW

#endif
#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0x00000000
#elif defined(CONFIG_ARMv8_64BIT_KERNEL)
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0xffffffff
#endif

    //Initial Global Interrupt Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    //Initial Internal Interrupt Controller
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000003
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

    ldr         r1, =REG_GIC_DIST_BASE_ADDRESS + 0x80       // GICD_IGROUPR
#if defined(CONFIG_ARMv8_64BIT_KERNEL)
    ldr         r0, =~0                                     // Grp1 interrupts
    ldr         r2, =~(3 << 30)                             // mboot will be secure_group, so bit_30 need to be 0
#elif defined(CONFIG_ARMv8_32BIT_KERNEL)
    ldr         r0, =0                                      // Grp1 interrupts
    ldr         r2, =0                                      // mboot will be secure_group, so bit_30 need to be 0
#endif

    and         r0, r0, r2
    str         r0, [r1]
    str         r0, [r1],#4

#ifdef CONFIG_ARM_GIC_SPI_MODE

    ldr         r2, =0xa
#if defined(CONFIG_ARMv8_64BIT_KERNEL)
    ldr         r0, =~0
#endif
_GIC_LOOP:
    str         r0, [r1],#0x4
    sub         r2,r2,#0x1
    cmp         r2,#0
    bne         _GIC_LOOP

#endif

    //Poke Core1/2/3 using software interrupt
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0F00), 0x000e8001
#endif

    // Clear data in BSS section
    ldr         r0, =_ld_bootbss_start
    ldr         r1, =_ld_bootbss_end
    mov         r2, #0

    cmp         r0, r1
    beq         _ClearBssExit

_ClearBssLoop:
    cmp         r0, r1
    str         r2, [r0], #0x04
    bne         _ClearBssLoop

_ClearBssExit:

//================================================================================================
// Initialize system stack pointer for unzip subroutine
#ifdef CONFIG_MSTAR_STR_MINISIZE
    ldr         r13, =CONFIG_KERNEL_START_ADDRESS
#else
    ldr         r13, =ADDR_STACK_POINTER
#endif

#if (defined(CONFIG_MSTAR_TITANIA_BD_FPGA) || defined(CONFIG_SBOOT_TO_KERNEL)) && (!defined(CONFIG_MSTAR_HAPS))
//================================================================================================
//Copy Image to DRAM

    ldr         r0, =LINUX_KERNEL_FLASH_ADDRESS     //Starting Address of Linux Kernel in SPI Flash
    ldr         r1, =LINUX_KERNEL_DRAM_ADDRESS      //Dest Address of Linux Kernel in DRAM
    ldr         r2, =LINUX_KERNEL_SIZE              //Image Size of Linux Kernel

_CopyImageLoop:
    LDMIA       r0!, {r3 - r10}
    STMIA       r1!, {r3 - r10}
    SUBS        r2, r2, #32
    bge         _CopyImageLoop

#if 0
_CopyImageLoop:

    ldr         r3, [r0], #0x04
    str         r3, [r1], #0x04

    subs        r2, r2, #0x04

    ldr         r3, =0x0007FFFF
    and         r4, r2, r3
    cmp         r4, #0
    bgt         _CopyImageLoop

    PUTK        '#'

    cmp         r2, #0
    bgt         _CopyImageLoop
#endif

//================================================================================================
//Jump to Kernel Entry of Linux

    PUTK        '\n'
    PUTK        '\r'

    ldr         pc, =LINUX_KERNEL_ENTRY

#endif

//================================================================================================
//launch TEE and jump to resume addr in resume mode
#ifdef CONFIG_MSTAR_FAST_STR
    ldr         r0, =resume
    ldr         r0, [r0]
    cmp         r0, #0x0
    beq         BootRam_Str_AcFlow
    //jump to TEE_STR flow
    bl          _fast_str_miu_reg_resume
    bl          _fast_str_console_init
    bl          TEE_SetUp_MagicID
    @bl          print_str_status
    bl          FastBootTEE
    bl          print_str_status
    //bl          _CRCCheck
    PUTK        'F'
    PUTK        'S'
    PUTK        'T'
    PUTK        'R'
    PUTK        '\n'
    PUTK        '\r'
    //32bit kernel jump to resume addr

    ldr         r0, =0x1F001D48
    ldr         r1, [r0]
    bic         r1, r1, #0x01
    mov         r1, r1, lsl #16
    ldr         r2, [r1,#0x4]
    mov         pc, r2

BootRam_Str_AcFlow:
    nop
    PUTK        'D'
    PUTK        '\n'
    PUTK        '\r'
    @bl          print_str_status
#endif

//================================================================================================
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    //LOAD CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    bl          drvNAND_LoaduBoot

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE) && defined(CONFIG_SECURITY_BOOT)

    //Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail

BootRam_AuthChunkHeader_Exit:
#endif

    // LOAD UBOOT
#ifdef CONFIG_MSTAR_NS_UBOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r3, [r0, #0]    @// UBOOT_ROM_START in MBOOT PARTITION
    ldr         r1, [r0, #0x4C] @// CUSTOMER_KEY_BANK_ROM_OFFSET
    ldr         r8, [r0, #0x4C] @// CUSTOMER_KEY_BANK_ROM_OFFSET
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    ldr         r7, [r0, #68]    @//SECURITY_INFO_LOADER_SIZE
    add         r1, r1, r3      @// LOAD SIZE
    add         r1, r1, r7      @// add SECURITY INFO Size
    add         r1, r1, #0x3000 @// add two customer keybank and one TEE keybank
    sub         r0, r0, r3
    add         r9, r8, r0       @// keybank DRAM address
    bl          drvNAND_LoaduBoot
#else
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r3, [r0, #0]    // UBOOT_ROM_START in MBOOT PARTITION
    ldr         r1, [r0, #4]    // UBOOT_RAM_START
    ldr         r2, [r0, #8]    // UBOOT_RAM_END
    ldr         r5, [r0, #4]    // UBOOT_RAM_START
    ldr         r6, [r0, #8]    // UBOOT_RAM_END
    sub         r1, r2, r1      // UBOOT_SIZE = UBOOT_RAM_END - UBOOT_RAM_START
    add         r1, r1, r3      // LOAD SIZE
    add         r1, r1, #0x110  // SECURITY INFO Size 0x110
    sub         r0, r0, r3
    bl          drvNAND_LoaduBoot
#endif

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE) && defined(CONFIG_SECURITY_BOOT)

    //Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail

BootRam_AuthUBoot_Exit:
#endif
#ifdef CONFIG_MSTAR_NS_UBOOT
    bl          ns_uboot_init

#ifdef CONFIG_OPTEE_ANTIBRICK
    @// ns_uboot_check_optee_consistency
    bl          ns_uboot_check_optee_consistency
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif

    mov         r0, r9    @// CUSTOMER_KEY_BANK DRAM address
    bl          ns_uboot_read_keybank
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

#if defined (CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    @// load bl31.bin
    bl          ns_uboot_load_armfw
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif

    @// load optee.bin
    bl          ns_uboot_load_optee
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

    bl          ns_uboot_support_faststr
    cmp         r0, #0
    bne         BOOT_POINT
LOAD_OPTEE_FINISH:
#endif

#elif defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#ifdef CONFIG_MSTAR_NS_UBOOT
    bl          UartHandShake
    cmp         r0, #1
    beq         CUSTOMER_UBOOT_EXIT

    PUTK        '\r'
    PUTK        'K'
    PUTK        'O'
    PUTK        '\r'

    bl          UartWaitRXSTop
    PUTK        '\r'
    PUTK        'S'
    PUTK        '\r'

    bl          UartLoadContent
    cmp         r0, #1
    beq         BootRam_AuthenticationFail
    PUTK        '\r'
    DPUTK       'A'
    DPUTK       'U'
    DPUTK       'T'
    DPUTK       'H'
    DPUTK       '_'
    DPUTK       'O'
    DPUTK       'K'
    PUTK        '\r'

    ldr         r0, =0xFFCC
    ldr         r1, =CONFIG_UBOOT_LOADADDR
    ldr         r3, =0x16001080
    ldr         r2, [r3]
    ldr         r4, =(0x3 << 30)
    orr         r2, r2, r4
    str         r2, [r3]
    ldr         r3, =0x16001000
    mov         r2, #3
    str         r2, [r3]
    ldr         r3, =0x16002000
    mov         r2, #3
    str         r2, [r3]
    ldr         r3, =0x16002004
    mov         r2, #0xf8
    str         r2, [r3]
    .arch_extension sec
    smc         #0

CUSTOMER_UBOOT_EXIT:
    bl BootRam_LoadUBoot
#endif
    //LOAD CHUNK HEADER
#ifdef CONFIG_DOUBLE_MBOOT
    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))        //check mboot.bin in MBOOT or MBOOTBAK partition
    ldr         r1, =0x0100
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         emmc_ReadChunkHead_MBOT2                       //reg[0x103381]#0=1b, MBOOT partition authen fail, try MBOOTBAK partition
#endif

emmc_ReadChunkHead_MBOT:
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
#if defined(CONFIG_MTK_LOADER)
    RREG        r3, (MS_BUS_RIU_ADDR + (0x001322 << 1)) // read mtk loader flag
    ldr         r4, =0x0001
    and         r3, r4, r3
    cmp         r3, #0
    beq         MTK_CHUNKB
    ldr         r2, =0x1000
    b           MTK_CHUNKA_EXIT
MTK_CHUNKB:
    ldr         r2, =0x5000                                    // MBOOT+MPOOL size is 8 MB
MTK_CHUNKA_EXIT:
#else
    ldr         r2, =0x1000
#endif
    bl          eMMC_ReadData_MIU

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
    //Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
#if defined(CONFIG_DOUBLE_MBOOT)
    beq         BootRam_LoadUBoot

    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x01    //reg[0x103381]#0=1b, Set ChunkHead of MBOOT Authen fail flag
    ldr         pc, =BOOTRAM_Entry_Hash                          //MBOOT authen fail, jump to BOOTRAM_Entry_Hash to load MBOOTBAK

emmc_ReadChunkHead_MBOT2:
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x8000
    bl          eMMC_ReadData_MIU

    //Authenticate CHUNK HEADER
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x2F0
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x300
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail
#elif defined(CONFIG_SECURITY_BOOT) || defined(CONFIG_TEE_LOADER)
    bne         BootRam_AuthenticationFail
#endif
BootRam_AuthChunkHeader_Exit:
#else
    bl          BootRam_LoadUBoot
#endif

    // LOAD UBOOT
BootRam_LoadUBoot:

#ifdef CONFIG_MSTAR_NS_UBOOT
    bl          ns_uboot_init

#ifdef CONFIG_OPTEE_ANTIBRICK
    @// ns_uboot_check_optee_consistency
    bl          ns_uboot_check_optee_consistency
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif

    mov         r0, #0  // CUSTOMER_KEY_BANK_ROM_OFFSET, not used
    bl          ns_uboot_read_keybank
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

#if defined (CONFIG_ARMv8_64BIT_KERNEL) || defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    @// load bl31.bin
    bl          ns_uboot_load_armfw
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH
#endif

    @// load optee.bin
    bl          ns_uboot_load_optee
    cmp         r0, #0
    beq         LOAD_OPTEE_FINISH

    bl          ns_uboot_support_faststr
    cmp         r0, #0
    bne         BOOT_POINT
LOAD_OPTEE_FINISH:
#endif


#ifdef CONFIG_DOUBLE_MBOOT
    RREG        r0, (MS_BUS_RIU_ADDR + (0x103380 << 1))        //check mboot.bin in MBOOT or MBOOTBAK partition
    ldr         r1, =0x0100
    and         r0, r0, #0xff00
    cmp         r0, r1
    beq         emmc_ReadUBOOT_MBOT2
#endif

emmc_ReadUBOOT_MBOT:
#ifdef CONFIG_MSTAR_NS_UBOOT
    // read chunk header again because optee/armfw also use uboot loadaddr
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x1000
    bl          eMMC_ReadData_MIU
#endif

    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    @// UBOOT_RAM_START
    ldr         r6, [r0, #8]    @// UBOOT_RAM_END
    sub         r1, r6 , r5
    bl          ns_uboot_set_uboot_version

#if defined(CONFIG_MTK_LOADER)
    rreg        r0, (MS_BUS_RIU_ADDR + (0x0e70 << 1))   //disable divisor latch access
    bic         r0, r0, #0x00ff
    bic         r0, r0, #0x0f00
    ldr         r1, =0xF000                             //if eqal 0xF000, AC ON
    cmp         r0, r1
    beq         LOAD_MTK_LOADER_AC

LOAD_MTK_LOADER_STR:
    ldr         r0, =CONFIG_UBOOT_LOADADDR              //chunk header start
    ldr         r5, [r0, #4]                            //UBOOT_RAM_START
    ldr         r6, [r0, #8]                            //UBOOT_RAM_END
    sub         r1, r6 , r5
    add         r1, r1 , #0x110                         //SECURITY INFO Size 0x110
    ldr         r2, [r0, #148]                          //MTK_ROM_END
    mov         r2, r2, lsr #9                          //divided 512byte to caculate mmc block number
    add         r2, #0x1000                             //0x1000 is mmc normal partition based
    bl          eMMC_ReadData_MIU
    b           LOAD_MTK_LOADER_EXIT

LOAD_MTK_LOADER_AC:
    RREG        r0, (MS_BUS_RIU_ADDR + (0x001322 << 1)) //read dummy register
    ldr         r1, =0x0001
    and         r0, r0, #0x0001
    cmp         r0, r1
    bne         Load_MTK_LoaderB

    WREG_B      (MS_BUS_RIU_ADDR + (0x001322 << 1)), 0xFE // clear flag and set bit 1
    ldr         r0, =CONFIG_MTK_LOADER_START_ADDRESS
    ldr         r5, =CONFIG_UBOOT_LOADADDR
    ldr         r6, [r5, #144]    @// MTK_ROM_START
    ldr         r7, [r5, #148]    @// MTK_ROM_END
    sub         r1, r7 , r6
    ldr         r2, =0x1012                              // CONFIG_TEE_LOADER
    bl          eMMC_ReadData_MIU
    b           BootRam_AuthUBoot_Exit                   // bypass MTK loader authentication

Load_MTK_LoaderB:
    ldr         r0, =CONFIG_MTK_LOADER_START_ADDRESS
    ldr         r5, =CONFIG_UBOOT_LOADADDR
    ldr         r6, [r5, #144]    // MTK_ROM_START
    ldr         r7, [r5, #148]    // MTK_ROM_END
    sub         r1, r7 , r6
    ldr         r2, =0x5012                             // Loader B partition from 8MB and CONFIG_TEE_LOADER
    bl          eMMC_ReadData_MIU
    b           BootRam_AuthUBoot_Exit // bypass MTK loader authentication

LOAD_MTK_LOADER_EXIT:

#else
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    // UBOOT_RAM_START
    ldr         r6, [r0, #8]    // UBOOT_RAM_END
    sub         r1, r6 , r5
    add         r1, r1 , #0x110  // SECURITY INFO Size 0x110
#ifdef CONFIG_DOUBLE_MBOOT
//  ldr         r2, =0x1023
    ldr         r2, [r0]              // UBOOT_ROM_START in MBOOT PARTITION
    add         r2, r2 , #0x200000    // UBOOT addr in EMMC
    mov         r2, r2, lsr #9
#else
#if !defined(CONFIG_TEE_LOADER)
    ldr         r2, =0x1002
#else
    ldr         r2, =0x1012
#endif
#endif
    bl          eMMC_ReadData_MIU
#endif 

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
    //Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
#if defined(CONFIG_DOUBLE_MBOOT)
    beq         ANTIROLLBACK_POINT
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x01    //Set ChunkHead of MBOOT Authenticate fail flag
    ldr         pc, =BOOTRAM_Entry_Hash                          //MBOOT authen fail, jump to BOOTRAM_Entry_Hash to load MBOOTBAK

emmc_ReadUBOOT_MBOT2:

#ifdef CONFIG_MSTAR_NS_UBOOT
    // read chunk header again because optee/armfw also use uboot loadaddr
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r1, =0x400
    ldr         r2, =0x8000
    bl          eMMC_ReadData_MIU
#endif

    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r5, [r0, #4]    // UBOOT_RAM_START
    ldr         r6, [r0, #8]    // UBOOT_RAM_END
    sub         r1, r6 , r5
    add         r1, r1 , #0x110  // SECURITY INFO Size 0x110
//  ldr         r2, =0x8023
    ldr         r2, [r0]              // UBOOT_ROM_START in MBOOT PARTITION get from chunk_header
    add         r2, r2, #0x1000000    // UBOOTBAK addr in EMMC
    mov         r2, r2, lsr #9
    bl          eMMC_ReadData_MIU

    //Authenticate U-BOOT
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    sub         r1, r6, r5
    ldr         r2, =CONFIG_UBOOT_LOADADDR + 0x10
    add         r2, r2, r1
    ldr         r3, =0x0
    bl          MDrv_AESDMA_SecureMain

    //If authentication fail, jump to reset whole system
    cmp         r0, #1
    bne         BootRam_AuthenticationFail
#elif defined(CONFIG_SECURITY_BOOT) || defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
    bne         BootRam_AuthenticationFail
#endif
BootRam_AuthUBoot_Exit:
#else
    bl          ANTIROLLBACK_POINT
#endif


#else
#ifndef CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN          //Programming NAND for ROM Boot
    bl          DoMSDecompression
#else
    ldr         r0, =CONFIG_UBOOT_LOADADDR
    ldr         r11, = (0x20000000 + 0x20000 + 0x400)   // UBOOT_START ADDRESS in DRAM
    ldr         r1, =0x200000

_CopyUbootImgLoop:
    ldr         r3, [r11], #4
    str         r3, [r0],  #4
    subs        r1, r1, #4
    bne         _CopyUbootImgLoop
    ldr         pc, =CONFIG_UBOOT_LOADADDR
#endif
#endif

//================================================================================================
ANTIROLLBACK_POINT:
    bl          ns_uboot_anti_rollback

BOOT_POINT:
#ifdef CONFIG_MSTAR_NS_UBOOT
#if defined (CONFIG_ARMv8_64BIT_KERNEL)
    //WREG        0x1f203C08, 0x3000
    //WREG        0x1f2473F8, 0x0
    //WREG        0x1f203C4C, 0x300
    //WREG        0x1f003DC0, 0x1
    //WREG        0x1f005D88, 0x1
    ldr         r0, =NS_UBOOT_EXIT
    bl          ns_uboot_boot
#else
#if defined(CONFIG_ARMv7_OPTEE) // ARMv7 optee flow with 32bit kernel
    bl          ns_uboot_get_optee_addr
    and         r4, r0, r0
    ldr         r0, #0
    ldr         r1, #0
    ldr         r2, #0
    ldr         r3, #0
    bl          r4
#elif defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) //SMP ARMv8 optee flow with 32bit kernel
    ldr         r0, =NS_UBOOT_EXIT
    bl          ns_uboot_boot
#endif
#endif
NS_UBOOT_EXIT:
#endif

#if defined(CONFIG_OPTEE_ANTIBRICK)
    WREG        (MS_BUS_RIU_ADDR + (0x003008 << 1)), 0x0000          // Disable watchdog lower reg
    WREG        (MS_BUS_RIU_ADDR + (0x00300A << 1)), 0x0000          // Disable watchdog higher reg
    WREG_B      (MS_BUS_RIU_ADDR + (0x001220 << 1) + 0x01), 0x02     // OPTEECHECKOUT
#endif

    PUTK       'U'

    WREG        0x1f006040, 0x0000          //Disable Hardware Timer 1

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH) || \
    defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#if defined(CONFIG_MTK_LOADER)
    RREG        r0, (MS_BUS_RIU_ADDR + (0x0E70 << 1))   @//Disable Divisor Latch Access
    bic         r0, r0, #0x00FF
    bic         r0, r0, #0x0F00
    ldr         r1, =0xF000
    cmp         r0, r1
    beq         MTK_LOADER_AC
    ldr         pc, =CONFIG_UBOOT_LOADADDR              //MTK_LOADER_STR and jump uboot

MTK_LOADER_AC:
    RREG        r0, (MS_BUS_RIU_ADDR + (0x001322 << 1)) // read mtk loader flag
    ldr         r1, =0x0001
    and         r0, r1, r0
    cmp         r0, #0
    beq         MTK_LoaderB
MTK_LoaderB:
    ldr         pc, =CONFIG_MTK_LOADER_START_ADDRESS+0x400
#else
    ldr         pc, =CONFIG_UBOOT_LOADADDR
#endif
#else

#ifndef CONFIG_BOOTING_FROM_EXT_SPI_WITH_PM51
#ifdef CONFIG_MSTAR_PM_SWIR
    ldr         r0, =0x14030000
#else
    ldr         r0, =0x14020000
#endif
#else
    ldr         r0, =0x14030000
#endif

    ldr         pc, [r0, #16]
#endif

#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
.sect ".ldr.teeloader.text"
.globl      bootramTL_init
bootramTL_init:
#if defined(CONFIG_OPTEE_TEELOADER)
    ldr         r0, =_ld_TEE_MCT_Address
    ldr         r1, =_gMultiCoreTable
    str         r1,[r0]
    ldr         r1, =0x0
    str         r1,[r0,#4]
#endif
    ldr         r0, =Bootram_SecondaryCore
    ldr         r1, =0xFFFF
    mov         r2, r0, lsr #16
    and         r2, r2, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH

    and         r2, r0, r1
    REG_C2M     r2, BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW

#if defined(CONFIG_ARMv8_32BIT_KERNEL)
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0x00000000
#elif defined(CONFIG_ARMv8_64BIT_KERNEL)
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0080), 0xffffffff
#endif

    //Initial Global Interrupt Interface
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0004), 0x000000f0
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0180), 0xffff0000
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0100), 0x0000ffff

    //Initial Internal Interrupt Controller
    WREG        (REG_GIC_CPU_BASE_ADDRESS + 0x0000), 0x00000003
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0000), 0x00000003

    ldr         r1, =REG_GIC_DIST_BASE_ADDRESS + 0x80       // GICD_IGROUPR
#if defined(CONFIG_ARMv8_64BIT_KERNEL)
    ldr         r0, =~0                                     // Grp1 interrupts
    ldr         r2, =~(2 << 30)                             // mboot will be secure_group, so bit_30 need to be 1
#elif defined(CONFIG_ARMv8_32BIT_KERNEL)
    ldr         r0, =0                                      // Grp1 interrupts
    ldr         r2, =0                                      // mboot will be secure_group, so bit_30 need to be 0
#endif

    and         r0, r0, r2
    str         r0, [r1]
    str         r0, [r1],#4
    
#ifdef CONFIG_ARM_GIC_SPI_MODE
    ldr         r2, =0xa
#if defined(CONFIG_ARMv8_64BIT_KERNEL)
    ldr         r0, =~0
#endif
_GIC_LOOP_TEE:
    str         r0, [r1],#0x4
    sub         r2,r2,#0x1
    cmp         r2,#0
    bne         _GIC_LOOP_TEE
#endif
    //Poke Core1/2/3 using software interrupt
    WREG        (REG_GIC_DIST_BASE_ADDRESS + 0x0F00), 0x000e8001
    bx          lr
    .text
#endif

#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .sect ".ldr.teepostloader.text"
#endif
    .align 4
    .globl      Bootram_SecondaryCore
Bootram_SecondaryCore:
#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
    .word 0xd53e1007 	//mrs	x7, sctlr_el3
    .word 0xd2820008 	//mov	x8, #0x1000                	// #4096
    .word 0xb27e0108 	//orr	x8, x8, #0x4
    .word 0xb2400108 	//orr	x8, x8, #0x1
    .word 0xb2750108 	//orr	x8, x8, #0x800
    .word 0x8a2800e7 	//bic	x7, x7, x8
    .word 0xb27b00e7 	//orr	x7, x7, #0x20
    .word 0xb27d00e7 	//orr	x7, x7, #0x8
    .word 0xd51e1007 	//msr	sctlr_el3, x7
    .word 0x58000464 	//ldr	x4, b0 <NormalPath+0x3c>
    .word 0xf9400083 	//ldr	x3, [x4]
    .word 0x91001064 	//add	x4, x3, #0x4
FirmwarePath:
    .word 0xd503205f 	//wfe
    .word 0xb9400081 	//ldr	w1, [x4]
    .word 0x58000402 	//ldr	x2, b8 <NormalPath+0x44>
    .word 0xd53800a0 	//mrs	x0, mpidr_el1
    .word 0x12001c07 	//and	w7, w0, #0xff
    .word 0x12181c00 	//and	w0, w0, #0xff00
    .word 0x0b4020e0 	//add	w0, w7, w0, lsr #8
    .word 0x2a000042 	//orr	w2, w2, w0
    .word 0x6b01005f 	//cmp	w2, w1
    .word 0x54000101 	//b.ne	74 <NormalPath>
    .word 0x52800000 	//mov	w0, #0x0                   	// #0
    .word 0xb9000080 	//str	w0, [x4]
    .word 0xb9400060 	//ldr	w0, [x3]
    .word 0xd65f0000 	//ret	x0
    .word 0xd503201f 	//nop
    .word 0xd503201f 	//nop
    .word 0x17fffff0 	//b	30 <FirmwarePath>

NormalPath:
    .word 0xb9400081 	//ldr	w1, [x4]
    .word 0x180001a2 	//ldr	w2, ac <NormalPath+0x38>
    .word 0x6b02003f 	//cmp	w1, w2
    .word 0x54fffd81 	//b.ne	30 <FirmwarePath>
    .word 0xd2800009 	//mov	x9, #0x0                   	// #0
    .word 0xb9400069 	//ldr	w9, [x3]
    .word 0xd5384240 	//mrs	x0, currentel
    .word 0xd51e4029 	//msr	elr_el3, x9
    .word 0xd51c101f 	//msr	sctlr_el2, xzr
    .word 0xd2807920  //mov	x0, #0x3c9                      // #969
    .word 0xd51e4000 	//msr	spsr_el3, x0
    .word 0xd2808020 	//mov	x0, #0x401                 	// #1025
    .word 0xd51e1100 	//msr	scr_el3, x0
    .word 0xd69f03e0 	//eret
    .word	0x0000babe
#if defined(CONFIG_OPTEE_TEELOADER)
    .word	0x20028bf0    //This value must be equal to _ld_TEE_MCT_Address
#else
    .word	0x20080000
#endif
    .word	0x00000000
    .word	0x0000fac0
    .word	0x00000000
#elif defined(CONFIG_ARMv8_32BIT_KERNEL)
    .word 0xd503205f    //wfe
    .word 0xd53e1007    //mrs x7, sctlr_el3
    .word 0xd2820008    //mov   x8, #0x1000                 // #4096
    .word 0xb27e0108    //orr   x8, x8, #0x4
    .word 0xb2400108    //orr   x8, x8, #0x1
    .word 0xb2750108    //orr   x8, x8, #0x800
    .word 0x8a2800e7    //bic   x7, x7, x8
    .word 0xb27b00e7    //orr   x7, x7, #0x20
    .word 0xb27d00e7    //orr   x7, x7, #0x8
    .word 0xd51e1007    //msr   sctlr_el3, x7
    .word 0xd2800000    //mov   x0, #0x0                    // #0
    .word 0xd51be000    //msr   cntfrq_el0, x0
    .word 0xd51e1320    //msr   mdcr_el3, x0
Init_CP15 :
    .word 0xd51ec01f    //msr   vbar_el3, xzr
    .word 0xd51cc01f    //msr   vbar_el2, xzr
    .word 0xd51c101f    //msr   sctlr_el2, xzr
    .word 0xd518101f    //msr   sctlr_el1, xzr
    .word 0xd51e115f    //msr   cptr_el3, xzr
    .word 0xd51c115f    //msr   cptr_el2, xzr
    .word 0xd518105f    //msr   cpacr_el1, xzr
    .word 0xd51c11ff    //msr   hacr_el2, xzr
    .word 0xd51c117f    //msr   hstr_el2, xzr
    .word 0xd51ce07f    //msr   cntvoff_el2, xzr
    .word 0xd53800a0    //mrs   x0, mpidr_el1
    .word 0xd51c00a0    //msr   vmpidr_el2, x0
    .word 0xd5380000    //mrs   x0, midr_el1
    .word 0xd51c0000    //msr   vpidr_el2, x0
    .word 0xd51c111f    //msr   hcr_el2, xzr
    .word 0xd51e403f    //msr   elr_el3, xzr
    .word 0xd51c101f    //msr   sctlr_el2, xzr
    .word 0xd51e111f    //msr   scr_el3, xzr

Init_ACTLR :
    .word 0xd53e1021    //mrs   x1, actlr_el3
    .word 0xb2400021    //orr   x1, x1, #0x1
    .word 0xb27f0021    //orr   x1, x1, #0x2
    .word 0xb27c0021    //orr   x1, x1, #0x10
    .word 0xb27b0021    //orr   x1, x1, #0x20
    .word 0xb27a0021    //orr   x1, x1, #0x40
    .word 0xd51e1021    //msr   actlr_el3, x1

    .word 0xd2a2c000    //mov   x0, #0x16000000             // #369098752
    .word 0xd2821001    //mov   x1, #0x1080                 // #4224
    .word 0xaa010000    //orr   x0, x0, x1
    .word 0xd2800001    //mov   x1, #0x0                    // #0
    .word 0xf9000001    //str   x1, [x0]

WaitMagic_NORMAL :
    .word 0xd2a40400    //mov   x0, #0x20200000             // #538968064
    .word 0xd2900001    //mov   x1, #0x8000                 // #32768
    .word 0xaa010000    //orr   x0, x0, x1
    .word 0xb9400001    //ldr   w1, [x0]
    .word 0xd29757c0    //mov   x0, #0xbabe                 // #47806
    .word 0xeb00003f    //cmp   x1, x0
    .word 0x54ffff41    //b.ne  94 <WaitMagic_NORMAL>
    .word 0xd2a40400    //mov   x0, #0x20200000             // #538968064
    .word 0xd2900081    //mov   x1, #0x8004                 // #32772
    .word 0xaa010000    //orr   x0, x0, x1
    .word 0xb9400007    //ldr   w7, [x0]
    .word 0xd51e4027    //msr   elr_el3, x7
    .word 0xd2803a61    //mov   x1, #0x1d3                  // #467
    .word 0xd51e4001    //msr   spsr_el3, x1
    .word 0xd69f03e0    //eret

#elif defined(CONFIG_ARMv8_64BIT_KERNEL)
    .word 0xd503205f  //wfe
    .word 0xd5380000  //mrs     x0, midr_el1
    .word 0x58000541  //ldr     x1, c8 <Bootram_SecondaryCore+0xd0>
    .word 0x8a010000  //and     x0, x0, x1
    .word 0xd344fc00  //lsr     x0, x0, #4
    .word 0xf134141f  //cmp     x0, #0xd05
    .word 0x540000a1  //b.ne    28  <Bootram_SecondaryCore+0x2c>
    .word 0xd503201f  //nop
    .word 0xd503201f  //nop
    .word 0xd503201f  //nop
    .word 0xd503201f  //nop
    //init cpu register
    .word 0xd53e1007  // mrs  x7, sctlr_el3
    .word 0xd2820008  // mov  x8, #0x1000
    .word 0xb27e0108  // orr  x8, x8, #0x4
    .word 0xb2400108  // orr  x8, x8, #0x1
    .word 0xb2750108  // orr  x8, x8, #0x800
    .word 0x8a2800e7  // bic  x7, x7, x8
    .word 0xb27b00e7  // orr  x7, x7, #0x20
    .word 0xb27d00e7  // orr  x7, x7, #0x8
    .word 0xd51e1007  // msr  sctlr_el3, x7
    .word 0xd2800000    //mov   x0, #0x0                    // #0
    .word 0xd51be000    //msr   cntfrq_el0, x0
    .word 0xd51e1320    //msr   mdcr_el3, x0
    .word 0x580002fa  //ldr x26, b8 <Bootram_SecondaryCore+0xC0>
    .word 0xb9400341  //ldr w1, [x26]
    .word 0x18000222  //ldr w2, ac <Bootram_SecondaryCore+0xb0>
    .word 0x6b02003f  //cmp w1, w2
    .word 0x54fffca1  //b.ne Bootram_SecondaryCore
    .word 0xd2800009  //mov x9, #0x0
    .word 0x5800027a  //ldr x26, c0 <Bootram_SecondaryCore+0xc8>
    .word 0xb9400347  //ldr w7, [x26]
    .word 0x5800027a  //ldr x26, c0 <Bootram_SecondaryCore+0xd0>
    .word 0xb9400348  //ldr w8, [x26]
    .word 0x0b074109  //add w9, w8, w7, lsl #16
    .word 0xd5384240  //mrs x0, currentel
    .word 0xd51e4029  //msr elr_el3, x9
    .word 0xd51c101f  //msr sctlr_el2, xzr
    .word 0xd2807920  //mov x0, #0x3c9
    .word 0xd51e4000  //msr spsr_el3, x0
    .word 0xd2808020  //mov x0, #0x401
    .word 0xd51e1100  //msr scr_el3, x0
    //.word 0x14000000  //eret
    .word 0xd69f03e0  //eret
    //.word 0x0000fff0

    .align 3
    //data
    .word 0x0000babe
    .word 0x00000000
    .word 0x0000fff0
    .word 0x00000000
    .word 0x1f200a28
    .word 0x00000000
    .word 0x1f200a24
    .word 0x00000000
    .word 0x1f200a20
    .word 0x00000000
#endif
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
    .text
#endif

#if defined(CONFIG_MSTAR_AUTHEN_ENABLE)
BootRam_AuthenticationFail:
    PUTK        '\n'
    PUTK        '\r'
    PUTK       'A'
    PUTK       'U'
    PUTK       'T'
    PUTK       'H'
    PUTK       '_'
    PUTK       'F'
    PUTK       'A'
    PUTK       'I'
    PUTK       'L'
#ifdef CONFIG_DOUBLE_MBOOT
    WREG_B      (MS_BUS_RIU_ADDR + (0x103380 << 1) + 1), 0x03    //Set MBOOT & MBOOTBAK partition both authen fail flag
#endif
#if defined(CONFIG_MTK_LOADER)
    WREG_B      (MS_BUS_RIU_ADDR + (0x001322 << 1)), 0xFE // clear boot flag and set bit0 to 0
#endif

BootRam_AuthenticationFailLoop:
    b           BootRam_AuthenticationFailLoop
#endif

//================================================================================================
Bootram_PowerSaving:

    //eMMC Internal LDO tune off
    ldr         r1, =(MS_BUS_RIU_ADDR + (0x123f00 << 1))
    ldr         r0, [r1, #(0x6c << 1)]
    orr         r0, #(0x01 << 5)
    str         r0, [r1, #(0x6c << 1)]

    bx          lr

    .globl  amzn_identifier
amzn_identifier:
    .ascii "YAMA"
#if defined(CONFIG_MSTAR_BD_M7322_MB170)|| defined(CONFIG_MSTAR_BD_M7322_MB175)
    // sboot version <LO, HI>
    .byte 0x18, 0x00
    .ascii "SOPH"
#elif defined(CONFIG_MSTAR_BD_M7322_G22)
    // sboot version <LO, HI>
    .byte 0x1C, 0x00
    .ascii "GRET"
#elif defined(CONFIG_MSTAR_BD_M7322_G22_abc123)
    // sboot version <LO, HI>
    .byte 0x0D, 0x00
    .ascii "CLAR"
#elif defined(CONFIG_MSTAR_BD_M7322_abc123)
    // sboot version <LO, HI>
    .byte 0x04, 0x00
    .ascii "WINS"
#endif
