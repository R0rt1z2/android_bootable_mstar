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

#ifndef __DRVFCIE_H__
#define __DRVFCIE_H__



#include "hal_sdio_platform.h"
//#include "drvFCIE_config.h"

#define NONE			"\033[m"
#define RED				"\033[0;32;31m"
#define LIGHT_RED		"\033[1;31m"
#define GREEN			"\033[0;32;32m"
#define LIGHT_GREEN		"\033[1;32m"
#define BLUE			"\033[0;32;34m"
#define LIGHT_BLUE		"\033[1;34m"
#define DARY_GRAY		"\033[1;30m"
#define CYAN			"\033[0;36m"
#define LIGHT_CYAN		"\033[1;36m"
#define PURPLE			"\033[0;35m"
#define LIGHT_PURPLE	"\033[1;35m"
#define BROWN			"\033[0;33m"
#define YELLOW			"\033[1;33m"
#define LIGHT_GRAY		"\033[0;37m"
#define WHITE			"\033[1;37m"


/**************************************
* Function define
**************************************/
#define SD_TIMEOUT_VALUE       1000 // 1 second
#define SD_DMA_TIMEOUT_VALUE  10000 // 10 second
#define BYTES_8          0
#define BYTES_16         1
#define BYTES_32         2
#define MIU_BURST_LEN    BYTES_8

/**************************************
* Structure define
**************************************/
#define TEST_WRITECRC		0
#define	TEST_READSTOPCLOCK  0
#define	TEST_RESPONSESHIFT	0
#define	TEST_POWERSAVE		0
#define	TEST_CMDNORESPONSE	1
#define	TEST_RW_SBIT_TOUT	0
#define TEST_RESPONSECRCERR	0
#define TEST_READCRCERR		0
#define	TEST_WRITECRCERR	0
#define TEST_BIST			0
#define REGRESSION_TEST		1
#define TEST_DMA			0
#define TEST_R2N			0
#define	TEST_ADMA			0

#define DMA_MODE 0
#define ADMA_MODE 1
#define R2N_MODE  2

typedef union _CMD
{
    U8 Array[20];

	struct
	{
		U8 Dummy[3];
		U8 Index;
		U32 Arg;
		U8 Reserve[12];

	} __attribute__((__packed__)) Cmd;

} CMD;

typedef union _RSP
{
    U8 Array[20];

	struct
	{
		U8 Dummy[3];
		U8 Index;
		U32 DeviceStatus;
		U8 Reserve[12];

	} __attribute__((__packed__)) Rsp;

	U32 Resp[5];

} RSP;

typedef enum
{
    CLK_STOP	= 0,
    CLK_KEEP	= 1,

} E_CLK_STOP;

typedef enum
{
    CMD_TYPE_BC     = 0, // broadcast command
    CMD_TYPE_BCR    = 1, // broadcast command with response
    CMD_TYPE_AC     = 2, // addressed command
    CMD_TYPE_ADTC   = 3, // addressed data transfer command
    CMD_TYPE_NDTC   = 4, // no data transfer command

} E_CMD_TYPE;

typedef enum
{
    RSP_TYPE_NO  = 0,
    RSP_TYPE_R1  = 1,
    RSP_TYPE_R1b = 2,
    RSP_TYPE_R2  = 3,
    RSP_TYPE_R3  = 4,
    RSP_TYPE_R6  = 5,
    RSP_TYPE_R7  = 6, // CMD08 send if cond

} E_RSP_TYPE;

typedef enum
{
    DIR_R = 0,
    DIR_W = 1,

} E_RWDIR;

typedef enum
{
    PATH_R2N	= 0, // only GPIO_PAD & 8 bits macro can use R2N
    PATH_DMA	= 1,
    PATH_ADMA	= 2,

} E_PATH;

typedef enum
{
    BUS_1_BIT   = 0,
    BUS_4_BITS  = 1,
    BUS_8_BITS  = 2,

} E_DATA_BUS;

typedef struct _CMD_RSP_INFO
{
	U32				CardClock;
	CMD				Command;
	RSP				Response;

	E_CLK_STOP		ClockStop;
	E_CMD_TYPE		CmdType;
	E_RSP_TYPE		RspType;

	// add for data transfer
	E_RWDIR			ReadWriteDir;
	E_PATH			DataPath; // R2N or DMA
	E_DATA_BUS		BusWidth;

	U32				BusAddr;	// MIU0, MIU1, MIU2 base added address
	U32				MiuAddr;	// DMA & ADMA use only, this is a MIU device address
	U32 *			pBufferAddr; // R2N use only, this is a data buffer pointer
	U16				BlockSize;
	U32				FakeDmaLength; // IP verify use only
	U16				BlockCount; // ADMA don't care
	U16				ReadWriteTimeOut;
	U8				MaskMiuReq; // IP verify use only

} CMD_RSP_INFO;


struct kernel_like_scatterlist {

	U32	u32_dma_addr;
	U32	u32_length;
};

struct kernel_like_mmc_data {

	unsigned int		sg_len;		/* size of scatter list */
	struct kernel_like_scatterlist	*sg;		/* I/O scatter list */
};

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _ADMA_DESCRIPTOR
{
	U32 adma_end_flag   :1;
	U32 adma_miu_sel    :2;
	U32                 :13;    // reserve
	U32 adma_job_cnt    :16;
	U32 adma_miu_addr;
	U32 adma_length;
	U32 reserved;

} ADMA_DESCRIPTOR ;

#define SG_SEG_NUM 512

typedef struct _ADMA_INFO
{
    ADMA_DESCRIPTOR Descriptor[SG_SEG_NUM];
    //U32 const DescriptorAddr;
    U32 DescriptorBusAddr;

} ADMA_INFO ;

typedef enum
{
    EC_NO_ERROR = 0,
    EC_CRC_ERROR,
    EC_CMD_TIME_OUT,
    EC_DMA_TIME_OUT,
    EC_CMD_NO_RSP,

} ERR_CODE_SD;

typedef enum
{
    IO_SUCCESS              = 0x00,
    IO_TIME_OUT             = 0x01,
    IO_CARD_REMOVED         = 0x02,
    IO_ERROR                = 0x03,
    IO_ERROR_DETECT         = 0x04,

    IO_VOLTAGE_NOT_ACCEPT   = 0x05,
    IO_MISS_CHECK_PATTERN   = 0x06,

    IO_CMD_NO_RSP           = 0x16,
    IO_RSP_CRC_ERROR        = 0x17,
    IO_RSP_ERROR            = 0x18,

    IO_R_DATA_CRC_ERROR     = 0x20,
    IO_W_DATA_STS_ERROR     = 0x21,
    IO_W_DATA_STS_NEGATIVE  = 0x22,

    IO_STILL_POWERING_UP    = 0x30,

    IO_CARD_CHANGED         = 0x40,

} E_IO_STS;


extern U8 gu8SdioSilenceTuning;
extern U32 gu32BestSdioSkew4;

void		HalSdio_Init(void);
void		HalSdio_DumpRegister(void);
void		HalSdio_OpenClock(U8 On_off);
U8			HalSdio_CmdFifoGet(U8 u8addr);
void		HalSdio_CmdFifoSet(U8 u8addr, U8 u8value);
E_IO_STS	HalSdio_WaitGetCIFD(U32 * pu32_DataBuf, U32 u32_ByteCnt);
E_IO_STS	HalSdio_WaitSetCIFD(U32 * pu32_DataBuf, U32 u32_ByteCnt);
E_IO_STS	HalSdio_WaitD0High(U32);
E_IO_STS	HalSdio_SendCommand(CMD_RSP_INFO * pCmdRspInfo);
E_IO_STS	HalSdio_SetBusWidth(U8 * pu8BusWidth, E_DATA_BUS DataBus);
E_IO_STS	HalSdio_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs);
U32			HalSdio_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec);
//void		HalSdio_SetVDD(U8 u8OnOff);
U8			HalSdio_SendClocknWaitD0High(void);
U8			HalSdio_TestAdvanceDMA(U8 u8Pattern);
U32			HalSdio_PowerSaveMode(void);
U32 		HalSdio_BistTest(U8 u8_PatSel);
U16			HalSdio_ReadCRCFifo(U32 i);
void		HalSdio_SetShiftTune(U8 u8Select, U8 u8Type, U8 u8Count);
void		HalSdio_TestDataOneIntr(void);
E_IO_STS	HalSdio_DetectBusyEnd(U32 timeout);
void		HalSdio_SetSkew(U8 u8SkewIdx, U32 u32SkewValue);
void		HalSdio_DumpRegisterBank(U16 u16BankNo, U16 u16_16BitAddrOffset, U16 u16WordCount);
void		HalSdio_TimerTest(void);
void		HalSdio_TimerStart(void);
U32			HalSdio_TimerStop(void);
void		HalSdio_ClearWaitQueue(void);

#ifdef ENABLE_SDIO_ADMA
U32			HalSdio_PrepareDescriptors(struct mmc_data *data);
void		HalSdio_SetupDescriptorAddr(CMD_RSP_INFO * pCmdRspInfo);
#endif

#ifdef SDIO_SUPPORT_SD30
void		HalSdio_SetDqs(U32 u32DqsValue);
void		HalSdio_SetIOVoltage(U8 u8Volt);
#endif

void		fcie_dump_mem(unsigned char *buf, U32 cnt);
void		sd_bist_test_n_check_CRC_value(void);
void		sd_adma_read_write_test(void);
void		sd_rw_timeout_error_detect(U8 u8ReadWriteDir);


#endif
