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
///////////////////////////////////////////////////////////////////////////////
///
/// @brief  MStar Graphics Driver Interface
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup GFX GFX interface

  \brief
  GE:Graphic Engine \n\n
  Basic primitives supported:\n
  Line draw: Draw a straight line \n
  Rect fill(Rectangular Fill) : Draw rectangle \n
  Bitblt(Bit Transfer Block) : Memory copy \n

  <b> GFX Init Flow: </b> \n
  \image html apiGFX_pic1.png
  \n\n
  <b> GFX Execute procedure: </b> \n
  \image html apiGFX_pic2.png

  *! \defgroup GFX_INIT GFX init related functions
  *   \ingroup   GFX

  *! \defgroup GFX_CAPS GFX get capability related functions
  *   \ingroup   GFX

  *! \defgroup GFX_CB GFX call back related functions
  *   \ingroup   GFX

  *! \defgroup GFX_DRAW_SETTING GFX draw setting related functions
  *   \ingroup   GFX

  *! \defgroup GFX_LOCK_BEGINE_END GFX lock begine/end related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_BUFFER_INFO GFX buffer info related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_COLOR_KEY GFX color key related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_ROP GFX ROP related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_ALPHA_COMPARE GFX alpha compare related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_ALPHA_BLENDING GFX alpha blending related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_DFB GFX DFB related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_PALETTE GFX palette related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_CLIP GFX clip window related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_DRAW GFX draw related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_DRAW_MISC GFX draw misc related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_WAIT_DONE GFX wait done related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_CLEAR GFX clear buffer related functions
  *   \ingroup   GFX_DRAW_SETTING

  *! \defgroup GFX_OPTION GFX option related functions
  *   \ingroup   GFX

  *! \defgroup GFX_STRETCH GFX stretch related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_MIRROR GFX mirror related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_ROTATE GFX rotate related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_DITHER GFX dither related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_R2Y GFX r2y related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_WRITE_PROTECT GFX write protect related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_VCMQ GFX virtual CMDQ related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_PIXEL_MODE GFX pixel mode related functions
  *   \ingroup   GFX_OPTION

  *! \defgroup GFX_POWER_STATE GFX power state related functions
  *   \ingroup   GFX

  *! \defgroup GFX_MISC GFX misc related functions
  *   \ingroup   GFX

  *! \defgroup GFX_MISC_NOS GFX misc nos related functions
  *   \ingroup   GFX

  *! \defgroup GFX_MISC_BOX GFX misc box related functions
  *   \ingroup   GFX

  *! \defgroup GFX_ToBeRemove GFX to be remove related functions
  *   \ingroup   GFX

  */
//-------------------------------------------------

#ifndef _API_GFX_H_
#define _API_GFX_H_

//#include "drvGE.h"
//#define MAX_FONT                    20  //5
//#define MAX_BITMAP                  127

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

// For CHAKRA
#define GE_ALIGNED_VALUE(value, align)  ( ((value) + ((align)-1)) & ~((align)-1) )
#define GE_ADDR_ALIGNMENT(addr)         GE_ALIGNED_VALUE(addr, 8)    // 8-byte alignment

// END CHAKRA

#define ERR_HANDLE                  -1
#define ERR_NULL_INDEX                  0xFFFFUL

#define MSIF_GFX_LIB_CODE               {'G','F','X','_'} //Lib code
#define MSIF_GFX_LIBVER                 {'1','9'}      //LIB version
#define MSIF_GFX_BUILDNUM               {'1','9' }    //Build Number
#define MSIF_GFX_CHANGELIST             {'0','0','6','1','8','4','6','4'} //P4 ChangeList Number

#define GFX_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_GFX_LIB_CODE,                  /* IP__                                             */  \
    MSIF_GFX_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_GFX_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_GFX_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//GE return code
/// GFX API return value
typedef enum
{
    GFX_FAIL                        = 0,
    GFX_SUCCESS                        ,
    GFX_NON_ALIGN_ADDRESS             ,
    GFX_NON_ALIGN_PITCH               ,
    GFX_INVALID_PARAMETERS            ,
    /// not support
    GFX_DRV_NOT_SUPPORT               ,
    /// pixel format error
    GFX_DRV_FAIL_FORMAT               ,
    /// bitblit start address error
    GFX_DRV_FAIL_BLTADDR              ,
    /// bitblt overlap (if STRETCH, ITALIC, MIRROR, ROTATE)
    GFX_DRV_FAIL_OVERLAP              ,
    /// stretch bitblt fail
    GFX_DRV_FAIL_STRETCH              ,
    /// italic bitblt fail (if MIRROR, ROTATE)
    GFX_DRV_FAIL_ITALIC               ,
    /// engine is locked by others
    GFX_DRV_FAIL_LOCKED               ,
    /// primitive will not be drawn
    GFX_DRV_NOT_DRAW                   ,
    /// Dependent functions are not linked
    GFX_DRV_NO_DEPENDENT              ,
    /// context not inited.
    GFX_DRV_CTXMAG_FAIL               ,

    GFX_INVALID_INTENSITY_ID = 0x100,
    GFX_INVALID_BUFF_INFO             ,
    GFX_INVALID_FONT_HANDLE           ,
    GFX_INVALID_BMP_HANDLE            ,
    GFX_INIT_FAIL                                  ,
} GFX_Result;


///Define Raster Operation
typedef enum
{
    /// rop_result = 0;
    ROP2_OP_ZERO                    = 0,
    /// rop_result = ~( rop_src | rop_dst );
    ROP2_OP_NOT_PS_OR_PD            ,
    /// rop_result = ((~rop_src) & rop_dst);
    ROP2_OP_NS_AND_PD               ,
    /// rop_result = ~(rop_src);
    ROP2_OP_NS                      ,
    /// rop_result = (rop_src & (~rop_dst));
    ROP2_OP_PS_AND_ND               ,
    /// rop_result = ~(rop_dst);
    ROP2_OP_ND                      ,
    /// rop_result = ( rop_src ^ rop_dst);
    ROP2_OP_PS_XOR_PD               ,
    /// rop_result = ~(rop_src & rop_dst);
    ROP2_OP_NOT_PS_AND_PD           ,
    /// rop_result = (rop_src & rop_dst);
    ROP2_OP_PS_AND_PD               ,
    /// rop_result = ~(rop_dst ^ rop_src);
    ROP2_OP_NOT_PS_XOR_PD           ,
    /// rop_result = rop_dst;
    ROP2_OP_PD                      ,
    /// rop_result = (rop_dst | (~rop_src));
    ROP2_OP_NS_OR_PD                ,
    /// rop_result = rop_src;
    ROP2_OP_PS                      ,
    /// rop_result = (rop_src | (~rop_dst));
    ROP2_OP_PS_OR_ND                ,
    /// rop_result = (rop_dst | rop_src);
    ROP2_OP_PD_OR_PS                ,
    /// rop_result = 0xffffff;
    ROP2_OP_ONE                     ,
} GFX_ROP2_Op;


/// Color Key Operation Mode
typedef enum
{
    /// If EQUAL then perform CK operation
    CK_OP_EQUAL                     = 0,
    /// If NOT EQUAL then perform CK operation
    CK_OP_NOT_EQUAL                 = 1,
    /// If EQUAL then perform Alpha Key operation
    AK_OP_EQUAL                     = 2,
    /// If NOT EQUAL then Alpha Key operation
    AK_OP_NOT_EQUAL                 = 3,
} GFX_ColorKeyMode;

///Define Blending Coefficient
typedef enum
{
    /// Csrc
    COEF_ONE                        = 0,
    /// Csrc * Aconst + Cdst * (1 - Aconst)
    COEF_CONST                      ,
    ///  Csrc * Asrc + Cdst * (1 - Asrc)
    COEF_ASRC                       ,
    /// Csrc * Adst + Cdst * (1 - Adst)
    COEF_ADST                       ,

    /// Cdst
    COEF_ZERO                       ,
    /// Csrc * (1 - Aconst) + Cdst * Aconst
    COEF_1_CONST                    ,
    /// Csrc * (1 - Asrc) + Cdst * Asrc
    COEF_1_ASRC                     ,
    ///  Csrc * (1 - Adst) + Cdst * Adst
    COEF_1_ADST                     ,

    /// ((Asrc * Aconst) * Csrc + (1-(Asrc *Aconst)) * Cdst) / 2
    COEF_ROP8_ALPHA                 ,
    /// ((Asrc * Aconst) * Csrc + Adst * Cdst * (1-(Asrc * Aconst))) / (Asrc * Aconst) + Adst * (1- Asrc * Aconst))
    COEF_ROP8_SRCOVER               ,
    /// ((Asrc * Aconst) * Csrc * (1-Adst) + Adst * Cdst) / (Asrc * Aconst) * (1-Adst) + Adst)
    COEF_ROP8_DSTOVER               ,

    /// Csrc * Aconst
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_CONST_SRC</em></b>
    COEF_CONST_SRC              ,
    /// Csrc * (1 - Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_1_CONST_SRC</em></b>
    COEF_1_CONST_SRC            ,

    /// Csrc * Adst * Asrc * Aconst + Cdst * Adst * (1 - Asrc * Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_SRC_ATOP_DST</em></b>
    COEF_SRC_ATOP_DST               ,
    /// Cdst * Asrc * Aconst * Adst + Csrc * Asrc * Aconst * (1 - Adst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_DST_ATOP_SRC</em></b>
    COEF_DST_ATOP_SRC               ,
    /// (1 - Adst) * Csrc * Asrc * Aconst + Adst * Cdst * (1 - Asrc * Aconst)
    ///@note
    /// <b>[URANUS] <em>It does not support COEF_SRC_XOR_DST</em></b>
    COEF_SRC_XOR_DST                ,
} GFX_BlendCoef;

///Define Blending Source from
typedef enum
{
    /// constant
    ABL_FROM_CONST                  = 0,
    /// source alpha
    ABL_FROM_ASRC                   ,
    /// destination alpha
    ABL_FROM_ADST                   ,
    /// Aout = Asrc*Aconst
    ABL_FROM_ROP8_SRC       ,
    /// Aout = Asrc*Aconst * Adst
    ABL_FROM_ROP8_IN  ,
    /// Aout = (1-Asrc*Aconst) * Adst
    ABL_FROM_ROP8_DSTOUT  ,
    /// Aout = (1-Adst) * Asrc*Aconst
    ABL_FROM_ROP8_SRCOUT           ,
    /// Aout = (Asrc*Aconst) + Adst*(1-Asrc*Aconst) or (Asrc*Aconst)*(1-Adst) + Adst
    ABL_FROM_ROP8_OVER             ,

    /// 1 - Aconst
    ABL_FROM_ROP8_INV_CONST       ,
    /// 1 - Asrc
    ABL_FROM_ROP8_INV_ASRC         ,
    /// 1 - Adst
    ABL_FROM_ROP8_INV_ADST         ,
    /// Adst * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A atop B
    ABL_FROM_ROP8_SRC_ATOP_DST    ,
    /// Asrc * Aconst * Adst + Asrc * Aconst * (1 - Adst) B atop A
    ABL_FROM_ROP8_DST_ATOP_SRC    ,
    /// (1 - Adst) * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A xor B
    ABL_FROM_ROP8_SRC_XOR_DST     ,
    /// Asrc * Asrc * Aconst + Adst * (1 - Asrc * Aconst)
    ABL_FROM_ROP8_INV_SRC_ATOP_DST     ,
    /// Asrc * (1 - Asrc * Aconst) + Adst * Asrc * Aconst
    ABL_FROM_ROP8_INV_DST_ATOP_SRC
} GFX_AlphaSrcFrom;


///Define Colorkey Mode
typedef enum
{
    /// max(Asrc,Adst)
    GFX_ACMP_OP_MAX                = 0,
    /// min(Asrc,Adst)
    GFX_GE_ACMP_OP_MIN                = 1,

} GFX_ACmpOp;


/// GE buffer format
typedef enum
{
    /// font mode I1
    GFX_FMT_I1                      = E_MS_FMT_I1,
    /// font mode I2
    GFX_FMT_I2                      = E_MS_FMT_I2,
    /// font mode I4
    GFX_FMT_I4                      = E_MS_FMT_I4,
    /// color format palette 256(I8)
    GFX_FMT_I8                      = E_MS_FMT_I8,
    /// color format RGB332  sync with MsType.h E_MS_FMT_RGB332
    GFX_FMT_RGB332                 = 0x5,
     /// color format blinking display
    GFX_FMT_FABAFGBG2266 = E_MS_FMT_FaBaFgBg2266,
    /// Uranus GOP only support this blinking
    /// color format for blinking display format
    GFX_FMT_1ABFGBG12355            = E_MS_FMT_1ABFgBg12355,
    /// color format RGB565
    GFX_FMT_RGB565                  = E_MS_FMT_RGB565,
    // color format ORGB1555
    GFX_FMT_ARGB1555                = E_MS_FMT_ARGB1555,
    // color format ARGB4444
    GFX_FMT_ARGB4444                = E_MS_FMT_ARGB4444,
    // color format for blinking display format
    // Uranus GOP does NOT support this blinking format
    // GE_FMT_1BAAFGBG123433       = 0xb,
    // @FIXME: Venus does not have this color format. Need to take care of it.
    /// color format ARGB1555 DST
    GFX_FMT_ARGB1555_DST            = E_MS_FMT_ARGB1555_DST,
    /// color format YUV422
    GFX_FMT_YUV422                  = E_MS_FMT_YUV422,
    /// color format ARGB8888
    GFX_FMT_ARGB8888                = E_MS_FMT_ARGB8888,
    /// color format RGBA5551
    GFX_FMT_RGBA5551                = E_MS_FMT_RGBA5551,
     /// color format RGBA4444
    GFX_FMT_RGBA4444               = E_MS_FMT_RGBA4444,
    /// color format ABGR8888
    GFX_FMT_ABGR8888                = E_MS_FMT_ABGR8888,
    /// New Color Format
    /// color format BGRA5551
    GFX_FMT_BGRA5551               = E_MS_FMT_BGRA5551,
    /// color format ABGR1555
    GFX_FMT_ABGR1555               = E_MS_FMT_ABGR1555,
    /// color format ABGR4444
    GFX_FMT_ABGR4444               = E_MS_FMT_ABGR4444,
    /// color format BGRA4444
    GFX_FMT_BGRA4444               = E_MS_FMT_BGRA4444,
    /// color format BGR565
    GFX_FMT_BGR565               = E_MS_FMT_BGR565,
    /// color format RGBA8888
    GFX_FMT_RGBA8888               = E_MS_FMT_RGBA8888,
    /// color format RGBA8888
    GFX_FMT_BGRA8888               = E_MS_FMT_BGRA8888,
    /// color format ACrYCb444 sync with MsType.h E_MS_FMT_ACRYCB444
    GFX_FMT_ACRYCB444              = 0x18,
    /// color format CrYCbA444 sync with MsType.h E_MS_FMT_CRYCBA444
    GFX_FMT_CRYCBA444              = 0x19,
    /// color format ACbYCr444 sync with MsType.h E_MS_FMT_ACBYCR444
    GFX_FMT_ACBYCR444              = 0x1a,
    /// color format CbYCrA444 sync with MsType.h E_MS_FMT_CBYCRA444
    GFX_FMT_CBYCRA444              = 0x1b,
} GFX_Buffer_Format;

///Define RGB2YUV conversion formula
typedef enum
{
    GFX_YUV_OP1            ,  // GFX_YUV_Rgb2Yuv
    GFX_YUV_OP2            ,  // GFX_YUV_OutRange
    GFX_YUV_OP3            ,  // GE_YUV_InRange
    GFX_YUV_OP4            ,  // GFX_YUV_422
} GFX_YUV_OpType;

///Define RGB2YUV conversion formula
typedef enum
{
    GFX_YUV_RGB2YUV_PC             = 0,
    GFX_YUV_RGB2YUV_255            = 1,

} GFX_YUV_Rgb2Yuv;

///Define output YUV color domain
typedef enum
{
    GFX_YUV_OUT_255                = 0,
    GFX_YUV_OUT_PC                 = 1,

} GFX_YUV_OutRange;

///Define input YUV color domain
typedef enum
{
    GFX_YUV_IN_255                 = 0,
    GFX_YUV_IN_127                 = 1,

} GFX_YUV_InRange;

///Define YOU 422 format
typedef enum
{
    GFX_YUV_YVYU                   = 0,
    GFX_YUV_YUYV                   = 1,
    GFX_YUV_VYUY                   = 2,
    GFX_YUV_UYVY                   = 3,

} GFX_YUV_422;

///Define paramter for YUV operation
typedef struct
{
    GFX_YUV_Rgb2Yuv                  rgb2yuv;
    GFX_YUV_OutRange                 out_range;
    GFX_YUV_InRange                  in_range;
    GFX_YUV_422                      dst_fmt;
    GFX_YUV_422                      src_fmt;
} GFX_YUVMode;


//-------------------------------------------------
/// Define Virtual Command Buffer Size
typedef enum
{
    /// 4K
    GFX_VCMD_4K = 0,
    /// 8K
    GFX_VCMD_8K = 1,
    /// 16K
    GFX_VCMD_16K = 2,
    /// 32K
    GFX_VCMD_32K = 3,
    /// 64K
    GFX_VCMD_64K = 4,
    /// 128K
    GFX_VCMD_128K = 5,
    /// 256K
    GFX_VCMD_256K = 6,
    /// 512K
    GFX_VCMD_512K = 7,
    /// 1024k
    GFX_VCMD_1024K = 8,
} GFX_VcmqBufSize;



//-------------------------------------------------
/// Define RGB color in LE
typedef struct DLL_PACKED
{
    /// Blue
    MS_U8 b;
    /// Green
    MS_U8 g;
    /// Red
    MS_U8 r;
    /// Alpha
    MS_U8 a;
} GFX_RgbColor;

/// Define YUV color
typedef struct
{
    MS_U32         Y:8;
    MS_U32         U:4;
    MS_U32         V:4;
} GFX_YuvColor;


/// General GFX color type, union of rgb and yuv color.
#ifndef MSOS_TYPE_OPTEE
typedef struct{
    union{
        GFX_RgbColor    rgb;
        GFX_YuvColor    yuv;
    };
} GFX_Color;
#endif
//-------------------------------------------------
/// Define the start color & end color
typedef struct DLL_PACKED
{
    /// start color
    GFX_RgbColor color_s;
    /// end color
    GFX_RgbColor color_e;
} GFX_ColorRange;

//-------------------------------------------------
/// Define the position of one point.
typedef struct DLL_PACKED
{
    /// X address
    MS_U16 x;
    /// Y address
    MS_U16 y;
} GFX_Point;

//-------------------------------------------------
/// Specify the blink data
        //          1 A B Fg Bg
        //          1 2 3  5  5
#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    /// BG color (for blink mode use)
    MS_U8                  background;             // 5 bits
    /// FG color (for blink mode use)
    MS_U8                  foreground;             // 5 bits
    /// Control flag\n
    union{
        MS_U16             ctrl_flag;
        struct{
            MS_U16         Blink:3;                // 3 bits
            MS_U16         Alpha:2;                // 2 bits
            MS_U16         blink_en:1;
            MS_U16         reserved1:10;
        } Bits;
        struct{
            MS_U16 BlinkAlpha:4;
            MS_U16 Alpha:2;
            MS_U16 Blink:3;
            MS_U16 reserved:7;
        }Bits2;
        struct{
            MS_U16 Fa:2;
            MS_U16 Ba:2;
            MS_U16 reserved:12;

        }Bits3;
    };


} GFX_BlinkData;
#endif

/// Define the start blink color & end blink color.
#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    /// start blink color
    GFX_BlinkData blink_data_s;
    /// end blink color
    GFX_BlinkData blink_data_e;
} GFX_BlinkDataRange;
#endif

//-------------------------------------------------
//-------------------------------------------------
#define GFXLINE_FLAG_COLOR_CONSTANT             (0x00000000UL)  /// Constant color
#define GFXLINE_FLAG_COLOR_GRADIENT             (0x00000001UL)  /// Gradient color
#define GFXLINE_FLAG_DRAW_LASTPOINT             (0x40000000UL)  /// Otherwide, not draw last point
#define GFXLINE_FLAG_INWIDTH_IS_OUTWIDTH        (0x80000000UL)  /// Otherwise, output width is input width +1

/// Define the attribute of line.
#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    /// start X address
    MS_U16 x1;
    /// Start Y address
    MS_U16 y1;
    /// End X address
    MS_U16 x2;
    /// End Y address
    MS_U16 y2;
    /// Color format
    GFX_Buffer_Format fmt;
    union
    {
        /// Color
        ///For all RGB color, the color set as the ARGB8888 format.\n
        ///Each color component need to shift to high bit.\n
        ///Use ARGB1555 as the example, the source color key as the following:\n
        ///ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
        ///colorRange.color_s --> A0000000RRRRR000GGGGG000BBBBB000   (every character represents one bit)\n\n
        ///For GFX_FMT_I8 format, the index set to b component (colorRange.color_s.b = b).\n
        GFX_ColorRange       colorRange;
        ///Blink attribute
        ///For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
        GFX_BlinkDataRange  blkDataRange;
    };
    /// Line width
    MS_U32 width;
    /// GFXLINE_FLAG_COLOR_CONSTANT: Constant color\n
    /// GFXLINE_FLAG_COLOR_GRADIENT: Gradient color\n
    /// GFXLINE_FLAG_DRAW_LASTPOINT: Draw last point, otherwise not draw\n
    /// GFXLINE_FLAG_INWIDTH_IS_OUTWIDTH: Input width is output width, not input width +1
    MS_U32 flag;

} GFX_DrawLineInfo;
#endif
//-------------------------------------------------

/// Define the dimension of one block
typedef struct DLL_PACKED
{
    /// X start address
    MS_U16 x;
    /// Y start address
    MS_U16 y;
    /// width
    MS_U16 width;
    /// height
    MS_U16 height;
} GFX_Block;

/*******************************************************************
// Define the Trapezoid                                            *
// [u16X0, u16Y0]: The Left point of top of Trapezoid;             *
// [u16X1, u16Y1]: The Left point of bottom of Trapezoid;          *
// [u16DeltaTop]: The Width of top of Trapezoid;                   *
// [u16DeltaBottom]: The Width of bottom of Trapezoid;             *
// To Simplify, For DIRECTION_X: u16Y0 < u16Y1                     *
// To Simplify, For DIRECTION_Y: u16X0 > u16X1                     *
********************************************************************/
typedef struct DLL_PACKED
{
    /// x0 start address
    MS_U16                          u16X0;
    /// y0 start address
    MS_U16                          u16Y0;
    /// x1 start address
    MS_U16                          u16X1;
    /// y1 start address
    MS_U16                          u16Y1;
    /// delta of Top
    MS_U16                          u16DeltaTop;
    // delta of Bottom
    MS_U16                          u16DeltaBottom;
} GFX_Trapezoid;

//-------------------------------------------------

#define GFXRECT_FLAG_COLOR_CONSTANT     0x0UL
#define GFXRECT_FLAG_COLOR_GRADIENT_X   0x1UL
#define GFXRECT_FLAG_COLOR_GRADIENT_Y   0x2UL
#define GFXRECT_FLAG_TRAPE_DIRECTION_X  0x4UL //excluded with GFXRECT_FLAG_TRAPE_DIRECTION_Y
#define GFXRECT_FLAG_TRAPE_DIRECTION_Y  0x8UL //excluded with GFXRECT_FLAG_TRAPE_DIRECTION_X

/// Define the info. of one block.
#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    /// dst block info
    union
    {
        GFX_Block       dstBlock;
        GFX_Trapezoid   dstTrapezoidBlk;
    };
    /// Color format
    GFX_Buffer_Format fmt;
    union
    {
        /// Color
        ///For all RGB color, the color set as the ARGB8888 format.\n
        ///Each color component need to shift to high bit.\n
        ///Use ARGB1555 as the example, the source color key as the following:\n
        ///ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
        ///color_s   --> A0000000RRRRR000GGGGG000BBBBB000   (every character represents one bit)\n\n
        ///For GFX_FMT_I8 format, the index set to b component (colorRange.color_s.b = b).\n
        GFX_ColorRange       colorRange;
        ///Blink attribute
        ///For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
        GFX_BlinkDataRange  blkDataRange;
    };

    /// GERECT_FLAG_COLOR_CONSTANT: Constant color\n
    /// GERECT_FLAG_COLOR_GRADIENT_X: X direction gradient color\n
    /// GERECT_FLAG_COLOR_GRADIENT_Y: Y direction gradient color\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_X: for trapezoid, top & bottom in X direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_Y\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_Y: for trapezoid, top & bottom in Y direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_X\n
    MS_U32 flag;

} GFX_RectFillInfo;
#endif

#ifndef MSOS_TYPE_OPTEE
typedef struct
{
    GFX_RectFillInfo *BatchFillInfo;
    MS_U32 u32BatchNum;
} GFX_BatchRectFillInfo;
#endif

/// Define structure for OVAL drawing.
#ifndef MSOS_TYPE_OPTEE
typedef struct
{
    /// dst block info
    GFX_Block       dstBlock;
    /// Color format
    GFX_Buffer_Format fmt;
    union{
        GFX_RgbColor color;
        GFX_BlinkData blink_data;
    };
    MS_U32 u32LineWidth;
} GFX_OvalFillInfo;
#endif

typedef struct DLL_PACKED
{
     int            x0;  /* X coordinate of first edge */
     int            y0;  /* Y coordinate of first edge */
     int            x1;  /* X coordinate of second edge */
     int            y1;  /* Y coordinate of second edge */
     int            x2;  /* X coordinate of third edge */
     int            y2;  /* Y coordinate of third edge */
} GFX_Triangle;

#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    GFX_Triangle tri;
    GFX_Block    clip_box;
    GFX_Buffer_Format fmt;
    union
    {
        /// Color
        ///For all RGB color, the color set as the ARGB8888 format.\n
        ///Each color component need to shift to high bit.\n
        ///Use ARGB1555 as the example, the source color key as the following:\n
        ///ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
        ///color_s   --> A0000000RRRRR000GGGGG000BBBBB000   (every character represents one bit)\n\n
        ///For GFX_FMT_I8 format, the index set to b component (colorRange.color_s.b = b).\n
        GFX_ColorRange       colorRange;
        ///Blink attribute
        ///For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
        GFX_BlinkDataRange  blkDataRange;
    };

    /// GERECT_FLAG_COLOR_CONSTANT: Constant color\n
    /// GERECT_FLAG_COLOR_GRADIENT_X: X direction gradient color\n
    /// GERECT_FLAG_COLOR_GRADIENT_Y: Y direction gradient color\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_X: for trapezoid, top & bottom in X direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_Y\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_Y: for trapezoid, top & bottom in Y direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_X\n
    MS_U32 flag;
} GFX_TriFillInfo;
#endif

typedef struct DLL_PACKED
{
    int            x;   /* X coordinate */
    int            w;   /* width of span */
} Span;

typedef struct DLL_PACKED
{
     int y;
     Span *spans;
     int num_spans;
} GFX_Span;

#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    GFX_Span     span;
    GFX_Block    clip_box;
    GFX_Buffer_Format fmt;
    union
    {
        /// Color
        ///For all RGB color, the color set as the ARGB8888 format.\n
        ///Each color component need to shift to high bit.\n
        ///Use ARGB1555 as the example, the source color key as the following:\n
        ///ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
        ///color_s   --> A0000000RRRRR000GGGGG000BBBBB000   (every character represents one bit)\n\n
        ///For GFX_FMT_I8 format, the index set to b component (colorRange.color_s.b = b).\n
        GFX_ColorRange       colorRange;
        ///Blink attribute
        ///For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
        GFX_BlinkDataRange  blkDataRange;
    };

    /// GERECT_FLAG_COLOR_CONSTANT: Constant color\n
    /// GERECT_FLAG_COLOR_GRADIENT_X: X direction gradient color\n
    /// GERECT_FLAG_COLOR_GRADIENT_Y: Y direction gradient color\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_X: for trapezoid, top & bottom in X direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_Y\n
    /// GFXRECT_FLAG_TRAPE_DIRECTION_Y: for trapezoid, top & bottom in Y direction parallel, excluded with GFXRECT_FLAG_TRAPE_DIRECTION_X\n
    MS_U32 flag;
} GFX_SpanFillInfo;
#endif

//-------------------------------------------------
#define GFXFONT_FLAG_DEFAULT            0x0UL
#define GFXFONT_FLAG_ITALIC             0x1UL
#define GFXFONT_FLAG_SCALE              0x2UL
#define GFXFONT_FLAG_COMPACT            0x4UL
#define GFXFONT_FLAG_BACKGROUND         0x8UL
#define GFXFONT_FLAG_BLINK              0x10UL
#define GFXFONT_FLAG_VARWIDTH           0x20UL
#define GFXFONT_FLAG_GAP                0x40UL
#define GFXFONT_FLAG_BOLD               0x80UL

/// Define font overlap info.
typedef struct
{
    MS_S8 s8Xoffset;         ///
    MS_S8 s8Yoffset;
} GFX_GlyphOverlapInfo;

/// Glyph display information
typedef struct
{
    MS_U8  u8X0;               ///< Left position to display
    MS_U8  u8Width;            ///< Width of the glyph to display
    MS_U16 u16Index;           ///< Glyph index in the font table
    GFX_GlyphOverlapInfo overlap_info;
} GFX_GlyphDispInfo;




/// Font format
#ifndef MSOS_TYPE_OPTEE
typedef struct
{
    /// GEFONT_FLAG_DEFAULT: Default font format\n
    /// GEFONT_FLAG_SCALE: scalable font\n
    /// GEFONT_FLAG_COMPACT: Compact font\n
    /// GEFONT_FLAG_GAP: Gap font\n
    /// GEFONT_FLAG_VARIABLE_WIDTH: Variable font width, depend on horizontal font length.
    MS_U32 flag;

    /// dst block first character position , and each character width , height
    GFX_Block  dstblk;

    /// set compact distant(take indent effect) when GEFONT_FLAG_COMPACT is set
    MS_U16 dis;
    MS_U16 gap;

    GFX_GlyphDispInfo *pGlyphDispInfo;

    union{
        MS_U32  u32data;
        GFX_RgbColor color;
        GFX_BlinkData blink_data;
    };

} GFX_TextOutInfo;
#endif

/// Define blink type for blinking data
typedef enum
{
    GEBLINK_NONE = 0x0,
    GEBLINK_BACKGROUND,
    GEBLINK_FOREGROUND,
    GEBLINK_BOTH,
} GFX_BlinkType;

// Define write protect type
typedef enum
{
    GFX_WP_IN_RANGE                = 0,
    GFX_WP_OUT_RANGE               = 1,
    GFX_WP_DISABLE                 = 0xFF,

} GFX_WPType;

/// Bounding box (BBox) of a glyph
typedef struct
{
    MS_U8  u8X0;       ///< Position of the left edge of the bounding box
    MS_U8  u8Width;    ///< Width of the bounding box
    MS_U8  u8Y0;       ///< Position of the top edge of the bounding box
    MS_U8  u8Height;   ///< Height of the bounding box
} GFX_GlyphBbox;

/// X-direction information of the bounding box of a glyph
typedef struct DLL_PACKED
{
    MS_U8  u8X0;       ///< Position of the left edge of the bounding box
    MS_U8  u8Width;    ///< Width of the bounding box
} GFX_GlyphBbox_X;

/// Y-direction information of the bounding box of a glyph
typedef struct DLL_PACKED
{
    MS_U8  u8Y0;       ///< Position of the top edge of the bounding box
    MS_U8  u8Height;   ///< Height of the bounding box
} GFX_GlyphBbox_Y;


/// Define structure for color delta (Gradient color)
typedef struct DLL_PACKED
{
    MS_U8 flag;
    MS_U32 delta_r;
    MS_U32 delta_g;
    MS_U32 delta_b;
} GFX_GFX_ColorDelta;


//-------------------------------------------------
/// Specify the attribute of a bitmap
typedef struct DLL_PACKED
{
    /// Do scaling operation
    MS_BOOL bScale;
    /// X start address
    MS_U16 x;
    /// Y start address
    MS_U16 y;
    /// width
    MS_U16 width;
    /// height
    MS_U16 height;
} GFX_DrawBmpInfo;


//-------------------------------------------------
/// Define rotation angle
typedef enum
{
    /// Do not rotate
    GEROTATE_0,
    /// Rotate 90 degree
    GEROTATE_90,
    /// Rotate 180 degree
    GEROTATE_180,
    /// Rotate 270 degree
    GEROTATE_270,
} GFX_RotateAngle;

//=============================================================================
// Font information
//=============================================================================
#if 0
/// Bounding box (BBox) of a glyph
typedef struct
{
    ///Position of the left edge of the bounding box
    MS_U8  u8X0;
    ///Width of the bounding box
    MS_U8  u8Width;
    ///Position of the top edge of the bounding box
    MS_U8  u8Y0;
    ///Height of the bounding box
    MS_U8  u8Height;
}GFX_GFX_GLYPH_BBOX;
#endif

/// define character information
typedef struct
{
    /// Character start address (128 bits alignment)
    MS_PHY addr;
    /// Character width
    MS_U32 width;
    /// Character pitch
    MS_U32 pitch;
    /// Character height
    MS_U32 height;
    /// Character horizontal offset
    MS_U32 Hoffset;
    /// Character vertical offset
    MS_U32 Voffset;
    /// Character format
    GFX_Buffer_Format fmt;
} GFX_CharInfo;

/// Define font table information
typedef struct
{
    /// font start address
    MS_PHY addr;
    /// font length
    MS_U32 len;
    /// font structure is used
    MS_BOOL inUsed;
    /// character width (pixel unit)
    MS_U32 width;
    /// character pitch (byte unit)
    MS_U32 pitch;
    /// character height (pixel unit)
    MS_U32 height;
    /// one character length (byte unit)
    MS_U32 offset;
    /// Bounding Box offset
    GFX_GlyphBbox* pBBox;
    /// font format
    GFX_Buffer_Format fmt;
} GFX_FontInfo;


//=============================================================================
// Bitmap information
//=============================================================================
typedef struct
{
    MS_PHY addr;       // flat address of whole memory map
    MS_U32 len;
    MS_BOOL inUsed;
    MS_U32 width;
    MS_U32 pitch;
    MS_U32 height;
    GFX_Buffer_Format fmt;
} GFX_BitmapInfo;


//=============================================================================
// Draw Rect info
//=============================================================================
/// Define the bitblt source & destination block.
#ifndef MSOS_TYPE_OPTEE
typedef struct DLL_PACKED
{
    /// Source block
    GFX_Block srcblk;

    /// Destination block
    union
    {
        GFX_Trapezoid dsttrapeblk;
        GFX_Block dstblk;
    };
}GFX_DrawRect;
#endif

/// Define the bitblt two source & destination block.
typedef struct DLL_PACKED
{
    /// Source block
    GFX_Block srcblk0;
    // Attendtion, width and height of srcblk1 must be the same as dstblk
    GFX_Block srcblk1;
    /// Destination block
    GFX_Block dstblk;
}GFX_DrawTwoSourceRect;


/// Define the scaling factor for X & Y direction.
typedef struct DLL_PACKED
{
    MS_U32 u32DeltaX;
    MS_U32 u32DeltaY;
    MS_U32 u32InitDelatX;
    MS_U32 u32InitDelatY;
}GFX_ScaleInfo;

//=============================================================================
// Data Buffer info
//=============================================================================
/// Data buffer info.
typedef struct DLL_PACKED
{
    /// start memory address
    MS_PHY u32Addr;       // flat address of whole memory map
    /// width
    MS_U32 u32Width;
    /// height
    MS_U32 u32Height;
    /// pitch
    MS_U32 u32Pitch;
    /// Color format\n
    /// - GFX_FMT_I1\n
    /// - GFX_FMT_I2\n
    /// - GFX_FMT_I4\n
    /// - GFX_FMT_I8\n
    /// - GFX_FMT_RGB565\n
    /// - GFX_FMT_ARGB1555\n
    /// - GFX_FMT_ARGB4444\n
    /// - GFX_FMT_1BAAFGBG123433\n
    /// - GFX_FMT_ARGB8888\n
    GFX_Buffer_Format u32ColorFmt;
} GFX_BufferInfo;

/// Define the GFX init parameter.
typedef struct DLL_PACKED
{
    MS_U8                           u8Miu;
    MS_U8                           u8Dither;
    MS_U32                          u32VCmdQSize;                       /// MIN:4K, MAX:512K, <MIN:Disable
    MS_PHY                          u32VCmdQAddr;                       //  8-byte aligned
    MS_BOOL                         bIsHK;                              /// Running as HK or Co-processor
    MS_BOOL                         bIsCompt;                           /// Running as compatible mode. In compatible mode, the parameter checking loose for NOS APP compatibility.  TRUE=>compatible with 51/Chakra , FALSE=>linux OS style
}GFX_Config;

/// GE dbg info
typedef struct DLL_PACKED
{
    /// Specified format
    MS_U8                       verctrl[32];
    /// Specified format
    MS_U8                       gedump[256];
    /// Base alignment in byte unit
    MS_U32                      semstaus;
} GFX_DbgInfo;


typedef GFX_BufferInfo* PGFX_BufferInfo;



//=============================================================================
// GE palette information
//=============================================================================

typedef union
{
    /// ARGB8888
    struct
    {
        MS_U8 u8A;
        MS_U8 u8R;
        MS_U8 u8G;
        MS_U8 u8B;
    } RGB;
    // 32-bit direct access.
    MS_U8 u8Data[4];
} GFX_PaletteEntry;



#define GFXDRAW_FLAG_DEFAULT            0x0UL
#define GFXDRAW_FLAG_SCALE              0x1UL
#define GFXDRAW_FLAG_DUPLICAPE          0x2UL
#define GFXDRAW_FLAG_TRAPEZOID_X        0x4UL // excluded with GFXDRAW_FLAG_TRAPEZOID_Y
#define GFXDRAW_FLAG_TRAPEZOID_Y        0x8UL // excluded with GFXDRAW_FLAG_TRAPEZOID_X

//#define FB_FMT_AS_DEFAULT               0xFFFF

//=============================================================================
// YUV color setting
//=============================================================================
typedef MS_U8                           GFX_Rgb2yuvMode;
#define GFX_RGB2YUV_PC_MODE             ((GFX_Rgb2yuvMode)0x0UL)          // computer mode Y: 16~ 235, UV: 0~ 240
#define GFX_RGB2YUV_255_MODE            ((GFX_Rgb2yuvMode)0x1UL)          // To 0~ 255

typedef MS_U8                           GFX_YuvRangeOut;
#define GFX_YUV_RANGE_OUT_255           ((GFX_YuvRangeOut)0x0UL)         // To 0~ 255
#define GFX_YUV_RANGE_OUT_PC            ((GFX_YuvRangeOut)0x1UL)         // To Y: 16~ 235

typedef MS_U8                           GFX_YuvRangeIn;
#define GFX_YUV_RANGE_IN_255            ((GFX_YuvRangeIn)0x0UL)          // UV input range, 0~ 255
#define GFX_YUV_RANGE_IN_127            ((GFX_YuvRangeIn)0x1UL)          // UV input range, -128~ 127

typedef MS_U8                           GFX_Yuv422Fmt;
#define GFX_YUV_422_FMT_UY1VY0          ((GFX_Yuv422Fmt)0x00UL)          // CbY1CrY0
#define GFX_YUV_422_FMT_VY1UY0          ((GFX_Yuv422Fmt)0x01UL)          // CrY1CbY0
#define GFX_YUV_422_FMT_Y1UY0V          ((GFX_Yuv422Fmt)0x02UL)          // Y1CbY0Cr
#define GFX_YUV_422_FMT_Y1VY0U          ((GFX_Yuv422Fmt)0x03UL)          // Y1CrY0Cb

/// Pack of YUV CSC info
typedef struct
{
    GFX_Rgb2yuvMode                     rgb2yuv_mode;
    GFX_YuvRangeOut                    yuv_range_out;
    GFX_YuvRangeIn                     yuv_range_in;
    GFX_Yuv422Fmt                      yuv_mem_fmt_src;
    GFX_Yuv422Fmt                      yuv_mem_fmt_dst;
} GFX_YuvInfo;

//-------------------------------------------------
/// Define Stretch Bitblt with Color Key Type
typedef enum
{
    /// Do nothing
    GFX_DONOTHING = 0,
    /// Nearest When the Color Key happened
    GFX_NEAREST = 1,
    /// Replace the Key to Custom Color
    GFX_REPLACE_KEY_2_CUS = 2,
} GFX_StretchCKType;

//-------------------------------------------------
/// Define DFB Blending Related:
#define GFX_DFB_BLD_FLAG_COLORALPHA             0x0001UL
#define GFX_DFB_BLD_FLAG_ALPHACHANNEL           0x0002UL
#define GFX_DFB_BLD_FLAG_COLORIZE               0x0004UL
#define GFX_DFB_BLD_FLAG_SRCPREMUL              0x0008UL
#define GFX_DFB_BLD_FLAG_SRCPREMULCOL           0x0010UL
#define GFX_DFB_BLD_FLAG_DSTPREMUL              0x0020UL
#define GFX_DFB_BLD_FLAG_XOR                    0x0040UL
#define GFX_DFB_BLD_FLAG_DEMULTIPLY             0x0080UL
#define GFX_DFB_BLD_FLAG_SRCALPHAMASK           0x0100UL
#define GFX_DFB_BLD_FLAG_SRCCOLORMASK           0x0200UL
#define GFX_DFB_BLD_FLAG_ALL                    0x03FFUL

typedef enum
{
    GFX_DFB_BLD_OP_ZERO                 = 0,
    GFX_DFB_BLD_OP_ONE                  = 1,
    GFX_DFB_BLD_OP_SRCCOLOR             = 2,
    GFX_DFB_BLD_OP_INVSRCCOLOR          = 3,
    GFX_DFB_BLD_OP_SRCALPHA             = 4,
    GFX_DFB_BLD_OP_INVSRCALPHA          = 5,
    GFX_DFB_BLD_OP_DESTALPHA            = 6,
    GFX_DFB_BLD_OP_INVDESTALPHA         = 7,
    GFX_DFB_BLD_OP_DESTCOLOR            = 8,
    GFX_DFB_BLD_OP_INVDESTCOLOR         = 9,
    GFX_DFB_BLD_OP_SRCALPHASAT          = 10,
}GFX_DFBBldOP;

//=============================================================================
// GFX format capability setting
//=============================================================================
///Define Buffer Usage Type
typedef enum
{
    /// Desitination buffer for LINE, RECT, BLT
    E_GFX_BUF_DST                    = 0,
    /// Source buffer for BLT
    E_GFX_BUF_SRC                    = 1,
} EN_GFX_BUF_TYPE;

/// Define gfx format capability type
typedef enum
{
    E_GFX_FMT_CAP_NONE = 0x0,
    E_GFX_FMT_CAP_MULTI_PIXEL,
} EN_GFX_FMT_CAPS_TYPE;

typedef enum
{
    E_GFX_CONFIG_BURST_MIU_LENGTH,
} EN_GFX_CONFIG_TYPE;

typedef enum
{
    E_GFX_CONFIG_INIT,
} EN_GFX_CONFIG_INIT;


typedef struct
{
    MS_BOOL                     bEnable;
    MS_U32                      u32BurstLen;
}GFX_Set_BurstMiuLen;

typedef struct DLL_PACKED
{
    /// Color format\n
    /// - GFX_FMT_I1\n
    /// - GFX_FMT_I2\n
    /// - GFX_FMT_I4\n
    /// - GFX_FMT_I8\n
    /// - GFX_FMT_RGB565\n
    /// - GFX_FMT_ARGB1555\n
    /// - GFX_FMT_ARGB4444\n
    /// - GFX_FMT_1BAAFGBG123433\n
    /// - GFX_FMT_ARGB8888\n
    GFX_Buffer_Format    u32ColorFmt;
    EN_GFX_BUF_TYPE      eBufferType;
    EN_GFX_FMT_CAPS_TYPE eFmtCapsType;
    MS_U8                u8BaseAlign;
    MS_U8                u8PitchAlign;
}GFX_FmtAlignCapsInfo;

//=============================================================================
// GFX capability setting
//=============================================================================

//GFX Capability
typedef enum
{
    E_GFX_CAP_MULTI_PIXEL,
    E_GFX_CAP_BLT_DOWN_SCALE,
    E_GFX_CAP_DFB_BLENDING,
    E_GFX_CAP_ALIGN,
    E_GFX_CAP_TLB,
}EN_GFX_CAPS;

///GFX Capability MultiPixel Info
typedef struct DLL_PACKED
{
    MS_U16  WordUnit;
    MS_BOOL bSupportMultiPixel;
}GFX_CapMultiPixelInfo;

///GFX Capability Bitblt down scale Info
typedef struct DLL_PACKED
{
    /// Bitblt down scale range start
    MS_U8 u8RangeMax;
    /// Bitblt down scale range end
    MS_U8 u8RangeMin;
    /// Bitblt down scale continuous range end
    MS_U8 u8ContinuousRangeMin;
    /// Is full range support down scaling.
    /// - TRUE: The down scale value between u8RangeMax to u8RangeMin is fully supported.
    /// - FALSE: The down scale value between u8RangeMax to u8ContinuousRangeMin is fully supported.
    ///          The down scale value between u8ContinuousRangeMin to u8RangeMin is supported
    ///          if the value is power of two (e.g., 4, 8, 16, and 32).
    MS_BOOL bFullRangeSupport;
}GFX_BLT_DownScaleInfo;

//GFX TLB Mode
typedef enum
{
    /// TLB for None
    E_GFX_TLB_NONE = 0x0000,
    /// TLB for Source
    E_GFX_TLB_SRC = 0x0001,
    /// TLB for Destination
    E_GFX_TLB_DST = 0x0002,
    /// TLB for Source and Destination
    E_GFX_TLB_SRC_DST = 0x0003,
}EN_GFX_TLB_Mode;

#ifdef MSOS_TYPE_OPTEE
DLL_PUBLIC void MApi_GFX_Init(GFX_Config *geConfig);
DLL_PUBLIC GFX_Result MApi_GFX_WaitForTAGID(MS_U16 tagID);
DLL_PUBLIC MS_U16     MApi_GFX_SetNextTAGID(void);
DLL_PUBLIC GFX_Result MApi_GFX_BeginDraw(void);
DLL_PUBLIC GFX_Result MApi_GFX_EndDraw(void);
DLL_PUBLIC GFX_Result MApi_GFX_ClearFrameBufferByWord(MS_PHY StrAddr, MS_U32 length, MS_U32 ClearValue);
//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------
//-----------------
// GFX_INIT
//-----------------
//-------------------------------------------------------------------------------------------------
/// Initialize GFX module.
/// @ingroup GFX_INIT
/// @param  geConfig             \b IN: init config
/// @return void
//-------------------------------------------------------------------------------------------------
#else
DLL_PUBLIC extern void MApi_GFX_Init(GFX_Config *geConfig);

//-------------------------------------------------------------------------------------------------
/// Initialize GFX module.
/// @ingroup GFX_INIT
/// @param  geConfig             \b IN: init config
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetConfig(EN_GFX_CONFIG_INIT enType,void *plist,MS_U32 u32Len);

//-----------------
// GFX_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetFontInfo
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetFontInfo(MS_S32 handle, GFX_FontInfo* pinfo);

//-------------------------------------------------------------------------------------------------
/// Get clip window
/// @ingroup GFX_CLIP
/// @param  v0                  \b IN: left-top point
/// @param  v1                  \b IN: right-down point
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetClip(GFX_Point* v0, GFX_Point* v1);

//-------------------------------------------------------------------------------------------------
/// Get I2 I4 palette
/// @ingroup GFX_PALETTE
/// @param  id                  \b IN: index
/// @param  pColor              \b IN: color
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetIntensity(MS_U32 idx, MS_U32 *color);
//-------------------------------------------------------------------------------------------------
/// Get tag
/// @ingroup GFX_WAIT_DONE
/// @param  tagID                  \b IN: tag
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetTAGID(MS_U16 *tagID);

//-------------------------------------------------------------------------------------------------
/// Get next tag
/// @ingroup GFX_WAIT_DONE
/// @param  bStepTagBefore      \b IN: next tag
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_U16     MApi_GFX_GetNextTAGID(MS_BOOL bStepTagBefore);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetInfo
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetInfo( GFX_DbgInfo *dbg );
//-----------------
// GFX_CAPS
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get GE capability
/// @ingroup GFX_CAPS
/// @param  eCapType            \b IN: cap type
/// @param  pRet                \b IN: cap struct pointer
/// @param  ret_size            \b IN: cap struct size
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetGECaps(EN_GFX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);



//-----------------
// GFX_DRAW
//-----------------
//-------------------------------------------------------------------------------------------------
/// Draw line
/// @ingroup GFX_DRAW
/// @param  pline               \b IN: line info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_DrawLine(GFX_DrawLineInfo *pline);

//-------------------------------------------------------------------------------------------------
/// Fill Rect
/// @ingroup GFX_DRAW
/// @param  pfillblock          \b IN: Rect info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_RectFill(GFX_RectFillInfo *pfillblock);

//-------------------------------------------------------------------------------------------------
/// Fill Triangle
/// @ingroup GFX_DRAW
/// @param  ptriblock          \b IN: Triangle info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_TriFill(GFX_TriFillInfo *ptriblock);

//-------------------------------------------------------------------------------------------------
/// Fill Span
/// @ingroup GFX_DRAW
/// @param  pspanblock          \b IN: Span info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SpanFill(GFX_SpanFillInfo *pspanblock);

//-----------------
// GFX_BUFFER_INFO
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Src buffer info
/// @ingroup GFX_BUFFER_INFO
/// @param  bufInfo              \b IN: Src buffer info
/// @param  offsetofByte         \b IN: offset
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetSrcBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte);
//-------------------------------------------------------------------------------------------------
/// Set Dst buffer info
/// @ingroup GFX_BUFFER_INFO
/// @param  bufInfo              \b IN: Dst buffer info
/// @param  offsetofByte         \b IN: offset
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDstBufferInfo(GFX_BufferInfo *bufInfo, MS_U32 offsetofByte);



//-----------------
// GFX_ROP
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set ROP
/// @ingroup GFX_ROP
/// @param  enable              \b IN: Enable/Disable
/// @param  eRopMode              \b IN: ROP operation
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetROP2(MS_BOOL enable, GFX_ROP2_Op eRopMode);
//-----------------
// GFX_COLOR_KEY
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Src color-key info
/// @ingroup GFX_COLOR_KEY
/// @param  enable              \b IN: Enable/Disable
/// @param  opMode              \b IN: color key operation
/// @param  fmt                 \b IN: source color format.If src fmt is I8 ,but dst fmt is not the same; the fmt of src color key must be setted to ARGB8888
/// @param  ps_color            \b IN: color key range start
/// @param  pe_color            \b IN: color key range end
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetSrcColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color,
                                      void *pe_color);

//-------------------------------------------------------------------------------------------------
/// Set Dst color-key info
/// @ingroup GFX_COLOR_KEY
/// @param  enable              \b IN: Enable/Disable
/// @param  opMode              \b IN: color key operation
/// @param  fmt                 \b IN: destination color format
/// @param  ps_color            \b IN: color key range start
/// @param  pe_color            \b IN: color key range end
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDstColorKey(MS_BOOL enable, GFX_ColorKeyMode opMode,
                                      GFX_Buffer_Format fmt, void *ps_color,
                                      void *pe_color);
//-------------------------------------------------------------------------------------------------
/// Set alpha blending info
/// @ingroup GFX_ALPHA_BLENDING
/// @param  enable              \b IN: Enable/Disable
/// @param  coef                \b IN: color blending formular
/// @param  db_abl              \b IN: alpha blending formular
/// @param  abl_const           \b IN: constant alpha
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetAlpha(MS_BOOL enable, GFX_BlendCoef coef, GFX_AlphaSrcFrom db_abl, MS_U8 abl_const);
//-----------------
// GFX_ALPHA_BLENDING
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable alpha blending
/// @ingroup GFX_ALPHA_BLENDING
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EnableAlphaBlending(MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// Enable DFB
/// @ingroup GFX_DFB
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EnableDFBBlending(MS_BOOL enable);

//-------------------------------------------------------------------------------------------------
/// Set DFB extra flag
/// @ingroup GFX_DFB
/// @param  u16DFBBldFlags      \b IN: extra blending flag
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDFBBldFlags(MS_U16 u16DFBBldFlags);


//-------------------------------------------------------------------------------------------------
/// Set DFB fomular
/// @ingroup GFX_DFB
/// @param  gfxSrcBldOP         \b IN: src blending operation
/// @param  gfxDstBldOP         \b IN: dst blending operation
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDFBBldOP(GFX_DFBBldOP gfxSrcBldOP, GFX_DFBBldOP gfxDstBldOP);

//-------------------------------------------------------------------------------------------------
/// Set DFB constant color
/// @ingroup GFX_DFB
/// @param  gfxRgbColor         \b IN: constant color
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDFBBldConstColor(GFX_RgbColor gfxRgbColor);
//-------------------------------------------------------------------------------------------------
/// BitBlit
/// @ingroup GFX_DRAW
/// @param  pfillblock          \b IN: BitBlit info
/// @param  drawflag            \b IN: BitBlit flag
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_BitBlt(GFX_DrawRect *drawbuf,MS_U32 drawflag);
//-------------------------------------------------------------------------------------------------
/// BitBlit with scaling info
/// @ingroup GFX_DRAW
/// @param  pfillblock          \b IN: BitBlit info
/// @param  drawflag            \b IN: BitBlit flag
/// @param  ScaleInfo           \b IN: Scaling info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_BitBltEx(GFX_DrawRect * drawbuf, MS_U32 drawflag, GFX_ScaleInfo * ScaleInfo);

//-----------------
// GFX_PALETTE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set I2 I4 palette
/// @ingroup GFX_PALETTE
/// @param  id                  \b IN: index
/// @param  fmt                 \b IN: color format
/// @param  pColor              \b IN: color
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetIntensity(MS_U32 id, GFX_Buffer_Format fmt, MS_U32 *pColor);


//-----------------
// GFX_CLIP
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set clip window
/// @ingroup GFX_CLIP
/// @param  v0                  \b IN: left-top point
/// @param  v1                  \b IN: right-down point
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetClip(GFX_Point* v0, GFX_Point* v1);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetItalic
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetItalic(MS_BOOL enable,MS_U8 ini_line, MS_U8 ini_dis, MS_U8 delta);

//-----------------
// GFX_DITHER
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Dither
/// @ingroup GFX_DITHER
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDither(MS_BOOL enable);

//-----------------
// GFX_PIXEL_MODE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set one pixel mode
/// @ingroup GFX_PIXEL_MODE
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetOnePixelMode(MS_BOOL enable);
//-----------------
// GFX_OPTION
//-----------------
//-----------------
// GFX_STRETCH
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable Nearest mode
/// @ingroup GFX_STRETCH
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetNearestMode(MS_BOOL enable);
//-----------------
// GFX_MIRROR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Src mirror
/// @ingroup GFX_MIRROR
/// @param  isMirrorX           \b IN: Enable/Disable
/// @param  isMirrorY           \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetMirror(MS_BOOL isMirrorX, MS_BOOL isMirrorY);
//-------------------------------------------------------------------------------------------------
/// Set Dst mirror
/// @ingroup GFX_MIRROR
/// @param  isMirrorX           \b IN: Enable/Disable
/// @param  isMirrorY           \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDstMirror(MS_BOOL isMirrorX, MS_BOOL isMirrorY);

//-----------------
// GFX_ROTATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set rotation
/// @ingroup GFX_ROTATE
/// @param  angle               \b IN: angle
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetRotate(GFX_RotateAngle angle);
//-----------------
// GFX_WAIT_DONE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set tag
/// @ingroup GFX_WAIT_DONE
/// @param  tagID                  \b IN: tag
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetTAGID(MS_U16 tagID);
//-------------------------------------------------------------------------------------------------
/// wait for tag
/// @ingroup GFX_WAIT_DONE
/// @param  tagID               \b IN: tagID
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_WaitForTAGID(MS_U16 tagID);

//-------------------------------------------------------------------------------------------------
/// Set next tag
/// @ingroup GFX_WAIT_DONE
/// @param  void
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_U16     MApi_GFX_SetNextTAGID(void);
//-----------------
// GFX_VCMQ
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable GE virtual cmdq
/// @ingroup GFX_VCMQ
/// @param  enable              \b IN: Enable/Disable
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EnableVCmdQueue(MS_U16 blEnable);
//-------------------------------------------------------------------------------------------------
/// Set GE virtual cmdq info
/// @ingroup GFX_VCMQ
/// @param  u32Addr             \b IN: vcmdq addr
/// @param  enBufSize           \b IN: vcmdq size
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetVCmdBuffer(MS_PHY PhyAddr, GFX_VcmqBufSize enBufSize);
//-------------------------------------------------------------------------------------------------
/// Set GE write queue threadhold to DRAM
/// @ingroup GFX_VCMQ
/// @param  u8W_Threshold       \b IN: Threshold
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GE_SetVCmd_W_Thread(MS_U8 u8W_Threshold);
//-------------------------------------------------------------------------------------------------
/// Set GE read queue threadhold to DRAM
/// @ingroup GFX_VCMQ
/// @param  u8R_Threshold       \b IN: Threshold
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GE_SetVCmd_R_Thread(MS_U8 u8R_Threshold);
//-------------------------------------------------------------------------------------------------
/// flush all ge queue
/// @ingroup GFX_WAIT_DONE
/// @param  void
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_FlushQueue(void) ;
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_PowerOn
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_PowerOn(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_PowerOff
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_PowerOff(void);
//-----------------
// GFX_POWER_STATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GE suspend resume
/// @ingroup GFX_POWER_STATE
/// @param  u16PowerState       \b IN: suspend/resume
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_U32     MApi_GFX_SetPowerState(EN_POWER_MODE u16PowerState);

//-----------------
// GFX_WRITE_PROTECT
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set GE write protect
/// @ingroup GFX_WRITE_PROTECT
/// @param  miu                 \b IN: miu
/// @param  addr_low            \b IN: addr_low
/// @param  addr_high           \b IN: addr_high
/// @param  eWPType             \b IN: eWPType
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_WriteProtect(MS_U8 miu,MS_PHY addr_low,MS_PHY addr_high,GFX_WPType eWPType);
//-------------------------------------------------------------------------------------------------
/// Set I8 palette
/// @ingroup GFX_PALETTE
/// @param  pPalArray           \b IN: color array
/// @param  u32PalStart         \b IN: color index start
/// @param  u32PalEnd           \b IN: color index end
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
DLL_PUBLIC extern GFX_Result MApi_GFX_SetPaletteOpt( GFX_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd);



//-----------------
// GFX_DRAW_MISC
//-----------------
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_DrawBitmap
/// @ingroup GFX_DRAW_MISC
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_DrawBitmap(MS_S32 handle, GFX_DrawBmpInfo *pbmpfmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_TextOut
/// @ingroup GFX_DRAW_MISC
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_TextOut(MS_S32 fhandle, MS_U8 *pindex, MS_U32 strwidth, GFX_TextOutInfo *pfmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_CharacterOut
/// @ingroup GFX_DRAW_MISC
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_CharacterOut(GFX_CharInfo*  pChar, GFX_TextOutInfo *pfmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetBitmapInfo
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetBitmapInfo(MS_S32 handle, GFX_BitmapInfo* pinfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetFrameBufferInfo
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetFrameBufferInfo(MS_U32 *width, MS_U32 *height, MS_U32 *pitch, MS_U32 *fbFmt, MS_PHY *addr);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_TrapezoidFill
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_TrapezoidFill(GFX_RectFillInfo *pGfxFillInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_EnableTrapezoidAA
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EnableTrapezoidAA(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_EnableTrapSubPixCorr
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EnableTrapSubPixCorr(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_QueryTextDispLength
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_QueryTextDispLength(MS_S32 fhandle, MS_U8 *pu8index, MS_U32 u32strwidth,
                                               GFX_TextOutInfo *pfmt, MS_U32* pu32DispLength);
//-------------------------------------------------------------------------------------------------
/// Set alpha formular
/// @ingroup GFX_ALPHA_BLENDING
/// @param  eMode               \b IN: alpha blending formular
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetAlphaSrcFrom(GFX_AlphaSrcFrom eMode);
//-------------------------------------------------------------------------------------------------
/// Set color formular
/// @ingroup GFX_ALPHA_BLENDING
/// @param  blendcoef           \b IN: color blending formular
/// @param  blendfactor         \b IN: constant alpha
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
DLL_PUBLIC extern GFX_Result MApi_GFX_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 blendfactor);
//-----------------
// GFX_DFB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Query DFB capability
/// @ingroup GFX_DFB
/// @param  pU16SupportedBldFlags   \b IN: DFB capability
/// @return GFX_SUCCESS             - Success
/// @return GFX_FAIL                - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_QueryDFBBldCaps(MS_U16 *pU16SupportedBldFlags);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetDFBBldSrcColorMask
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDFBBldSrcColorMask(GFX_RgbColor gfxRgbColor);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_Line_Pattern_Reset
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_Line_Pattern_Reset(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_Set_Line_Pattern
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_Set_Line_Pattern(MS_BOOL enable, MS_U8 linePattern, MS_U8 repeatFactor);

//-----------------
// GFX_DRAW_SETTING
//-----------------
//-----------------
// GFX_BEGINE_END
//-----------------
//-------------------------------------------------------------------------------------------------
/// Begine of draw.
/// @ingroup GFX_LOCK_BEGINE_END
/// @param  void
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_BeginDraw(void);
//-------------------------------------------------------------------------------------------------
/// End of draw.
/// @ingroup GFX_LOCK_BEGINE_END
/// @param  void
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_EndDraw(void);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_YUV_Get
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_YUV_Get(GFX_YuvInfo* pYuvInfo);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_RectFrame
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void MApi_GFX_RectFrame(GFX_RectFillInfo *pblock, GFX_DrawLineInfo *plinefmt);
//-----------------
// GFX_R2Y
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set R2Y CSC
/// @ingroup GFX_R2Y
/// @param  mode                \b IN: mode
/// @param  yuv_out_range       \b IN: dst YUV range
/// @param  uv_in_range         \b IN: src UV range
/// @param  srcfmt              \b IN: src YUV fmt
/// @param  dstfmt              \b IN: dst YUV fmt
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetDC_CSC_FMT(GFX_YUV_Rgb2Yuv mode, GFX_YUV_OutRange yuv_out_range, GFX_YUV_InRange uv_in_range, GFX_YUV_422 srcfmt, GFX_YUV_422 dstfmt);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetPatchMode
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetPatchMode(MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// Get buffer info
/// @ingroup GFX_BUFFER_INFO
/// @param  srcbufInfo          \b IN: Src buffer info
/// @param  dstbufInfo          \b IN: Dst buffer info
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetBufferInfo(PGFX_BufferInfo srcbufInfo, PGFX_BufferInfo dstbufInfo);
//-------------------------------------------------------------------------------------------------
/// Clear frame by 32 bits value
/// @ingroup GFX_CLEAR
/// @param  StrAddr             \b IN: addr
/// @param  length              \b IN: len
/// @param  ClearValue          \b IN: value
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_ClearFrameBufferByWord(MS_PHY StrAddr, MS_U32 length, MS_U32 ClearValue);
//-----------------
// GFX_CLEAR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Clear frame by 8 bits value
/// @ingroup GFX_CLEAR
/// @param  StrAddr             \b IN: addr
/// @param  length              \b IN: len
/// @param  ClearValue          \b IN: value
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_ClearFrameBuffer(MS_PHY StrAddr, MS_U32 length, MS_U8 ClearValue);
//-------------------------------------------------------------------------------------------------
/// Set alpha value in ARGB1555
/// @ingroup GFX_ALPHA_BLENDING
/// @param  coef                \b IN: alpha value
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetAlpha_ARGB1555(MS_U8 coef);
//-------------------------------------------------------------------------------------------------
/// Get alpha value in ARGB1555
/// @ingroup GFX_ALPHA_BLENDING
/// @param  coef                \b IN: alpha value
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetAlpha_ARGB1555(MS_U8* coef);
//-----------------
// GFX_CB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get Font info call back.
/// @ingroup GFX_CB
/// @param  fpGFX_CB             \b IN: Get Font info call back
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_RegisterGetFontCB(MS_PHY (*fpGFX_CB)(MS_S8, GFX_FontInfo*)  );
//-------------------------------------------------------------------------------------------------
/// Get BMP info call back.
/// @ingroup GFX_CB
/// @param  fpGFX_CB             \b IN: Get BMP info call back
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_RegisterGetBMPCB(MS_PHY (*fpGFX_CB)(MS_S16, GFX_BitmapInfo*)  );
//-----------------
// GFX_ALPHA_COMPARE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set alpha compare info
/// @ingroup GFX_ALPHA_COMPARE
/// @param  enable              \b IN: Enable/Disable
/// @param  eMode               \b IN: alpha compare operation
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetAlphaCmp(MS_BOOL enable, GFX_ACmpOp eMode);
//-----------------
// GFX_MISC
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure GFX DBG level
/// @ingroup GFX_MISC
/// @param  level               \b IN: level
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void MApi_GFX_SetDbgLevel(MS_U32 level);
//-------------------------------------------------------------------------------------------------
/// Set source color key edge stretch mode
/// @ingroup GFX_STRETCH
/// @param  type                \b IN: stretch mode
/// @param  color               \b IN: customer color
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetStrBltSckType(GFX_StretchCKType type, GFX_RgbColor *color);
//-----------------
// GFX_MISC_NOS
//-----------------
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetHK
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetHK(MS_BOOL bIsHK) ;
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetHK
/// @ingroup GFX_MISC_NOS
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-----------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetHK(MS_BOOL *bIsHK) ;
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_DrawOval
/// @ingroup GFX_DRAW_MISC
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_DrawOval(GFX_OvalFillInfo *pOval);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetStatus
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetStatus( MS_BOOL *pStatus );
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_GetLibVer
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_LOCAL extern GFX_Result MApi_GFX_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// Get GE alignment capability
/// @ingroup GFX_CAPS
/// @param  pAlignCapsInfo      \b IN: AlignCaps
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_GetAlignCaps(GFX_FmtAlignCapsInfo* pAlignCapsInfo);
/// msAPI_GE_ClearFrameBufferByWord
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void msAPI_GE_ClearFrameBufferByWord (MS_PHY StrAddr, MS_U32 length, MS_U32 ClearValue);
//-------------------------------------------------------------------------------------------------
/// MDrv_GE_EnableAlphaBlending
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MDrv_GE_EnableAlphaBlending(MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// MDrv_GE_SetAlphaBlending
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MDrv_GE_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 u8ConstantAlpha);
//-------------------------------------------------------------------------------------------------
/// MDrv_GE_PE_SetIntensity
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void MDrv_GE_PE_SetIntensity(MS_U8 id, GFX_Buffer_Format fmt, MS_U32* pcolor);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetTLBMode
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetTLBMode(EN_GFX_TLB_Mode TLBmode);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetTLBBaseADDR
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetTLBBaseADDR(MS_PHY phyTLBSrcMiuAddr,MS_PHY phyTLBDstMiuAddr);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetTLBFlushTable
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetTLBFlushTable(MS_BOOL enable);
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_SetTLBTag
/// @ingroup GFX_ToBeRemove
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_SetTLBTag(MS_U16 tag);


//-----------------
// GFX_MISC_BOX
//-----------------
//-------------------------------------------------------------------------------------------------
/// MApi_GFX_BitbltByTwoSourceBuffer
/// @ingroup GFX_MISC_BOX
/// @return GFX_SUCCESS         - Success
/// @return GFX_FAIL            - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern GFX_Result MApi_GFX_BitbltByTwoSourceBuffer(PGFX_BufferInfo dstbufInfo, GFX_DrawTwoSourceRect *pbmpfmt, MS_U32 drawflag);

//-------------------------------------------------------------------------------------------------
/// Finalize GFX module.
/// @ingroup GFX_INIT
/// @param  void
/// @return void
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void MApi_GE_Exit(void);
#endif


#ifdef __cplusplus
}
#endif

#endif //_API_GFX_H_

