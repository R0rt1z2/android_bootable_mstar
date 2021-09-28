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
/// @brief  JPEG API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_JPEG_OJPD_H_
#define _API_JPEG_OJPD_H_

#ifdef __cplusplus
extern "C" {
#endif


void msAPI_JPEG_get_APP0_info_OJPD( MS_U8 *unit,MS_U16 *x, MS_U16 *y );

JPEG_Result MApi_JPEG_Init_OJPD(JPEG_InitParam *pInitParam);
JPEG_Result MApi_JPEG_DecodeHdr_OJPD(void);
JPEG_Result MApi_JPEG_Decode_OJPD(void);
void MApi_JPEG_Exit_OJPD(void);
JPEG_ErrCode MApi_JPEG_GetErrorCode_OJPD(void);
JPEG_Event MApi_JPEG_GetJPDEventFlag_OJPD(void);
void MApi_JPEG_SetJPDEventFlag_OJPD(JPEG_Event eEvtVal);
void MApi_JPEG_Rst_OJPD(void);
void MApi_JPEG_PowerOn_OJPD(void);
void MApi_JPEG_PowerOff_OJPD(void);
MS_U16 MApi_JPEG_GetCurVidx_OJPD(void);
MS_BOOL MApi_JPEG_IsProgressive_OJPD(void);
MS_BOOL MApi_JPEG_ThumbnailFound_OJPD(void);
MS_U16 MApi_JPEG_GetWidth_OJPD(void);
MS_U16 MApi_JPEG_GetHeight_OJPD(void);
MS_U16 MApi_JPEG_GetOriginalWidth_OJPD(void);
MS_U16 MApi_JPEG_GetOriginalHeight_OJPD(void);
MS_U16 MApi_JPEG_GetNonAlignmentWidth_OJPD(void);
MS_U16 MApi_JPEG_GetNonAlignmentHeight_OJPD(void);
MS_U16 MApi_JPEG_GetAlignedPitch_OJPD(void);
MS_U16 MApi_JPEG_GetAlignedPitch_H_OJPD(void);
MS_U16 MApi_JPEG_GetAlignedWidth_OJPD(void);
MS_U16 MApi_JPEG_GetAlignedHeight_OJPD(void);
MS_U8 MApi_JPEG_GetScaleDownFactor_OJPD(void);
void MApi_JPEG_SetMaxDecodeResolution_OJPD(MS_U16 u16Width, MS_U16 u16Height);
void MApi_JPEG_SetProMaxDecodeResolution_OJPD(MS_U16 u16ProWidth, MS_U16 u16ProHeight);
void MApi_JPEG_SetMRBufferValid_OJPD(JPEG_BuffLoadType u8MRBuffType);
void MApi_JPEG_UpdateReadInfo_OJPD(MS_U32 u32DataRead, MS_BOOL bEOFflag);
void MApi_JPEG_ProcessEOF_OJPD(JPEG_BuffLoadType u8MRBuffType);
void MApi_JPEG_SetErrCode_OJPD(JPEG_ErrCode ErrStatus);
void MApi_JPEG_SetDbgLevel_OJPD(MS_U8 u8DbgLevel);
MS_U8 MApi_JPEG_GetDbgLevel_OJPD(void);
void MApi_JPEG_GetInfo_OJPD(JPEG_Info *pJPEG_Info);
void MApi_JPEG_GetStatus_OJPD(JPEG_Status *pJPEG_Status);
JPEG_Result MApi_JPEG_GetLibVer_OJPD(const MSIF_Version **ppVersion);
JPEG_Result MApi_JPEG_HdlVidxChk_OJPD(void);
JPEG_Result MApi_JPEG_GetBuffLoadType_OJPD(JPEG_BuffLoadType *pBuffLoadType);
JPEG_Result MApi_JPEG_EnableISR_OJPD(JPEG_IsrFuncCb IsrCb);
JPEG_Result MApi_JPEG_DisableISR_OJPD(void);
JPEG_DecodeStatus MApi_JPEG_WaitDone_OJPD(void);
JPEG_Result MApi_JPEG_GetEXIFDateTime_OJPD(JPEG_EXIF_DateTime *DateTime);
JPEG_Result MApi_JPEG_GetEXIFOrientation_OJPD(JPEG_EXIF_Orientation *eOrientation);
#if SUPPORT_EXIF_EXTRA_INFO
JPEG_Result MApi_JPEG_GetEXIFManufacturer_OJPD(MS_U8 *pu8Manufacturer, MS_U8 u8size);
JPEG_Result MApi_JPEG_GetEXIFModel_OJPD(MS_U8 *pu8Model, MS_U8 u8size);
JPEG_Result MApi_JPEG_GetEXIFFlash_OJPD(MS_U16 *pu16Flash);
JPEG_Result MApi_JPEG_GetEXIFISOSpeedRatings_OJPD(MS_U32 *pu32ISOSpeedRatings);
JPEG_Result MApi_JPEG_GetEXIFShutterSpeedValue_OJPD(JPEG_RATIONAL *pShutterSpeedValue);
JPEG_Result MApi_JPEG_GetEXIFApertureValue_OJPD(JPEG_RATIONAL *pApertureValue);
JPEG_Result MApi_JPEG_GetEXIFExposureBiasValue_OJPD(JPEG_RATIONAL *pExposureBiasValue);
JPEG_Result MApi_JPEG_GetEXIFFocalLength_OJPD(JPEG_RATIONAL *pFocalLength);
JPEG_Result MApi_JPEG_GetEXIFImageWidth_OJPD(MS_U32 *pu32ImageWidth);
JPEG_Result MApi_JPEG_GetEXIFImageHeight_OJPD(MS_U32 *pu32ImageHeight);
JPEG_Result MApi_JPEG_GetEXIFExposureTime_OJPD(JPEG_RATIONAL *pExposureTime);
JPEG_Result MApi_JPEG_GetEXIFFNumber_OJPD(JPEG_RATIONAL *pFNumber);
#endif
JPEG_Result MApi_JPEG_GetControl_OJPD(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32Size);

MS_PHYADDR MApi_JPEG_GetFreeMemory_OJPD(MS_U32 size);
MS_U32 MApi_JPEG_GetDataOffset_OJPD(void);
MS_U32 MApi_JPEG_GetSOFOffset_OJPD(void);
void MApi_JPEG_SetNJPDInstance_OJPD(MS_U8 JPDNum);
JPEG_Result MApi_JPEG_SupportCMYK_OJPD(MS_BOOL bEnable);
JPEG_Result MApi_JPEG_SupportRGB_OJPD(MS_BOOL bEnable);
void MApi_JPEG_SetMHEG5_OJPD(MS_BOOL bEnable);

#if SUPPORT_MPO_FORMAT
//For MPO
MS_BOOL MApi_JPEG_IsMPOFormat_OJPD(void);
MS_BOOL MApi_JPEG_GetMPOIndex_OJPD(JPEG_MPO_INDEX_INFO **ppMPOIndex);
MS_BOOL MApi_JPEG_GetMPOAttr_OJPD(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr);
void MApi_JPEG_DumpMPO_OJPD(void);
MS_BOOL MApi_JPEG_SetMPOBuffer_OJPD(MS_U32 read_offset, MS_U32 output_start);
void MApi_JPEG_SetMPOMaxDecodeResolution_OJPD(MS_U16 u16Width, MS_U16 u16Height);
void MApi_JPEG_SetMPOProMaxDecodeResolution_OJPD(MS_U16 u16ProWidth, MS_U16 u16ProHeight);
#endif
void MApi_JPEG_SetVerificationMode_OJPD(NJPEG_VerificationMode VerificationMode);
NJPEG_VerificationMode MApi_JPEG_GetVerificationMode_OJPD(void);
void MApi_NJPD_Debug_OJPD(void);
void MApi_JPEG_DisableAddressConvert_OJPD(void);



#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
#endif // _API_JPEG_OJPD_H_
//------------------------------------------------------------------------------
