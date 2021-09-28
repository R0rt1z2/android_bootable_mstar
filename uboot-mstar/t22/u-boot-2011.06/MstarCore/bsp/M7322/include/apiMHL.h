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

/*
*/

/*! \defgroup MISC MISC modules
 * \defgroup MHL MHL interface (apiMHL.h)
 *  \ingroup MISC
    <b>Operation Flow</b>
    -# Check whether MHL cable is plugged or not in Rx side
    -# Disable R-term and output Vbus
    -# Check whether Cbus is connected(discovery pulse completed)
    -# Sink -> Source: "SET_HPD"
    -# Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    -# Sink -> Source: Write_Stat - "PATH_EN"


 * \defgroup MHL_BASIC MHL Module basic
 *  \ingroup MHL
 * \defgroup MHL_CAPABILITY MHL capability setting
 *  \ingroup MHL
 * \defgroup MHL_CONFIG MHL pad config
 *  \ingroup MHL
 * \defgroup MHL_STATUS MHL status flag
 *  \ingroup MHL
 * \defgroup MHL_SEND MHL Tx send function
 *  \ingroup MHL
 * \defgroup MHL_RECEIVE MHL Rx recevice data
 *  \ingroup MHL
 * \defgroup MHL_CALLBACK MHL call back function
 *  \ingroup MHL
 * \defgroup MHL_REMOVE MHL want to remove function
 *  \ingroup MHL
*/

#ifndef _API_MHL_H_
#define _API_MHL_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////
//                MHL SW Version                //
//////////////////////////////////////////////////
#define P4_CHANGELIST       "00522289"
#define FW_VER_MAJOR        "04"
#define FW_VER_MINOR        "18"

typedef MS_U8 (*link_cbus_event_handle_func)(MS_U8);

typedef struct
{
    MS_U8 ucSupportPath;
    MS_U8 *ucEDIDTable;
    MS_U8 *ucDevcapTable;
} stMHL_INITIAL_CONFIG;

typedef struct
{
    MS_U8 ucMHLSupportPath;
    MS_U8 ucDeviceCapabilityTable[20];
    MS_U32 ulReturnValue;
} stMHL_INITIAL_CONFIG_INFO;

typedef enum
{
    E_MHL_POWER_ON,
    E_MHL_POWER_STANDBY,
    E_MHL_POWER_DOWN
} E_MHL_POWERCTRL_Mode;

enum
{
    RCP_KEYID_SELECT = 0x00,
    RCP_KEYID_UP = 0x01,
    RCP_KEYID_DOWN = 0x02,
    RCP_KEYID_LEFT = 0x03,
    RCP_KEYID_RIGHT = 0x04,
    RCP_KEYID_RIGHT_UP = 0x05,
    RCP_KEYID_RIGHT_DOWN = 0x06,
    RCP_KEYID_LEFT_UP = 0x07,
    RCP_KEYID_LEFT_DOWN = 0x08,
    RCP_KEYID_ROOT_MENU = 0x09,
    RCP_KEYID_SETUP_MENU = 0x0A,
    RCP_KEYID_CONTENTS_MENU = 0x0B,
    RCP_KEYID_FAVORITE_MENU = 0x0C,
    RCP_KEYID_EXIT = 0x0D,
    //0x0E - 0x1F are reserved
    RCP_KEYID_NUM_0 = 0x20,
    RCP_KEYID_NUM_1 = 0x21,
    RCP_KEYID_NUM_2 = 0x22,
    RCP_KEYID_NUM_3 = 0x23,
    RCP_KEYID_NUM_4 = 0x24,
    RCP_KEYID_NUM_5 = 0x25,
    RCP_KEYID_NUM_6 = 0x26,
    RCP_KEYID_NUM_7 = 0x27,
    RCP_KEYID_NUM_8 = 0x28,
    RCP_KEYID_NUM_9 = 0x29,
    RCP_KEYID_DOT = 0x2A,
    RCP_KEYID_ENTER = 0x2B,
    RCP_KEYID_CLEAR = 0x2C,
    //0x2D - 0x2F are reserved
    RCP_KEYID_CH_UP = 0x30,
    RCP_KEYID_CH_DOWN = 0x31,
    RCP_KEYID_PRE_CH = 0x32,
    RCP_KEYID_SOUND_SELECT = 0x33,
    RCP_KEYID_INPUT_SELECT = 0x34,
    RCP_KEYID_SHOW_INFO = 0x35,
    RCP_KEYID_HELP = 0x36,
    RCP_KEYID_PAGE_UP = 0x37,
    RCP_KEYID_PAGE_DOWN = 0x38,
    //0x39 - 0x40 are reserved
    RCP_KEYID_VOL_UP = 0x41,
    RCP_KEYID_VOL_DOWN = 0x42,
    RCP_KEYID_MUTE = 0x43,
    RCP_KEYID_PLAY = 0x44,
    RCP_KEYID_STOP = 0x45,
    RCP_KEYID_PAUSE = 0x46,
    RCP_KEYID_RECORD = 0x47,
    RCP_KEYID_REWIND = 0x48,
    RCP_KEYID_FAST_FWD = 0x49,
    RCP_KEYID_EJECT  = 0x4A,
    RCP_KEYID_FORWARD = 0x4B,
    RCP_KEYID_BACKWARD = 0x4C,
    //0x4D - 0x4F are reserved
    RCP_KEYID_ANGLE = 0x50,
    RCP_KEYID_SUBPICTURE = 0x51,
    //0x52 - 0x5F are reserved
    RCP_KEYID_PLAY_FUNC = 0x60,
    RCP_KEYID_PAUSE_PLAY_FUNC = 0x61,
    RCP_KEYID_RECORD_FUNC = 0x62,
    RCP_KEYID_PAUSE_REC_FUNC = 0x63,
    RCP_KEYID_STOP_FUNC = 0x64,
    RCP_KEYID_MUTE_FUNC = 0x65,
    RCP_KEYID_UN_MUTE_FUNC = 0x66,
    RCP_KEYID_TUNE_FUNC = 0x67,
    RCP_KEYID_MEDIA_FUNC = 0x68,
    //0x69 - 0x70 are reserved
    RCP_KEYID_F1 = 0x71,
    RCP_KEYID_F2 = 0x72,
    RCP_KEYID_F3 = 0x73,
    RCP_KEYID_F4 = 0x74,
    RCP_KEYID_F5 = 0x75,
    //0x76 - 0x7D are reserved
    RCP_KEYID_VENDOR_SPECIFIC = 0x7E,
    RCP_KEYID_RESERVED = 0x7F,
    RCP_KEYID_SELECT_RELEASE = 0x80,
}; // MscRcpKcode_T;

typedef enum
{
    MSC_RAP_POLL = 0x00,
    MSC_RAP_CONTENT_ON = 0x10,
    MSC_RAP_CONTENT_OFF = 0x11,
} MscRapActionCode_T;

enum
{
    MSC_RCP_NO_ERROR = 0x00,     // RCP No Error
    MSC_RCP_INEFFECTIVE_KEY_CODE = 0x01,     // The key code in the RCP sub-command is not recognized
    MSC_RCP_RESPONDER_BUSY = 0x02,     // RCP Response busy
}; //MscRcpStatusCode_T;

enum
{
    MSC_RAP_NO_ERROR = 0x00,     // RAP No Error
    MSC_RAP_UNRECOGNIZED_ACTION_CODE = 0x01,
    MSC_RAP_UNSUPPORTED_ACTION_CODE = 0x02,
    MSC_RAP_RESPONDER_BUSY = 0x03,
}; //MscRapStatusCode_T;

enum MHL_CBUS_CONTROL_TYPE
{
    MHL_FORCE_HDMI_BY_PASS = 0,
    MHL_FORCE_24BIT_MODE,
    MHL_FORCE_PACKET_PIXEL_MODE,
    MHL_CBUS_ISOLATION_ON,
    MHL_CBUS_ISOLATION_OFF,
    MHL_CBUS_FLOATING_ON,
    MHL_CBUS_FLOATING_OFF,
    MHL_CBUS_RTERM_OFF,
    MHL_CBUS_FORCE_CLEAR_HPD,
    MHL_CBUS_FORCE_RESET_HPD,
    MHL_VBUS_HW_DETCET,
    MHL_VBUS_CHARGE_ON,
    MHL_VBUS_CHARGE_OFF,
    MHL_CBUS_FORCE_READY_DEVCAP,
    MHL_CBUS_FORCE_RECEIVE_3D_REQ,
    MHL_CBUS_FORCE_SEND_COMMAND_ENABLE,
    MHL_CBUS_FORCE_SEND_COMMAND_DISABLE,
    MHL_CBUS_CHECK_CONTEND_ON_ENABLE,
    MHL_CBUS_CHECK_CONTEND_ON_DISABLE,
};

enum MHL_CBUS_STATUS_FLAG_TYPE
{
    MHL_CBUS_STATUS_FLAG_CABLE_DETECT = 0,
    MHL_CBUS_STATUS_FLAG_CBUS_CONNECT,
    MHL_CBUS_STATUS_FLAG_PATH_EN,
    MHL_CBUS_STATUS_FLAG_RECEIVE_PATH_EN,
    MHL_CBUS_STATUS_FLAG_RECEIVE_MUTED,
    MHL_CBUS_STATUS_FLAG_COMMUNICATE_BUSY,
};

enum MHL_CABLE_DETECT_USE_TYPE
{
    MHL_CABLE_DETECT_USE_PORTA = 0x01,
    MHL_CABLE_DETECT_USE_PORTB = 0x02,
    MHL_CABLE_DETECT_USE_PORTC = 0x04,
    MHL_CABLE_DETECT_USE_PORTD = 0x08,
};

typedef enum
{
    //MHL property
    MAPI_CMD_MHL_SUPPORT_PATH,
    MAPI_CMD_MHL_INITIAL,
    MAPI_CMD_MHL_HANDLER,
    MAPI_CMD_MHL_AUTO_SWITCH,
    MAPI_CMD_MHL_SET_POWER_STATE,
    MAPI_CMD_MHL_CBUS_CONTROL,
    MAPI_CMD_MHL_LOAD_EDID,
    MAPI_CMD_MHL_READ_EDID,
    MAPI_CMD_MHL_LOAD_DEVCAP,
    MAPI_CMD_MHL_SET_VENDER_ID,
    MAPI_CMD_MHL_SET_CABLE_DETECT_INVERT,
    MAPI_CMD_MHL_VBUS_CONFIG_SETTING,
    MAPI_CMD_MHL_ADJUST_I_CONTROL,
    MAPI_CMD_MHL_ADJUST_IMPEDANCE,
    MAPI_CMD_MHL_CABLE_DETECT_FLAG,
    MAPI_CMD_MHL_CBUS_CONNECT_FLAG,
    MAPI_CMD_MHL_CBUS_WAKEUP_INT_FLAG,
    MAPI_CMD_MHL_SOURCE_SUPPORT_RCP,
    MAPI_CMD_MHL_SOURCE_SUPPORT_RAP,
    MAPI_CMD_MHL_GET_STATUS_FLAG,
    MAPI_CMD_MHL_SEND_RAP_COMMAND,
    MAPI_CMD_MHL_SEND_RCP_COMMAND,
    MAPI_CMD_MHL_SEND_UCP_COMMAND,
    MAPI_CMD_MHL_SEND_WRITE_BURST,
    MAPI_CMD_MHL_SEND_USER_WRITE_BURST,
    MAPI_CMD_MHL_GET_DEVICE_CAPABILITY,
    MAPI_CMD_MHL_GET_EXTEND_DEVICE_CAPABILITY,
    MAPI_CMD_MHL_GET_VENDER_ID,
    MAPI_CMD_MHL_GET_WRITE_BURST_DATA,
    MAPI_CMD_MHL_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_RCP_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_RAP_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_UCP_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_ATT_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_RBP_CALL_BACK_FUNCTION,
    MAPI_CMD_MHL_MSG_RESPONSE_PROCESS,
    MAPI_CMD_MHL_GET_CONFIGURATION,
}E_MHL_API_CMD_TYPE;

typedef struct
{
    MS_U8 ucSupportPath;
} stMHL_SET_SUPPORT_PATH, *pstMHL_SET_SUPPORT_PATH;

typedef struct DLL_PACKED
{
    MS_U8 *ucEDIDTable;
    MS_U8 *ucDevcapTable;
} stMHL_INITIAL_SETTING, *pstMHL_INITIAL_SETTING;

typedef struct
{
    MS_U16 usMSGKeyInfo;
} stMHL_POLLING_HANDLER, *pstMHL_POLLING_HANDLER;

typedef struct DLL_PACKED
{
    MS_BOOL bAutoSwitchFlag;
    MS_BOOL bResetFlag;
    MS_U8 *ucCbusPath;
} stMHL_AUTO_SWITCH_HANDLER, *pstMHL_AUTO_SWITCH_HANDLER;

typedef struct
{
    MS_U32 ulFailFlag;
    EN_POWER_MODE usPowerState;
} stMHL_SET_POWER_STATE, *pstMHL_SET_POWER_STATE;

typedef struct
{
    MS_U8 ucState;
} stMHL_CBUS_CONTROL, *pstMHL_CBUS_CONTROL;

typedef struct DLL_PACKED
{
    MS_U8 *ucEDIDTable;
} stMHL_LOAD_EDID, *pstMHL_LOAD_EDID;

typedef struct DLL_PACKED
{
    MS_U8 ucEDIDSize;
    MS_U8 *ucEDIDTable;
    MS_U16 u16EDIDSize;
} stMHL_READ_EDID, *pstMHL_READ_EDID;

typedef struct DLL_PACKED
{
    MS_U8 *ucDevcapTable;
} stMHL_LOAD_DEVCAP, *pstMHL_LOAD_DEVCAP;

typedef struct
{
    MS_U8 ucVenderID;
} stMHL_SET_VENDER_ID, *pstMHL_SET_VENDER_ID;

typedef struct
{
    MS_BOOL bCableDetectInvert;
} stMHL_SET_CABLE_DETECT_INVERT, *pstMHL_SET_CABLE_DETECT_INVERT;

typedef struct
{
    MS_U8 ucConfigMode;
} stMHL_VBUS_CONFIG_SETTING, *pstMHL_VBUS_CONFIG_SETTING;

typedef struct
{
    MS_U8 ucIControlValue;
} stMHL_ADJUST_I_CONTROL, *pstMHL_ADJUST_I_CONTROL;

typedef struct
{
    MS_U8 ucImpedanceValue;
} stMHL_ADJUST_IMPEDANCE, *pstMHL_ADJUST_IMPEDANCE;

typedef struct
{
    MS_BOOL bCableDetectFlag;
} stMHL_CABLE_DETECT_FLAG, *pstMHL_CABLE_DETECT_FLAG;

typedef struct
{
    MS_BOOL bCbusConnectFlag;
} stMHL_CBUS_CONNECT_FLAG, *pstMHL_CBUS_CONNECT_FLAG;

typedef struct
{
    MS_BOOL bCbusWakeupFlag;
} stMHL_CBUS_WAKEUP_INT_FLAG, *pstMHL_CBUS_WAKEUP_INT_FLAG;

typedef struct
{
    MS_BOOL bSourceSupportRCP;
} stMHL_SOURCE_SUPPORT_RCP, *pstMHL_SOURCE_SUPPORT_RCP;

typedef struct
{
    MS_BOOL bSourceSupportRAP;
} stMHL_SOURCE_SUPPORT_RAP, *pstMHL_SOURCE_SUPPORT_RAP;

typedef struct
{
    MS_BOOL bStatusFlag;
    MS_U8 ucStatusType;
} stMHL_GET_STATUS_FLAG, *pstMHL_GET_STATUS_FLAG;

typedef struct
{
    MS_BOOL bSendFlag;
    MS_U8 ucKeyCode;
} stMHL_SEND_RAP_COMMAND, *pstMHL_SEND_RAP_COMMAND;

typedef struct
{
    MS_BOOL bSendFlag;
    MS_U8 ucKeyCode;
} stMHL_SEND_RCP_COMMAND, *pstMHL_SEND_RCP_COMMAND;

typedef struct
{
    MS_BOOL bSendFlag;
    MS_U8 ucKeyCode;
} stMHL_SEND_UCP_COMMAND, *pstMHL_SEND_UCP_COMMAND;

typedef struct DLL_PACKED
{
    MS_BOOL bSendFlag;
    MS_U8 ucAddress;
    MS_U8 ucLength;
    MS_U8 *ucData;
} stMHL_SEND_WRITE_BURST, *pstMHL_SEND_WRITE_BURST;

typedef struct DLL_PACKED
{
    MS_BOOL bSendFlag;
    MS_U8 ucLength;
    MS_U8 *ucData;
} stMHL_SEND_USER_WRITE_BURST, *pstMHL_SEND_USER_WRITE_BURST;

typedef struct DLL_PACKED
{
    MS_BOOL bObtainFlag;
    MS_U16 usDevcapMask;
    MS_U8 *ucDeviceCapability;
} stMHL_GET_DEVICE_CAPABILITY, *pstMHL_GET_DEVICE_CAPABILITY;

typedef struct DLL_PACKED
{
    MS_BOOL bObtainFlag;
    MS_U16 usExtendDevcapMask;
    MS_U8 *ucExtendDeviceCapability;
} stMHL_GET_EXTEND_DEVICE_CAPABILITY, *pstMHL_GET_EXTEND_DEVICE_CAPABILITY;

typedef struct DLL_PACKED
{
    MS_BOOL bObtainFlag;
    MS_U8 *ucVenderID;
} stMHL_GET_VENDER_ID, *pstMHL_GET_VENDER_ID;

typedef struct DLL_PACKED
{
    MS_BOOL bObtainFlag;
    MS_U8 *ucWriteBurstData;
} stMHL_GET_WRITE_BURST_DATA, *pstMHL_GET_WRITE_BURST_DATA;

typedef struct
{
    MS_BOOL bRCPfunctionFlag;
    MS_BOOL bRAPfunctionFlag;
} stMHL_CALL_BACK_FUNCTION, *pstMHL_CALL_BACK_FUNCTION;

typedef struct
{
    MS_BOOL bRCPfunctionFlag;
} stMHL_RCP_CALL_BACK_FUNCTION, *pstMHL_RCP_CALL_BACK_FUNCTION;

typedef struct
{
    MS_BOOL bRAPfunctionFlag;
} stMHL_RAP_CALL_BACK_FUNCTION, *pstMHL_RAP_CALL_BACK_FUNCTION;

typedef struct
{
    MS_BOOL bUCPfunctionFlag;
} stMHL_UCP_CALL_BACK_FUNCTION, *pstMHL_UCP_CALL_BACK_FUNCTION;

typedef struct
{
    MS_BOOL bATTfunctionFlag;
} stMHL_ATT_CALL_BACK_FUNCTION, *pstMHL_ATT_CALL_BACK_FUNCTION;

typedef struct
{
    MS_BOOL bRBPfunctionFlag;
} stMHL_RBP_CALL_BACK_FUNCTION, *pstMHL_RBP_CALL_BACK_FUNCTION;

typedef struct
{
    MS_U8 ucMSGStatus;
} stMHL_MSG_RESPONSE_PROCESS, *pstMHL_MSG_RESPONSE_PROCESS;

typedef struct
{
    stMHL_INITIAL_CONFIG_INFO stInitialConfigInfo;
} stMHL_GET_CONFIGURATION, *pstMHL_GET_CONFIGURATION;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Assign which HDMI port support MHL
/// @ingroup MHL_BASIC
/// @param  ucSelect          \b IN: MHL support port
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_MHLSupportPath(MS_U8 ucSelect);

//-------------------------------------------------------------------------------------------------
/// Initialize MHL
/// @ingroup MHL_BASIC
/// @param  edid                \b IN: point to MHL EDID
/// @param  DevCap          \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_init(MS_U8 *edid, MS_U8 *DevCap);

//-------------------------------------------------------------------------------------------------
/// MHL polling function
/// @ingroup MHL_BASIC
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_handler(void);

//-------------------------------------------------------------------------------------------------
/// MHL auto switch handler function
/// @ingroup MHL_BASIC
/// @param  bReset              \b IN: Reset auto switch
/// @param  ucCbusPath       \b IN: Which port support auto switch
/// @return TRUE : MHL auto switch happen
/// @return FALSE : MHL auto switch not happen
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_AutoSwitchHandler(MS_BOOL bReset, MS_U8 *ucCbusPath);

//-------------------------------------------------------------------------------------------------
/// MHL set power state function
/// @ingroup MHL_BASIC
/// @param  u16PowerState  \b IN: Power state
/// @return TRUE : MHL set power state pass
/// @return FALSE : MHL set power state fail
//-------------------------------------------------------------------------------------------------
MS_U32 mapi_mhl_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
/// MHL Cbus control function
/// @ingroup MHL_BASIC
/// @param  ucState         \b IN: Cbus control state
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_CbusControl(MS_U8 ucState);

//-------------------------------------------------------------------------------------------------
/// MHL load EDID
/// @ingroup MHL_CAPABILITY
/// @param  edid                \b IN: point to MHL EDID
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_LoadEDID(MS_U8 *edid);

//-------------------------------------------------------------------------------------------------
/// MHL read EDID
/// @ingroup MHL_CAPABILITY
/// @param  usSize             \b IN: Size of EDID want to read back
/// @param  edid                \b IN: point to MHL EDID
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid);

//-------------------------------------------------------------------------------------------------
/// MHL load device capability
/// @ingroup MHL_CAPABILITY
/// @param  DevCap          \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_LoadDeviceCapability(MS_U8 *devcap);

//-------------------------------------------------------------------------------------------------
/// MHL load vender ID
/// @ingroup MHL_CAPABILITY
/// @param  ucVenderID     \b IN: point to MHL capability
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_SetVenderID(MS_U8 ucVenderID);

//-------------------------------------------------------------------------------------------------
/// MHL set cable detect invert
/// @ingroup MHL_CONFIG
/// @param  bCableDetectInvert     \b IN: cable detect invert flag
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_InvertCableDetect(MS_BOOL bCableDetectInvert);

//-------------------------------------------------------------------------------------------------
/// MHL set Vbus config mode
/// @ingroup MHL_CONFIG
/// @param  ucState         \b IN: Vbus config mode
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_VbusConfigSetting(MS_U8 ucState);

//-------------------------------------------------------------------------------------------------
/// MHL I-control adjust setting
/// @ingroup MHL_CONFIG
/// @param  ucIControl    \b IN: I-control adjust value
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_AdjustSettingIControl(MS_U8 ucIControl);

//-------------------------------------------------------------------------------------------------
/// MHL impedance adjust setting
/// @ingroup MHL_CONFIG
/// @param  ucImpedance    \b IN: Impedance adjust value
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance);

//-------------------------------------------------------------------------------------------------
/// MHL get cable detect flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL cable plugged
/// @return FALSE : MHL cable unplugged
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CableDetect(void);

//-------------------------------------------------------------------------------------------------
/// MHL get Cbus connection flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL Cbus connection
/// @return FALSE : MHL Cbus disconnection
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CbusStatus(void);

//-------------------------------------------------------------------------------------------------
/// MHL get wake up pulse interrupt  flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL receive wake up pulse
/// @return FALSE : MHL not receive wake up pulse
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CbusWakeupIntFlag(void);

//-------------------------------------------------------------------------------------------------
/// MHL source support RCP flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL source support RCP
/// @return FALSE : MHL source not support RCP or unknow
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SrcRCPSupportFlag(void);

//-------------------------------------------------------------------------------------------------
/// MHL source support RAP flag
/// @ingroup MHL_STATUS
/// @return TRUE : MHL source support RAP
/// @return FALSE : MHL source nots support RAP or unknow
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SrcRAPSupportFlag(void);

//-------------------------------------------------------------------------------------------------
/// MHL get some flag
/// @ingroup MHL_STATUS
/// @param  ucState         \b IN: status type
/// @return TRUE : MHL status type happen
/// @return FALSE : MHL status type not happen
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_CbusGetStatusFlag(MS_U8 ucState);

//-------------------------------------------------------------------------------------------------
/// MHL send RAP command
/// @ingroup MHL_SEND
/// @param  databuf         \b IN: key code
/// @return TRUE : MHL send RAP success
/// @return FALSE : MHL send RAP fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SendRAPCmd(MS_U8 databuf);

//-------------------------------------------------------------------------------------------------
/// MHL send RCP command
/// @ingroup MHL_SEND
/// @param  ucKeyCode      \b IN: key code
/// @return TRUE : MHL send RCP success
/// @return FALSE : MHL send RCP fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SendRCPAutoReleaseCmd(MS_U8 ucKeyCode);

//-------------------------------------------------------------------------------------------------
/// MHL send UCP command
/// @ingroup MHL_SEND
/// @param  databuf         \b IN: key code
/// @return TRUE : MHL send UCP success
/// @return FALSE : MHL send UCP fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SendUCPCmd(MS_U8 databuf);

//-------------------------------------------------------------------------------------------------
/// MHL send write burst command directly
/// @ingroup MHL_SEND
/// @param  ucAddr          \b IN: Write burst address always 0x40
/// @param  ucLength       \b IN: Write burst data length
/// @param  buffer           \b IN: Write burst data content
/// @return TRUE : MHL send write burst success
/// @return FALSE : MHL send write burst fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_SendWriteBurst(MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *buffer);

//-------------------------------------------------------------------------------------------------
/// MHL send write burst command use write request / write grant
/// @ingroup MHL_SEND
/// @param  ucLength       \b IN: Write burst data length
/// @param  pBuffer          \b IN: Write burst data content
/// @return TRUE : MHL send write burst success
/// @return FALSE : MHL send write burst fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK mapi_mhl_CbusSendUserWriteBurst(MS_U8 ucLength, MS_U8 *pBuffer);

//-------------------------------------------------------------------------------------------------
/// MHL send write read device capability
/// @ingroup MHL_SEND
/// @param  usDevcapMask     \b IN: Which data want to read
/// @param  ucDevcap            \b IN: Read device capability buffer
/// @return TRUE : MHL read device capability success
/// @return FALSE : MHL read device capability fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_GetDeviceCapacibility(MS_U16 usDevcapMask, MS_U8 *ucDevcap);

//-------------------------------------------------------------------------------------------------
/// MHL send write read extend device capability
/// @ingroup MHL_SEND
/// @param  usExtendDevcapMask     \b IN: Which data want to read
/// @param  ucExtendDevcap            \b IN: Read extend device capability buffer
/// @return TRUE : MHL read extend device capability success
/// @return FALSE : MHL read extend device capability fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK mapi_mhl_GetExtendDeviceCapacibility(MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap);

//-------------------------------------------------------------------------------------------------
/// MHL send get vender ID
/// @ingroup MHL_SEND
/// @param  ucVenderID     \b IN: Vender buffer
/// @return TRUE : MHL get vender ID success
/// @return FALSE : MHL get vender ID fail or not ready
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_GetDeviceVenderID(MS_U8 *ucVenderID);

//-------------------------------------------------------------------------------------------------
/// MHL receive write burst data
/// @ingroup MHL_RECEIVE
/// @param  ucData      \b IN: Write burst data buffer
/// @return TRUE : MHL receive write burst data success
/// @return FALSE : MHL receive write burst data fail or not yeah
//-------------------------------------------------------------------------------------------------
MS_BOOL mapi_mhl_GetWriteBurstData(MS_U8 *ucData);

//-------------------------------------------------------------------------------------------------
/// MHL set RCP and RAP call back function
/// @ingroup MHL_CALLBACK
/// @param  rcp         \b IN: RCP function pointer
/// @param  rap         \b IN: RAP function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_RegisterCallBackFunctions(link_cbus_event_handle_func rcp, link_cbus_event_handle_func rap);

//-------------------------------------------------------------------------------------------------
/// MHL set RCP call back function
/// @ingroup MHL_CALLBACK
/// @param  rcp         \b IN: RCP function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_RegisterRcpCallBackFunction(link_cbus_event_handle_func rcp);

//-------------------------------------------------------------------------------------------------
/// MHL set RAP call back function
/// @ingroup MHL_CALLBACK
/// @param  rap         \b IN: RAP function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_RegisterRapCallBackFunction(link_cbus_event_handle_func rap);

//-------------------------------------------------------------------------------------------------
/// MHL set UCP call back function
/// @ingroup MHL_CALLBACK
/// @param  Ucp         \b IN: UCP function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void mapi_mhl_RegisterUcpCallBackFunction(link_cbus_event_handle_func Ucp);

//-------------------------------------------------------------------------------------------------
/// MHL set ATT call back function
/// @ingroup MHL_CALLBACK
/// @param  Att         \b IN: ATT function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void SYMBOL_WEAK mapi_mhl_RegisterAttCallBackFunction(link_cbus_event_handle_func Att);

//-------------------------------------------------------------------------------------------------
/// MHL set RBP call back function
/// @ingroup MHL_CALLBACK
/// @param  Rbp         \b IN: RBP function pointer
/// @return None
//-------------------------------------------------------------------------------------------------
void SYMBOL_WEAK mapi_mhl_RegisterRbpCallBackFunction(link_cbus_event_handle_func Rbp);

//-------------------------------------------------------------------------------------------------
/// Get MHL init parameter
/// @ingroup MHL_BASIC
/// @param  stInitialConfig: Init parameters
/// @return UTOPIA_STATUS_ERR_NOT_AVAIL: MHL not system init
/// @return UTOPIA_STATUS_SUCCESS: MHL system init ready
//-------------------------------------------------------------------------------------------------
MS_U32 SYMBOL_WEAK mapi_mhl_GetConfig(stMHL_INITIAL_CONFIG *stInitialConfig);

void mapi_mhl_Send3DInformation(void);
void mapi_mhl_CbusWakeupIntSetting(void);
void mapi_mhl_PowerCtrl(E_MHL_POWERCTRL_Mode pctrl);

#ifdef __cplusplus
}
#endif

#endif //_API_MHL_H_
