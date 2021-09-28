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


/*! \defgroup JPEG JPEG interface (apiJPEG.h)
    \ingroup JPEG
    The supporting features of Mstar JPD HW IP are listed in the following:
-# Support JPEG sequential mode, single scan.
-# Mstar JPD IP supports sequential mode only. If the file is of JPD progressive mode, DDI will pre-do software variable length decoding (SVLD) and then call Mstar JPD IP to do the rest of the decoding procedure.
-# Support both color picture and grayscale format.
-# Operate in scan unit, the decoder will handle the part after scan header.
-# Support JPEG decoder based on programmable region of interest (ROI).
-# Support color format: YUV444/YUV422/YUV420/YUV411. The decoded result will be stored in YUV422 format.
-# One ECS read-from-memory channel MRC.
	   -It means that Mstar JPD only support interleaved baseline decoding.
-# JPD read data form read buffer which is defined by software. Its minimum size is 16 double words and multiple of 8.
-# Read Buffer is divided into low and high portion, and valid bit for each portion is set by software and clear by decoder.
-# Decode picture data output to write buffer which is for the picture of original size, 1/2 downscaled, 1/4 downscaled or 1/8 downscaled.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html JPD_decode_flow.png

*! \defgroup JPEG_Basic JPEG Basic APIs
*  \ingroup JPEG
*! \defgroup JPEG_MPO MPO related APIs
*  \ingroup JPEG
*! \defgroup JPEG_MJPEG MJPEG related APIs
*  \ingroup JPEG
*! \defgroup JPEG_HW_BUG Previous HW bug related APIs (to be removed)
*  \ingroup JPEG
*! \defgroup JPEG_oJPD oJPD use related APIs (to be removed)
*  \ingroup JPEG
*! \defgroup JPEG_MHEG5 MHEG5 use related APIs (to be removed)
*  \ingroup JPEG
*! \defgroup JPEG_Debug JPEG Debug APIs
*  \ingroup JPEG
*! \defgroup JPEG_to_be_integrated JPEG_to_be_integrated (to be removed)
*  \ingroup JPEG
*! \defgroup JPEG_to_be_removed JPEG_to_be_removed (to be removed)
*  \ingroup JPEG
*! \defgroup JPEG_to_be_added JPEG_to_be_added
*  \ingroup JPEG
*/

#ifndef _API_JPEG_H_
#define _API_JPEG_H_

#ifdef __cplusplus
extern "C" {
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_JPEG_LIB_CODE                     {'J','P','E','G'}                    ///<Lib code
#define MSIF_JPEG_LIBVER                       {'0','1'}                            ///<LIB version
#define MSIF_JPEG_BUILDNUM                     {'0','H'}                            ///<Build Number
#define MSIF_JPEG_CHANGELIST                   {'0','0','7','1','8','4','1','5'}    ///<P4 ChangeList Number

//------------------------------------------------------------------------------
/// @brief \b JPEG_API_VERSION : JPEG Version
//------------------------------------------------------------------------------
#define JPEG_API_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_JPEG_LIB_CODE,                 /* IP__                                             */  \
    MSIF_JPEG_LIBVER,                   /* 0.0 ~ Z.Z                                        */  \
    MSIF_JPEG_BUILDNUM,                 /* 00 ~ 99                                          */  \
    MSIF_JPEG_CHANGELIST,               /* CL#                                              */  \
    MSIF_OS

//------------------------------------------------------------------------------
/// @brief \b JPEG_DEFAULT_EXIF_SIZE : The buffer size for thumbnail
//------------------------------------------------------------------------------
#define JPEG_DEFAULT_EXIF_SIZE  (64*1024UL + 128)

#define SUPPORT_MPO_FORMAT          1

#define SUPPORT_EXIF_EXTRA_INFO       TRUE // FALSE

#define SUPPORT_GET_FULL_IMAGE_INFO          1

#if SUPPORT_MPO_FORMAT
#define JPEG_MPO_MAX_SUPPORT_IMAGE  4
#endif



#define JPEG_UTOPIA20       (1)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_Result
/// @brief \b Enum \b Description: JPEG decode result
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_FAILED   = 0
  , E_JPEG_OKAY     = 1
  , E_JPEG_DONE     = 2
  , E_JPEG_RETRY    = 3
} JPEG_Result;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_ErrCode
/// @brief \b Enum \b Description: JPEG error code
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_NO_ERROR                     = 0
  , E_JPEG_BAD_DHT_COUNTS               = -200
  , E_JPEG_BAD_DHT_INDEX                = -201
  , E_JPEG_BAD_DHT_MARKER               = -202
  , E_JPEG_BAD_DQT_MARKER               = -203
  , E_JPEG_BAD_DQT_TABLE                = -204
  , E_JPEG_BAD_PRECISION                = -205
  , E_JPEG_BAD_HEIGHT                   = -206
  , E_JPEG_BAD_WIDTH                    = -207
  , E_JPEG_TOO_MANY_COMPONENTS          = -208
  , E_JPEG_BAD_SOF_LENGTH               = -209
  , E_JPEG_BAD_VARIABLE_MARKER          = -210
  , E_JPEG_BAD_DRI_LENGTH               = -211
  , E_JPEG_BAD_SOS_LENGTH               = -212
  , E_JPEG_BAD_SOS_COMP_ID              = -213
  , E_JPEG_W_EXTRA_BYTES_BEFORE_MARKER  = -214
  , E_JPEG_NO_ARITHMETIC_SUPPORT        = -215
  , E_JPEG_UNEXPECTED_MARKER            = -216
  , E_JPEG_NOT_JPEG                     = -217
  , E_JPEG_UNSUPPORTED_MARKER           = -218
  , E_JPEG_BAD_DQT_LENGTH               = -219
  , E_JPEG_TOO_MANY_BLOCKS              = -221
  , E_JPEG_UNDEFINED_QUANT_TABLE        = -222
  , E_JPEG_UNDEFINED_HUFF_TABLE         = -223
  , E_JPEG_NOT_SINGLE_SCAN              = -224
  , E_JPEG_UNSUPPORTED_COLORSPACE       = -225
  , E_JPEG_UNSUPPORTED_SAMP_FACTORS     = -226
  , E_JPEG_DECODE_ERROR                 = -227
  , E_JPEG_BAD_RESTART_MARKER           = -228
  , E_JPEG_ASSERTION_ERROR              = -229
  , E_JPEG_BAD_SOS_SPECTRAL             = -230
  , E_JPEG_BAD_SOS_SUCCESSIVE           = -231
  , E_JPEG_STREAM_READ                  = -232
  , E_JPEG_NOTENOUGHMEM                 = -233
  , E_JPEG_STOP_DECODE                  = -234 //kevinhuang, add
  , E_JPEG_BAD_APP1_MARKER              = -235
  , E_JPEG_NO_THUMBNAIL                 = -236
  , E_JPEG_UNSUPPORTED_HUFF_DECODE      = -237
  , E_JPEG_READBUFFER_TOOSMALL          = -238
  , E_JPEG_NOT_ENOUGH_HEADER_INFO       = -240
  , E_JPEG_RE_INIT                      = -241
  , E_JPEG_NOT_INIT                     = -242
  , E_JPEG_BAD_APP0_MARKER              = -243
#if SUPPORT_MPO_FORMAT
  , E_JPEG_BAD_APP2_MARKER              = -250
#endif
  , E_JPEG_JPD_DECODE_ERROR             = 0x5566
} JPEG_ErrCode;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_DecType
/// @brief \b Enum \b Description: JPEG decode type
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_TYPE_MAIN        = 0
  , E_JPEG_TYPE_THUMBNAIL   = 1
  , E_JPEG_TYPE_MJPEG       = 2
}JPEG_DecType;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_BuffLoadType
/// @brief \b Enum \b Description: JPEG buffer loading mode
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_BUFFER_NONE  = 0
  , E_JPEG_BUFFER_HIGH  = 1
  , E_JPEG_BUFFER_LOW   = 2
} JPEG_BuffLoadType;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_DbgLevel
/// @brief \b Enum \b Description: JPEG debug mode
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_DEBUG_NONE   = 0x0
  , E_JPEG_DEBUG_API    = 0x01
  , E_JPEG_DEBUG_DRV    = 0x02
  , E_JPEG_DEBUG_HAL    = 0x04
  , E_JPEG_DEBUG_MSG    = 0x07
  , E_JPEG_DEBUG_ERR    = 0x08
  , E_JPEG_DEBUG_ALL    = 0x0F
  , E_JPEG_DEBUG_MJPEG    = 0x10
} JPEG_DbgLevel;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_Event
/// @brief \b Enum \b Description: JPEG decode event flag
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_EVENT_DEC_NONE       = 0x00
  , E_JPEG_EVENT_DEC_DONE       = 0x01
  , E_JPEG_EVENT_DEC_ECS_ERROR  = 0x02
  , E_JPEG_EVENT_DEC_IS_ERROR   = 0x04
  , E_JPEG_EVENT_DEC_RST_ERROR  = 0x08

  , E_JPEG_EVENT_DEC_ERROR_MASK = 0x0E    // This flag include all hw error mask
  , E_JPEG_EVENT_ERR_FLOW_MASK  = 0x0F    // This flag is used to indicate sw error flow, not reported by hw

  , E_JPEG_EVENT_DEC_MRBL_DONE  = 0x10
  , E_JPEG_EVENT_DEC_MRBH_DONE  = 0x20
  , E_JPEG_EVENT_DEC_MRB_DONE   = 0x30    // This flag means MRBL & MRBH both done
  , E_JPEG_EVENT_DEC_MWB_FULL   = 0x40
} JPEG_Event;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_DecodeStatus
/// @brief \b Enum \b Description: JPEG decoder status
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_DEC_DONE             = 0
  , E_JPEG_DEC_FAILED           = 1
  , E_JPEG_DEC_RST_ERROR        = 2
  , E_JPEG_DEC_BITSTREAM_ERROR  = 3
  , E_JPEG_DEC_MRBL_DONE        = 4
  , E_JPEG_DEC_MRBH_DONE        = 5
  , E_JPEG_DEC_DECODING         = 6
} JPEG_DecodeStatus;
//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: JPEG_EXIF_IFD0_Orientation
/// @brief \b Enum \b Description: The orientation of JPEG EXIF
//-----------------------------------------------------------------------------
typedef enum
{
    E_JPEG_EXIF_ORIENT_NOT_FOUND              = 0
  , E_JPEG_EXIF_ORIENT_ROTATE_0               = 1
  , E_JPEG_EXIF_ORIENT_MIRROR_LR              = 2
  , E_JPEG_EXIF_ORIENT_ROTATE_180             = 3
  , E_JPEG_EXIF_ORIENT_MIRROR_TB              = 4
  , E_JPEG_EXIF_ORIENT_ROTATE_90_MIRROR_LR    = 5
  , E_JPEG_EXIF_ORIENT_ROTATE_90              = 6
  , E_JPEG_EXIF_ORIENT_ROTATE_90_MIRROR_TB    = 7
  , E_JPEG_EXIF_ORIENT_ROTATE_270             = 8
  , E_JPEG_EXIF_ORIENT_RESERVED               = 9
} JPEG_EXIF_Orientation;

typedef enum
{
    E_JPEG_GET_EXIF_MANUFACTURER            = 0
  , E_JPEG_GET_EXIF_MODEL                   = 1
  , E_JPEG_GET_EXIF_FLASH                   = 2
  , E_JPEG_GET_EXIF_ISO_SPEED_RATINGS       = 3
  , E_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE     = 4
  , E_JPEG_GET_EXIF_APERTURE_VALUE          = 5
  , E_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE     = 6
  , E_JPEG_GET_EXIF_FOCAL_LENGTH            = 7
  , E_JPEG_GET_EXIF_IMAGE_WIDTH             = 8
  , E_JPEG_GET_EXIF_IMAGE_HEIGHT            = 9
  , E_JPEG_GET_EXIF_EXPOSURE_TIME           = 10
  , E_JPEG_GET_EXIF_F_NUMBER                = 11
  , E_JPEG_GET_EXIF_EXPOSURE_PROGRAM        = 12
  , E_JPEG_GET_Y_SAMPLING_FACTOR_HORIZONTAL = 13
  , E_JPEG_GET_Y_SAMPLING_FACTOR_VERTICAL   = 14
  , E_JPEG_GET_COMPONENTS_NUM               = 15
  , E_JPEG_GET_IS_PROGRESSIVE               = 16
  , E_JPEG_GET_COLOR_FORMAT                 = 17
#if SUPPORT_GET_FULL_IMAGE_INFO
  , E_JPEG_GET_FULL_IMAGE_WIDTH             = 18
  , E_JPEG_GET_FULL_IMAGE_HEIGHT            = 19
  , E_JPEG_IS_FULL_IMAGE_PROGRESSIVE        = 20
#endif
  , E_JPEG_GET_EXIF_COLOR_SPACE             = 21
  , E_JPEG_GET_EXIF_GPS_AREA_INFO           = 22
  , E_JPEG_GET_EXIF_GPS_LATITUDE_REF        = 23
  , E_JPEG_GET_EXIF_GPS_LATITUDE            = 24
  , E_JPEG_GET_EXIF_GPS_LONGITUDE_REF       = 25
  , E_JPEG_GET_EXIF_GPS_LONGITUDE           = 26
  , E_JPEG_GET_EXIF_GPS_ALTITUDE_REF        = 27
  , E_JPEG_GET_EXIF_GPS_ALTITUDE            = 28
  , E_JPEG_GET_EXIF_GPS_TIMESTAMP           = 29
  , E_JPEG_GET_EXIF_GPS_SPEED_REF           = 30
  , E_JPEG_GET_EXIF_GPS_SPEED               = 31
  , E_JPEG_GET_EXIF_GPS_IMGDIRECTION_REF    = 32
  , E_JPEG_GET_EXIF_GPS_IMGDIRECTION        = 33
  , E_JPEG_GET_EXIF_GPS_DEST_BEARING_REF    = 34
  , E_JPEG_GET_EXIF_GPS_DEST_BEARING        = 35
  , E_JPEG_GET_EXIF_GPS_DATE_STAMP_REF      = 36
  , E_JPEG_GET_EXIF_GPS_H_POS_ERROR         = 37
  , E_JPEG_GET_EXIF_GPS_VERSION_ID          = 38
} EN_JPEG_GET_CTRL_ID;

typedef enum
{
     E_JPEG_INPUT_FORMAT_GRAY_SCALE     = 0
,    E_JPEG_INPUT_FORMAT_YUV444         = 1
,    E_JPEG_INPUT_FORMAT_YUV422         = 2
,    E_JPEG_INPUT_FORMAT_YUV420         = 3
,    E_JPEG_INPUT_FORMAT_YUV411         = 4
,    E_JPEG_INPUT_FORMAT_CMYK           = 5
,    E_JPEG_INPUT_FORMAT_RGB            = 6
}EN_JPEG_INPUT_COLOR_FORMAT;



// the structure of NJPEG_VerificationMode should be the same as NJPD_VerificationMode
typedef enum
{
    E_NJPEG00_NONE,
    E_NJPEG01_TABLE_READ_WRITE,
    E_NJPEG13_ROI,
    E_NJPEG17_OBUF_OUTPUT_FORMAT_YC_SWAP,
    E_NJPEG17_OBUF_OUTPUT_FORMAT_UV_SWAP,
    E_NJPEG17_OBUF_OUTPUT_FORMAT_UV_7BIT,
    E_NJPEG17_OBUF_OUTPUT_FORMAT_UV_MSB,
    E_NJPEG18_IBUF_BURST_LENGTH,
    E_NJPEG21_NO_RESET_TABLE,
    E_NJPEG23_WRITE_PROTECT,
    E_NJPEG25_DOWNSCALE_1_2,
    E_NJPEG25_DOWNSCALE_1_4,
    E_NJPEG25_DOWNSCALE_1_8
}NJPEG_VerificationMode;

//-----------------------------------------------------------------------------
///@brief \b Typedef \b Name: *JPEG_FillHdrFunc
///@brief \b Typedef \b Description: the function for fill header information
///@param <IN> \b BufAddr : fill buffer address
///@param <IN> \b BufLength : fill buffer size
///@return The amount of byte read
//-----------------------------------------------------------------------------
typedef MS_S32 (*JPEG_FillHdrFunc)(MS_PHY BufAddr, MS_U32 BufLength);

//-----------------------------------------------------------------------------
///@brief \b Typedef \b Name: *JPEG_IsrFuncCb
///@brief \b Typedef \b Description: the function handling JPD interrupt
///@param None
///@return None
//-----------------------------------------------------------------------------
typedef void (*JPEG_IsrFuncCb)(void);

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_InitParam
/// @brief \b Struct \b Description: JPEG decode parameter setting
//-----------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_PHY u32MRCBufAddr;       ///< MRC buffer address
    MS_U32 u32MRCBufSize;           ///< MRC buffer size
    MS_PHY u32MWCBufAddr;       ///< MWC buffer address
    MS_U32 u32MWCBufSize;           ///< MWC buffer size
    MS_PHY u32InternalBufAddr;  ///< internal buffer address
    MS_U32 u32InternalBufSize;      ///< internal buffer size
    MS_U32 u32DecByteRead;          ///< how many byte read for JPEG decoder initialization
    MS_BOOL bEOF;                   ///< has read to the end of file
    MS_U8 u8DecodeType;             ///< JPEG decode type : JPEG main, thumbnail, or MJPEG
    MS_BOOL bInitMem;               ///< initialize memory pool or not
    JPEG_FillHdrFunc pFillHdrFunc;  ///< the function for fill header information
} JPEG_InitParam;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_DrvCap
/// @brief \b Struct \b Description: JPEG decode driver capability
//-----------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_U16 u16MaxWidth;     ///< max decoded width for baseline
    MS_U16 u16MaxHeight;    ///< max decoded height for baseline
    MS_U16 u16MaxProWidth;  ///< max decoded width for progressive
    MS_U16 u16MaxProHeight; ///< max decoded height for progressive
    MS_BOOL bBaseline;      ///< support baseline decode
    MS_BOOL bProgressive;   ///< support progressive decode
    MS_BOOL bMJPEG;         ///< support motion JPEG
} JPEG_DrvCap;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_Info
/// @brief \b Struct \b Description: JPEG decode information
//-----------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    const MSIF_Version* pu8DrvVer;  ///< JPEG DRV version
    MS_U8* pu8HalVer;               ///< JPEG HAL version
    MS_U8* pu8FwVer;                ///< JPEG FW version
    JPEG_DrvCap stDrvCap;           ///< JPEG driver capability
} JPEG_Info;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_Status
/// @brief \b Struct \b Description: JPD driver Status
//-----------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_U32  u32CurMRCAddr;  ///< JPD current decoded address
    MS_U16  u16CurVidx;     ///< JPD current decoded vertical index
    MS_U16  u16CurRow;      ///< JPD current decoded row
    MS_U16  u16CurCol;      ///< JPD current decoded column
    MS_BOOL bDrvBusy;       ///< JPD status
    MS_BOOL bIsrEnable;     ///< JPD ISR status
} JPEG_Status;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_EXIF_DateTime
/// @brief \b Struct \b Description: The DateTime info of JPEG EXIF.
//-----------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_U32 u32Year;         ///< The year info of JPEG EXIF DataTime
    MS_U32 u32Month;        ///< The month info of JPEG EXIF DataTime
    MS_U32 u32Day;          ///< The day info of JPEG EXIF DataTime
    MS_U32 u32Hour;         ///< The hour info of JPEG EXIF DataTime
    MS_U32 u32Minute;       ///< The minute info of JPEG EXIF DataTime
    MS_U32 u32Second;       ///< The second info of JPEG EXIF DataTime
    MS_BOOL bHasDataTime;   ///< JPEG EXIF DataTime info exist or not
}JPEG_EXIF_DateTime;

typedef enum
{
    E_DECODE_NONE = 0,
    E_DECODE_DONE,
    E_DECODING,
    E_DECODE_ERR,
    E_STREAM_READ_ERR
} EN_JPEG_DECODE_STATUS;

typedef struct DLL_PACKED
{
    MS_U16 u16Xdensity;      //inch or cm
    MS_U16 u16Ydensity;      //inch or cm
    MS_U8 u8Unit;                  //APP0 Mark units : 0-> no units, X and Y specify the pixel aspect ratio
                                      //                       1 -> X and Y are dots per inch
                                      //                       2 -> X and Y are dots per cm
}APP0_Unit;


typedef struct DLL_PACKED
{
    union
    {
        struct
        {
            MS_U32 numerator;
            MS_U32 denominator;
        };

        struct
        {
            MS_S32 s_numerator;
            MS_S32 s_denominator;
        };
    };
} JPEG_RATIONAL;

typedef struct DLL_PACKED _JPEG_CB_EVENT_PARAM
{
    MS_U32 type;
    MS_PHY BufAddr;
    MS_U32 BufLength;
    MS_S32 param;
}JPEG_CB_EVENT_PARAM, *PJPEG_CB_EVENT_PARAM;

typedef enum
{
    JPEG_CB_EVENT_NONE,
    JPEG_CB_EVENT_FILL_HDR,
    JPEG_CB_EVENT_ISR
}JPEG_CB_EVENT;

#if SUPPORT_MPO_FORMAT

typedef struct DLL_PACKED
{
    MS_U32 attribute;
    MS_U32 size;
    MS_U32 offset;
    MS_U16 image1_no;
    MS_U16 image2_no;
} JPEG_MPO_MP_ENTRY_INFO;


typedef struct DLL_PACKED
{
    MS_U32 start_of_offset;
    MS_U32 num_of_image;
    JPEG_MPO_MP_ENTRY_INFO mp_entry[JPEG_MPO_MAX_SUPPORT_IMAGE];
} JPEG_MPO_INDEX_INFO;


typedef struct DLL_PACKED
{
    MS_U32 MPIndividualNum;
    MS_U32 PanOrientation;
    JPEG_RATIONAL PanOverlap_H;
    JPEG_RATIONAL PanOverlap_V;
    MS_U32 BaseViewpointNum;
    JPEG_RATIONAL ConvergenceAngle;
    JPEG_RATIONAL BaselineLength;
    JPEG_RATIONAL VerticalDivergence;
    JPEG_RATIONAL AxisDistance_X;
    JPEG_RATIONAL AxisDistance_Y;
    JPEG_RATIONAL AxisDistance_Z;
    JPEG_RATIONAL YawAngle;
    JPEG_RATIONAL PitchAngle;
    JPEG_RATIONAL RollAngle;
    MS_BOOL used;
} JPEG_MPO_ATTRIBUTE_INFO;

#endif

//-------------------------------------------------------------------------------------------------
/// Enable oJPD for Edison, only Camera use(to be removed).
/// @ingroup JPEG_oJPD
/// @param bOnOff \b IN : enable/disable
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_EnableOJPD(MS_BOOL bOnOff);

//-------------------------------------------------------------------------------------------------
/// JPEG init oJPD for Edison, only Camera use(to be removed).
/// @ingroup JPEG_oJPD
/// @param pInitParam \b IN : pointer to initial parameter
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_Init_UsingOJPD(JPEG_InitParam *pInitParam);


//-------------------------------------------------------------------------------------------------
/// This function will get APP0 info.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param unit \b OUT : pointer to get the unit info
/// @param x \b OUT : pointer to get the x info
/// @param y \b OUT : pointer to get the y info
/// @return void.
//-------------------------------------------------------------------------------------------------
void msAPI_JPEG_get_APP0_info( MS_U8 *unit,MS_U16 *x, MS_U16 *y );


//-------------------------------------------------------------------------------------------------
/// This function will initialize the parameters of Mstar JPD
/// @ingroup JPEG_Basic
/// @param pInitParam \b IN : pointer to initial parameter
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_Init(JPEG_InitParam *pInitParam);

//-------------------------------------------------------------------------------------------------
/// This function will parse JPEG header
/// @ingroup JPEG_Basic
/// @param void.
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_DecodeHdr(void);

//-------------------------------------------------------------------------------------------------
/// This function will start JPEG decode
/// @ingroup JPEG_Basic
/// @param void.
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_Decode(void);

//-------------------------------------------------------------------------------------------------
/// This function will free all block of JPD and power off JPD IP
/// @ingroup JPEG_Basic
/// @param void.
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_Exit(void);

//-------------------------------------------------------------------------------------------------
/// This function will get JPD error code
/// @ingroup JPEG_Basic
/// @param void.
/// @return JPEG_ErrCode.
//-------------------------------------------------------------------------------------------------
JPEG_ErrCode MApi_JPEG_GetErrorCode(void);

//-------------------------------------------------------------------------------------------------
/// This function will get JPD IP decode event
/// @ingroup JPEG_Basic
/// @param void.
/// @return JPEG_Event.
//-------------------------------------------------------------------------------------------------
JPEG_Event MApi_JPEG_GetJPDEventFlag(void);

//-------------------------------------------------------------------------------------------------
/// This function will set JPD event.(to be removed).
/// @ingroup JPEG_HW_BUG
/// @param eEvtVal
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetJPDEventFlag(JPEG_Event eEvtVal);

//-------------------------------------------------------------------------------------------------
/// This function will reset JPD IP decoding
/// @ingroup JPEG_Basic
/// @param void.
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_Rst(void);

//-------------------------------------------------------------------------------------------------
/// This function will turn on JPD IP.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param void
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_PowerOn(void);

//-------------------------------------------------------------------------------------------------
/// This function will turn off JPD IP.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param void
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_PowerOff(void);

//-------------------------------------------------------------------------------------------------
/// Get current Memory write veritcal index(to be removed).
/// @ingroup JPEG_HW_BUG
/// @param void.
/// @return MS_U16: current Memory write veritcal index
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetCurVidx(void);

//-------------------------------------------------------------------------------------------------
/// This function will check JPEG image is progressive mode or not(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_BOOL: is progressive mode or not
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_IsProgressive(void);

//-------------------------------------------------------------------------------------------------
/// This function will check JPEG image has thumbnail or not(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_BOOL:  has thumbnail or not
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_ThumbnailFound(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the alignment width of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the alignment width of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetWidth(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the alignment Height of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the alignment Height of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetHeight(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the original width of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the original width of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetOriginalWidth(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the original Height of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the original Height of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetOriginalHeight(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the width before alignment(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the width before alignment
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetNonAlignmentWidth(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the Height before alignment(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the Height before alignment
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetNonAlignmentHeight(void);

//-------------------------------------------------------------------------------------------------
/// This function will get displayed pitch of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16: displayed pitch of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetAlignedPitch(void);

//-------------------------------------------------------------------------------------------------
/// Get information for MHEG5 use(to be removed).
/// @ingroup JPEG_MHEG5
/// @param void.
/// @return: MS_U16: information
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetAlignedPitch_H(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the displayed width of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the displayed width of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetAlignedWidth(void);

//-------------------------------------------------------------------------------------------------
/// This function will get the displayed Height of the image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U16:  the displayed Height of the image
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_JPEG_GetAlignedHeight(void);

//-------------------------------------------------------------------------------------------------
/// This function will get scale down factor of JPEG image(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param void
/// @return MS_U8:  scale down factor of JPEG image
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_JPEG_GetScaleDownFactor(void);

//-------------------------------------------------------------------------------------------------
/// This function will set maximum output resolution of JPD baseline mode
/// @ingroup JPEG_Basic
/// @param u16Width \b IN : width
/// @param u16Height \b IN : height
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height);

//-------------------------------------------------------------------------------------------------
/// This function will set maximum output resolution of JPD progressive mode
/// @ingroup JPEG_Basic
/// @param u16ProWidth \b IN : width
/// @param u16ProHeight \b IN : height
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight);

//-------------------------------------------------------------------------------------------------
/// This function will notify JPD IP that the data loaded to low/high portion of read buffer is ready
/// @ingroup JPEG_Basic
/// @param u8MRBuffType \b IN : JPEG_BuffLoadType
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetMRBufferValid(JPEG_BuffLoadType u8MRBuffType);

//-------------------------------------------------------------------------------------------------
/// This function will set JPEG EOF flag and the total byte-read of JPEG file. It is called after loading data to read buffer when JPD IP is decoding
/// @ingroup JPEG_Basic
/// @param u32DataRead \b IN : data be read
/// @param bEOFflag \b IN : if eof
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_UpdateReadInfo(MS_U32 u32DataRead, MS_BOOL bEOFflag);

//-------------------------------------------------------------------------------------------------
/// This function will set JPD IP of low/high portion done event. It is called when EOF.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param u8MRBuffType: IN : JPEG_BuffLoadType
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_ProcessEOF(JPEG_BuffLoadType u8MRBuffType);

//-------------------------------------------------------------------------------------------------
/// This function will set JPD error code when the upper layer encounter some error.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param ErrStatus: IN : JPEG_ErrCode
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetErrCode(JPEG_ErrCode ErrStatus);

//-------------------------------------------------------------------------------------------------
/// This function will enable/disable JPD API debug message
/// @ingroup JPEG_to_be_removed
/// @param u8DbgLevel: IN : debug level
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel);

//-------------------------------------------------------------------------------------------------
/// This function will get current dbg level.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param void.
/// @return MS_U8: dbg level
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_JPEG_GetDbgLevel(void);

//-------------------------------------------------------------------------------------------------
/// This function will get jpeg info.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param pJPEG_Info: OUT : pointer to jpeg info
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_GetInfo(JPEG_Info *pJPEG_Info);

//-------------------------------------------------------------------------------------------------
/// This function will get jpeg status.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param pJPEG_Status: OUT : pointer to jpeg status
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_GetStatus(JPEG_Status *pJPEG_Status);

//-------------------------------------------------------------------------------------------------
/// This function will get lib info.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param ppVersion OUT : pointer to lib version's pointer
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// This function will check current vertical index of JPD and handle a timeout of JPD IP decoding.(to be removed).
/// @ingroup JPEG_HW_BUG
/// @param void.
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_HdlVidxChk(void);

//-------------------------------------------------------------------------------------------------
/// This function will get buffer load type for preloading data when JPD IP is decoding
/// @ingroup JPEG_Basic
/// @pBuffLoadType u32DataRead \b IN : JPEG_BuffLoadType
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetBuffLoadType(JPEG_BuffLoadType *pBuffLoadType);

//-------------------------------------------------------------------------------------------------
/// This function will enable ISR.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param IsrCb: callback function
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_EnableISR(JPEG_IsrFuncCb IsrCb);

//-------------------------------------------------------------------------------------------------
/// This function will disable ISR.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param void.
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_DisableISR(void);

//-------------------------------------------------------------------------------------------------
/// wait decode done and get decode status, this API is only for bringup/FPGA use
/// @ingroup JPEG_Debug
/// @param void.
/// @return: JPEG_DecodeStatus
//-------------------------------------------------------------------------------------------------
JPEG_DecodeStatus MApi_JPEG_WaitDone(void);

//-------------------------------------------------------------------------------------------------
/// This function will get date time of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param DateTime: This function will get date time of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFDateTime(JPEG_EXIF_DateTime *DateTime);

//-------------------------------------------------------------------------------------------------
/// This function will get Orientation of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param eOrientation: This function will get Orientation of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFOrientation(JPEG_EXIF_Orientation *eOrientation);
#if SUPPORT_EXIF_EXTRA_INFO

//-------------------------------------------------------------------------------------------------
/// This function will get Manufacturer of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu8Manufacturer: This function will get Manufacturer of JPEG EXIF
/// @param u8size: size of Manufacturer
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFManufacturer(MS_U8 *pu8Manufacturer, MS_U8 u8size);

//-------------------------------------------------------------------------------------------------
/// This function will get Model of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu8Model: This function will get Model of JPEG EXIF
/// @param u8size: size of Model
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFModel(MS_U8 *pu8Model, MS_U8 u8size);

//-------------------------------------------------------------------------------------------------
/// This function will get Flash of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu16Flash: This function will get Flash of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFFlash(MS_U16 *pu16Flash);

//-------------------------------------------------------------------------------------------------
/// This function will get ISOSpeedRatings of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu32ISOSpeedRatings: This function will get ISOSpeedRatings of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFISOSpeedRatings(MS_U32 *pu32ISOSpeedRatings);

//-------------------------------------------------------------------------------------------------
/// This function will get ShutterSpeedValue of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pShutterSpeedValue: This function will get ShutterSpeedValue of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFShutterSpeedValue(JPEG_RATIONAL *pShutterSpeedValue);

//-------------------------------------------------------------------------------------------------
/// This function will get pApertureValue of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pApertureValue: This function will get ApertureValue of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFApertureValue(JPEG_RATIONAL *pApertureValue);

//-------------------------------------------------------------------------------------------------
/// This function will get ExposureBiasValue of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pExposureBiasValue: This function will get ExposureBiasValue of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFExposureBiasValue(JPEG_RATIONAL *pExposureBiasValue);

//-------------------------------------------------------------------------------------------------
/// This function will get FocalLength of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pFocalLength: This function will get FocalLength of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFFocalLength(JPEG_RATIONAL *pFocalLength);

//-------------------------------------------------------------------------------------------------
/// This function will get ImageWidth of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu32ImageWidth: This function will get ImageWidth of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFImageWidth(MS_U32 *pu32ImageWidth);

//-------------------------------------------------------------------------------------------------
/// This function will get ImageHeight of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pu32ImageHeight: This function will get ImageHeight of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFImageHeight(MS_U32 *pu32ImageHeight);

//-------------------------------------------------------------------------------------------------
/// This function will get ExposureTime of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pExposureTime: This function will get ExposureTime of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFExposureTime(JPEG_RATIONAL *pExposureTime);

//-------------------------------------------------------------------------------------------------
/// This function will get FNumber of JPEG EXIF(to be JPEG_to_be_integrated by get ctrl function).
/// @ingroup JPEG_to_be_integrated
/// @param pFNumber: This function will get FNumber of JPEG EXIF
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetEXIFFNumber(JPEG_RATIONAL *pFNumber);
#endif

//-------------------------------------------------------------------------------------------------
/// This function will get the jpeg information
/// @ingroup JPEG_Basic
/// @EN_JPEG_GET_CTRL_ID eGetID \b IN : control ID
/// @param param \b IN : control info
/// @param u32Size\b IN : control info size
/// @return JPEG_Result
///     - E_JPEG_FAILED: The result of API execution is failed
///     - E_JPEG_OKAY: The result of API execution is successful
///     - E_JPEG_DONE: For CMYK/progressive decoding, it means one line decode done
///     - E_JPEG_RETRY: Cannot get any information, need to re-call API
//-------------------------------------------------------------------------------------------------
JPEG_Result MApi_JPEG_GetControl(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// This function would disable address convert for MJPEG use(to be removed).
/// @ingroup JPEG_MJPEG
/// @param void
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_DisableAddressConvert(void);
//-------------------------------------------------------------------------------------------------
/// This function will get free memory to use.(to be removed).
/// @ingroup JPEG_HW_BUG
/// @param size
/// @return MS_PHYADDR
//-------------------------------------------------------------------------------------------------
MS_PHY MApi_JPEG_GetFreeMemory(MS_U32 size);
//-------------------------------------------------------------------------------------------------
/// Get current data offset(to be removed).
/// @ingroup JPEG_HW_BUG
/// @param void.
/// @return MS_U32: current data offset
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_JPEG_GetDataOffset(void);

//-------------------------------------------------------------------------------------------------
/// This function will sof offset.(to be removed).
/// @ingroup JPEG_to_be_removed
/// @param void
/// @return MS_U32: offset
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_JPEG_GetSOFOffset(void);

//-------------------------------------------------------------------------------------------------
/// This function would set NJPD number for MJPEG use
/// @ingroup JPEG_MJPEG
/// @param JPDNum: JPD number
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetNJPDInstance(MS_U8 JPDNum);
JPEG_Result MApi_JPEG_SupportCMYK(MS_BOOL bEnable);
JPEG_Result MApi_JPEG_SupportRGB(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable MHEG5 function(to be removed).
/// @ingroup JPEG_MHEG5
/// @param bEnable \b IN : enable/disable
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetMHEG5(MS_BOOL bEnable);

#if SUPPORT_MPO_FORMAT
//For MPO

//-------------------------------------------------------------------------------------------------
/// This function would check if it is MPO format
/// @ingroup JPEG_MPO
/// @param void
/// @return BOOL: MPO:1, not MPO: 0
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_IsMPOFormat(void);

//-------------------------------------------------------------------------------------------------
/// This function would get MPO index information
/// @ingroup JPEG_MPO
/// @param ppMPOIndex: pointer to MPO index info pointer
/// @return BOOL: ok:1, fail: 0
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_GetMPOIndex(JPEG_MPO_INDEX_INFO **ppMPOIndex);

//-------------------------------------------------------------------------------------------------
/// This function would get MPO attribute information
/// @ingroup JPEG_MPO
/// @param ppMPOIndex: pointer to MPO attribute info pointer
/// @return BOOL: ok:1, fail: 0
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_GetMPOAttr(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr);

//-------------------------------------------------------------------------------------------------
/// This function would print MPO information
/// @ingroup JPEG_MPO
/// @param void
/// @return void
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_DumpMPO(void);

//-------------------------------------------------------------------------------------------------
/// This function would set MPO Buffer information (to be removed)
/// @ingroup JPEG_MPO
/// @param read_offset \b IN : read_offset
/// @param output_start \b IN : output_start
/// @return BOOL: ok:1, fail: 0
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_SetMPOBuffer(MS_U32 read_offset, MS_U32 output_start);

//-------------------------------------------------------------------------------------------------
/// This function would set MPO max decode resolution(baseline)
/// @ingroup JPEG_MPO
/// @param u16Width \b IN : width
/// @param u16Height \b IN : height
/// @return void
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetMPOMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height);

//-------------------------------------------------------------------------------------------------
/// This function would set MPO max decode resolution(progressive)
/// @ingroup JPEG_MPO
/// @param u16ProWidth \b IN : width
/// @param u16ProHeight \b IN : height
/// @return void
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetMPOProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight);
#endif

//-------------------------------------------------------------------------------------------------
/// set verification mode, this API is only for bringup/FPGA use
/// @ingroup JPEG_Debug
/// @param IN : NJPEG_VerificationMode.
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_JPEG_SetVerificationMode(NJPEG_VerificationMode VerificationMode);

//-------------------------------------------------------------------------------------------------
/// get current verification mode, this API is only for bringup/FPGA use
/// @ingroup JPEG_Debug
/// @param void.
/// @return: NJPEG_VerificationMode.
//-------------------------------------------------------------------------------------------------
NJPEG_VerificationMode MApi_JPEG_GetVerificationMode(void);

//-------------------------------------------------------------------------------------------------
/// print the debug message
/// @ingroup JPEG_Debug
/// @param void.
/// @return void.
//-------------------------------------------------------------------------------------------------
void MApi_NJPD_Debug(void);

//-------------------------------------------------------------------------------------------------
/// This function would check if it is nJPD or oJPD for MJPEG use
/// @ingroup JPEG_MJPEG
/// @param void
/// @return BOOL: nJPD:1, oJPD: 0
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_JPEG_IsNJPD(void);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
#endif // _API_JPEG_H_
//------------------------------------------------------------------------------
