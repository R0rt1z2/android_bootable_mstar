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
// ********************************************************
//
//  $Workfile: PCMCIA.h $
//
//   Author/Copyright    Gero Kuehn / GkWare e.K.
//                        Humboldtstrasse 177
//                        45149 Essen
//                        GERMANY
//                        Tel: +49 174 520 8026
//                        Email: support@gkware.com
//                        Web: http://www.gkware.com
//
//    $Date:  $
//    $Revision:  $
//
// ********************************************************
//
// @brief This file contains the API of the PCMCIA CIS parser and the declaration
// for the PCMCIA driver API. PCMCIA drivers are expected to implement the functions starting with PCMCIA_ except for MDrv_PCMCIA_ReadAttribMem(),
// The MDrv_PCMCIA_ReadAttribMem() implementation is provided together with the GkWare CI Stack


//-------------------------------------------------
/*! \defgroup PCMCIA PCMCIA interface
    \ingroup  PCMCIA
    
    \brief 
 
    PCMCIA I/O interface (EN50221)

    <b> Operation Code Flow: </b> \n
    \image html pcmcia.png
        
  *! \defgroup PCMCIA_General PCMCIA general functions
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_Task PCMCIA process related functions
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_INT PCMCIA interrupt functions
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_AttributeMem PCMCIA Attribute memory related functions.
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_IOMem PCMCIA IO memory related functions.
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_STR PCMCIA STR functions
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_ToBeModified PCMCIA api to be modified
  *   \ingroup   PCMCIA

  *! \defgroup PCMCIA_ToBeRemove PCMCIA api to be removed
  *   \ingroup   PCMCIA
  */
//-------------------------------------------------

#ifndef _DRV_PCMCIA_H_
#define _DRV_PCMCIA_H_

#include "MsTypes.h"
#include "MsDevice.h"

#include "ULog.h"
//-----------------------------------------------------------------------------
//  Macro and Define
//-----------------------------------------------------------------------------
// PCMCIA
#define MAX_PCMCIA_CONFIGS              6       //!< The maximum number of configurations supported by a PCMCIA card
#define MAX_PCMCIA_STRLEN               (64)    //!< The maximum name of vendor/manufacturer/info strings
#define MAX_CIS_SIZE                    0x100   //!< The maximum size of a CIS, that is understood by this driver
#define PCMCIA_HW_TIMEOUT               3000
#define PCMCIA_IRQ_ENABLE               1

// IRQ_force status bits
#define PCMCIA_MEM_READY                (0x04)  //The same pin as IRQ in I/O mode

// Command Interface Hardware Registers
#define PCMCIA_PHYS_REG_DATA            (0)
#define PCMCIA_PHYS_REG_COMMANDSTATUS   (1)
#define PCMCIA_PHYS_REG_SIZELOW         (2)
#define PCMCIA_PHYS_REG_SIZEHIGH        (3)

// Status Register Bits
#define PCMCIA_STATUS_DATAAVAILABLE     (0x80)  //!< PCMCIA Status Register Bit - The module wants to send data
#define PCMCIA_STATUS_FREE              (0x40)  //!< PCMCIA Status Register Bit - The module can accept data
#define PCMCIA_STATUS_IIR               (0x10)  //!< PCMCIA Status Register Bit - IIR
#define PCMCIA_STATUS_RESERVEDBITS      (0x2C)  //!< PCMCIA Status Register Bits - reserved
#define PCMCIA_STATUS_WRITEERROR        (0x02)  //!< PCMCIA Status Register Bit - Write error
#define PCMCIA_STATUS_READERROR         (0x01)  //!< PCMCIA Status Register Bit - Read error

// Command Register Bits
#define PCMCIA_COMMAND_DAIE             (0x80)  //!< PCMCIA Command register bit - DAIE
                                                // when this bit is set, the module asserts IREQ# each time it has data to send
#define PCMCIA_COMMAND_FRIE             (0x40)  //!< PCMCIA Command register bit - FRIE
                                                // when this bit is set, the module asserts IREQ# each time it is free to receive data
#define PCMCIA_COMMAND_RESERVEDBITS     (0x30)  //!< PCMCIA Command register bits - reserved
#define PCMCIA_COMMAND_RESET            (0x08)  //!< PCMCIA Command register bit - Reset
#define PCMCIA_COMMAND_SIZEREAD         (0x04)  //!< PCMCIA Command register bit - Size read
#define PCMCIA_COMMAND_SIZEWRITE        (0x02)  //!< PCMCIA Command register bit - Size Write
#define PCMCIA_COMMAND_HOSTCONTROL      (0x01)  //!< PCMCIA Command register bit - Host control

#define PCMCIAINFO_MANID_VALID          (0x00000001)
#define PCMCIAINFO_VERS1_VALID          (0x00000002)
#define PCMCIAINFO_FUNCID_VALID         (0x00000004)

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
/// Version string.
#define MSIF_PCMCIA_LIB_CODE            {'P','C','M','_'}  //Lib code
#define MSIF_PCMCIA_LIBVER              {'0','1'}          //LIB version
#define MSIF_PCMCIA_BUILDNUM            {'0','1'}          //Build Number
#define MSIF_PCMCIA_CHANGELIST          {'0','0','2','5','8','4','9','3'} //P4 ChangeList Number

#define PCMCIA_DRV_VERSION  /* Character String for DRV/API version */  \
    MSIF_TAG,               /* 'MSIF' */                                \
    MSIF_CLASS,             /* '00'  */                                 \
    MSIF_CUS,               /* 0x0000 */                                \
    MSIF_MOD,               /* 0x0000 */                                \
    MSIF_CHIP,                                                          \
    MSIF_CPU,                                                           \
    MSIF_PCMCIA_LIB_CODE,   /* IP */                                    \
    MSIF_PCMCIA_LIBVER,     /* 00 ~ 99 */                               \
    MSIF_PCMCIA_BUILDNUM,   /* 00 ~ 99 */                               \
    MSIF_PCMCIA_CHANGELIST, /* CL# */                                   \
    MSIF_OS

typedef void (*IsrCallback)( void* wparam, void* lparam );

//-----------------------------------------------------------------------------
//  Enums
//-----------------------------------------------------------------------------
//! This enum contains the card types, that can be encoded in CISTPL_FUNCID (0x21)

typedef enum _PCMCIA_DbgLv
{
    E_PCMCIA_DBGLV_NONE,          //no debug message
    E_PCMCIA_DBGLV_ERR_ONLY,      //show error only
    E_PCMCIA_DBGLV_REG_DUMP,      //show error & reg dump
    E_PCMCIA_DBGLV_INFO,          //show error & informaiton
    E_PCMCIA_DBGLV_ALL            //show error, information & funciton name
}PCMCIA_DbgLv;

typedef enum {
    E_PCMCIA_FUNC_CUSTOM        = 0,
    E_PCMCIA_FUNC_MEMORY        = 1,
    E_PCMCIA_FUNC_SERIAL        = 2,
    E_PCMCIA_FUNC_PARALLEL      = 3,
    E_PCMCIA_FUNC_DRIVE         = 4,
    E_PCMCIA_FUNC_VIDEO         = 5,
    E_PCMCIA_FUNC_NETWORK       = 6,
    E_PCMCIA_FUNC_AIMS          = 7,
    E_PCMCIA_FUNC_SCSI          = 8,
    E_PCMCIA_FUNC_CARDBUS       = 9,
    E_PCMCIA_FUNC_MANUFACTURER  = 0xFF      // manifacturer designed purpose
} PCMCIA_FUNCTYPE;

typedef enum {
    E_PCMCIA_MODULE_A           = 0,
    E_PCMCIA_MODULE_B           = 1,
    E_PCMCIA_MODULE_MAX         = 2
} PCMCIA_MODULE;

typedef enum {
    E_PCMCIA_CD1_OR_CD2         = 0,
    E_PCMCIA_PCM1_CE_LOW        = 1,
    E_PCMCIA_PCM2_CE_LOW        = 2,
    E_PCMCIA_PCM1_RESET_IN      = 3,
    E_PCMCIA_PCM2_RESET_IN      = 4
} PCMCIA_OPTION;
//-----------------------------------------------------------------------------
//  Structures
//-----------------------------------------------------------------------------
//! This structure contains information about exactly one possible PCMCIA card configuration.
typedef struct /* 13 Bytes */
{
    MS_U32 dwEAAddr;            //!< EA-address
    MS_U32 dwEALen;             //!< size of the EA area (zero for none)
    MS_U16 wIRQData;            //!< supported IRQ number mask
    MS_U8  bConfigIndex;        //!< cor //!< The value of bConfigIndex has to be written into the card configuration register to activate this configuration.
    MS_U8  bIRQDesc1;           //!< Interrupt descriptor byte
    MS_U8  fCITagsPresent;      //!< Bitfield which is 0x03 if both required CI tags are present. 0x01 for DVB_HOST, 0x02 for DVB_CI_MODULE
} PCMCIA_CONFIG;

//! This structure provides simple access to the PCMCIA card information
//! after decoding of the Card Information Structure. This decoding is performed by MDrv_CI_PCMCIA_ReadAttribMem()
typedef struct
{
    MS_U32 ConfigOffset;                            //!< Offset of the Configuration byte in the Attribute Memory
    MS_U32 dwValidFlags;                            //!< Bitmask that defines which of the other fields are valid
    MS_U16 wManufacturerId;                         //!< 16Bit Manufacturer ID (PCMCIAINFO_MANID_VALID)
    MS_U16 wCardID;                                 //!< 16Bit Card ID (PCMCIAINFO_MANID_VALID)

    /* 99 Bytes */
    MS_U16 wPCMCIAStdRev;                           //!< PCMCIA Standard version supported by the card (PCMCIAINFO_VERS1_VALID)
    MS_U8 pszManufacturerName[MAX_PCMCIA_STRLEN];   //!< Name of the card manufacturer (PCMCIAINFO_VERS1_VALID)
    MS_U8 pszProductName[MAX_PCMCIA_STRLEN];        //!< Product name (PCMCIAINFO_VERS1_VALID)
    MS_U8 pszProductInfo1[MAX_PCMCIA_STRLEN];       //!< (PCMCIAINFO_VERS1_VALID)
    MS_U8 pszProductInfo2[MAX_PCMCIA_STRLEN];       //!< (PCMCIAINFO_VERS1_VALID)
    MS_U8 bCI_PLUS;                                  //!< PCMCIA card CI Plus Compatibility Identification
    
    PCMCIA_FUNCTYPE FuncType;                       //!< Card function type (PCMCIAINFO_FUNCID_VALID)
    MS_U8 bFuncIDSysInfo;                           //!< SysInitByte from the FuncID block (PCMCIAINFO_FUNCID_VALID)

    MS_U8 bINT;                                     //!< PCMCIA card Support interrupt or not
    MS_U8 bNumConfigs;
    /* 13 * MAX_PCMCIA_CONFIGS(=10) = 130 Bytes*/   //!< The number of configurations supported by the card. Exactly bNumConfigs entries are valid in the Config array.
    PCMCIA_CONFIG Config[MAX_PCMCIA_CONFIGS];       //!< The array of possible card configurations
} PCMCIA_INFO;

//! This structure provides ISR data for upper layer
//! DO NOT change the current data member
//! Only allow to add new member in the tail of the structure
typedef struct {
    MS_BOOL bISRCardInsert;
    MS_BOOL bISRCardRemove;    
    MS_BOOL bISRCardData;    
} PCMCIA_ISR;

//! This file typedefs PCMCIA_HANDLE as void*.
//! Actual PCMCIA driver implementations can hide whatever they want inside this handle.
//! Higher level drivers pass
typedef void *PCMCIA_HANDLE;

//-----------------------------------------------------------------------------
//  Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Global Functions
//-----------------------------------------------------------------------------
// PCMCIA

// revision history
// V2
//    for multiple PCMCIA slot support
//

//-----------------
// PCMCIA_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Initialize PCMCIA module.
/// @ingroup PCMCIA_General
/// @param  bCD_Reverse     \b IN: Set TRUE:High active, FALSE: Low active
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Init(MS_BOOL bCD_Reverse);
//-------------------------------------------------------------------------------------------------
/// Get PCMCIA initial parameter.
/// @ingroup PCMCIA_General
/// @param  pointer to bCD_Reverse     \b IN: Set TRUE:High active, FALSE: Low active
/// @return TRUE  : Succeed 
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_GetConfig(MS_BOOL* bCD_Reverse);
//-------------------------------------------------------------------------------------------------
/// Get/Set PCMCIA configuration
/// @ingroup PCMCIA_General
/// @param  eOpt             \b IN: Configuration type
/// @param  pInput(option)   \b IN: input type when bSet = TRUE
/// @param  pOutput(option)  \b IN/OUT: curren type result when bSet = FALSE
/// @param  bSet             \b IN: TRUE=set, FALSE=get
/// @return TRUE  : Succeed 
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_OptConfig(PCMCIA_MODULE eModule, PCMCIA_OPTION eOpt, MS_U32* pInput, MS_U32* pOutput, MS_BOOL bSet);

//-----------------
// PCMCIA_STR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Control PCMCIA STR functions.
/// @ingroup PCMCIA_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PCMCIA_SetPowerState(EN_POWER_MODE u16PowerState);
//-----------------
// PCMCIA_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check CI CAM is detect or not. (Still call MDrv_PCMCIA_DetectV2)
/// @ingroup PCMCIA_ToBeRemove
/// @param  eModule       \b IN: CAM module.
/// @return TRUE  : Succeed - CAM detect
/// @return FALSE : Fail - CAM not detect
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_PollingV2( PCMCIA_MODULE eModule );
//-----------------
// PCMCIA_Task
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check CI CAM is detect or not.
/// @ingroup PCMCIA_Task
/// @param  eModule       \b IN: CAM module.
/// @return TRUE  : Succeed - CAM detect
/// @return FALSE : Fail - CAM not detect
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_DetectV2( PCMCIA_MODULE eModule );
//-------------------------------------------------------------------------------------------------
/// Set reset HW duration.
/// @ingroup PCMCIA_Task
/// @param  u8HW_ResetDuration       \b IN: Duration.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Set_HW_ResetDuration( MS_U8 u8HW_ResetDuration );
//-------------------------------------------------------------------------------------------------
/// Reset PCMCIA HW
/// @ingroup PCMCIA_Task
/// @param  eModule       \b IN: CAM module.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_ResetHW_V2( PCMCIA_MODULE eModule );
//-----------------
// PCMCIA_AttributeMem
//-----------------
//-------------------------------------------------------------------------------------------------
/// Copy data from attribute memory.
/// @ingroup PCMCIA_AttributeMem
/// @param  eModule       \b IN: CAM module.
/// @param  u16Addr       \b IN: Read address.
/// @param  pDest           \b OUT: Copyed buffer.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest );
//-------------------------------------------------------------------------------------------------
/// Parse attribute memory data into an easily readable PCMCIA_INFO structure.
/// @ingroup PCMCIA_AttributeMem
/// @param  pAttribMem   \b IN: Attribute memory buffer.
/// @param  dwLen          \b IN: Data length.
/// @param  pInfo           \b OUT: PCMCIA information.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_ParseAttribMem( MS_U8 *pAttribMem, MS_U16 dwLen, PCMCIA_INFO *pInfo );
//-------------------------------------------------------------------------------------------------
/// Write data to attribute memory.
/// @ingroup PCMCIA_AttributeMem
/// @param  eModule       \b IN: CAM module.
/// @param  wAddr          \b IN: Write address.
/// @param  bData          \b IN: Write data.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 wAddr, MS_U8 bData );
//-------------------------------------------------------------------------------------------------
/// Write data to IO memory. (Only write 1 byte)
/// @ingroup PCMCIA_IOMem
/// @param  eModule       \b IN: CAM module.
/// @param  wAddr         \b IN: Write address.
/// @param  bData          \b IN: Write data.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 wAddr, MS_U8 bData );
//-------------------------------------------------------------------------------------------------
/// Write data to IO memory. (Write long size data to the IO memory)
/// @ingroup PCMCIA_ToBeModified
/// @param  eModule              \b IN: CAM module.
/// @param  u16Addr              \b IN: Write address.
/// @param  u8Value               \b IN: Write data. (Not used? It's use for 1 byte write)
/// @param  u16DataLen          \b IN: Write length.
/// @param  u8pWriteBuffer     \b IN: Write buffer.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer);
//-----------------
// PCMCIA_IOMem
//-----------------
//-------------------------------------------------------------------------------------------------
/// Read data from IO memory. (Only read 1 byte)
/// @ingroup PCMCIA_IOMem
/// @param  eModule       \b IN: CAM module.
/// @param  wAddr          \b IN: Read address.
/// @return: The data from PCMCIA IO memory.
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 wAddr );
//-------------------------------------------------------------------------------------------------
/// Check CI CAM still plug or not
/// @ingroup PCMCIA_ToBeRemove
/// @param  eModule       \b IN: CAM module.
/// @return TRUE  : Still plug
/// @return FALSE : Already removed
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_IsModuleStillPluggedV2( PCMCIA_MODULE eModule );
//-------------------------------------------------------------------------------------------------
/// Set PCMCIA detect trigger is high active or low active
/// @ingroup PCMCIA_Task
/// @param  bActiveHigh     \b IN: Set TRUE:High active, FALSE: Low active
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Set_Detect_Trigger( MS_BOOL bActiveHigh );
//-------------------------------------------------------------------------------------------------
/// Set PCMCIA detect function can work or not
/// @ingroup PCMCIA_Task
/// @param  bEnable     \b IN: Enable:enable detect function, Disable: disable detect function
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Set_Detect_Enable( MS_BOOL bEnable );
//-------------------------------------------------------------------------------------------------
/// Get CAM detect interval time.
/// @ingroup PCMCIA_Task
/// @param  eModule       \b IN: CAM module.
/// @return: The CAM detect interval time
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PCMCIA_Get_CD_IntervalV2( PCMCIA_MODULE eModule );
//--------------------------------------------------------------------------------------------------
/// Get PCMCIA driver library information
/// @ingroup PCMCIA_General
/// @param ppVersion      \b OUT: PCMCIA library version infomation
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_GetLibVer( const MSIF_Version **ppVersion );
//-------------------------------------------------------------------------------------------------
/// Set PCMCIA command. (IO memory?)
/// @ingroup PCMCIA_Task
/// @param  eModule              \b IN: CAM module.
/// @param  u8CommandBit    \b IN: Command.
/// @param  bValue                \b IN:  TRUE: enable command bit. FALSE: disable command bit.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_SetCommandBitV2( PCMCIA_MODULE eModule, MS_U8 u8CommandBit, MS_BOOL bValue );
//-------------------------------------------------------------------------------------------------
/// Reset PCMCIA interface. Send reset flag (RS).
/// @ingroup PCMCIA_Task
/// @param  eModule              \b IN: CAM module.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_ResetInterfaceV2( PCMCIA_MODULE eModule );
//-------------------------------------------------------------------------------------------------
/// Check data is avaliable or not. (IO memory?)
/// @ingroup PCMCIA_Task
/// @param  eModule              \b IN: CAM module.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_IsDataAvailableV2( PCMCIA_MODULE eModule );
//-------------------------------------------------------------------------------------------------
/// Read data from CAM
/// @ingroup PCMCIA_Task
/// @param  eModule                  \b IN: CAM module.
/// @param  u8pReadBuffer         \b OUT: Read buffer.
/// @param  u16ReadBufferSize   \b IN: Buffer size.
/// @return : Read data length.
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PCMCIA_ReadDataV2( PCMCIA_MODULE eModule, MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize );
//-------------------------------------------------------------------------------------------------
/// Write data to CAM
/// @ingroup PCMCIA_Task
/// @param  eModule                  \b IN: CAM module.
/// @param  u8pWriteBuffer         \b IN: Write buffer.
/// @param  u16DataLen             \b IN: Buffer size.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_WriteDataV2( PCMCIA_MODULE eModule, MS_U8* u8pWriteBuffer, MS_U16 u16DataLen );

//-------------------------------------------------------------------------------------------------
/// Switch to I/O mode. (Attribute memory !?)
/// @ingroup PCMCIA_Task
/// @param  eModule     \b IN: CAM module.
/// @param  pInfo         \b IN: PCMCIA info: offset and offset index.
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_SwitchToIOmodeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
//-----------------
// PCMCIA_ToBeModified
//-----------------
//-------------------------------------------------------------------------------------------------
/// Negotiate with PCMCIA biffer. Use size read (SR) to check.
/// @ingroup PCMCIA_Task
/// @param  eModule     \b IN: CAM module.
/// @param  pInfo         \b IN: PCMCIA info (Not used)
/// @return : Buffer size.
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PCMCIA_NegotiateBufferSizeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
//-------------------------------------------------------------------------------------------------
/// Write PCMCIA buffer size.
/// @ingroup PCMCIA_Task
/// @param  eModule           \b IN: CAM module.
/// @param  u16BufferSize  \b IN: Buffer size
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_WriteBufferSizeV2( PCMCIA_MODULE eModule, MS_U16 u16BufferSize );

//-------------------------------------------------------------------------------------------------
/// Check status bit
/// @ingroup PCMCIA_Task
/// @param  eModule           \b IN: CAM module.
/// @param  u8StatusBit      \b IN: Status bit which you want to check.
/// @return TRUE  : Status bit check success.
/// @return FALSE : Can't find the match status bit.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_WaitForStatusBitV2( PCMCIA_MODULE eModule, MS_U8 u8StatusBit );

//-----------------
// PCMCIA_INT
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check interrupt status.
/// @ingroup PCMCIA_INT
/// @return : The register value of interrupt.
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_PCMCIA_ReadyStatus( void );
//-------------------------------------------------------------------------------------------------
/// Finalize PCMCIA module
/// @ingroup PCMCIA_General
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Exit( void );

#if PCMCIA_IRQ_ENABLE
//-------------------------------------------------------------------------------------------------
/// Enable PCMCIA interrupt.
/// @ingroup PCMCIA_INT
/// @param  eModule     \b IN: CAM module.
/// @param  bEnable      \b IN: Enable/Disable
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Enable_InterruptV2( PCMCIA_MODULE eModule, MS_BOOL bEnable );

//-------------------------------------------------------------------------------------------------
/// Set PCMCIA IRQ Status.
/// @ingroup PCMCIA_INT
/// @param  eModule     \b IN: CAM module.
/// @param  Status        \b IN: TRUE/FALSE
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_Set_InterruptStatusV2( PCMCIA_MODULE eModule, MS_BOOL Status );

//-------------------------------------------------------------------------------------------------
/// Get PCMCIA IRQ Status.
/// @ingroup PCMCIA_INT
/// @param  eModule     \b IN: CAM module.
/// @return : PCMCIA IRQ Status (TRUE/FALSE)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PCMCIA_Get_InterruptStatusV2( PCMCIA_MODULE eModule );
//-------------------------------------------------------------------------------------------------
/// Set PCMCIA callback function.
/// @ingroup PCMCIA_ToBeRemove
/// @param  eModule            \b IN: CAM module.
/// @param  fnIsrCallback     \b IN: Callback function.
/// @return : PCMCIA IRQ Status (TRUE/FALSE)
//-------------------------------------------------------------------------------------------------
void MDrv_PCMCIA_InstarllIsrCallbackV2( PCMCIA_MODULE eModule, IsrCallback fnIsrCallback );
#endif


//-----------------------------------------------------------------------------
//  Function define for backward compatible
//-----------------------------------------------------------------------------
#define PCMCIA_DEFAULT_MODULE                   E_PCMCIA_MODULE_A
#if 0
    //#define MDrv_PCMCIA_Detect(p...)              MDrv_PCMCIA_DetectV2(E_PCMCIA_MODULE_A)
    #define MDrv_PCMCIA_Polling(p...)               MDrv_PCMCIA_PollingV2(PCMCIA_DEFAULT_MODULE)
    #define MDrv_PCMCIA_IsModuleStillPlugged(p...)  MDrv_PCMCIA_IsModuleStillPluggedV2(PCMCIA_DEFAULT_MODULE)
    #define MDrv_PCMCIA_SetCommandBit(p...)         MDrv_PCMCIA_SetCommandBitV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_ResetInterface(p...)        MDrv_PCMCIA_ResetInterfaceV2(PCMCIA_DEFAULT_MODULE)
    #define MDrv_PCMCIA_IsDataAvailable(p...)       MDrv_PCMCIA_IsDataAvailableV2(PCMCIA_DEFAULT_MODULE)
    #define MDrv_PCMCIA_ReadData(p...)              MDrv_PCMCIA_ReadDataV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_WriteData(p...)             MDrv_PCMCIA_WriteDataV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_SwitchToIOmode(p...)        MDrv_PCMCIA_SwitchToIOmodeV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_NegotiateBufferSize(p...)   MDrv_PCMCIA_NegotiateBufferSizeV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_WriteBufferSize(p...)       MDrv_PCMCIA_WriteBufferSizeV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_WaitForStatusBit(p...)      MDrv_PCMCIA_WaitForStatusBitV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_ResetHW(p...)               MDrv_PCMCIA_ResetHW_V2(PCMCIA_DEFAULT_MODULE)
    #define MDrv_PCMCIA_WriteAttribMem(p...)        MDrv_PCMCIA_WriteAttribMemV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_ReadAttribMem(p...)         MDrv_PCMCIA_ReadAttribMemV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_WriteIOMem(p...)            MDrv_PCMCIA_WriteIOMemV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_WriteIOMemLong(p...)        MDrv_PCMCIA_WriteIOMemLongV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_ReadIOMem(p...)             MDrv_PCMCIA_ReadIOMemV2(PCMCIA_DEFAULT_MODULE, p)
    #define MDrv_PCMCIA_Get_CD_Interval(p...)       MDrv_PCMCIA_Get_CD_IntervalV2(PCMCIA_DEFAULT_MODULE)

    #if PCMCIA_IRQ_ENABLE
        #define MDrv_PCMCIA_Enable_Interrupt(p...)      MDrv_PCMCIA_Enable_InterruptV2(PCMCIA_DEFAULT_MODULE, p)
        #define MDrv_PCMCIA_Set_InterruptStatus(p...)   MDrv_PCMCIA_Set_InterruptStatusV2(PCMCIA_DEFAULT_MODULE, p)
        #define MDrv_PCMCIA_Get_InterruptStatus(p...)   MDrv_PCMCIA_Get_InterruptStatusV2(PCMCIA_DEFAULT_MODULE)
        #define MDrv_PCMCIA_InstarllIsrCallback(p...)   MDrv_PCMCIA_InstarllIsrCallbackV2(PCMCIA_DEFAULT_MODULE, p)
    #endif    
#else
    //-------------------------------------------------------------------------------------------------
    /// Check CI CAM is detect or not. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return TRUE  : Succeed - CAM detect
    /// @return FALSE : Fail - CAM not detect
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_Polling( void );
    //-------------------------------------------------------------------------------------------------
    /// Check CI CAM still plug or not. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return TRUE  : Still plug
    /// @return FALSE : Already removed
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_IsModuleStillPlugged( void );

    //-------------------------------------------------------------------------------------------------
    /// Set PCMCIA command. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u8CommandBit    \b IN: Command.
    /// @param  bValue                \b IN:  TRUE: enable command bit. FALSE: disable command bit.
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_SetCommandBit( MS_U8 u8CommandBit, MS_BOOL bValue );

    //-------------------------------------------------------------------------------------------------
    /// Reset PCMCIA interface. Send reset flag (RS). (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return TRUE  : Succeed
    /// @return FALSE : Fail
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_ResetInterface( void );

    //-------------------------------------------------------------------------------------------------
    /// Check data is avaliable or not. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return TRUE  : Succeed
    /// @return FALSE : Fail
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_IsDataAvailable( void );

    //-------------------------------------------------------------------------------------------------
    /// Read data from CAM. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u8pReadBuffer         \b OUT: Read buffer.
    /// @param  u16ReadBufferSize   \b IN: Buffer size.
    /// @return : Read data length.
    //-------------------------------------------------------------------------------------------------
    MS_U16 MDrv_PCMCIA_ReadData( MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize );

    //-------------------------------------------------------------------------------------------------
    /// Write data to CAM. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u8pWriteBuffer         \b IN: Write buffer.
    /// @param  u16DataLen             \b IN: Buffer size.
    /// @return TRUE  : Succeed
    /// @return FALSE : Fail
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_WriteData( MS_U8* u8pWriteBuffer, MS_U16 u16DataLen );

    //-------------------------------------------------------------------------------------------------
    /// Switch to I/O mode. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  pInfo         \b IN: PCMCIA info: offset and offset index.
    /// @return TRUE  : Succeed
    /// @return FALSE : Fail
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_SwitchToIOmode( PCMCIA_INFO *pInfo );

    //-------------------------------------------------------------------------------------------------
    /// Negotiate with PCMCIA biffer. Use size read (SR) to check. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  pInfo         \b IN: PCMCIA info (Not used)
    /// @return : Buffer size.
    //-------------------------------------------------------------------------------------------------
    MS_U16 MDrv_PCMCIA_NegotiateBufferSize( PCMCIA_INFO *pInfo );

    //-------------------------------------------------------------------------------------------------
    /// Write PCMCIA buffer size. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u16BufferSize  \b IN: Buffer size
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_WriteBufferSize( MS_U16 u16BufferSize );

    //-------------------------------------------------------------------------------------------------
    /// Check status bit. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u8StatusBit      \b IN: Status bit which you want to check.
    /// @return TRUE  : Status bit check success.
    /// @return FALSE : Can't find the match status bit.
    //-------------------------------------------------------------------------------------------------
    MS_BOOL MDrv_PCMCIA_WaitForStatusBit( MS_U8 u8StatusBit );

    //-------------------------------------------------------------------------------------------------
    /// Reset PCMCIA HW. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_ResetHW( void );

    //-------------------------------------------------------------------------------------------------
    /// Write data to attribute memory. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  wAddr          \b IN: Write address.
    /// @param  bData          \b IN: Write data.
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_WriteAttribMem( MS_U16 wAddr, MS_U8 bData );

    //-------------------------------------------------------------------------------------------------
    /// Copy data from attribute memory. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u16Addr       \b IN: Read address.
    /// @param  pDest           \b OUT: Copyed buffer.
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_ReadAttribMem( MS_U16 u16Addr, MS_U8 *pDest );

    //-------------------------------------------------------------------------------------------------
    /// Write data to IO memory. (Only write 1 byte) (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  wAddr         \b IN: Write address.
    /// @param  bData          \b IN: Write data.
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_WriteIOMem( MS_U16 wAddr, MS_U8 bData );

    //-------------------------------------------------------------------------------------------------
    /// Write data to IO memory. (Write long size data to the IO memory) (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  u16Addr              \b IN: Write address.
    /// @param  u8Value               \b IN: Write data. (Not used? It's use for 1 byte write)
    /// @param  u16DataLen          \b IN: Write length.
    /// @param  u8pWriteBuffer     \b IN: Write buffer.
    /// @return None
    //-------------------------------------------------------------------------------------------------
    void MDrv_PCMCIA_WriteIOMemLong( MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer);

    //-------------------------------------------------------------------------------------------------
    /// Read data from IO memory. (Only read 1 byte) (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  wAddr          \b IN: Read address.
    /// @return: The data from PCMCIA IO memory.
    //-------------------------------------------------------------------------------------------------
    MS_U8 MDrv_PCMCIA_ReadIOMem( MS_U16 wAddr );

    //-------------------------------------------------------------------------------------------------
    /// Get CAM detect interval time. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return: The CAM detect interval time
    //-------------------------------------------------------------------------------------------------
    MS_U32 MDrv_PCMCIA_Get_CD_Interval( void );

    #if PCMCIA_IRQ_ENABLE
    //-------------------------------------------------------------------------------------------------
    /// Enable PCMCIA interrupt. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  bEnable      \b IN: Enable/Disable
    /// @return None
    //-------------------------------------------------------------------------------------------------
        void MDrv_PCMCIA_Enable_Interrupt( MS_BOOL bEnable );

    //-------------------------------------------------------------------------------------------------
    /// Set PCMCIA IRQ Status. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  Status        \b IN: TRUE/FALSE
    /// @return None
    //-------------------------------------------------------------------------------------------------
        void MDrv_PCMCIA_Set_InterruptStatus( MS_BOOL Status );

    //-------------------------------------------------------------------------------------------------
    /// Get PCMCIA IRQ Status. (use V2 version: for CAM module select)
    /// @ingroup PCMCIA_ToBeRemove
    /// @return : PCMCIA IRQ Status (TRUE/FALSE)
    //-------------------------------------------------------------------------------------------------
        MS_BOOL MDrv_PCMCIA_Get_InterruptStatus( void );

    //-------------------------------------------------------------------------------------------------
    /// Set PCMCIA callback function.
    /// @ingroup PCMCIA_ToBeRemove
    /// @param  fnIsrCallback     \b IN: Callback function.
    /// @return : PCMCIA IRQ Status (TRUE/FALSE)
    //-------------------------------------------------------------------------------------------------
        void MDrv_PCMCIA_InstarllIsrCallback( IsrCallback fnIsrCallback );
    #endif
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_PCMCIA_H_
