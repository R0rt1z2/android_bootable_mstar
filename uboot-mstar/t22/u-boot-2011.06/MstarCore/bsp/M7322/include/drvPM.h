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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  PM Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_PM PM interface
   *  \ingroup  G_PERIPHERAL
   
     \brief
   
     Power Management
     
     <b>Features</b>
     
     - Normal mode:HK is alive
     - Standby mode:Power down die-domain
     - Sleep mode:Power down die-domain & turn off 8051 clock,external XTAL is on to provide RTC wake-up
     - Deep sleep mode:Power down die-domain & turn off 8051 clock & turn off external crystal,Use internal RC (4MHz) only
     
     <b>Power Domain Diagram</b> \n
     \image html drvPM_pic.png
     
     <b>PM Power Down Routine</b> \n
     -# Clock Switch
     -# Set Power Down Mode: Standby/Sleep
     -# Enable Isolation link to GPIO_PM[4]
     -# Enter Password: Lock/Lock3
     -# Pull GPIO PM4 down (low)
     
     <b>PM Power Up Routine</b> \n
     -# Record wakeup source
     -# Pull GPIO PM4 Up (high)
     -# Clear Password: Lock3
     -# Mask All Wakeup Sources
     -# Waiting for Power Good
     -# Clear Passwords: Lock
     -# Recovery Settings before Power Down
   
     \defgroup G_PM_INIT Initialization Task relative
     \ingroup  G_PM
     \defgroup G_PM_COMMON Common Task relative
     \ingroup  G_PM
     \defgroup G_PM_CONTROL Control relative
     \ingroup  G_PM
     *  \defgroup G_PM_RTC RTC Task relative
     *  \ingroup  G_PM
     \defgroup G_PM_PWM PWM Task relative
     \ingroup  G_PM
     * \defgroup G_PM_STR STR Task relative
     *  \ingroup  G_PM
     \defgroup G_PM_OTHER  other relative
     \ingroup  G_PM
     * \defgroup G_PM_ToBeModified PM api to be modified
     *  \ingroup  G_PM
     \defgroup G_PM_ToBeRemove PM api to be removed
     \ingroup  G_PM
*/

#ifndef _DRV_PM_H_
#define _DRV_PM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define BOOT_REASON_PM_ADDR_OFFSET              0x060a

#define PM_SPARE_COLD_BOOT_POWER_SUPPLY          0
#define PM_SPARE_WARM_BOOT_HW_WDOG               (1U << 1)
#define PM_SPARE_WARM_BOOT_KERNEL_WDOG           (1U << 2)
#define PM_SPARE_WARM_BOOT_SW                    (1U << 3)
#define PM_SPARE_WARM_BOOT_KERNEL_PANIC          (1U << 4)
#define PM_SPARE_BOOT_LONG_PWR_KEY_PRESS         (1U << 5)
#define PM_SPARE_THERMAL_BOOT_SOC                (1U << 6)
#define PM_SPARE_THERMAL_SHUTDOWN_SOC            (1U << 6)
#define PM_SPARE_SHUTDOWN_SW                     (1U << 7)
#define PM_SPARE_SPECIAL_MODE_SILENT_OTA        (1U << 8)
#define PM_SPARE_SPECIAL_MODE_SILENT_LED        (1U << 9)
#define PM_SPARE_SCREEN_STATE                    (1U << 10)
/* special mode */
#define PM_SPARE_SPECIAL_MODE_OTA                (1U << 11)
#define PM_SPARE_SPECIAL_MODE_FACTORY_RESET      (1U << 12)


/// define PM library version
#define MSIF_PM_LIB_CODE               {'P','M','_','_'}
#define MSIF_PM_LIBVER                 {'0','3'}
#define MSIF_PM_BUILDNUM               {'0','0'}
#define MSIF_PM_CHANGELIST             {'0','0','3','5','2','1','6','3'}
#define MSIF_PM_OS                           '0'                  //OS

#define PM_DRV_VERSION                 /* Character String for DRV/API version  */  \
    MSIF_TAG,                           /* 'MSIF' */  \
    MSIF_CLASS,                         /* '00' */  \
    MSIF_CUS,                           /* 0x0000 */  \
    MSIF_MOD,                           /* 0x0000 */  \
    MSIF_CHIP,                                       \
    MSIF_CPU,                                        \
    MSIF_PM_LIB_CODE,                  /* IP__ */  \
    MSIF_PM_LIBVER,                    /* 0.0 ~ Z.Z */  \
    MSIF_PM_BUILDNUM,                  /* 00 ~ 99 */  \
    MSIF_PM_CHANGELIST,                /* CL# */  \
    MSIF_PM_OS


#define PM_MAX_BUF_WAKE_IR                  (32) //Both IR and Keypad share this pool
#define PM_MAX_BUF_WAKE_IR2                 (16) //Only 2nd IR share this pool
#define PM_MAX_BUF_WAKE_KEYPAD              (32) //RESERVED
#define PM_MAX_BUF_WAKE_MAC_ADDRESS         (6)//For Mac address
#define PM_MAX_BUF_WAKE_IR_WAVEFORM_ADDR    (4)
#define PM_MAX_BUF_RESERVERD                (4)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// PM_CMD opcode
#define WRITE_CMD       0
#define READ_CMD        1
#define SLEEP_CMD       2
#define CTRL_WRITE_CMD  3
#define CTRL_READ_CMD   4

/// select sleep mode
#define SLEEP_MODE      0
#define DEEP_SLEEP_MODE 1

/// enable wakeup source
#define IR_WAKEUP       (1<<0)
#define CEC_WAKEUP      (1<<1)
#define GPIO5_WAKEUP    (1<<2)
#define GPIO6_WAKEUP    (1<<3)
#define KEYPAD_WAKEUP   (1<<4)
#define EASYSYNC_WAKEUP (1<<5)
#define SYNC_WAKEUP     (1<<6)
#define RTC_WAKEUP      (1<<7)

#define DVI_DET_WAKEUP  (1<<10)
#define DVI_DET2_WAKEUP (1<<11)

/// power off PM4 polarity
#define POWER_OFF_HIGH  (1<<15)

/// IR types
#define IR_TYPE_FULLDECODE_MODE 1
#define IR_TYPE_RAWDATA_MODE    2
#define IR_TYPE_SWDECODE_MODE   3

/// IRQ
#define MDRV_IRQ_BEGIN      0x40
#define MDRV_IRQ_END        0x7F
#define IRQ_PMSLEEP     (MDRV_IRQ_BEGIN + 12)

/// Enable GPIO
#define ENABLE_GPIO0    (1<<0)
#define ENABLE_GPIO1    (1<<1)
#define ENABLE_GPIO2    (1<<2)
#define ENABLE_GPIO3    (1<<3)  //GPIO 4&5 are reserved
#define ENABLE_GPIO6    (1<<6)
#define ENABLE_GPIO7    (1<<7)
#define ENABLE_GPIO8    (1<<8)
#define ENABLE_GPIO9    (1<<9)
#define ENABLE_GPIO10   (1<<10)
#define ENABLE_GPIO11   (1<<11)
#define ENABLE_GPIO12   (1<<12)


/// MICOM(PM_SLEEP) DDI Layer Command
typedef enum
{
    /// write data from host to PM : direction
    CP_WRITE_HOST_MEM_TO_MICOM = 0xE0UL,

    /// write data from host to PM : start
    CP_WRITE_HOST_MEM_TO_MICOM_START = 0xE1UL,

    /// write data from host to PM : data content
    CP_WRITE_HOST_MEM_TO_MICOM_DATA = 0xE2UL,

    /// write keypad boundary parameter 1 to PM
    CP_WRITE_KEYPAD_BOUND_1 = 0xE6UL,

    /// write keypad boundary parameter 2 to PM
    CP_WRITE_KEYPAD_BOUND_2 = 0xE7UL,

    /// read data from PM to host : direction
    CP_READ_MICOM_TO_HOST_MEM = 0xE3UL,

    /// read data from PM to host : start
    CP_WRITE_MICOM_TO_HOST_MEM_START = 0xE4UL,

    /// read data from PM to host : data content
    CP_READ_MICOM_TO_HOST_MEM_DATA = 0xE5UL,

    /// no command
    CP_NO_CMD = 0

} DDI_MICOM_CMDTYPE;

/// For PM mailbox command header
typedef struct
{
    MS_U8 Preamble  : 2;    /// two bits for command index
    MS_U8 Opcode    : 6;    /// 6 bits for operation codes
} PM_Cmd;

/// For initializing IR timing configuration
typedef struct
{
    MS_S16 s16Time;    /// time
    MS_S16 s16UpBnd;   /// upper bound
    MS_S16 s16LowBnd;  /// low bound
} PM_IrTimeCfg;

/// For initializing IR registers configuration
typedef struct
{
    MS_U8  u8IrModeSel;                 /// IR mode selection
    MS_U8  u8IrCtrl;                    /// IR enable control
    MS_U8  u8IrHdrCode0;                /// IR header code 0
    MS_U8  u8IrHdrCode1;                /// IR header code 1
    MS_U32 u32IrTimOutCyc;              /// IR timerout counter
    PM_IrTimeCfg tIrHdrCoTim;           /// header code time
    PM_IrTimeCfg tIrOffCoTim;           /// off code time
    PM_IrTimeCfg tIrOffCoRpTim;         /// off code repeat time
    PM_IrTimeCfg tIrLg01HighTim;        /// logical 0/1 high time
    PM_IrTimeCfg tIrLg0Tim;             /// logical 0 time
    PM_IrTimeCfg tIrLg1Tim;             /// logical 1 time
} PM_IrRegCfg;

/// For initializing keypad boundary configuration
typedef struct
{
    MS_U8 u8UpBnd;   /// upper bound
    MS_U8 u8LowBnd;  /// low bound
} PM_SarBndCfg;

/// For initializing keypad boundary configuration
typedef struct
{
    PM_SarBndCfg tSarChan1;          /// Channel 1 interrupt trigger boundary
    PM_SarBndCfg tSarChan2;          /// Channel 2 interrupt trigger boundary
    PM_SarBndCfg tSarChan3;          /// Channel 3 interrupt trigger boundary
    PM_SarBndCfg tSarChan4;          /// Channel 4 interrupt trigger boundary
} PM_SarRegCfg;

/// For initializing keypad wake-up parameter configuration
typedef struct
{
    PM_SarBndCfg tChanMinus;    /// parameter for Channel Minus
    PM_SarBndCfg tChanPlus;     /// parameter for Channel Plus
    PM_SarBndCfg tInput;        /// parameter for Input Source
    PM_SarBndCfg tMenu;         /// parameter for Menu
    PM_SarBndCfg tOk;           /// parameter for OK
    PM_SarBndCfg tPower;        /// parameter for Power
    PM_SarBndCfg tVolumeMinus;  /// parameter for Volume Minus
    PM_SarBndCfg tVolumePlus;   /// parameter for Volume Plus
} PM_SarParamCfg;

typedef enum
{
    E_PM_RTC_0=0,
    E_PM_RTC_2=1,
}E_PM_RTC;

///Define PM IRQ Type
typedef enum
{
    /// IRQ Type for IR
    E_PM_IRQ_IR         = 0,
    /// IRQ Type for CEC Wake-up
    E_PM_IRQ_CEC_WAKE   = 1,
    /// IRQ Type for External Interrup
    E_PM_IRQ_EX_INT     = 2,
    /// IRQ Type for External Interrup 2
    E_PM_IRQ_EX_INT2    = 3,
    /// IRQ Type for SAR(Keypad)
    E_PM_IRQ_SAR        = 4,
    /// IRQ Type for Sync Detection
    E_PM_IRQ_SYNC_DET   = 5,
    /// IRQ Type for DVI clock Detection
    E_PM_IRQ_DVICLK_DET = 6,
    /// IRQ Type for RTC
    E_PM_IRQ_RTC        = 7,
    /// IRQ Type for WDT
    E_PM_IRQ_WDT        = 8,
    /// IRQ Type for Mail Box 0
    E_PM_IRQ_MBOX0      = 9,
    /// IRQ Type for Mail Box 1
    E_PM_IRQ_MBOX1      = 10,
    /// IRQ Type for Mail Box 2
    E_PM_IRQ_MBOX2      = 11,
    /// IRQ Type for Mail Box 3
    E_PM_IRQ_MMOX3      = 12,
    /// Maximum IRQ Type
    E_PM_IRQ_MAX        = E_PM_IRQ_MMOX3,

} PM_IRQ_TYPE;

///Define PM Saved Registers
typedef struct
{
    unsigned long   r[32];          /// GPR regs
#ifdef __AEONR2__
    unsigned long   machi2;         // Highest 32-bits of new 32x32=64 multiplier
#endif
    unsigned long   machi;          /// High and low words of
    unsigned long   maclo;          /// multiply/accumulate reg

    unsigned long   pc;             /// Program Counter

    /// Saved only for exceptions, and not restored when continued:
    /// Effective address of instruction/data access that caused exception
    unsigned long   eear;           /// Exception effective address reg
    /// These are only saved for exceptions and interrupts
    int             vector;         /// Vector number
    int             sr;             /// Status Reg

} PM_SavedRegisters;

typedef void (*PM_IsrCb_Type)(PM_SavedRegisters *regs, MS_U32 vector);


typedef void ( *IRRecord_Callback ) (MS_VIRT virtSrcAddr, MS_U16 u16DataSize);

///Define PM IRQ Struct
typedef struct
{
    PM_IsrCb_Type  isr;     /// PM ISR call back type
    MS_U32         data;    /// PM ISR data parameter
} PM_IrqStruct;





///Define PM debug level
typedef enum _PM_DbgLv
{
    E_PM_DBGLV_NONE,          /// no debug message
    E_PM_DBGLV_ERR_ONLY,      /// show error only
    E_PM_DBGLV_INFO,          /// show error & informaiton
    E_PM_DBGLV_ALL,           /// show error, information & funciton name
}PM_DbgLv;

///Define PM Return Value
typedef enum
{
    /// fail
    E_PM_FAIL = 0,
    /// success
    E_PM_OK = 1,

} PM_Result;

///Define PM return to HK PowerOn Mode
typedef enum
{
    E_PM_POWERON_STANBY = 1,
    E_PM_POWERON_SLEEP  = 2,
    E_PM_POWERON_DEEPSLEEP = 3,
    E_PM_POWERON_ACON = 0xffUL,

} PM_PowerOnMode;

typedef enum
{
	E_PM_WAKEUPSRC_NONE = 0,
	E_PM_WAKEUPSRC_IR,
	E_PM_WAKEUPSRC_DVI,
	E_PM_WAKEUPSRC_DVI2,
	E_PM_WAKEUPSRC_CEC,
	E_PM_WAKEUPSRC_SAR,
	E_PM_WAKEUPSRC_ESYNC,
	E_PM_WAKEUPSRC_SYNC,
	E_PM_WAKEUPSRC_RTC,
	E_PM_WAKEUPSRC_RTC2,
	E_PM_WAKEUPSRC_AVLINK,
	E_PM_WAKEUPSRC_UART,
	E_PM_WAKEUPSRC_GPIO,
	E_PM_WAKEUPSRC_MHL,
	E_PM_WAKEUPSRC_WOL,

} PM_WakeupSource;

///Define PM Acknoledge Flags
typedef enum
{
    E_PM_ACKFLG_NULL        = 0,                    /// Ack flag for NULL
    E_PM_ACKFLG_WAIT_INIT   = (1<<0),               /// Ack flag for Init
    E_PM_ACKFLG_WAIT_STATUS = (1<<1),               /// Ack flag for Status
    E_PM_ACKFLG_WAIT_LIBVER = (1<<2),               /// Ack flag for Library version
    E_PM_ACKFLG_WAIT_POWERDOWN = (1<<3),            /// Ack flag for Power down
    E_PM_ACKFLG_WAIT_RTCINIT = (1<<4),              /// Ack flag for Rtc Init
    E_PM_ACKFLG_WAIT_RTCSETCOUNTER = (1<<5),        /// Ack flag for set rtc counter
    E_PM_ACKFLG_WAIT_RTCGETCOUNTER = (1<<6),        /// Ack flag for get rtc counter
    E_PM_ACKFLG_WAIT_RTCSETMATCHCOUNTER = (1<<7),   /// Ack flag for set match counter
    E_PM_ACKFLG_WAIT_RTCGETMATCHCOUNTER = (1<<8),   /// Ack flag for get match counter
    E_PM_ACKFLG_WAIT_INFO = (1<<9),                 /// Ack flag for Get info
    E_PM_ACKFLG_WAIT_GETMASTERKEY = (1<<10),        /// Ack flag for Get master key
    E_PM_ACKFLG_WAIT_GETDEVICEID = (1<<11),         /// Ack flag for Get device ID
    E_PM_ACKFLG_WAIT_GETCHIPID = (1<<12),           /// Ack flag for Get chip ID
    E_PM_ACKFLG_WAIT_GPIOINIT = (1<<13),            /// Ack flag for init GPIO
    E_PM_ACKFLG_WAIT_PWMINIT = (1<<14),             /// Ack flag for PWM init
    E_PM_ACKFLG_WAIT_PWMCONFIG = (1<<15),           /// Ack flag for PWM config
    E_PM_ACKFLG_WAIT_GETPMMEMADDR = (1<<16),        /// Ack flag for Get PM mem addr
    E_PM_ACKFLG_WAIT_GETEXTRASRAMDATA = (1<<17),    /// Ack flag for Get PM extra sram data
    E_PM_ACKFLG_WAIT_SETEXTRASRAMDATA = (1<<18),    /// Ack flag for Set PM extra sram data
    E_PM_ACKFLG_WAIT_WDT_KICK   = (1<<19),          /// Ack flag for Wdt kick
    E_PM_ACKFLG_WAIT_WOC_INIT   = (1<<20),          /// Ack flag for WoC Init
} PM_AckFlags;

///Define PM PWM Acknowledge Flags
typedef enum
{
    E_PM_PWM_ACKFLG_NULL        = 0,                    /// Ack flag for NULL
    E_PM_PWM_ACKFLG_WAIT_INIT   = (1<<0),               /// Ack flag for Init
    E_PM_PWM_ACKFLG_WAIT_CONF   = (1<<1),               /// Ack flag for Init
    E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_REQUIRE           = (1<<2),               /// Ack flag for ir recored reciecve require
    E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_COMPLETE          = (1<<3),              /// Ack flag for ir recored reciecve complete
    E_PM_PWM_ACKFLG_WAIT_IRRECORD_TRANSMIT                 = (1<<4),              /// Ack flag for ir record transmit
    E_PM_PWM_ACKFLG_WAIT_IRRECORD_SETCALLBACK              = (1<<5),               /// Ack flag for ir record reciecve set call back
    E_PM_PWM_ACKFLG_WAIT_IRRECORD_RECIEVE_EXIT             = (1<<6),               /// Ack flag for ir record reciecve exit
    E_PM_PWM_ACKFLG_WAIT_GPIO_PWM_REQUIRE                  = (1<<7),
} PM_PWM_AckFlags;

#if defined(MSOS_TYPE_ECOS)
typedef enum
{
    E_PM_ST_NULL = 0,          /// PM Driver Status is NULL
    E_PM_ST_BOOTING = 1,       /// PM Driver Status is Booting
    E_PM_ST_READY = 2,         /// PM Driver Status is Ready
    E_PM_ST_STANDBY = 3,       /// PM Driver Status is Standby
    E_PM_ST_PMMODE = 4,        /// PM Driver Status is PmMode
    E_PM_ST_MAX_STATUS = 255   /// Maximum status for PM Driver Status

} __attribute__ ((aligned (4096))) PM_DrvStatus;

#else
///Define PM Driver Status
typedef enum __attribute__ ((aligned (4096)))
{
    E_PM_ST_NULL = 0,          /// PM Driver Status is NULL
    E_PM_ST_BOOTING = 1,       /// PM Driver Status is Booting
    E_PM_ST_READY = 2,         /// PM Driver Status is Ready
    E_PM_ST_STANDBY = 3,       /// PM Driver Status is Standby
    E_PM_ST_PMMODE = 4,        /// PM Driver Status is PmMode
    E_PM_ST_MAX_STATUS = 255   /// Maximum status for PM Driver Status

} PM_DrvStatus;
#endif

///Define PM Power Down Mode
#define E_PM_STANDBY     0      /// PM Power Down Mode is Standby
#define E_PM_SLEEP      1       /// PM Power Down Mode is Sleep
#define E_PM_DEEP_SLEEP 2       /// PM Power Down Mode is Deep Sleep
#define E_PM_NORMAL     3

///Define PM WakeUp Mode
#define E_PM_WAKE_ZERO  0       /// PM Wakeup Mode is zero
#define E_PM_WAKE_LAST  1       /// PM Wakeup Mode is last
#define E_PM_ZERO_TWOSTAGE_POWERDOWN    2
#define E_PM_LAST_TWOSTAGE_POWERDOWN    3

///Define PM Command Index
typedef enum
{
    //Aeon-->51 Command Index
    PM_CMDIDX_INIT = 0x01UL,                  /// PM Command Index is INIT
    PM_CMDIDX_GET_STATUS = 0x02UL,            /// PM Command Index is GET STATUS
    PM_CMDIDX_GET_LIBVER = 0x03UL,            /// PM Command Index is GET LIBVER
    PM_CMDIDX_POWER_DOWN = 0x04UL,            /// PM Command Index is POWER DOWN
    PM_CMDIDX_RTC_INIT = 0x05UL,              /// PM Command Index is RTC INIT
    PM_CMDIDX_RTC_SETCOUNTER = 0x06UL,        /// PM Command Index is SET COUNTER
    PM_CMDIDX_RTC_GETCOUNTER = 0x07UL,        /// PM Command Index is GET COUNTER
    PM_CMDIDX_RTC_SETMATCHCOUNTER = 0x08UL,   /// PM Command Index is SET MATCH COUNTER
    PM_CMDIDX_RTC_GETMATCHCOUNTER = 0x09UL,   /// PM Command Index is GET MATCH COUNTER
    PM_CMDIDX_GET_INFO = 0x0AUL,              /// PM Command Index is GET INFO
    PM_CMDIDX_GET_MASTERKEY = 0x0BUL,
    PM_CMDIDX_GET_DEVICEID = 0x0CUL,
    PM_CMDIDX_GET_CHIPID = 0x0DUL,
    PM_CMDIDX_GPIO_INIT = 0x0EUL,
    PM_CMDIDX_PWM_INIT =0x0FUL,
    PM_CMDIDX_PWM_CONFIG =0x10UL,

    PM_CMDIDX_PWM_IRRECORD_RECEIVED_SETCALLBACK = 0x11UL,
    PM_CMDIDX_PWM_IRRECORD_RECEIVED_REQUIRE = 0x12UL,
    PM_CMDIDX_PWM_IRRECORD_RECEIVED_COMPLETE = 0x13UL,
    PM_CMDIDX_PWM_IRRECORD_TRANSMIT = 0x14UL,
    PM_CMDIDX_PWM_IRRECORD_RECEIVED_EXIT = 0x15UL,
    PM_CMDIDX_GETEXTRASRAMDATA = 0x16UL,
    PM_CMDIDX_SETEXTRASRAMDATA = 0x17UL,
    PM_CMDIDX_SET_ONBOARD_LED = 0x18UL,             // Control On Board LED
    PM_CMDIDX_SET_POWER_LED_N_PROGRAM_TIMER_LED = 0x19UL,

    PM_CMDIDX_LED_FLASH     = 0x20UL,             // PM Command Index is LED Start        //@@++-- 20110329 Arki
    PM_CMDIDX_GLOBAL_CHIP_RESET = 0x21UL,
    PM_CMDIDX_GET_PM_MEMADDR = 0x22UL,
    PM_CMDIDX_GET_SYSTEM_ALIVE_STATUS    = 0x23UL,
    PM_CMDIDX_WDT_STATUS  = 0x24UL,
    PM_CMDIDX_UPATE_POWER_LED_STATUS = 0x25UL,
    PM_CMDIDX_LED_FLASH_TOGGLE     = 0x28UL,             // facory LED toggle
    PM_CMDIDX_POWER_ON_TIME_RECORD     = 0x29UL,  // PM Command Index is record AC and DC on time  Kenny

    PM_CMDIDX_ACK_51ToAEON = 0x30UL,          /// PM Command Index is ACK 51 To AEON
    PM_CMDIDX_WDT_KICK = 0x40UL,
#if defined(UFO_PM_BACKLIGHT_CTRL)
    PM_CMDIDX_LED = 0x42,
    PM_CMDIDX_START_BACKLIGHT_PROCESSING = 0x44,
    PM_CMDIDX_SET_BACKLIGHT_CTRL_MODE = 0x45,   //PM command idx is set BL control mode
    PM_CMDIDX_SET_BACKLIGHT_CTRL_ON_OFF = 0x46, //PM command idx is control BL on/off
    PM_CMDIDX_SET_SOURCE_SIGNAL_CTRL_MODE = 0x47,   //PM command idx is set BL control mode
    PM_CMDIDX_SET_OFL_STB = 0x48,
#endif
    PM_CMDIDX_SN_INIT_ACK   =  0x50UL,        /// to inform PM SN is start up
    PM_CMDIDX_PWM_GPIO_REQUIRE = 0x60UL,
    //51->Aeon Command Index
    PM_CMDIDX_ACK_AEONTo51 = 0xA0UL,          /// PM Command Index is ACK AEON To 51

    PM_CMDIDX_WOC_INIT = 0xB0UL,              /// PM Command Index is WoC INIT
} PM_CmdIndex;

typedef enum
{
    E_WDT_NOT_ENABLE,
    E_WDT_REFRESH,
    E_WDT_STOP_REFRESH,
    E_WDT_RESET_SYSTEM_IMMEDIATE,
} WDT_STATUS;



typedef enum
{
    E_ON_BOARD_LED_OFF= 0,
    E_ON_BOARD_LED_PATTERN_1 = 1,
    E_ON_BOARD_LED_PATTERN_2 = 2,
    E_ON_BOARD_LED_PATTERN_3 = 3,
    E_ON_BOARD_LED_PATTERN_4 = 4,
    E_ON_BOARD_LED_PATTERN_5 = 5,
    E_ON_BOARD_LED_ON= 255,
} USB_LED_PATTERN;

typedef enum
{
    E_ON_BOARD_LED_STAGE_BIST= 0,
    E_ON_BOARD_LED_STAGE_MBOOT_SUCCESS=1,
    E_ON_BOARD_LED_STAGE_APL_SUCCESS= 2,
    E_ON_BOARD_LED_STAGE_UPDATE_SW= 3,
} ON_BOARD_LED_STAGE;


typedef enum
{
    E_PROGRAM_TIMER_IS_SET= 0,
    E_PROGRAM_TIMER_IS_REMINDING= 1,
    E_PROGRAM_TIMER_IS_RECORDING=2,
    E_PROGRAM_TIMER_IS_NOT_SET= 3,
} PROGRAM_TIMER_LED_STATUS;

typedef enum
{
    E_SW_UPDATE_NOT_IN_PROGRESS= 0,
    E_SW_UPDATE_IN_PROGRESS= 1,
    E_SW_UPDATE_FAILED= 2,
    E_SW_UPDATE_SUCCESSFUL= 3,
    E_SW_UPDATE_FAN_ERROR=4,
    E_SW_UPDATE_FAN_ERROR_RECOVER=5,
} SW_UPDATE_STATUS;

typedef enum
{
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE1= 1,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE2= 2,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE3= 3,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE4= 4,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE5= 5,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE6= 6,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE7= 7,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE8= 8,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE9= 9,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE10= 10,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE11= 11,
    E_POWER_LED_N_PROGRAM_TIMER_LED_MODE12= 12,
} POWER_LED_N_PROGRAM_TIMER_LED_MODE;

typedef enum
{
    E_POWER_LED_KEEP= 0,
    E_POWER_LED_GREEN,
    E_POWER_LED_RED,
    E_POWER_LED_ORANGE,
} UPATE_POWER_LED_PATTERN;


///Define PM STR Mode enum
typedef enum
{
    E_PM_NON_STR=0,           /// non-str dc on
    E_PM_NON_STR_AC,        /// non-str ac on
    E_PM_STR,               /// str
    E_PM_STR_CRC,           /// str and crc
}PM_STRMode;

/// Define PM Wake-up Configuration
#define CRC_KERNEL_BUF                 (3)
typedef struct __attribute__ ((aligned (4096)))
{
    MS_U8 bPmWakeEnableIR         : 1;/// For PM IR Wake-up
    MS_U8 bPmWakeEnableSAR        : 1;/// For PM SAR Wake-up
    MS_U8 bPmWakeEnableGPIO0      : 1;/// For PM GPIO0 Wake-up
    MS_U8 bPmWakeEnableGPIO1      : 1;/// For PM GPIO1 Wake-up
    MS_U8 bPmWakeEnableUART1      : 1;  /// For PM UART1 Wake-up
    MS_U8 bPmWakeEnableSYNC       : 1;/// For PM SYNC Wake-up
    MS_U8 bPmWakeEnableESYNC      : 1;/// For PM EasySYNC Wake-up

    MS_U8 bPmWakeEnableRTC0       : 1;/// For PM RTC0 Wake-up
    MS_U8 bPmWakeEnableRTC1       : 1;/// For PM RTC1 Wake-up
    MS_U8 bPmWakeEnableDVI0       : 1;/// For PM DVI0 Wake-up
    MS_U8 bPmWakeEnableDVI2       : 1;/// For PM DVI1 Wake-up
    MS_U8 bPmWakeEnableCEC        : 1;/// For PM CEC Wake-up
    MS_U8 bPmWakeEnableAVLINK     : 1;/// For PM AVLINK Wake-up
    MS_U8 bPmWakeEnableMHL        : 1;/// For PM MHL Wake-up
    MS_U8 bPmWakeEnableWOL        : 1;/// For PM WOL Wake-up
    MS_U8 bPmWakeEnableCM4        : 1;/// For PM CM4 Wake-up

    MS_U8 u8PmWakeIR[PM_MAX_BUF_WAKE_IR];///For PM IR Wake-up key define
    MS_U8 u8PmWakeIR2[PM_MAX_BUF_WAKE_IR2];///For PM IR Wake-up key define
    MS_U8 u8PmWakeMACAddress[PM_MAX_BUF_WAKE_MAC_ADDRESS];///For PM WOL Wake-up Mac define

    MS_U8 u8PmStrMode;
    MS_BOOL bLxCRCMiu[CRC_KERNEL_BUF];
    MS_U32 u32LxCRCAddress[CRC_KERNEL_BUF];
    MS_U32 u32LxCRCSize[CRC_KERNEL_BUF];
    MS_U8 u8PmWakeEnableWOWLAN;
    MS_U8 u8PmWakeWOWLANPol;
    MS_U8 u8PmWakeKeyShotCountAddr[PM_MAX_BUF_WAKE_IR_WAVEFORM_ADDR];///For PM IR Wake-up key waveform define
    MS_U8 u8HdmiByPass;
    MS_U8 u8Reserved[PM_MAX_BUF_RESERVERD];
} PM_WakeCfg;

/// Define PM WoC Wake-up Configuration
#define PM_MAX_BUF_WAKE_WOC_MAC_ADDRESS (6)
#define PM_MAX_BUF_WAKE_WOC_IP_ADDRESS  (4)
#define PM_MAX_BUF_WAKE_WOC_PORT        (2)

typedef struct __attribute__ ((aligned (4096)))
{
    MS_U8 bPmWakeEnableWOC                 : 1;
    MS_U8 bPmWakeEnableWOCdesMAC_f1        : 1;
    MS_U8 bPmWakeEnableWOCIP_f1            : 1;
    MS_U8 bPmWakeEnableWOCProtocol_f1      : 1;
    MS_U8 bPmWakeEnableWOCPort_f1          : 1;

    MS_U8 bPmWakeEnableWOCdesMAC_f2        : 1;
    MS_U8 bPmWakeEnableWOCIP_f2            : 1;
    MS_U8 bPmWakeEnableWOCProtocol_f2      : 1;
    MS_U8 bPmWakeEnableWOCPort_f2          : 1;

    MS_U8 u8PmWakeWOCMACAddress_f1[PM_MAX_BUF_WAKE_WOC_MAC_ADDRESS];
    MS_U8 u8PmWakeWOCMACAddress_f2[PM_MAX_BUF_WAKE_WOC_MAC_ADDRESS];
    MS_U8 u8PmWakeWOCIP_f1[PM_MAX_BUF_WAKE_WOC_IP_ADDRESS];
    MS_U8 u8PmWakeWOCIP_f2[PM_MAX_BUF_WAKE_WOC_IP_ADDRESS];
    MS_U8 u8PmWakeWOCProtocol_f1;
    MS_U8 u8PmWakeWOCProtocol_f2;
    MS_U8 u8PmWakeWOCPort_f1[PM_MAX_BUF_WAKE_WOC_PORT];
    MS_U8 u8PmWakeWOCPort_f2[PM_MAX_BUF_WAKE_WOC_PORT];
} PM_WoC_WakeCfg;


/// Define PWM  simaulator IR mode
typedef struct __attribute__ ((aligned (4096)))
{
    MS_U8 u8IREncodeMode;
    MS_U8 u8Customercode1;
    MS_U8 u8Customercode2;
    MS_U8 u8IRKeycode;
    MS_U8 u8IRRepeat;
    MS_U8 u8Reserved0;              /// Reserved
    MS_U8 u8Reserved1;              /// Reserved
    MS_U8 u8Reserved2;              /// Reserved
} PWMSimIR_CFG;

typedef struct __attribute__ ((aligned (4096)))
{
    //MS_U32 u32RtcCounter;
    //MS_U8 u8WakeupSource;
    MS_U16 u16Offset;
    MS_U16 u16DataLen;
    MS_U8  u8ExtraSramData[]; //Max size 512 bytes
} PM_ExtraSramData;

/// Define PM Power Down Modes
typedef struct __attribute__ ((aligned (4096)))
{
    MS_U8 u8PowerDownMode;        /// Power Down Mode
    MS_U8 u8WakeAddress;

} PM_PowerDownCfg;

/// Define PM RTC Parameters
typedef struct __attribute__ ((aligned (4096)))
{
    MS_U32 u32RtcCtrlWord;          /// Define PM RTC Control Word
    MS_U32 u32RtcSetMatchCounter;   /// Define PM RTC for Set Match Counter
    MS_U32 u32RtcGetMatchCounter;   /// Define PM RTC for Get Match Counter
    MS_U32 u32RtcSetCounter;        /// Define PM RTC for Set Counter
    MS_U32 u32RtcGetCounter;        /// Define PM RTC for Get Counter
    MS_U8 u8PmRtcIndex;             /// Define PM RTC Index
    MS_U8 u8Reserved0;              /// Reserved
    MS_U8 u8Reserved1;              /// Reserved
    MS_U8 u8Reserved2;              /// Reserved

} PM_RtcParam;

/// Define PM Driver Information
typedef struct __attribute__ ((aligned (4096)))
{
    MS_U8 u8PmSupSleepMode; /// Spuuort PM Sleep Mode
    MS_U8 u8PmSupRtcIdxMax; /// Support RTC Index Max

}PM_DrvInfo;

typedef struct
{
    MS_U8 u8Reserved[16]; ///Dummy Write for MIU FIFO

}PM_Dummy;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_isRunning(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_InterruptRequest( void );
void MDrv_PM_LoadFw( MS_U8 *pPmCode, MS_U16 u16Len );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//------------------------------------------------------------------------------------------------- 
MS_BOOL MDrv_PM_RegWrite( MS_U16 u16Addr, MS_U8 u8Data );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_RegRead( MS_U16 u16Addr );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWriteBit(MS_U16 u16Addr, MS_U8 bBit, MS_U8 u8BitPos );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_RegReadBit(MS_U16 u16Addr, MS_U8 u8BitPos);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWrite2byte( MS_U16 u16RegIndex, MS_U16 u16Value );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RegWrite3byte( MS_U16 u16Regndex, MS_U32 u32Value );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_Control( MS_U8 u8opcode, MS_U8 u8Data, MS_U16 u16Data );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_CtrlWrite( MS_U8 u8Opcode, MS_U8 u8CmdType, MS_U8 u8DataNum, MS_U8 *pu8Data );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_CtrlRead( MS_U8 u8Opcode, MS_U8 u8CmdType, MS_U8 u8DataNum, MS_U8 *pu8Data );
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_MemoryWrite(MS_U8 *pu8Data, MS_U16 u16MemLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_MemoryRead(MS_U8 *pu8Data, MS_U16 u16MemLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCEnableInterrupt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTC2EnableInterrupt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTC2SetMatchTime(MS_U32 u32PmSysTime);
/*add by owen.qin end*/
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCSetMatchTime(MS_U32 u32PmSysTime);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PM_RTCGetMatchTime(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCSetSystemTime(MS_U32 u32PmSysTime);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PM_RTCGetSystemTime(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RTCInit(MS_U32 u32CtrlWord);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IRInit(MS_U8 irclk_mhz, PM_IrRegCfg *irRegCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_KeypadInit(PM_SarRegCfg *sarRegCfg, PM_SarParamCfg *sarParamCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_CalibrateRC(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_isDownloaded(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_Set_Download(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqAttach(PM_IRQ_TYPE irq, PM_IsrCb_Type isr, MS_U32 data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqDetach(PM_IRQ_TYPE irq);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqUnmask(PM_IRQ_TYPE irq);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqMask(PM_IRQ_TYPE irq);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_IrqMaskAll(void);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_ActiveStandbyMode(MS_BOOL bTrigger);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_IsActiveStandbyMode(MS_BOOL *bActive);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_Init(PM_WakeCfg *pPmWakeCfg);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_WoC_Init(PM_WoC_WakeCfg *pPmWakeCfg);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetStatus(PM_DrvStatus *pDrvStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PowerDown(PM_PowerDownCfg *pPmPowerDownCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetMasterKey(MS_U8 *pPmPowerDownCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetDeviceID(MS_U8 *DeviceID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetChipID(MS_U8 *ChipParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetPMMemAddr(MS_U8 *PmMemAddr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//--------------------------------------------------------------------------------------------------
const PM_DrvInfo* MDrv_PM_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GPIOInit(MS_U16 u16GPIOIndex);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_SetDbgLevel(PM_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcInit(PM_RtcParam *pPmRtcParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_Rtc_DisableInit(E_PM_RTC eRtc);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcSetCounter(PM_RtcParam *pPmRtcParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcGetCounter(PM_RtcParam *pPmRtcParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcSetMatchCounter(PM_RtcParam *pPmRtcParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTC
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_RtcGetMatchCounter(PM_RtcParam *pPmRtcParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GetExtraSramData(PM_ExtraSramData *pPmExtraSramData);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_SetExtraSramData(PM_ExtraSramData *pPmExtraSramData);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_SetSPIOffsetForMCU(MS_U32 BANK);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_SetSRAMOffsetForMCU(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PM_SetDRAMOffsetForMCU(MS_U32 u32Offset);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_PowerOnMode MDrv_PM_PowerOnMode(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_WakeupSource MDrv_PM_GetWakeupSource(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_GetWakeupKey(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_Disable51(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_GPIO4_SetPower(MS_BOOL bOn);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_Init(MS_U8 u8PWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_Config(PWMSimIR_CFG *pPmPWMCfg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Complete(MS_U32 u32BufferAddr,MS_U16 u16BufferSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Require(MS_U32 u32BufferAddr,MS_U16 u16BufferSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Transmit(MS_U32 u32BufferAddr,MS_U16 u16BufferSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_SetCallBackFunction(IRRecord_Callback pCallback);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_PWM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_IRRecord_Receive_Exit(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTPM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_RunTimePM_Disable_PassWord(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_RTPM
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_WDT_Kick(MS_U8 u8Kick);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_STR
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PM_STR_CheckFactoryPowerOnModePassword(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_STR
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_STR_CheckFactoryPowerOnMode_Second(MS_BOOL bCheck);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_Check_Version(MS_BOOL bCheck);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PM_GetSRAMSize(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PM_GetIRPowerOnKey(void);
void MDrv_PM_GetRT51Status(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PM_COMMON
/// @param u8cfg \b IN: Pin Config
/// @param u8step \b IN: Period
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PM_Result MDrv_PM_PWM_Duty(MS_U8 u8cfg,MS_U8 u8step);
#ifdef MSOS_TYPE_LINUX
PM_Result MDrv_PM_IO_INIT(void);
PM_Result MDrv_PM_WriteDramInfo_Data(MS_U32 u32Addr, MS_U32 u32Size);
PM_Result MDrv_PM_WriteDramInfo_Code(MS_U32 u32Addr, MS_U32 u32Size);
#endif
#if defined(UFO_PM_BACKLIGHT_CTRL)
  PM_Result MDrv_PM_LED(MS_U8 u8Status);
  PM_Result MDrv_PM_Backlight_Processing(void);
  PM_Result MDrv_PM_Backlight_OnOff(MS_U8 eStatus, MS_U8 u8BacklightDuty);
  PM_Result MDrv_PM_Backlight_Ctrl_Mode(MS_U8 eBLCtrlMode, MS_U8 u8BacklightDuty);
  PM_Result MDrv_PM_Backlight_Operation(MS_U8 u8status);
#endif
PM_Result MDrv_PM_STR_CRC_Check(MS_U16 u16CRCChecksum);
#ifdef __cplusplus
}
#endif

#endif // _DRV_PM_H_

