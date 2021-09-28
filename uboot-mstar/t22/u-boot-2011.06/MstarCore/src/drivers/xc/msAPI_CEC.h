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
/// file   msAPI_CEC.h
/// @brief  CEC Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_MSAPI_CEC_SOURCE_BEHAVIOR_ACTIVE   = 0x01,
    E_MSAPI_CEC_SOURCE_BEHAVIOR_INACTIVE = 0x02,
    E_MSAPI_CEC_SOURCE_BEHAVIOR_UNKNOW   = 0xff,
}E_MSAPI_CEC_SOURCE_BEHAVIOR;

typedef enum
{
//----- One Touch Play ----------------------------
    E_MSAPI_CEC_ACTIVE_SOURCE                         = 0x82,
    E_MSAPI_CEC_OTP_IMAGE_VIEW_ON                     = 0x04,
    E_MSAPI_CEC_OTP_TEXT_VIEW_ON                      = 0x0D,

//----- Routing Control ---------------------------
    //E_MSAPI_CEC_RC_ACTIVE_SOURCE                      = 0x82,
    E_MSAPI_CEC_RC_INACTIVE_SOURCE                    = 0x9D,

    E_MSAPI_CEC_RC_REQ_ACTIVE_SOURCE                  = 0x85, // should be removed
    E_MSAPI_CEC_RC_REQUEST_ACTIVE_SOURCE              = 0x85,

    E_MSAPI_CEC_RC_ROUTING_CHANGE                     = 0x80,

    E_MSAPI_CEC_RC_ROUTING_INFO                       = 0x81, // should be removed
    E_MSAPI_CEC_RC_ROUTING_INFORMATION                = 0x81,

    E_MSAPI_CEC_RC_SET_STREM_PATH                     = 0x86,

//----- Standby Command ---------------------------
    E_MSAPI_CEC_STANDBY                               = 0x36,

//----- One Touch Record---------------------------
    E_MSAPI_CEC_OTR_RECORD_OFF                        = 0x0B,
    E_MSAPI_CEC_OTR_RECORD_ON                         = 0x09,
    E_MSAPI_CEC_OTR_RECORD_STATUS                     = 0x0A,
    E_MSAPI_CEC_OTR_RECORD_TV_SCREEN                  = 0x0F,

//----- Timer programmer -------------------------- CEC1.3a
    E_MSAPI_CEC_TP_CLEAR_ANALOG_TIMER                 = 0x33, // should be removed
    E_MSAPI_CEC_TP_CLEAR_ANALOGUE_TIMER               = 0x33,

    E_MSAPI_CEC_TP_CLEAR_DIGITAL_TIMER                = 0x99,

    E_MSAPI_CEC_TP_CLEAR_EXT_TIMER                    = 0xA1, // should be removed
    E_MSAPI_CEC_TP_CLEAR_EXTERNAL_TIMER               = 0xA1,

    E_MSAPI_CEC_TP_SET_ANALOG_TIMER                   = 0x34, // should be removed
    E_MSAPI_CEC_TP_SET_ANALOGUE_TIMER                 = 0x34,

    E_MSAPI_CEC_TP_SET_DIGITAL_TIMER                  = 0x97,

    E_MSAPI_CEC_TP_SET_EXT_TIMER                      = 0xA2, // should be removed
    E_MSAPI_CEC_TP_SET_EXTERNAL_TIMER                 = 0xA2,

    E_MSAPI_CEC_TP_SET_TIMER_PROGRAM_TITLE            = 0x67,
    E_MSAPI_CEC_TP_TIMER_CLEARD_STATUS                = 0x43,
    E_MSAPI_CEC_TP_TIMER_STATUS                       = 0x35,

//----- System Information ------------------------
    E_MSAPI_CEC_SI_CEC_VERSION                        = 0x9E,       //1.3a
    E_MSAPI_CEC_SI_GET_CEC_VERSION                    = 0x9F,       //1.3a

    E_MSAPI_CEC_SI_REQUEST_PHY_ADDR                   = 0x83, // should be removed
    E_MSAPI_CEC_SI_GIVE_PHYSICAL_ADDRESS              = 0x83,

    E_MSAPI_CEC_SI_GET_MENU_LANGUAGE                  = 0x91,
    //E_MSAPI_CEC_SI_POLLING_MESSAGE                    = ?,

    E_MSAPI_CEC_SI_REPORT_PHY_ADDR                    = 0x84, // should be removed
    E_MSAPI_CEC_SI_REPORT_PHYSICAL_ADDRESS            = 0x84,

    E_MSAPI_CEC_SI_SET_MENU_LANGUAGE                  = 0x32,

    //E_MSAPI_CEC_SI_REC_TYPE_PRESET                    = 0x00,  //parameter   ?
    //E_MSAPI_CEC_SI_REC_TYPE_OWNSRC                    = 0x01,  //parameter   ?

//----- Deck Control Feature-----------------------
    E_MSAPI_CEC_DC_DECK_CTRL                          = 0x42, // should be removed
    E_MSAPI_CEC_DC_DECK_CONTROL                       = 0x42,

    E_MSAPI_CEC_DC_DECK_STATUS                        = 0x1B,
    E_MSAPI_CEC_DC_GIVE_DECK_STATUS                   = 0x1A,
    E_MSAPI_CEC_DC_PLAY                               = 0x41,

//----- Tuner Control ------------------------------
    E_MSAPI_CEC_TC_GIVE_TUNER_STATUS                  = 0x08, // should be removed
    E_MSAPI_CEC_TC_GIVE_TUNER_DEVICE_STATUS           = 0x08,

    E_MSAPI_CEC_TC_SEL_ANALOG_SERVICE                 = 0x92, // should be removed
    E_MSAPI_CEC_TC_SEL_ANALOGUE_SERVICE               = 0x92,

    E_MSAPI_CEC_TC_SEL_DIGITAL_SERVICE                = 0x93, // should be removed
    E_MSAPI_CEC_TC_SELECT_DIGITAL_SERVICE             = 0x93,

    E_MSAPI_CEC_TC_TUNER_DEVICE_STATUS                = 0x07,

    E_MSAPI_CEC_TC_TUNER_STEP_DEC                     = 0x06, // should be removed
    E_MSAPI_CEC_TC_TUNER_STEP_DECREMENT               = 0x06,

    E_MSAPI_CEC_TC_TUNER_STEP_INC                     = 0x05, // should be removed
    E_MSAPI_CEC_TC_TUNER_STEP_INCREMENT               = 0x05,

//---------Vendor Specific -------------------------
    //E_MSAPI_CEC_VS_CEC_VERSION                        = 0x9E,       //1.3a
    E_MSAPI_CEC_VS_DEVICE_VENDOR_ID                   = 0x87,
    //E_MSAPI_CEC_VS_GET_CEC_VERSION                    = 0x9F,       //1.3a

    E_MSAPI_CEC_VS_GIVE_VENDOR_ID                     = 0x8C, // should be removed
    E_MSAPI_CEC_VS_GIVE_DEVICE_VENDOR_ID              = 0x8C,

    E_MSAPI_CEC_VS_VENDOR_COMMAND                     = 0x89,
    E_MSAPI_CEC_VS_VENDOR_COMMAND_WITH_ID             = 0xA0,      //1.3a

    E_MSAPI_CEC_VS_VENDOR_RC_BUT_DOWN                 = 0x8A, // should be removed
    E_MSAPI_CEC_VS_VENDOR_REMOTE_BUTTON_DOWN          = 0x8A,

    E_MSAPI_CEC_VS_VENDOR_RC_BUT_UP                   = 0x8B, // should be removed
    E_MSAPI_CEC_VS_VENDOR_REMOTE_BUTTON_UP            = 0x8B,

//----- OSD Display --------------------------------
    E_MSAPI_CEC_SET_OSD_STRING                        = 0x64,

//----- Device OSD Name Transfer  -------------------------
    E_MSAPI_CEC_OSDNT_GIVE_OSD_NAME                   = 0x46,
    E_MSAPI_CEC_OSDNT_SET_OSD_NAME                    = 0x47,

//----- Device Menu Control ------------------------
    E_MSAPI_CEC_DMC_MENU_REQUEST                      = 0x8D,
    E_MSAPI_CEC_DMC_MENU_STATUS                       = 0x8E,
    //E_MSAPI_CEC_DMC_MENU_ACTIVATED                    = 0x00,   //parameter
    //E_MSAPI_CEC_DMC_MENU_DEACTIVATED                  = 0x01,   //parameter

    E_MSAPI_CEC_UI_PRESS                              = 0x44, // should be removed
    E_MSAPI_CEC_DMC_USER_CONTROL_PRESSED              = 0x44,

    E_MSAPI_CEC_UI_RELEASE                            = 0x45, // should be removed
    E_MSAPI_CEC_DMC_USER_CONTROL_RELEASED             = 0x45,

//----- Remote Control Passthrough ----------------
//----- UI Message --------------------------------
//    E_MSAPI_CEC_RCP_USER_CONTROL_PRESSED              = 0x44,
//    E_MSAPI_CEC_RCP_USER_CONTROL_RELEASED             = 0x45,

//----- Power Status  ------------------------------
    E_MSAPI_CEC_PS_GIVE_POWER_STATUS                  = 0x8F, // should be removed
    E_MSAPI_CEC_PS_GIVE_DEVICE_POWER_STATUS           = 0x8F,

    E_MSAPI_CEC_PS_REPORT_POWER_STATUS                = 0x90,

//----- General Protocal Message ------------------

//----- Feature Abort -----------------------------
    E_MSAPI_CEC_FEATURE_ABORT                         = 0x00,

//----- Abort Message -----------------------------
    E_MSAPI_CEC_ABORT_MESSAGE                         = 0xFF,

//----- System Audio Control ------------------
    E_MSAPI_CEC_SAC_GIVE_AUDIO_STATUS                 = 0x71,
    E_MSAPI_CEC_SAC_GIVE_SYSTEM_AUDIO_MODE_STATUS     = 0x7D,
    E_MSAPI_CEC_SAC_REPORT_AUDIO_STATUS               = 0x7A,

    E_MSAPI_CEC_SAC_REPORT_SHORT_AUDIO_DESCRIPTOR     = 0xA3,
    E_MSAPI_CEC_SAC_REQUEST_SHORT_AUDIO_DESCRIPTOR    = 0xA4,

    E_MSAPI_CEC_SAC_SET_SYSTEM_AUDIO_MODE             = 0x72,
    E_MSAPI_CEC_SAC_SYSTEM_AUDIO_MODE_REQUEST         = 0x70,
    E_MSAPI_CEC_SAC_SYSTEM_AUDIO_MODE_STATUS          = 0x7E,

//----- System Audio Control ------------------
    E_MSAPI_CEC_SAC_SET_AUDIO_RATE                    = 0x9A,

//----- Audio Return Channel  Control ------------------
    E_MSAPI_CEC_ARC_INITIATE_ARC                      = 0xC0,
    E_MSAPI_CEC_ARC_REPORT_ARC_INITIATED              = 0xC1,
    E_MSAPI_CEC_ARC_REPORT_ARC_TERMINATED             = 0xC2,

    E_MSAPI_CEC_ARC_REQUEST_ARC_INITATION             = 0xC3, // should be removed
    E_MSAPI_CEC_ARC_REQUEST_ARC_INITIATION            = 0xC3,

    E_MSAPI_CEC_ARC_REQUEST_ARC_TERMINATION           = 0xC4,

    E_MSAPI_CEC_ARC_TERMINATED_ARC                    = 0xC5, // should be removed
    E_MSAPI_CEC_ARC_TERMINATE_ARC                     = 0xC5,

//----- Capability Discovery and Control ------------------
    E_MSAPI_CEC_CDC_CDC_MESSAGE                       = 0xF8,

} E_MSAPI_CEC_OPCODE;

typedef enum
{
    E_MSAPI_CEC_UI_SELECT             = 0x00,
    E_MSAPI_CEC_UI_UP                 = 0x01,
    E_MSAPI_CEC_UI_DOWN               = 0x02,
    E_MSAPI_CEC_UI_LEFT               = 0x03,
    E_MSAPI_CEC_UI_RIGHT              = 0x04,
    E_MSAPI_CEC_UI_RIGHT_UP           = 0x05,
    E_MSAPI_CEC_UI_RIGHT_DOWN         = 0x06,
    E_MSAPI_CEC_UI_LEFT_UP            = 0x07,
    E_MSAPI_CEC_UI_LEFT_DOWN          = 0x08,
    E_MSAPI_CEC_UI_ROOTMENU           = 0x09,
    E_MSAPI_CEC_UI_SETUP_MENU         = 0x0A,
    E_MSAPI_CEC_UI_CONTENTS_MENU      = 0x0B,
    E_MSAPI_CEC_UI_FAVORITE_MENU      = 0x0C,
    E_MSAPI_CEC_UI_EXIT               = 0x0D,

// 0x0E ~ 0x1F  reserved

    E_MSAPI_CEC_UI_NUMBER_0           = 0x20,
    E_MSAPI_CEC_UI_NUMBER_1           = 0x21,
    E_MSAPI_CEC_UI_NUMBER_2           = 0x22,
    E_MSAPI_CEC_UI_NUMBER_3           = 0x23,
    E_MSAPI_CEC_UI_NUMBER_4           = 0x24,
    E_MSAPI_CEC_UI_NUMBER_5           = 0x25,
    E_MSAPI_CEC_UI_NUMBER_6           = 0x26,
    E_MSAPI_CEC_UI_NUMBER_7           = 0x27,
    E_MSAPI_CEC_UI_NUMBER_8           = 0x28,
    E_MSAPI_CEC_UI_NUMBER_9           = 0x29,

    E_MSAPI_CEC_UI_DOT                = 0x2A,
    E_MSAPI_CEC_UI_ENTER              = 0x2B,
    E_MSAPI_CEC_UI_CLEAR              = 0x2C,

// 0x2D ~ 0x2E reserved
    E_MSAPI_CEC_UI_NEXT_FAVORITE      = 0x2F,

    E_MSAPI_CEC_UI_CHANNEL_UP         = 0x30,
    E_MSAPI_CEC_UI_CHANNEL_DOWN       = 0x31,
    E_MSAPI_CEC_UI_PREVIOUS_CHANNEL   = 0x32,
    E_MSAPI_CEC_UI_SOUND_SELECT       = 0x33,
    E_MSAPI_CEC_UI_INPUT_SELECT       = 0x34,
    E_MSAPI_CEC_UI_DISPLAY_INFO       = 0x35,
    E_MSAPI_CEC_UI_HELP               = 0x36,
    E_MSAPI_CEC_UI_PAGE_UP            = 0x37,
    E_MSAPI_CEC_UI_PAGE_DOWN          = 0x38,

// 0x39 ~ 0x3F reserved

    E_MSAPI_CEC_UI_POWER              = 0x40,
    E_MSAPI_CEC_UI_VOLUME_UP          = 0x41,
    E_MSAPI_CEC_UI_VOLUME_DOWN        = 0x42,
    E_MSAPI_CEC_UI_MUTE               = 0x43,
    E_MSAPI_CEC_UI_PLAY               = 0x44,
    E_MSAPI_CEC_UI_STOP               = 0x45,
    E_MSAPI_CEC_UI_PAUSE              = 0x46,
    E_MSAPI_CEC_UI_RECORD             = 0x47,
    E_MSAPI_CEC_UI_REWIND             = 0x48,
    E_MSAPI_CEC_UI_FAST_FORWARD       = 0x49,
    E_MSAPI_CEC_UI_EJECT              = 0x4A,
    E_MSAPI_CEC_UI_FORWARD            = 0x4B,
    E_MSAPI_CEC_UI_BACKWARD           = 0x4C,
    E_MSAPI_CEC_UI_STOP_RECORD        = 0x4D,
    E_MSAPI_CEC_UI_PAUSE_RECORD       = 0x4E,

// 0x4F reserved

    E_MSAPI_CEC_UI_ANGLE                      = 0x50,
    E_MSAPI_CEC_UI_SUB_PICTURE                = 0x51,
    E_MSAPI_CEC_UI_VIDEO_ON_DEMAND            = 0x52,
    E_MSAPI_CEC_UI_ELECTRONIC_PROGRAM_GUIDE   = 0x53,
    E_MSAPI_CEC_UI_TIMER_PROGRAMMING          = 0x54,
    E_MSAPI_CEC_UI_INITIAL_CONFIGURATION      = 0x55,

// 0x56 ~ 0x5F reserved

  //0x60 ~ 0x6D, identified as function
    E_MSAPI_CEC_UI_PLAY_FUN               = 0x60,
    E_MSAPI_CEC_UI_PSUSE_PLAY_FUN         = 0x61,
    E_MSAPI_CEC_UI_RECORD_FUN             = 0x62,
    E_MSAPI_CEC_UI_PAUSE_RECORD_FUN       = 0x63,
    E_MSAPI_CEC_UI_STOP_FUN               = 0x64,
    E_MSAPI_CEC_UI_MUTE_FUN               = 0x65,
    E_MSAPI_CEC_UI_RESTORE_VOLUME_FUN     = 0x66,
    E_MSAPI_CEC_UI_TUNE_FUN               = 0x67,
    E_MSAPI_CEC_UI_SELECT_MEDIA_FUN       = 0x68,
    E_MSAPI_CEC_UI_SELECT_AV_INPUT_FUN    = 0x69,
    E_MSAPI_CEC_UI_SELECT_AUDIO_INPUT_FUN = 0x6A,
    E_MSAPI_CEC_UI_POWER_TOGGLE_FUN       = 0x6B,
    E_MSAPI_CEC_UI_POWER_OFF_FUN          = 0x6C,
    E_MSAPI_CEC_UI_POWER_ON_FUN           = 0x6D,

// 0x6E ~ 0x70 reserved

    E_MSAPI_CEC_UI_F1_BLUE            = 0x71,
    E_MSAPI_CEC_UI_F2_RED             = 0x72,
    E_MSAPI_CEC_UI_F3_GREEN           = 0x73,
    E_MSAPI_CEC_UI_F4_YELLOW          = 0x74,
    E_MSAPI_CEC_UI_F5                 = 0x75,
    E_MSAPI_CEC_UI_DATA               = 0x76,

// 0x77 ~ 0xFF reserved
} E_MSAPI_CEC_UI_COMMAND;

typedef enum
{
    E_MSAPI_CEC_MENU_REQ_ACTIVATED    = 0,  // menu request activated status
    E_MSAPI_CEC_MENU_REQ_DEACTIVATED  = 1,  // menu request deactivated status
    E_MSAPI_CEC_MENU_REQ_QUERY        = 2,  // menu request query status
} E_MSAPI_CEC_MENU_REQUEST_TYPE;

//Deck Control, Deck Control Mode 1 byte
typedef enum
{
    E_MSAPI_CEC_DCM_CMD_SKIP_FORWARD          = 1,  //parameter
    E_MSAPI_CEC_DCM_CMD_SKIP_REVERSE          = 2,  //parameter
    E_MSAPI_CEC_DCM_CMD_STOP                  = 3,  //parameter
    E_MSAPI_CEC_DCM_CMD_EJECT                 = 4,  //parameter
} E_MSAPI_CEC_DECK_CONTROL_MODE;

//Deck status, Deck info 1 byte
typedef enum
{
    E_MSAPI_CEC_DI_CMD_PLAY                   = 0x11,  //parameter
    E_MSAPI_CEC_DI_CMD_RECORD                 = 0x12,  //parameter
    E_MSAPI_CEC_DI_CMD_PLAY_REVERSE           = 0x13,  //parameter
    E_MSAPI_CEC_DI_CMD_STILL                  = 0x14,  //parameter
    E_MSAPI_CEC_DI_CMD_SLOW                   = 0x15,  //parameter
    E_MSAPI_CEC_DI_CMD_SLOW_REVERSE           = 0x16,  //parameter
    E_MSAPI_CEC_DI_CMD_FAST_FORWARD           = 0x17,  //parameter
    E_MSAPI_CEC_DI_CMD_FAST_REVERSE           = 0x18,  //parameter
    E_MSAPI_CEC_DI_CMD_NO_MEDIA               = 0x19,  //parameter
    E_MSAPI_CEC_DI_CMD_STOP                   = 0x1A,  //parameter
    E_MSAPI_CEC_DI_CMD_SKIP_FORWARD           = 0x1B,  //parameter
    E_MSAPI_CEC_DI_CMD_SKIP_REVERSE           = 0x1C,  //parameter
    E_MSAPI_CEC_DI_CMD_INDEX_SEARCH_FORWARD   = 0x1D,  //parameter
    E_MSAPI_CEC_DI_CMD_INDEX_SEARCH_REVERSE   = 0x1E,  //parameter
    E_MSAPI_CEC_DI_CMD_OTHER_STATUS           = 0x1F,  //parameter
} E_MSAPI_CEC_DECK_INFO;

//Give deck status, Status request 1 byte
typedef enum
{
    E_MSAPI_CEC_SR_CMD_ON                     = 1,  //parameter
    E_MSAPI_CEC_SR_CMD_OFF                    = 2,  //parameter
    E_MSAPI_CEC_SR_CMD_ONCE                   = 3,  //parameter
} E_MSAPI_CEC_STATUS_REQUEST;

//Play, [Play Mode] 1 byte
typedef enum
{
    E_MSAPI_CEC_PM_CMD_PLAY_FORWARD               = 0x24,  //parameter
    E_MSAPI_CEC_PM_CMD_PLAY_REVERSE               = 0x20,  //parameter
    E_MSAPI_CEC_PM_CMD_PLAY_STILL                 = 0x25,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_FORWARD_MINI_SPEED    = 0x05,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_FORWARD_MEDIUM_SPEED  = 0x06,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_FORWARD_MAXI_SPEED    = 0x07,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_REVERSE_MINI_SPEED    = 0x09,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_REVERSE_MEDIUM_SPEED  = 0x0A,  //parameter
    E_MSAPI_CEC_PM_CMD_FAST_REVERSE_MAXI_SPEED    = 0x0B,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_FORWARD_MINI_SPEED    = 0x15,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_FORWARD_MEDIUM_SPEED  = 0x16,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_FORWARD_MAXI_SPEED    = 0x17,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_REVERSE_MINI_SPEED    = 0x19,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_REVERSE_MEDIUM_SPEED  = 0x1A,  //parameter
    E_MSAPI_CEC_PM_CMD_SLOW_REVERSE_MAXI_SPEED    = 0x1B,  //parameter
} E_MSAPI_CEC_PLAY_MODE;

//Logical Address
typedef enum
{
    E_MSAPI_CEC_LA_TV              =0,
    E_MSAPI_CEC_LA_RECORDER1       =1,
    E_MSAPI_CEC_LA_RECORDER2       =2,
    E_MSAPI_CEC_LA_TUNER1          =3,
    E_MSAPI_CEC_LA_PLAYBACK1       =4,
    E_MSAPI_CEC_LA_AUDIO_SYS       =5,
    E_MSAPI_CEC_LA_TUNER2          =6,
    E_MSAPI_CEC_LA_TUNER3          =7,
    E_MSAPI_CEC_LA_PLAYBACK2       =8,
    E_MSAPI_CEC_LA_RECORER3        =9,
#if 1 //HDMI 1.4 and after
    E_MSAPI_CEC_LA_TUNER4          =10,
    E_MSAPI_CEC_LA_PLYBACK3        =11,
#endif
    E_MSAPI_CEC_RESERVED_1         =12,
    E_MSAPI_CEC_RESERVED_2         =13,
    E_MSAPI_CEC_LA_FREE_USE        =14,
    E_MSAPI_CEC_LA_UNREGISTERED    =15,
    E_MSAPI_CEC_LA_BROADCAST       =15,
    E_MSAPI_CEC_LA_MAX =15,
} E_MSAPI_CEC_DEVICELA;

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------

/// Define the message data for CEC message
typedef struct
{
    E_MSAPI_CEC_OPCODE eOpcode;
    MS_U8 *pOperand;
    E_MSAPI_CEC_DEVICELA eDeviceLA;
    MS_U8 u8Length;
} MSAPI_CEC_MESSAGE;

typedef MS_BOOL (*Fn_Demo_CEC_Response)(MS_U8 u8header, MS_U8 u8opcode , MS_U8 *pu8Cmd, MS_U16 u16ContentLen);

typedef struct
{
    Fn_Demo_CEC_Response pFn_Demo_CEC_Response;
}MSAPI_CEC_CB;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL msAPI_CEC_Init(void);
MS_BOOL msAPI_CEC_SetOnOff(MS_BOOL bStart);
MS_BOOL msAPI_CEC_Exit(void);
MS_BOOL msAPI_CEC_SendMessage(MSAPI_CEC_MESSAGE stCECMessage);
void msAPI_CECSetResponseCB(Fn_Demo_CEC_Response pCBFunc);

#ifdef __cplusplus
}
#endif

