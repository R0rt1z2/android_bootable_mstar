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
/// @brief  UART Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_UART UART interface
    \ingroup  G_PERIPHERAL

    \brief

    UART is a piece of computer hardware that translates data between parallel and serial forms.
    UARTs are commonly used in conjunction with communication standards such as EIA, RS-232, RS-422 or RS-485.
    The universal designation indicates that the data format and transmission speeds are configurable.
    The electric signaling levels and methods are handled by a driver circuit external to the UART.

    <b>Features</b>

    - A clock generator, usually a multiple of the bit rate to allow sampling in the middle of a bit period.
    - Input and Output shift registers
    - Transmit/Receive control
    - Read/Write control logic
    - Transmit/Receive buffers (optional)
    - Parallel data bus buffer (optional)
    - First-in, first-out (FIFO) buffer memory (optional)

    <b> UART Block Diagram: </b> \n
    \image html drvUART_pic1.png

    <b> Operation Code Flow: </b> \n
    -# Prepare UART setting for each operation
    -# Set and start UART in command handle
    -# Trigger UART

     \defgroup G_UART_INIT Initialization Task relative
     \ingroup  G_UART
     \defgroup G_UART_TXRX Transmit/Receive control Task relative
     \ingroup  G_UART
     \defgroup G_UART_OTHER  other relative
     \ingroup  G_UART
*/

#ifndef __MDRV_UART_H__
#define __MDRV_UART_H__

///
/// @brief MStar driver for UART devices
///
/// @par Example: connect AEON uart to UART port0, output message
/// @code
/// ms_uart_dev_t uart;
///
/// mdrv_uart_connect(E_UART_PORT0, E_UART_AEON);
/// uart = mdrv_uart_open(E_UART_AEON);
/// if (uart != UART_DEV_NULL)
/// {
///     mdrv_uart_set_baudrate(uart, 38400);    // must do this after base clock changed
///     setconsole(uart);                       // set this uart device as console
/// }
/// printf("uart test");
/// mdrv_uart_close(uart);
/// @endcode
///
///
/// @par Example: use buffered output
/// @code
/// ms_uart_dev_t uart;
/// char tx_buffer[1024];
///
/// uart = mdrv_uart_open(E_UART_PIU_UART0);
/// if (uart != UART_DEV_NULL)
/// {
///     mdrv_uart_set_tx_buffer(uart, tx_buffer, sizeof(tx_buffer));
/// }
/// @endcode



#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define UART_CAPS_AEON                  0x00000001
#define UART_CAPS_PIU                   0x00000002


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define UART_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'U','A','R','T'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','6'},                          /* 00 ~ 99                                          */  \
    {'0','0','3','1','0','0','5','0'},  /* CL#                                              */  \
    MSIF_OS

#define UART_TYPE_AEON      0x10
#define UART_TYPE_PIU       0x20
#define UART_TYPE_EYWA		0


// Baud rate
//#define UART_BAUD_RATE                38400 //9600 //19200 //38400 //57600 //115200 =>too frequent Rx INT
#define UART_BAUD_RATE              115200

#define UART_DEV_NULL   ((ms_uart_dev_t)0)

struct msdevio
{
    MS_VIRT (*open) (device_t dev, int mode);
    int     (*close)(device_t dev);
//    int     (*read) (device_t dev, char *buf, size_t len, int off);
    int     (*read) (device_t dev, char *buf, MS_U32 len, int off);
//    int     (*write)(device_t dev, const char *buf, size_t len, int off);
    int     (*write)(device_t dev, const char *buf, MS_U32 len, int off);
    int     (*poll) (device_t dev, int types);
    int     (*ioctl)(device_t dev, int request, unsigned long arg);
    int     (*event)(eDEVEVT evt);
};

struct msdevice
{
    int             magic;              // magic number
    int             refcnt;             // reference count
    int             flags;              // device characteristics
    struct msdevio  *devio;             // device i/o table
};

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_UART_OK,
    E_UART_FAIL,
} UART_Result;

typedef struct _UART_DrvInfo
{
    MS_BOOL                         Init;
    MS_U32                          Caps;
} UART_DrvInfo;

typedef struct _UART_DrvStatus
{
    MS_BOOL                         HwBusy;
} UART_DrvStatus;

/// UART devices, not all of them can be access directly
typedef enum
{
    E_UART_INVALID = -1,    ///< INVALID
    E_UART_AEON,            ///< AEON
    E_UART_VDEC,            ///< VD_MHEG5 (only used in mdrv_uart_connect())
    E_UART_TSP,             ///< TSP (only used in mdrv_uart_connect())
    E_UART_PIU_UART0,       ///< PIU0
    E_UART_PIU_UART1,       ///< PIU1 (only available for some chips)
    E_UART_PIU_UART2,       ///< PIU2 (only available for some chips)
    E_UART_PIU_FUART0,      ///< PIU Fast UART (only available for some chips)
    E_UART_HK51_UART0,      ///< HK51 UART0 (only used in mdrv_uart_connect())
    E_UART_HK51_UART1,      ///< HK51 UART1 (only used in mdrv_uart_connect())
    E_UART_VD51_UART0,      ///< VD51 UART0 (only used in mdrv_uart_connect())
    E_UART_VD51_UART1,      ///< VD51 UART1 (only used in mdrv_uart_connect())
                            ///<            (only available for some chips)
    E_UART_AEON_R2,         ///< AEON_R2 for HK
    E_UART_AEON_AUDIO_R2,   ///< AEON_R2 for Audio
    E_UART_DMD51_UART0,     ///< DMD51 UART0 (only used in mdrv_uart_connect())
    E_UART_DMD51_UART1,     ///< DMD51 UART1 (only used in mdrv_uart_connect())
    E_UART_SECURE_R2,       ///< SECURE_R2 (only available for some chips)
    E_UART_OFF,             ///< UART Disable
    E_UART_AEON_AUDIO_R2_1, ///< AEON_R2 1 for Audio
    E_UART_VD_R2_L,         ///< VD R2 L
    E_UART_SECURE51,        ///< Secure51

    E_UART_PIU_UART3,       ///< PIU3 (only available for some chips)
    E_UART_PIU_UART4,       ///< PIU4 (only available for some chips)
    E_UART_EVD_R2,          ///< EVD R2
    E_UART_FRC_R2,          ///< FRC R2
    E_UART_CM4_UART_0, /// CM4 dbg msg
} UART_DEVICE_TYPE;

/// UART port
/// different port have different pad mux, please check package spec.
/// sometimes the net name will mislead the real port number
typedef enum
{
    E_UART_PORT_INVALID = -1,///< invalid uart port
    E_UART_PORT0,           ///< usually muxed with DDCA
    E_UART_PORT1,           ///< port 1
    E_UART_PORT2,           ///< port 2
    E_UART_PORT3,           ///< port 3
    E_UART_PORT4,           ///< port 4
} UART_PORT_TYPE;

/// UART Pad
/// different port have different pad mux
/// sometimes the net name will mislead the real pad number
typedef enum
{
	//<< port 0 pad default with DDCA
	//port1 --> second uart mode
    E_UART_PAD_1_0,         ///<disable port 1 pad
    E_UART_PAD_1_1,         ///<port1 pad 1
    E_UART_PAD_1_2,         ///<port1 pad 2
    E_UART_PAD_1_3,         ///<port1 pad 3
    //port2 --> third uart mode
    E_UART_PAD_2_0,         ///<disable port 2 pad
    E_UART_PAD_2_1,         ///<port2 pad 1
    E_UART_PAD_2_2,         ///<port2 pad 2
    E_UART_PAD_2_3,         ///<port2 pad 3
    //port3 --> fourth uart mode
    E_UART_PAD_3_0,         ///<disable port 3 pad
    E_UART_PAD_3_1,         ///<port3 pad 1
    E_UART_PAD_3_2,         ///<port3 pad 2
    E_UART_PAD_3_3,         ///<port3 pad 3
    //port4 --> fast uart mode
    E_UART_PAD_4_0,         ///<disable port 4 pad
    E_UART_PAD_4_1,         ///<port4 pad 1
    E_UART_PAD_4_2,         ///<port4 pad 2
    E_UART_PAD_4_3,         ///<port4 pad 3
    //<< port 0 pad default with DDCA
    E_UART_PAD_0_0,         ///<port0 default pad
    E_UART_PAD_0_1,         ///<port0 pad 1 : DDCDA CK/DA
    E_UART_PAD_0_2,         ///<port0 pad 2 : NA
    E_UART_PAD_0_3,         ///<port0 pad 3 : NA
}UART_PAD_TYPE;

typedef enum _UART_DbgLv
{
    E_UART_DBGLV_NONE           //no debug message
    ,E_UART_DBGLV_ERR_ONLY      //show error only
    ,E_UART_DBGLV_INFO          //show error & informaiton
    ,E_UART_DBGLV_ALL           //show error, information & funciton name
}UART_DbgLv;


/// prototypr of UART rx callback
typedef void (*ms_uart_rx_callback)(int c);     // TODO: add user data?

/// prototypr of UART rx callback
typedef void (*ms_uart_rx_callback_halreg)(int c, MS_U32 pHalReg);

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// @brief open UART device, both rx/tx is enabled after this
/// @ingroup G_UART_INIT
/// @param uart_dev:    uart device instance to open (AEON / PIU_UARTx)
/// @return uart handle. when failed, it returns UART_DEV_NULL
///////////////////////////////////////////////////////////////////////////////
MS_VIRT mdrv_uart_open(UART_DEVICE_TYPE uart_dev);

// control power state for fast booting
MS_U32 MDrv_UART_SetPowerState(EN_POWER_MODE u16PowerState);

///////////////////////////////////////////////////////////////////////////////
/// @brief close a UART device handle
/// @ingroup G_UART_INIT
/// @param uart \b    uart handle to close
/// @return 0 if sucess, -1 if failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_close(MS_VIRT uart);

///////////////////////////////////////////////////////////////////////////////
/// Please note the connectivity should be a one-to-one mapping
/// @ingroup G_UART_INIT
/// @brief connect a UART device to a UART port
/// @param uart_port:   uart port
/// @param uart_dev:    uart device
/// @return 0: sucess, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_connect(UART_PORT_TYPE uart_port, UART_DEVICE_TYPE uart_dev);

///////////////////////////////////////////////////////////////////////////////
/// @brief get the connection of UART port to UART device, (get device by port)
/// @ingroup G_UART_INIT
/// @param uart_port:   uart port
/// @return enum UART_DEVICE_TYPE, E_UART_INVALID if error occurs
///////////////////////////////////////////////////////////////////////////////
UART_DEVICE_TYPE mdrv_uart_get_connection(UART_PORT_TYPE uart_port);

///////////////////////////////////////////////////////////////////////////////
/// Please note the connectivity should be a one-to-one mapping
/// @brief connect a UART device to a UART port , which connect to  a UART port pad
/// @ingroup G_UART_INIT
/// @param uart_port:   uart port
/// @param uart_dev:    uart device
/// @param uart_dev:    uart device
/// @return 0: sucess, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_connect_mux(UART_DEVICE_TYPE uart_dev, UART_PORT_TYPE uart_port, UART_PAD_TYPE uart_pad);

///////////////////////////////////////////////////////////////////////////////
/// @brief Invert the signal polarity of UART
/// @ingroup G_UART_OTHER
/// @param uart_port:   uart device
/// @param bInv:        0: not invert, 1 invert
/// @return 0: sucess -1: failed (uart_port out of range)
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_invert(UART_PORT_TYPE uart_port, MS_BOOL bInv);

///////////////////////////////////////////////////////////////////////////////
/// Set the baudrate of the UART device, the base clock must be set before set
/// baudrate. If the clock is changed (like CKG_MCU change for E_UART_AEON),
/// this function must be called again for
/// @ingroup G_UART_INIT
/// @brief Set the baudrate of the UART device
/// @param uart:   uart device handle
/// @param baudrate: baudrate
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_set_baudrate(MS_VIRT uart, MS_U32 baudrate);

///////////////////////////////////////////////////////////////////////////////
/// The rx_cb will be called every incoming byte is received
/// @ingroup G_UART_INIT
/// @brief Set RX callback by interrupt serive routine
/// @param uart:    uart device handle
/// @param rx_cb:   rx callback, 0 to disable the rx callback
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_set_rx_callback(MS_VIRT uart, ms_uart_rx_callback rx_cb);

///////////////////////////////////////////////////////////////////////////////
/// If tx_buffer is not NULL, uart tx will use buffered mode, otherwise use polling mode
/// For Fast UART DMA mode, the buffer and len must aligned to MIU unit size
/// @ingroup G_UART_INIT
/// @brief set tx buffer for buffered mode
/// @param uart:        uart device handle
/// @param tx_buffer:   pointer to tx buffer
/// @param len:         tx buffer size
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_set_tx_buffer(MS_VIRT uart, void *tx_buffer, MS_U16 len);

///////////////////////////////////////////////////////////////////////////////
/// If tx_buffer is not NULL, uart tx will use buffered mode, otherwise use polling mode
/// For Fast UART DMA mode, the buffer and len must aligned to MIU unit size
/// @ingroup G_UART_INIT
/// @brief set tx buffer for buffered mode
/// @param uart:        uart device handle
/// @param rx_buffer:   pointer to tx buffer
/// @param len:         tx buffer size
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_set_rx_buffer(MS_VIRT uart, void *rx_buffer, MS_U16 len);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief enable uart rx
/// @param uart:        uart device handle
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_rx_enable(MS_VIRT uart);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief enable uart tx
/// @param uart:        uart device handle
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_tx_enable(MS_VIRT uart);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief disable uart rx
/// @param uart:        uart device handle
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_rx_disable(MS_VIRT uart);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief disable uart tx
/// @param uart:        uart device handle
/// @return 0: success, -1: failed
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_tx_disable(MS_VIRT uart);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief read data from uart
/// @param uart:        uart device handle
/// @param buf:         buffer to store read data
/// @param len:         buffer length
/// @return -1: failed, others: bytes read
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_read(MS_VIRT uart, MS_U8 *buf, MS_U32 len);

///////////////////////////////////////////////////////////////////////////////
/// @ingroup G_UART_TXRX
/// @brief write data to uart
/// @param uart:        uart device handle
/// @param buf:         buffer for write data
/// @param len:         data length
/// @return -1: failed, others: bytes written
///////////////////////////////////////////////////////////////////////////////
MS_U16 mdrv_uart_write(MS_VIRT uart, const MS_U8 *buf, MS_U16 len);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 mdrv_uart_poll(MS_VIRT hUart, int types);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 mdrv_uart_set_rx_callback_halreg(MS_VIRT uart, ms_uart_rx_callback_halreg rx_cb);

/// UART callback function used in interrupt context
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
typedef void(*P_UART_ISR_Proc)(MS_U8 u8UARTID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
UART_Result MDrv_UART_SetIOMapBase(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_Init(UART_DEVICE_TYPE uart_dev, MS_U32 baudrate);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_Standby_Init (void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_PutChar(MS_U8 u8Ch);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_PutString(char *u8str);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_UART_GetChar(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_GetFile(MS_U8 *pu8Buf, MS_U32 u32Len);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const UART_DrvInfo* MDrv_UART_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
struct msdevice* MDrv_UART_GetDev(UART_DEVICE_TYPE uart_dev);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
UART_Result MDrv_UART_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
UART_Result MDrv_UART_GetStatus(UART_DrvStatus *pStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_UART_SetDbgLevel(MS_U8 level);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_UART_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
UART_Result MDrv_UART_SetPMRxEnable(MS_BOOL bEnable);
void MDrv_UART_SetUARTSecurityBank(UART_DEVICE_TYPE uart_dev);

#if UART_TYPE_EYWA
#define EYWA_PAYLOAD_LEN 8
#define EYWA_DEFAULT_PARITY 0

typedef struct _Eywa_FastUart_struct
{
        MS_U8 data_type;
        MS_U8 data_type_inv;
        MS_U16 len;
        MS_U8 payload[EYWA_PAYLOAD_LEN];
        MS_U8 parity;
} Eywa_FastUart_struct;

typedef enum
{
        E_EYWA_INVALID = 0,
        E_EYWA_KEYPAD,
        E_EYWA_VOICE,
        E_EYWA_GYROSCOPE,
        E_EYWA_GSENSOR,
        E_EYWA_TOUCHPAD
} EYWA_DATATYPE;

typedef enum
{
        E_EYWA_INV_INVALID = 0xFF,
        E_EYWA_INV_KEYPAD = 0xFE,
        E_EYWA_INV_VOICE = 0xFD,
        E_EYWA_INV_GYROSCOPE = 0xFC,
        E_EYWA_INV_GSENSOR = 0xFB,
        E_EYWA_INV_TOUCHPAD = 0xFA
} EYWA_DATATYPE_INV;

void Eywa_UART_RecvHandler(MS_U8);
MS_U8 Eywa_Cal8BitsChecksum(MS_U8 *, MS_U32);
void Eywa_UART_Init(void);

#endif


#ifdef __cplusplus
}
#endif

#endif /* __MDRV_UART_H__ */
