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


#ifndef _DRV_MFE_ST_H_
#define _DRV_MFE_ST_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define MFE_SUPPORT_REDUCE_MODE
// This version of MFE driver supports "reduce mode" which includes
// two flags that can be turned on/off seperately. Refer to
// 'enableReduceBW' and 'enableFastMode' in struct PVR_Info below.

// MFE driver state
typedef enum {
    MFE_Invalid = 0x00,  // invalid state
    MFE_Init    = 0x20,  // driver is initialized
    MFE_Idle    = 0x30,  // driver is idle
    MFE_Busy    = 0x40,  // driver is busy handling a command
    MFE_DeInit  = 0x50,  // driver is deinitialized
    MFE_Error   = 0x60,  // driver is in an error state
    MFE_ReInit  = 0x70,  // marker for share mem copy of encode info that
                         //   was used during initialization
 } MFE_STATE;

// MFE clock level
typedef enum
{
MFE_CLK_VERY_SLOW = 0,
MFE_CLK_SLOW = 1,
MFE_CLK_MEDIUM = 2,
MFE_CLK_FAST = 3,
} MFE_CLK_LEVEL;

// DO NOT USE
typedef enum
{
BITS_INFO_ZERO = 0,
BITS_INFO_UNKNOW,
BITS_INFO_FRAMEDONE,
BITS_INFO_BUFFULL,
BITS_INFO_IMG_BUFFULL,
} BITS_STATUS_INFO;

// OBSOLETE
typedef enum
{
GETBITS_NOT_READY=0,
GETBITS_READY=1,
} MFE_HW_STATUS;

// Progressive or interlaced video
typedef enum
{
PROGRESSIVE = 0,
INTERLACE_SPLITED,
INTERLACE_NONSPLITED,
} MFE_INTERLACE_INFO;

// Input data format
typedef enum
{
YUVTILE = 0,         // OBSOLETE
MSTTILE = 1,         // YUV420, 16x32 tiled
MFE_YUYV = 2,        // YUV422, linear YUYV
MFE_YVYU = 3,        // YUV422, linear YVYU
MFE_UYVY = 4,        // YUV422, linear UYVY
MFE_VYUY = 5,        // YUV422, linear VYUY
MFE_EVDTILE = 6,     // YUV420, 32x16 tiled
MFE_32x32TILE = 7,   // YUV420, 32x32 tiled
MFE_NV12 = 8,        // YUV420SP, planar Y with interleaved uv
MFE_NV21 = 9,        // YUV420SP, planar Y with interleaved vu
} MFE_COLOR_FORMAT_INFO;

#define MAX_OUTBUFFER 10UL

// DO NOT USE
typedef struct {
    MS_PHY miuAddress;    //hardware physical
    MS_VIRT miuPointer;    //need to OR 0xA0000000
    MS_VIRT virtual_addr;
    MS_S32 size;
    BITS_STATUS_INFO status;    // 1: frame_done. 2: buffer full,3: image buffer full.
    MS_S32 is_more_bits; //1: there are more bitstream packet;  0: this is the last bitstream packet
    MS_U8  voptype;
    MS_PHY IVOP_address;
    MS_S32 stuffing_bytes;     //CABAC stuffing byte
} BITSFRAME_INFO;

/* Linear memory area descriptor */
typedef struct MEMMAP
{
    MS_U8 *miuPointer;  // virtual addr
    MS_PHY miuAddress;  // physical addr
    MS_U32 size;        // length
} MEMMAP_t;

// Input frame descriptor
typedef struct MEMMAP_CUR
{
    MEMMAP_t Cur_Y0;    // Lumam compoment of input frame
    MEMMAP_t Cur_C0;    // Chroma component of input frame
    MEMMAP_t Cur_Y1;    // OBSOLETE
    MEMMAP_t Cur_C1;    // OBSOLETE
} MEMMAP_CUR_t;

// Crop information to be inserted in SPS
typedef struct _MFE_CROP_INFO
{
    MS_BOOL crop_en;
    MS_U32 crop_top;
    MS_U32 crop_bottom;
    MS_U32 crop_left;
    MS_U32 crop_right;
} MFE_CROP_INFO_t;

// Descriptor on buffer allocated for MFE use
typedef struct _MFE_API_INFO
{
    MS_PHY miu_offset;          // miu offset
    MS_PHY mfe_phy_buf_addr;    // physical address
    MS_VIRT mfe_vir_buf_ptr;    // virtual address
    MS_U32 mfe_phy_buf_size;    // buffer size
    MS_U32 Encode_stop;         // OBSOLETE
} MFE_API_INFO_t;

// DIP-MFE handhsake mode and IMI setting
typedef struct _MFE_ADV_INFO
{
    MS_U32 low_bandwidth_en;    // Enable low bandwidth mode with IMI
    MS_U32 input_imi_en;        // Enable DIP-MFE handshake mode (input goes through IMI)
    MEMMAP_t imi_buf_addr;      // OBSOLETE
} MFE_ADV_INFO_t;

// DO NOT USE
typedef struct{
    MS_U32 is_off;
    MFE_CLK_LEVEL clock_level;
} POWERON_t;

// SPS/PPS information
typedef struct _SPSPPS_INFO
{
    MS_U8* pHeaderSPS;  // pointer to SPS
    MS_U32 SPSLen;      // SPS size
    MS_U8* pHeaderPPS;  // pointer to PPS
    MS_U32 PPSLen;      // PPS size
} SPSPPS_INFO_t;

// OBSOLETE
typedef struct _VOL_INFO
{
    MS_U8* pHeader;
    MS_U32 Len;
} VOL_INFO_t;

// Callback function invoked after frame is encoded
// bFirst_or_last_byte_of_Frame: 0x00 error state, 0x10 first , 0x01 last at frame, 0x11 one frame
typedef void (*StreamCallback)(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame);

// Callback function invoked to signal input buffer status
typedef void (*CleanCallback)(MS_U32 u32FrameCoded, MS_VIRT u32YUVPlane);

// Encode info structure
// OBSOLETE fields should be initialized to 0 and not tampered with afterwards.
// MADP-layer "simplified" encoder configuration
// NOTE: "Advanced" configurations are hidden inside DRV-layer.
typedef struct{
    // MFE-compliant
    MS_S32 width;                           // frame width
    MS_S32 height;                          // frame height
    MS_PHY BitsBuffStart;                   // OBSOLETE
    MS_U32 BitsBuffSize;                    // OBSOLETE
    MS_S32 nOutBuffer;                      // OBSOLETE
    MS_S32 OutBufferSize;                   // OBSOLETE
    MEMMAP_t InBuffer;                      // OBSOLETE
    MEMMAP_t BitstreamOutBuffer;            // output buffer descriptor
    MS_S32 BitstreamOutBufferSize;          // output buffer size

    // MFE new settings
    //MFE_COLOR_FORMAT_INFO nColorFormat;
    MS_S32 nCodecType;                      // Encoder format. MUST be H264_ENC.
    MS_S32 nDispWidth, nDispHeight;         // Display resolution
    MS_S32 nBufWidth, nBufHeight;           // Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
    MFE_INTERLACE_INFO bInterlace;          // Frame mode to encode
    MFE_COLOR_FORMAT_INFO bColorFormat;     // input data format
    // GOP
    MS_S32 nPbetweenI;                      // number of P-frames between I-frames
    MS_S32 nBbetweenP;                      // OBSOLETE
    MS_S32 nP_is_infinite;                  // One I-frame followed by infinite number of P-frames
    // Rate control
    MS_S32 FrameRatex100;                   // frames per second x 100
    MS_S32 nBitrate;                        // bits per second
    MS_S32 m_bFrameMode;                    // Input frame mode. MUST be 1.
    MS_S32 out_frametag_top;                // frame tag set/retrieved using MFE_ENC_SETCONF_FRAME_TAG/MFE_ENC_GETCONF_FRAME_TAG
    // Video length
    MS_S32 nTotalFrames;                    // OBSOLETE
    MS_S32 TimeIncreamentRes;               // OBSOLETE
    MS_S32 VopTimeIncreament;               // OBSOLETE
    //jpeg parameter
    MS_S32 quality;                         // OBSOLETE
    MS_S32 fdc_mode;                        // OBSOLETE
    MS_S32 exif_enable;                     // OBSOLETE
    MFE_CLK_LEVEL MFEClk;                   // MFE clock level to use

    MFE_API_INFO_t MfeApiInfo;              // buffer allocated for MFE use
    MFE_ADV_INFO_t MfeAdvInfo;              // DIP-MFE handhsake mode and IMI setting

    MS_U32 nDropFlag;                       // OBSOLETE
    MS_U32 nSkipFlag;                       // OBSOLETE
    MS_U32 enableVTRateControl;             // OBSOLETE
    MS_U32 enableSecurityMode;              // security mode
    MS_U32 enableISR;                       // frame done detection: 1: ISR mode; 0: polling mode
    MS_U32 enableCABAC;                     // main profile requires CABAC

    MFE_CROP_INFO_t  MfeCropInfo;
    MS_U32 enableReduceBW;                  // Memory BW reduced mode
    MS_U32 enableFastMode;                  // intra mode inly 0~2, drop i16 and disable fme.
    MFE_STATE mfe_state;                    // driver state
    void* g_pCustomCtx;
    StreamCallback g_StreamCallBackFunc;    // pointer to stream callback function
    CleanCallback g_CleanCallBackFunc;      // pointer to input buffer callback function

    void* mfe_drvHandle;                    // driver handle for internal use. *** DO NOT MODIFY ***

    MS_S32 reserved0;                       // 1: cached memory; 0: non-cached memory
    MS_S32 reserved1;                       // bit[0] 1: enable TLB; 0: disable TLB
    MS_S32 reserved2;                       // bit[15:8] = Profile, bit[7:0] = Level
    MS_S32 reserved3;                       // reserved
    MS_S32 reserved4;                       // reserved
    MS_S32 reserved5;                       // reserved
}PVR_Info;

#define API_HWCAP_VERSION0 1UL

// Hardware capability strcuture
typedef struct{
    MS_U32 u32ApiHW_Version;                // Version of current structure
    MS_U16 u16ApiHW_Length;                 // Length of this structure
    MS_BOOL bIsSupportEncoder;              // Support encoder
    MS_U8 u8MaxEncodeFrame;                 // Max encode frame
    MS_U16 u16MaxEncodeWidth;               // Max encode width
    MS_U16 u16MaxEncodeHeight;              // Max encode height
    MS_BOOL bSupportSubVidWin;              // Support sub video window
    MS_BOOL bSupportMpeg4;                  // Support MPEG4
    MS_BOOL bSupportH264;                   // Support H.264
    /*New Cap add in struct add at the end*/
}MFE_HwCapV0;

// Encode profile
typedef enum
{
    E_MFE_PROFILE_DEFAULT = 0,
    E_MFE_PROFILE_BASELINE,
    E_MFE_PROFILE_MAIN,
} MFE_PROFILE;

// Encode level
typedef enum
{
    E_MFE_LEVEL_DEFAULT = 0,
    E_MFE_LEVEL_1,
    E_MFE_LEVEL_1b,
    E_MFE_LEVEL_1_1,
    E_MFE_LEVEL_1_2,
    E_MFE_LEVEL_1_3,
    E_MFE_LEVEL_2,
    E_MFE_LEVEL_2_1,
    E_MFE_LEVEL_2_2,
    E_MFE_LEVEL_3,
    E_MFE_LEVEL_3_1,
    E_MFE_LEVEL_3_2,
    E_MFE_LEVEL_4,
    E_MFE_LEVEL_4_1,
} MFE_LEVEL;

#ifdef __cplusplus
}
#endif

#endif
