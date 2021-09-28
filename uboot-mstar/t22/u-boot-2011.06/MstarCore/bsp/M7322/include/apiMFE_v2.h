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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MFE Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_MFE_V2_H_
#define _API_MFE_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "drv_mfe_st.h"

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

// MFE_UTOPIA2 return codes.
// These codes remap those defined in utopia.h

#define MFE_UTOPIA2_SUCCESS               0x00000000       ///< operation succeeded
#define MFE_UTOPIA2_FAIL                  0x00000001       ///< operation failed
#define MFE_UTOPIA2_NOT_SUPPORTED         0x00000002       ///< operation is not supported
#define MFE_UTOPIA2_PARAMETER_ERROR       0x00000003       ///< parameter is invalid
#define MFE_UTOPIA2_DONT_CARE             0x00000004       ///< obsolete
#define MFE_UTOPIA2_ERR_SYS               0x00000005       ///< operating system call failure
#define MFE_UTOPIA2_ERR_INV               0x00000006       ///< invalid argument passed
#define MFE_UTOPIA2_ERR_TIMEDOUT          0x00000007       ///< timeout occured
#define MFE_UTOPIA2_ERR_DEFERRED          0x00000008       ///< action has been deferred
#define MFE_UTOPIA2_ERR_UNIMPLEM          0x00000009       ///< Unimplemented service
#define MFE_UTOPIA2_ERR_LOCKED            0x00000010       ///< object locked
#define MFE_UTOPIA2_ERR_DELETED           0x00000011       ///< Object has been deleted
#define MFE_UTOPIA2_ERR_HEAP              0x00000012       ///< Heap full
#define MFE_UTOPIA2_ERR_UNINIT            0x00000013       ///< module is not initialised
#define MFE_UTOPIA2_ERR_UNIQUE            0x00000014       ///< unique id needed
#define MFE_UTOPIA2_ERR_STATE             0x00000015       ///< invalid state to invoke function
#define MFE_UTOPIA2_ERR_DEV               0x00000016       ///< no valid device exists
#define MFE_UTOPIA2_ERR_NOT_FOUND         0x00000017       ///< Not found
#define MFE_UTOPIA2_ERR_EOL               0x00000018       ///< a list traversal has ended
#define MFE_UTOPIA2_ERR_TERM              0x00000019       ///< module has terminated
#define MFE_UTOPIA2_ERR_LIMIT             0x00000020       ///< System limit was reached.
#define MFE_UTOPIA2_ERR_RESOURCE          0x00000021       ///< insufficient resources for request
#define MFE_UTOPIA2_ERR_NOT_AVAIL         0x00000022       ///< resources not available
#define MFE_UTOPIA2_ERR_BAD_DATA          0x00000023       ///< Bad data in transport stream
#define MFE_UTOPIA2_ERR_INLINE_SUCCESS    0x00000024       ///< obsolete
#define MFE_UTOPIA2_ERR_DEFERRED_SUCCESS  0x00000025       ///< obsolete
#define MFE_UTOPIA2_ERR_IN_USE            0x00000026       ///< Object is in use.
#define MFE_UTOPIA2_ERR_INTERMEDIATE      0x00000027       ///< Intermediate return 0x0000000 successful but not complete
#define MFE_UTOPIA2_ERR_OVERRUN           0x00000028       ///< Buffer overrun occured (e.g. in filters).
#define MFE_UTOPIA2_ERR_ABORT             0x00000029       ///< Request not supported.
#define MFE_UTOPIA2_ERR_NOTIMPLEMENTED    0x00000030       ///< Request not implemented.
#define MFE_UTOPIA2_ERR_INVALID_HANDLE    0x00000031       ///< handle is invalid
#define MFE_UTOPIA2_ERR_NOMEM             0x00000032       ///< No Memory
#define MFE_UTOPIA2_SHM_EXIST             0x00000033       ///< target share memory already exists
#define MFE_UTOPIA2_RPOOL_ESTABLISHED     0x00000034       ///< target resource pool is already established
#define MFE_UTOPIA2_NO_RESOURCE           0x00000035       ///< try to obtain resource but fail
#define MFE_UTOPIA2_SIGNAL_OK             0x00000036       ///< signal ok
#define MFE_UTOPIA2_ERR_USER              0x00000099       ///< start of subsystem specific errors


//-------------------------------------------------------------------------------------------------
/// I/O control commands for MFE_Ioctl.
/// The required type of pArgs is in the brackets.
///
///     MApi_CMD_MFE_Init (PMFE_INIT_PARAM)
///         Initializes MFE driver.
///
///     MApi_CMD_MFE_Encode (PMFE_INIT_PARAM)
///         Notifies driver to start encoder.
///
///     MApi_CMD_MFE_GetVOL (PMFE_GETVOL_PARAM)
///         Gets bitstream header.
///
///     MApi_CMD_MFE_CompressOnePicture (PMFE_COMPRESSONEPICTURE_PARAM)
///         Encodes one frame.
///
///     MApi_CMD_MFE_DeInit (PMFE_INIT_PARAM)
///         De-initializes MFE driver.
///
///     MApi_CMD_MFE_PowerOff (PMFE_INIT_PARAM)
///         Turns off MFE clock.
///
///     MApi_CMD_MFE_GetOutBuffer (PMFE_GET_OBUF_PARAM)
///         Gets descriptor of output buffer.
///
///     MApi_CMD_MFE_SetBitrateFramerate (PMFE_INIT_PARAM)
///         Sets frame rate and bit rate using PVR_Info's nBitrate and FrameRatex100 fields.
///
///     MApi_CMD_MFE_SetColorFormat (PMFE_INIT_PARAM)
///         Sets input data format.
///
///     MApi_CMD_MFE_ResetSPSPPS (PMFE_INIT_PARAM)
///         Resets SPS and PPS header content with current configuration.
///
///     MApi_CMD_MFE_SetFrameType (PMFE_SET_PARAM)
///         If frame type is I_VOP, force compress I frame.
///
///     MApi_CMD_MFE_SetISR (PMFE_INIT_PARAM)
///         Sets ISR mode.
///
///     MApi_CMD_MFE_GetHWCap (PMFE_GET_HWCAP_PARAM)
///         Gets MFE HW capability.
///
///     MApi_CMD_MFE_GetOutputInfo (PMFE_GETOUTPUT_PARAM)
///         Gets type and length of encoded frame data.
///
///     MApi_CMD_MFE_SetVUI (PMFE_SETVUI_PARAM)
///         Sets VUI information to be included in stream header.
///
///     MApi_CMD_MFE_GetConfig (PMFE_GET_CONFIG_PARAM)
///         Gets a copy of encode info used during initialization.
///
//-------------------------------------------------------------------------------------------------
typedef enum {
    MApi_CMD_MFE_Init = 0x0,
    MApi_CMD_MFE_Encode,
    MApi_CMD_MFE_GetVOL,
    MApi_CMD_MFE_CompressOnePicture,
    MApi_CMD_MFE_DeInit,
    MApi_CMD_MFE_PowerOff,
    MApi_CMD_MFE_GetOutBuffer,
    MApi_CMD_MFE_SetBitrateFramerate,
    MApi_CMD_MFE_SetColorFormat,
    MApi_CMD_MFE_ResetSPSPPS,
    MApi_CMD_MFE_SetFrameType,
    MApi_CMD_MFE_SetISR,
    MApi_CMD_MFE_GetHWCap,
    MApi_CMD_MFE_GetOutputInfo,
    MApi_CMD_MFE_SetVUI,
    MApi_CMD_MFE_GetConfig,
} E_MFE_V2_IOCTL_CMD;

//-------------------------------------------------------------------------------------------------
/// Encoding parameters.
///     mfe_Info               IN: pointer to encode info structure
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_INIT_PARAM
{
    PVR_Info* mfe_Info;
    MS_BOOL retVal;
}MFE_INIT_PARAM, *PMFE_INIT_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for setting frame type.
/// Used with MApi_CMD_MFE_SetFrameType.
///     mfe_Info               IN: pointer to encode info structure
///     frameType              IN: If frame type is I_VOP, force compress I frame.
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_SET_PARAM
{
    PVR_Info* mfe_Info;
    MS_S32 frametype;
    MS_BOOL retVal;
}MFE_SET_PARAM, *PMFE_SET_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for getting SPS and PPS infomation.
/// Used with MApi_CMD_MFE_GetVOL.
///     mfe_Info               IN: pointer to encode info structure
///     header_info            OUT: (SPSPPS_INFO_t*) SPS, PPS information
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_GETVOL_PARAM
{
    PVR_Info* mfe_Info;
    void *header_info;
    MS_BOOL retVal;
}MFE_GETVOL_PARAM, *PMFE_GETVOL_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for encoding one input frame.
/// Used with MApi_CMD_MFE_CompressOnePicture.
///     mfe_Info               IN: pointer to encode info structure
///     YUVPlane               IN: DRAM location of input frame data.
///     bForceIframe           IN: If TRUE, force this frame to be I-frame.
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_COMPRESSONEPICTURE_PARAM
{
    PVR_Info* mfe_Info;
    MEMMAP_CUR_t YUVPlane;
    MS_BOOL bForceIframe;
    MS_BOOL retVal;
}MFE_COMPRESSONEPICTURE_PARAM, *PMFE_COMPRESSONEPICTURE_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for getting the descriptor on output buffer.
/// Used with MApi_CMD_MFE_GetOutBuffer.
///     mfe_Info               IN: pointer to encode info structure
///     outbuf                 OUT: Descriptor of output buffer
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_GET_OBUF_PARAM
{
    PVR_Info* mfe_Info;
    MEMMAP_t* outbuf;
    MS_BOOL retVal;
}MFE_GET_OBUF_PARAM, *PMFE_GET_OBUF_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for getting hardware capability.
/// Used with MApi_CMD_MFE_GetHWCap.
///     mfe_Info               IN: pointer to encode info structure
///     pDrv                   OUT: Hardware capability information
///     pHwCap_len             IN: Length of hardware capability structure
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_GET_HWCAP_PARAM
{
    PVR_Info* mfe_Info;
    MFE_HwCapV0 *pDrv;
    MS_U32 *HwCap_len;
    MS_BOOL retVal;
}MFE_GET_HWCAP_PARAM, *PMFE_GET_HWCAP_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for getting information on encoded frame.
/// Used with MApi_CMD_MFE_GetOutputInfo.
///     mfe_Info               IN: pointer to encode info structure
///     frametype              OUT: encoded frame type (I_VOP or P_VOP)
///     size                   OUT: encoded bitstream size
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_GETOUTPUT_PARAM
{
    PVR_Info* mfe_Info;
    MS_S32 frametype;
    MS_S32 size;
    MS_BOOL retVal;
}MFE_GETOUTPUT_PARAM, *PMFE_GETOUTPUT_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for setting video usability information (VUI).
/// Used with MApi_CMD_MFE_SetVUI.
///     mfe_Info                                IN: pointer to encode info structure
///     setLevel                                IN: level_idc
///     setVUI_aspect_ratio_info_present_flag   IN: aspect_ratio_info_present_flag
///     setVUI_aspect_ratio_idc                 IN: aspect_ratio_idc
///     setVUI_sar_width                        IN: sar_width
///     setVUI_sar_height                       IN: sar_height
///     retVal                                  OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_SETVUI_PARAM
{
    PVR_Info* mfe_Info;
    MS_U32 setLevel;
    MS_BOOL setVUI_aspect_ratio_info_present_flag;
    MS_U32 setVUI_aspect_ratio_idc;
    MS_U32 setVUI_sar_width;
    MS_U32 setVUI_sar_height;
    MS_BOOL retVal;
}MFE_SETVUI_PARAM, *PMFE_SETVUI_PARAM;

//-------------------------------------------------------------------------------------------------
/// Parameters for getting a copy of encode info used during initialization.
/// Used with MApi_CMD_MFE_GetConfig.
///     mfe_Info               OUT: pointer to encode info structure
///     retVal                 OUT: TRUE: success; FALSE: fail
//-------------------------------------------------------------------------------------------------
typedef struct _MFE_GET_CONFIG_PARAM
{
    PVR_Info* mfe_Info;
    MS_BOOL retVal;
}MFE_GET_CONFIG_PARAM, *PMFE_GET_CONFIG_PARAM;

//-------------------------------------------------------------------------------------------------
/// *** DO NOT USE ***
/// Register MFE driver to Utopia framework.
/// To be called by UTOPIA FRAMEWORK ONLY.
//-------------------------------------------------------------------------------------------------
void MFERegisterToUtopia(FUtopiaOpen ModuleType);


//-------------------------------------------------------------------------------------------------
/// Gets MFE driver instance.
/// @param  ppInstance             \b OUT: pointer to MFE driver instance
/// @param  u32ModuleVersion       \b IN: Module version. Use 0.
/// @param  pAttribute             \b IN: not used
/// @return MFE_UTOPIA2_SUCCESS : success
/// @return Other values : see return code definitions
//-------------------------------------------------------------------------------------------------
MS_U32 MFE_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);


//-------------------------------------------------------------------------------------------------
/// Releases MFE driver instance.
/// @param  pInstance              \b IN: MFE drvier instance
/// @return MFE_UTOPIA2_SUCCESS : success
/// @return Other values : see return code definitions
//-------------------------------------------------------------------------------------------------
MS_U32 MFE_Close(void* pInstance);


//-------------------------------------------------------------------------------------------------
/// Notify driver to start encoder.
/// @param  pInstance              \b IN: MFE drvier instance
/// @param  u32Cmd                 \b IN: MFE I/O control command from E_MFE_V2_IOCTL_CMD
/// @param  pArgs                  \b IN: pointer to I/O control parameter structure
/// @return MFE_UTOPIA2_SUCCESS : success
/// @return Other values : see return code definitions
//-------------------------------------------------------------------------------------------------
MS_U32 MFE_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif

#endif // _API_MFE_V2_H_


