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
#include <config.h>
#include "drvFCIE_config.h"

#ifdef SD_USE_FCIE5

#include "common.h"
#include <mmc.h>
#include "drvFCIE5.h"
//#include "eMMC_err_codes.h"


U8 gu8TuningMode = TUNE_FINISH;
U32 gu32BestSkew4 = 0xBEEF;

///////////////////////////////////////////////////////////////////////////////////////////////////





/**************************************
* Macro define
**************************************/
#define TIMEOUT_NORMAL 1000  // 1000ms
#define DBGMMC(X)    //X



/**************************************
* Local Function
**************************************/
void DrvSD_CheckR1(U32 u32CardStatusR1);



/*********************************************************************
*
*   PROCEDURE NAME:
*       HalSdio_Init
*
*   DESCRIPTION:
*       Initial SDIO engine
*
*   NOTE:
*
*********************************************************************/
void HalSdio_Init(void)
{
	HalSdio_Platform_InitChiptop();

	// FCIE_RIU_W16(MIE_PATH_CTL, 0x0000);
	// FCIE_RIU_W16(MIE_EVENT   , 0xFFFF);

	HalSdio_ResetIP();

	HalSdio_SwitchPad(SDIO_PAD_SDR12);

	#if (ALTERNATIVE_IP == FCIE_IP)
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SD_EN);
	#elif (ALTERNATIVE_IP == SDIO_IP)
		FCIE_RIU_W16(FCIE_MIE_FUNC_CTL, BIT_SDIO_MOD);
	#endif
}

/*static U32 gu32Fcie5DebugBus[4];
static char *gStrDebugMode[] = {"CMD state",
								"FIFO status",
								"ADMA Address",
								"ADMA blk cnt"};
*/
//1: CMDstate
//2: {FIFOsts, WRstate, RDstate}l
//3: ADMA Address
//4: ADMA block cnt

void HalSdio_DumpDebugBus(void)
{
	U16 u16CardDbgMode;

	// debug mode: 5: card
	REG_FCIE_CLRBIT(FCIE_EMMC_DEBUG_BUS1, BIT_DEBUG_MODE_MSK);
	REG_FCIE_SETBIT(FCIE_EMMC_DEBUG_BUS1, BIT_DEBUG_MODE_SET);

	printf("Debug mode = %d (5: Card)\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS1)&BIT_DEBUG_MODE_MSK)>>BIT_SHIFT_DEBUG_MODE);

	// card debug mode, 1: CMDstate, 2: FIFO_sts + WRstate + RDstate
	for(u16CardDbgMode=1; u16CardDbgMode<=2; u16CardDbgMode++)
	{
		REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_DEBUG_MODE_MASK);
		REG_FCIE_SETBIT(FCIE_TEST_MODE, u16CardDbgMode<<BIT_DEBUG_MODE_SHIFT);

		if(u16CardDbgMode==1) // CMDstate
		{
			printf("\tCMD  = %Xh\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0)&BIT_STATE_MSK_CMD)	>>BIT_SHIFT_CMD);
		}
		else if(u16CardDbgMode==2) // FIFO_sts + WRstate + RDstate
		{
			//printf("0x38 = %04Xh, 0x39 = %04Xh\n", FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0), FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS1));
			printf("\tRD   = %Xh\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0)&BIT_STATE_MSK_RD)	>>BIT_SHIFT_RD);
			printf("\tWR   = %Xh\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0)&BIT_STATE_MSK_WR)	>>BIT_SHIFT_WR);
			printf("\tFIFO = %Xh\n", (FCIE_RIU_R16(FCIE_EMMC_DEBUG_BUS0)&BIT_STATE_MSK_FIFO)>>BIT_SHIFT_FIFO);
		}
	}
}

void HalSdio_DumpRegisterBank(U16 u16BankNo, U16 u16_16BitAddrOffset, U16 u16WordCount)
{
	U16 u16_i;
	volatile U16 u16_reg;
	U32 u32BankAddr;

	u32BankAddr = RIU_BASE+((U32)u16BankNo<<9);

	printf("Bank NO.: %04Xh\n", u16BankNo);
	printf("Bank Addr: %04Xh\n", u32BankAddr);

	if(u16_16BitAddrOffset) {
		u32BankAddr += u16_16BitAddrOffset << 2;
		printf("After addr offset: %04Xh\n", u32BankAddr);
	}

	printf("\n\t    | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F");
	printf("\n\t----+------------------------------------------------");

	for(u16_i=0 ; u16_i<u16WordCount; u16_i++)
	{
		if(0 == u16_i%8)
			printf("\n\t%02Xh:| ", u16_i+u16_16BitAddrOffset);

		REG_FCIE_R(GET_REG_ADDR(u32BankAddr, u16_i), u16_reg);
		printf("%04Xh ", u16_reg);
	}
    printf("\n");
}

void HalSdio_DumpRegister(void)
{
	//U16 u16_i;

	printf("\n\nfcie reg:");
	HalSdio_DumpRegisterBank(RIU_BANK_SDIO0, 0, 0x40);

#if 0
	printk("SDIO JobCnt: \n");
	printk("JobCnt: %Xh \n", REG_FCIE(FCIE_JOB_BL_CNT));

	printk("\nFCIE CMDFIFO:");
	//eMMC_FCIE_GetCMDFIFO(0, 0x09, (U16*)sgau16_eMMCDebugReg);
	for(u16_i=0 ; u16_i<0x20; u16_i++)
	{
		if(0 == u16_i%8)
			printk("\n%02Xh:| ", u16_i);

		printk("%04Xh ", sgau16_eMMCDebugReg[u16_i]);
	}
#endif

    printf("\n\nCRC reg:");
	HalSdio_DumpRegisterBank(RIU_BANK_SDIO1, 0x40, 0x10);

    printf("\n\nchiptop reg:");
	HalSdio_DumpRegisterBank(RIU_BANK_CHIPTOP, 0, 0x80);

    printf("\n\nclockgen reg:");
	HalSdio_DumpRegisterBank(RIU_BANK_CLKGEN0, 0, 0x80);

	#ifdef SDIO_SUPPORT_SD30
    	printf("\n\nsdio_pll reg:");
		HalSdio_DumpRegisterBank(RIU_BANK_SDIO_PLL, 0, 0x80);
	#endif

}

U16 HalSdio_ReadCRCFifo(U32 i)
{
	return REG_FCIE(SDIO_CRC_ADDR(i));
}

void HalSdio_OpenClock(U8 On_off)
{
    if (On_off)
        FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN);
	else
		FCIE_RIU_16_OF(FCIE_SD_MODE, BIT_CLK_EN);
}

// total 17 bytes 0~16
U8 HalSdio_CmdFifoGet(U8 u8addr)
{
    U16 u16Tmp;

	if(u8addr>16) printf("SD Err: Cmd FIFO addr overflow in get %d\n", u8addr);

	//REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_CHK_CMD);

	u16Tmp = REG_FCIE(FCIE_CMDFIFO_ADDR(u8addr>>1));

	//REG_FCIE_CLRBIT(FCIE_SD_CTRL, BIT_CHK_CMD);

    if(u8addr&0x1)
    {
        return (U8)(u16Tmp>>8);
    }
    else
    {
        return (U8)(u16Tmp&0xFF);
    }
}


// total 17 bytes 0~16
void HalSdio_CmdFifoSet(U8 u8addr, U8 u8value)
{
    U16 u16Tmp;

	if(u8addr>16) printf("SD Err: Cmd FIFO addr overflow to set %d\n", u8addr);

	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_CHK_CMD);

	u16Tmp = REG_FCIE(FCIE_CMDFIFO_ADDR(u8addr>>1));

    if(u8addr & 1)
    {
        u16Tmp = (u8value<<8) + (u16Tmp&0xFF);
    }
    else
    {
        u16Tmp = (u16Tmp&0xFF00)+u8value;
    }

	REG_FCIE_W(FCIE_CMDFIFO_ADDR(u8addr>>1), u16Tmp);

	REG_FCIE_CLRBIT(FCIE_SD_CTRL, BIT_CHK_CMD);

}


void HalSdio_ClearEvents(void)
{
	volatile U16 u16_reg;
	while(1){
		REG_FCIE_W(FCIE_MIE_EVENT, BIT_ALL_CARD_INT_EVENTS);
		REG_FCIE_R(FCIE_MIE_EVENT, u16_reg);
		if(0==(u16_reg&BIT_ALL_CARD_INT_EVENTS))
			break;
		REG_FCIE_W(FCIE_MIE_EVENT, 0);
		REG_FCIE_W(FCIE_MIE_EVENT, 0);
	}
	REG_FCIE_W1C(FCIE_SD_STATUS, BIT_SD_FCIE_ERR_FLAGS); // W1C
}

E_IO_STS ParsingErrStatus(U16 u16_reg)
{
	E_IO_STS IoSts = IO_SUCCESS;

	if(u16_reg&BITS_ERROR)
	{
		if(!(gu8TuningMode&TUNE_SILENCE))
		{
			printf("SD Err: SD_STATUS = %04X, ", REG_FCIE(FCIE_SD_STATUS));
			if(u16_reg&BIT_SD_R_CRC_ERR)	printf("read CRC err ");
			if(u16_reg&BIT_DAT_WR_CERR)		printf("write CRC status err ");
			if(u16_reg&BIT_DAT_WR_TOUT)		printf("transmitted data phase timeout ");
			if(u16_reg&BIT_CMD_NO_RSP)		printf("response timeout");
			if(u16_reg&BIT_CMD_RSP_CERR)	printf("response CRC err ");
			if(u16_reg&BIT_DAT_RD_TOUT)		printf("received data phase timeout ");
			printf("\n\n");
		}

		//HalSdio_DumpRegister();
		//HalSdio_ClearErrSts();
		//IoSts = u16_reg&BITS_ERROR;

			 if(u16_reg&BIT_SD_R_CRC_ERR)	IoSts = IO_R_DATA_CRC_ERROR;
		else if(u16_reg&BIT_DAT_WR_CERR)	IoSts = IO_W_DATA_STS_ERROR;
		else if(u16_reg&BIT_DAT_WR_TOUT)	IoSts = IO_TIME_OUT;
		else if(u16_reg&BIT_CMD_NO_RSP)		IoSts = IO_CMD_NO_RSP;
		else if(u16_reg&BIT_CMD_RSP_CERR)	IoSts = IO_RSP_CRC_ERROR;
		else if(u16_reg&BIT_DAT_RD_TOUT)	IoSts = IO_TIME_OUT;

		return IoSts;
	}
	else
	{
		return IO_SUCCESS;
	}
}

/*********************************************************************
*
*   PROCEDURE NAME:
*       HalSdio_SendCommand
*
*   DESCRIPTION:
*       Send command to card (for all driver use)
*   NOTE:
*
*********************************************************************/



//#define PRINT_CMD_RSP

#define TRANSMIT_BIT 0x40

E_IO_STS HalSdio_SendCommand(CMD_RSP_INFO * pCmdRspInfo)
{
    E_IO_STS IoSts = IO_SUCCESS;
    U8 i;
	U16 u16_reg;
	U16 u16Index = 0;
	//U32 u32_err = eMMC_ST_SUCCESS;
	const U8 u8CmdSize = 5; // command always 6 bytes, exclude CRC is fix to 5
	U8 u8RspSize = 0;

#ifdef PRINT_CMD_RSP
	//printf("\r\n");
	//printf("\33[1;31mCMD%d_", (~TRANSMIT_BIT)&pCmdRspInfo->Command.Cmd.Index);
	printf("CMD%d_", pCmdRspInfo->Command.Cmd.Index);
	printf("[%d] ", u8CmdSize);
    for(i=0; i<u8CmdSize; i++)
    {
		if(i==0)
			printf("%02X ", pCmdRspInfo->Command.Array[i+3]|TRANSMIT_BIT); // will add transmitter bit in CIFC
        else
			printf("%02X ", pCmdRspInfo->Command.Array[i+3]);
    }
	if(pCmdRspInfo->MiuAddr)
	{
		printf("%08Xh ", pCmdRspInfo->MiuAddr);
	}
#endif

	// setup response size from response type
	switch(pCmdRspInfo->RspType) {

		case RSP_TYPE_NO:
			//printf("no rsp\n");
			u8RspSize = 0;
			break;
		case RSP_TYPE_R2:
			//printf("rsp 16 bytes\n");
			u8RspSize = 16; // not include CRC byte
			break;
		default:
			//printf("rsp 5 bytes\n");
			u8RspSize = 5; // not include CRC byte
			break;
	}

	//HalSdio_ResetIP();
	HalSdio_ClearEvents();

	//printf(("FCIE_MIE_EVENT = %04X\r\n", REG_FCIE(FCIE_MIE_EVENT));
    //HalSdio_SetClock(pCmdRspInfo->CardClock);

    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_CLK_EN);
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_DATA_SYNC);

	// setup command & response size
	// hardware generate the last CRC byte automatically
	// hardware check the last CRC byte automatically
	REG_FCIE_W(FCIE_CMD_RSP_SIZE, (u8CmdSize<<BIT_CMD_SIZE_SHIFT) + u8RspSize );

	// prepare command FIFO
    for(i=0; i<u8CmdSize; i++)
    {
		if(i==0)
			HalSdio_CmdFifoSet(i, pCmdRspInfo->Command.Array[i+3]|TRANSMIT_BIT); // add host transmitter bit
		else
			HalSdio_CmdFifoSet(i, pCmdRspInfo->Command.Array[i+3]);
    }

	REG_FCIE_W(FCIE_SD_CTRL, 0); // clear first

	#if SDIO_ERRDET
		FCIE_RIU_16_ON(FCIE_SD_CTRL, BIT_ERR_DET_ON);
	#endif

	if(pCmdRspInfo->CmdType!=CMD_TYPE_ADTC) // simple command
	{
		switch(pCmdRspInfo->RspType) {
			case RSP_TYPE_NO:
				//printf("command only");
		        REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_CMD_EN);
				break;
			case RSP_TYPE_R2:
				//printf("cmd + r2\r\n");
    	        REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_CMD_EN + BIT_SD_RSP_EN + BIT_SD_RSPR2_EN);
				break;
			case RSP_TYPE_R3:
				#ifdef SDIO_R3_CRC_PATCH
			        FCIE_RIU_16_ON(FCIE_SD_CTRL, BIT_RSP_CRC_NOT_CHK);
				#else
					FCIE_RIU_16_OF(FCIE_SD_CTRL, BIT_ERR_DET_ON); // R3 has no CRC, IP detect wrong error
				#endif
			case RSP_TYPE_R1:
			case RSP_TYPE_R1b:
			//case RSP_TYPE_R4:
			//case RSP_TYPE_R5:
			default:
				//printf("cmd + rsp\r\n");
    	        REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_CMD_EN + BIT_SD_RSP_EN);
				break;
		}

		#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
			REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_CMD_END);
		#endif

	}
	else // data transfer command
	{
		// we can enable cmd + data at the same time in read
		if(pCmdRspInfo->ReadWriteDir==DIR_R)
		{
           	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_CMD_EN+BIT_SD_RSP_EN+BIT_SD_DTRX_EN);

			if(pCmdRspInfo->DataPath==PATH_ADMA) {

				REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_ADMA_EN); // ADMA read
			}
			#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
				REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_DMA_END);
			#endif

			#if	SDIO_ERRDET_R_SBIT_TO
				FCIE_RIU_16_ON(FCIE_RD_SBIT_TIMER, pCmdRspInfo->ReadWriteTimeOut&0x7FFF); // what is max timer?
				FCIE_RIU_16_ON(FCIE_RD_SBIT_TIMER, BIT_RD_SBIT_TIMER_EN);
			#endif
		}
		else // write can not send data with cmd at the same time
		{
           	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_CMD_EN+BIT_SD_RSP_EN); // command first
			#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
				REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_CMD_END);
			#endif
		}

		switch(pCmdRspInfo->DataPath)
		{
			case PATH_DMA:
			case PATH_ADMA:
				REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_DATA_DEST);
				#ifdef SDIO_NEW_R2N_PATCH
					FCIE_RIU_16_OF(FCIE_DDR_MODE, BIT_CIF_PAD_IN_RDY_SEL|BIT_CIF_PRE_FULL_SEL_MSK);
				#endif
				break;
			case PATH_R2N:
				REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_DATA_DEST);
				#ifdef SDIO_NEW_R2N_PATCH
					FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_CIF_PAD_IN_RDY_SEL);
					FCIE_RIU_16_OF(FCIE_DDR_MODE, BIT_CIF_PRE_FULL_SEL_MSK);
					switch(HalSdio_GetPadType())
					{
						case SDIO_MODE_GPIO_PAD_BPS:
							FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_CIF_PRE_FULL_SEL_BYPASS);
							break;
						case SDIO_MODE_GPIO_PAD_SDR:
							FCIE_RIU_16_ON(FCIE_DDR_MODE, BIT_CIF_PRE_FULL_SEL_SDR);
							break;
						default:
							printf("R2N only run with bypass or SDR mode %d\n", HalSdio_GetPadType()); while(1);
							break;
					}
				#endif
				break;
			default:
				printf("SD Err: wrong data destination\r\n");
				break;
		}

		//printf("FCIE_SD_MODE = %04X\r\n", REG_FCIE(FCIE_SD_MODE));

		// data bus width setup
		REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_MASK);
		if(pCmdRspInfo->BusWidth==BUS_4_BITS) {

			REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_4); // printf("4 bits\n");
		}
		else if(pCmdRspInfo->BusWidth==BUS_8_BITS) {

			REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_8); // printf("8 bits\n");
		}
		else {
			//printf("1 bit\n");
		}

		REG_FCIE_W(FCIE_BLK_SIZE, pCmdRspInfo->BlockSize);

		REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0,  pCmdRspInfo->MiuAddr & 0xFFFF);
		REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, pCmdRspInfo->MiuAddr >> 16);

		if(pCmdRspInfo->DataPath!=PATH_ADMA) // normal DMA or R2N
		{
			REG_FCIE_W(FCIE_JOB_BL_CNT, pCmdRspInfo->BlockCount);
			REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0,  (pCmdRspInfo->BlockCount*pCmdRspInfo->BlockSize) & 0xFFFF);
			REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, (pCmdRspInfo->BlockCount*pCmdRspInfo->BlockSize) >> 16);
		}
		else // ADMA
		{
			REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
			REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0,  0x0010);
			REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 0x0000);
		}
	}


	#if 0 // debug use only ------------------------------------------------------------------------
	if( pCmdRspInfo->Command.Cmd.Index==17 /*&& pCmdRspInfo->DataPath==PATH_R2N*/ )
	{
		//printf("while(1) on purpose\n"); while(1);
		HalSdio_DumpRegisterBank(RIU_BANK_SDIO0, 0, 0x80);
	}
	#endif // debug use only -----------------------------------------------------------------------

	#if 0
	if( pCmdRspInfo->Command.Cmd.Index == 17 || pCmdRspInfo->Command.Cmd.Index == 24 )
	{
		printf("\nhit any key to fire command\n"); getchar();
	}
	#endif


	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START); // start action, fire cmd for simple cmd & write, fire all for read


	if(pCmdRspInfo->CmdType!=CMD_TYPE_ADTC) // simple command
	{
		IoSts = HalSdio_WaitMieEvent(BIT_SD_CMD_END, SD_TIMEOUT_VALUE);
		if(IoSts==IO_ERROR_DETECT)
		{
			//printf("SD Err: detect error while wait simple cmd end, SD_STATUS = %04X\n", REG_FCIE(FCIE_SD_STATUS));
			goto ErrorHandle;
		}
		else if(IoSts==IO_TIME_OUT)
		{
			printf("SD Err: wait simple cmd timeout\r\n");
			goto ErrorHandle;
		}
    }
    else // data transfer command
    {
		if(pCmdRspInfo->ReadWriteDir==DIR_R) // read adtc
		{
			// R2N need wait for more events
			if(pCmdRspInfo->DataPath==PATH_R2N)
			{
				// read out 64 bytes per time, read 8 times in 512 bytes case
				for(u16Index=0; u16Index<8; u16Index++)
				{
					IoSts = HalSdio_WaitGetCIFD( pCmdRspInfo->pBufferAddr + (u16Index<<4), 64);
					if(IoSts)
					{
						printf("SD Err: wait get CIFD fail %d\n", u16Index);
						goto ErrorHandle;
					}
				}
			}

			IoSts = HalSdio_WaitMieEvent(BIT_DMA_END, SD_DMA_TIMEOUT_VALUE);

			if(IoSts==IO_ERROR_DETECT)
			{
				//printf("SD Err: detect error while wait read end, SD_STATUS = %04X\n", REG_FCIE(FCIE_SD_STATUS));
				goto ErrorHandle;
			}
			else if(IoSts==IO_TIME_OUT)
			{
				printf("SD Err: wait read timeout\r\n");
				goto ErrorHandle;
			}
			#if 0 // test CRC data use only
				printf("\nCRC info:\n");
				for(i=0; i<0x10; i++)
				{
					printf("%04Xh ", HalSdio_ReadCRCFifo(i));
					if(i%8==7) printf("\n");
				}
				printf("\n");
			#endif

		}
		else // write adtc
		{
			// need to wait cmd finish then start the data transfer in write
			IoSts = HalSdio_WaitMieEvent(BIT_SD_CMD_END, SD_TIMEOUT_VALUE);
			if(IoSts==IO_ERROR_DETECT)
			{
				//printf("SD Err: detect error while wait write cmd end, SD_STATUS = %04X\n", REG_FCIE(FCIE_SD_STATUS));
				goto ErrorHandle;
			}
			else if(IoSts==IO_TIME_OUT)
			{
				printf("SD Err: wait write cmd timeout\r\n");
				goto ErrorHandle;
			}
			#if 0
			else // debug use only, write command OK, check response here
			{
				printf("RSP: ");
				for(i=0; i<u8RspSize; i++) // last CRC didn't copy to CIFC
				{
					printf("%02X ", HalSdio_CmdFifoGet(i));
				}
				printf("\n");
			}
			#endif


			//REG_FCIE_CLRBIT(FCIE_SD_CTRL, BIT_SD_RSPR2_EN+BIT_SD_RSP_EN+BIT_SD_CMD_EN+BIT_SD_DTRX_EN+BIT_SD_DAT_DIR_W);
			FCIE_RIU_W16(FCIE_SD_CTRL, BIT_SD_DTRX_EN+BIT_SD_DAT_DIR_W); // data transfer second

			#if	((defined SDIO_ERRDET_W_MIU_TO ) && (SDIO_ERRDET_W_MIU_TO==1)) || \
				((defined SDIO_ERRDET_W_CRC_STS) && (SDIO_ERRDET_W_CRC_STS==1))
				FCIE_RIU_16_ON(FCIE_SD_CTRL, BIT_ERR_DET_ON);
			#endif

			if(pCmdRspInfo->DataPath==PATH_ADMA) {

				REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_ADMA_EN); // ADMA write
			}

			#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
				REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_DMA_END);
			#endif

			#if	SDIO_ERRDET_W_MIU_TO
				FCIE_RIU_16_ON(FCIE_WR_SBIT_TIMER, pCmdRspInfo->ReadWriteTimeOut&0x7FFF); // what is max timer?
				FCIE_RIU_16_ON(FCIE_WR_SBIT_TIMER, BIT_WR_SBIT_TIMER_EN);
			#endif


			//HalSdio_DumpRegisterBank(RIU_BANK_SDIO0, 0, 0x80); // debug use, dump reg before write
			REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START); // start action

			if(pCmdRspInfo->MaskMiuReq)
			{
				HalSdio_MaskMiuReq(1);
			}

			// R2N
			if(pCmdRspInfo->DataPath==PATH_R2N)
			{
				// write in 64 bytes per time, read 8 times in 512 bytes case
				for(u16Index=0; u16Index<8; u16Index++)
				{
					IoSts = HalSdio_WaitSetCIFD( pCmdRspInfo->pBufferAddr + (u16Index<<4), 64);
					if(IoSts)
					{
						printf("SD Err: wait set CIFD fail\r\n");
						goto ErrorHandle;
					}
					#ifdef SDIO_R2N_WRSTATE_PATCH
					if(u16Index<7) // No need for last 64 bytes
					{
						HalSdio_PatchCheckR2nWriteState();
						if(u16Index==6) printf("patch sdio R2N overrun issue\n");
					}
					#endif
				}
			}

	        IoSts = HalSdio_WaitMieEvent(BIT_DMA_END, SD_DMA_TIMEOUT_VALUE);

			if(pCmdRspInfo->MaskMiuReq)
			{
				HalSdio_MaskMiuReq(0);
			}

			if(IoSts==IO_ERROR_DETECT)
			{
				//printf("SD Err: detect error while wait write dma end, SD_STATUS = %04X\n", REG_FCIE(FCIE_SD_STATUS));
				goto ErrorHandle;
			}
			else if(IoSts==IO_TIME_OUT)
			{
				printf("SD Err: wait write dma timeout\r\n");
				goto ErrorHandle;
			}
		}

    }

	// need to check SD_STATUS here ?
	#if (!defined TEST_RW_SBIT_TOUT) || (TEST_RW_SBIT_TOUT==0)
	u16_reg = REG_FCIE(FCIE_SD_STATUS);
	//printf("FCIE_SD_STATUS = %04X", u16_reg);
	#ifndef SDIO_R3_CRC_PATCH
	if( ((pCmdRspInfo->RspType==RSP_TYPE_R2)||(pCmdRspInfo->RspType==RSP_TYPE_R3)) && ((u16_reg&BITS_ERROR)==BIT_CMD_RSP_CERR) ) {
		// patch hardware false alarm of R2 response CRC error
		//printf("patch hardware false alarm of R2 response CRC error\r\n");
	    REG_FCIE_W(FCIE_SD_STATUS, BIT_CMD_RSP_CERR);
	}
	else
	#endif
	IoSts = ParsingErrStatus(u16_reg);
	if(IoSts) goto ErrorHandle;
	#endif

    if(pCmdRspInfo->RspType==RSP_TYPE_R1b)
    {
		#if 0
			IoSts = HalSdio_WaitD0High(TIMEOUT_NORMAL);
		#else
			IoSts = HalSdio_DetectBusyEnd(TIMEOUT_NORMAL);
		#endif
        if(IoSts)
        {
			printf("SD Err: wait D0 high timeout in send cmd\n");
			//while(1);
            goto ErrorHandle;
        }
    }

    if(u8RspSize)
    {
		#ifdef PRINT_CMD_RSP
		printf(", RSP(%d): ", u8RspSize);
		#endif
        for(i=0; i<u8RspSize; i++) // last CRC didn't copy to CIFC
        {
			pCmdRspInfo->Response.Array[i+3] = HalSdio_CmdFifoGet(i);
			#ifdef PRINT_CMD_RSP
			printf("%02X ", pCmdRspInfo->Response.Array[i+3]);
			#endif
        }
		#ifdef PRINT_CMD_RSP
        printf("\n");
		#endif
        /*if(HalSdio_CheckResponse())
        {
            LOG_FCIE(L_FCIE_ERROR, "Error!!! Response Error!\r\n");
            goto ErrorHandle;
        }*/
    }
    else
    {
		#ifdef PRINT_CMD_RSP
        printf("\n");
		#endif
    }

	#if	(defined SDIO_ERRDET_R_SBIT_TO) || (defined SDIO_ERRDET_W_MIU_TO)
		FCIE_RIU_W16(FCIE_RD_SBIT_TIMER, 0x0000);
		FCIE_RIU_W16(FCIE_WR_SBIT_TIMER, 0x0000);
	#endif

	if(pCmdRspInfo->ClockStop==CLK_STOP)
	{
    	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN); // turn off clock after command finish
	}

	if(FCIE_RIU_R16(FCIE_SD_CTRL)&BIT_JOB_START)
	{
		printf(LIGHT_RED"Sd Warn: BIT_JOB_START not cleared\n"NONE);
	}

    return IO_SUCCESS;

ErrorHandle:

	if(!(gu8TuningMode&TUNE_SILENCE))
	{
		//U32 u32CardStatusR1;
    	printf("SD Err: send CMD%d", pCmdRspInfo->Command.Cmd.Index);
		printf("[%d] ", u8CmdSize);
    	for(i=0; i<u8CmdSize; i++)
    	{
        	printf("%02X ", pCmdRspInfo->Command.Array[i+3]);
    	}
		printf("fail %02Xh, ", IoSts);
		printf("RSP(%d): ", u8RspSize);
		for(i=0; i<u8RspSize; i++) // last CRC didn't copy to CIFC
        {
    		printf("%02X ", HalSdio_CmdFifoGet(i));
        }
		printf("\n");
		//HalSdio_DumpDebugBus();
		#if 0
		u32CardStatusR1  = ((U32)HalSdio_CmdFifoGet(1)) << 24;
		u32CardStatusR1 += ((U32)HalSdio_CmdFifoGet(2)) << 16;
		u32CardStatusR1 += ((U32)HalSdio_CmdFifoGet(3)) <<  8;
		u32CardStatusR1 += ((U32)HalSdio_CmdFifoGet(4)) <<  0;
		DrvSD_CheckR1(u32CardStatusR1);
		#endif
		//HalSdio_DumpRegisterBank(FCIE_REG_BASE_ADDR, 0x40);
	}

	u16_reg = REG_FCIE(FCIE_SD_STATUS);
	//printf("FCIE_SD_STATUS = %04X", u16_reg);
	#ifndef SDIO_R3_CRC_PATCH
	if( ((pCmdRspInfo->RspType==RSP_TYPE_R2)||(pCmdRspInfo->RspType==RSP_TYPE_R3)) && ((u16_reg&BITS_ERROR)==BIT_CMD_RSP_CERR) ) {
		// patch hardware false alarm of R2 response CRC error
		//printf("patch hardware false alarm of R2 response CRC error\r\n");
	    REG_FCIE_W(FCIE_SD_STATUS, BIT_CMD_RSP_CERR);
	}
	else
	#endif
	IoSts = ParsingErrStatus(u16_reg);

	#if	(defined SDIO_ERRDET_R_SBIT_TO) || (defined SDIO_ERRDET_W_MIU_TO)
		FCIE_RIU_W16(FCIE_RD_SBIT_TIMER, 0x0000);
		FCIE_RIU_W16(FCIE_WR_SBIT_TIMER, 0x0000);
	#endif

	if(pCmdRspInfo->ClockStop==CLK_STOP)
	{
	    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN); // turn off clock after command finish even fail
	}

	if(FCIE_RIU_R16(FCIE_SD_CTRL)&BIT_JOB_START)
	{
		if(!(gu8TuningMode&TUNE_NOW))
		{
			printf(LIGHT_RED"Sd Warn: BIT_JOB_START not cleared\n"NONE);
		}
	}

	#if (defined PATCH_MINUS2PLUS2) && (PATCH_MINUS2PLUS2)
	if(!(gu8TuningMode&TUNE_NOW))
	{
		if(gu32BestSkew4==gu32BestSkew4sub2)
		{
			gu32BestSkew4 = gu32BestSkew4add2;
			HalSdio_SetSkew(4, gu32BestSkew4);
			printf(LIGHT_RED"meet meta issue, use +2 skew4 --> %d\n"NONE, gu32BestSkew4);
		}
	}
	#endif

	HalSdio_ResetIP();

    return IoSts;
}


U32 HalSdio_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec)
{
	U32 u32_Count;
	U16 u16_Reg;

	for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
	{
		REG_FCIE_R(NC_CIFD_EVENT, u16_Reg);
		if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
		{
			//printf("Got CIFD event\n");
			break;
		}
		udelay(HW_TIMER_DELAY_1us);
	}
	if (u32_Count >= u32_MicroSec)
	{
		REG_FCIE_R(NC_CIFD_EVENT, u16_Reg);
		printf("CIFD event timeout %d, %d\n", u32_Count, u32_MicroSec);
	}
	return u32_Count;
}

E_IO_STS HalSdio_WaitGetCIFD(U32 * pu32_DataBuf, U32 u32_ByteCnt)
{
	U16 u16_i, *pu16_Data = (U16*)pu32_DataBuf;

	if(u32_ByteCnt > FCIE_CIFD_BYTE_CNT)
	{
		return IO_ERROR;
	}
	if (HalSdio_WaitCIFD_Event(BIT_WBUF_FULL, HW_TIMER_DELAY_500ms)== (HW_TIMER_DELAY_500ms ))
	{
		printf("SdErr: CIFD timeout\n");
		return IO_ERROR; // timeout
	}

	for(u16_i=0; u16_i<(u32_ByteCnt>>1); u16_i++)
		REG_FCIE_R(NC_WBUF_CIFD_ADDR(u16_i), pu16_Data[u16_i]);

	REG_FCIE_W(NC_CIFD_EVENT, BIT_WBUF_FULL);

	REG_FCIE_W(NC_CIFD_EVENT, BIT_WBUF_EMPTY_TRI);

	return IO_SUCCESS;
}

E_IO_STS HalSdio_WaitSetCIFD(U32 * pu32_DataBuf, U32 u32_ByteCnt)
{
	U16 u16_i, *pu16_Data = (U16*)pu32_DataBuf;

	if(u32_ByteCnt > FCIE_CIFD_BYTE_CNT)
	{
		return IO_ERROR;
	}

	for(u16_i=0; u16_i<(u32_ByteCnt>>1); u16_i++)
		REG_FCIE_W(NC_RBUF_CIFD_ADDR(u16_i), pu16_Data[u16_i]);

	REG_FCIE_SETBIT(NC_CIFD_EVENT, BIT_RBUF_FULL_TRI);

	if (HalSdio_WaitCIFD_Event(BIT_RBUF_EMPTY, HW_TIMER_DELAY_500ms)== (HW_TIMER_DELAY_500ms ))
	{
		printf("Error: wait set CIFD timeout\r\n");
		HalSdio_DumpRegisterBank(RIU_BANK_SDIO0, 0,0x40);
		return IO_TIME_OUT;
	}

	#if (ALTERNATIVE_IP == FCIE_IP)

	if(HalSdio_WaitMieEvent(BIT_R2N_RDY_INT, HW_TIMER_DELAY_500ms))
	{
		printf("SdErr: wait mie event r2n ready to\n");
	}

	#endif

	REG_FCIE_SETBIT(NC_CIFD_EVENT, BIT_RBUF_EMPTY);

	//printf("set CIFD OK\n");

	return IO_SUCCESS;
}










/*********************************************************************
*
*   PROCEDURE NAME:
*       HalSdio_WaitD0High
*
*   DESCRIPTION:
*       Check SD data 0 is high or low.
*   NOTE:
*
*********************************************************************/
E_IO_STS HalSdio_WaitD0High(U32 timeout)
{
	volatile U32 u32_cnt;

	REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_CLK_EN);

	for(u32_cnt=0; u32_cnt < timeout; u32_cnt++)
	{
		if( !(FCIE_RIU_R16(FCIE_SD_STATUS) & BIT_SD_CARD_BUSY) ) return IO_SUCCESS;
		udelay(1000);
	}

	printf("SDIO Err: wait D0 H timeout: %04Xh\n", REG(FCIE_SD_STATUS));
	return IO_TIME_OUT;

}

E_IO_STS HalSdio_DetectBusyEnd(U32 timeout)
{
	REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_CLK_EN);
	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_BUSY_DET_ON);

	if(HalSdio_WaitMieEvent(BIT_BUSY_END_INT, timeout)) {
		printf("Wait busy end event timeout\n");
		return IO_TIME_OUT;
	}

	// print out this message to confirm busy end detect
	//printf("Got the busy end event, FCIE_SD_CTRL = %04X\n", REG_FCIE(FCIE_SD_CTRL));

	return IO_SUCCESS;
}

/*********************************************************************
*
*   PROCEDURE NAME:
*       HalSdio_SetBusWidth
*
*   DESCRIPTION:
*       Set SDIO data bus width.
*   NOTE:
*
*********************************************************************/
E_IO_STS HalSdio_SetBusWidth(U8 * pu8BusWidth, E_DATA_BUS DataBus)
{
    FCIE_RIU_16_OF(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_4+BIT_SD_DATA_WIDTH_8);

    switch(DataBus)
    {
        case BUS_1_BIT:
            //printf("set 1 bit width\n");
            *pu8BusWidth = BUS_1_BIT;
            break;

        case BUS_4_BITS:
			//FCIE_RIU_16_ON(SD_MODE, R_SD_4BITS);
            //printf("set 4 bits width\n");
			REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_4);
            *pu8BusWidth = BUS_4_BITS;
            break;

        case BUS_8_BITS:
			// FCIE_RIU_16_ON(SD_MODE, R_SD_8BITS);
			REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_DATA_WIDTH_8);
            *pu8BusWidth = BUS_8_BITS;
            break;

        default:
            printf("wrong para in HalSdio_SetBusWidth()\r\n");
            return IO_ERROR;
    }
    return IO_SUCCESS;
}

void HalSdio_SetShiftTune(U8 u8Select, U8 u8Type, U8 u8Count)
{
	if(u8Count>=16) {
		printf("SDIO Err: wrong shift counter\n");
		return;
	}

	if(u8Type==1) // response
	{
		if(u8Select)
		{
			REG_FCIE_CLRBIT(FCIE_RSP_SHIFT_CNT, BIT_RSP_SHIFT_TUNE_MASK);
			REG_FCIE_SETBIT(FCIE_RSP_SHIFT_CNT, u8Count|BIT_RSP_SHIFT_SEL);
		}
		else
		{
			REG_FCIE_CLRBIT(FCIE_RSP_SHIFT_CNT, BIT_RSP_SHIFT_TUNE_MASK|BIT_RSP_SHIFT_SEL);
		}
	}
	else if(u8Type==2) // read clock stop
	{
		if(u8Select)
		{
			REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_RSTOP_SHIFT_TUNE_MASK);
			REG_FCIE_SETBIT(FCIE_RX_SHIFT_CNT, u8Count|BIT_RSTOP_SHIFT_SEL);
		}
		else
		{
			REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_RSTOP_SHIFT_TUNE_MASK|BIT_RSTOP_SHIFT_SEL);
		}
	}
	else if(u8Type==3) // write crc status
	{
		if(u8Select)
		{
			REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_WRSTS_SHIFT_TUNE_MASK);
			REG_FCIE_SETBIT(FCIE_RX_SHIFT_CNT, (u8Count<<8)|BIT_WRSTS_SHIFT_SEL);
		}
		else
		{
			REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_WRSTS_SHIFT_TUNE_MASK|BIT_WRSTS_SHIFT_SEL);
		}

	}
	else
	{
		printf("SDIO Err: wrong shft type\n");
	}

	//printf("RSP_SHIFT_CNT = %04Xh, RX_SHIFT_CNT = %04Xh\n", REG_FCIE(FCIE_RSP_SHIFT_CNT), REG_FCIE(FCIE_RX_SHIFT_CNT));
}



U8 HalSdio_SendClocknWaitD0High(void)
{
    E_IO_STS IoSts = IO_SUCCESS;

    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_CLK_EN);

	IoSts = HalSdio_WaitD0High(TIMEOUT_NORMAL);
    if(IoSts)
    {
		printf("SD Err: wait D0 high timeout in HalSdio_SendClocknWaitD0High()\n");
        goto ErrorHandle;
    }

    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_CLK_EN);

	return 0;

ErrorHandle:

	return 1;

}

void HalSdio_SetMiuBurstLength(U8 u8BurstLength)
{
	FCIE_RIU_16_OF(FCIE_MMA_PRI_REG, BIT_MIU_BUS_TYPE_MASK);

	switch(u8BurstLength)
	{
		case 1:
			break;
		case 2:
			FCIE_RIU_16_ON(FCIE_MMA_PRI_REG, BIT_MIU_BURST2);
			break;
		case 4:
			FCIE_RIU_16_ON(FCIE_MMA_PRI_REG, BIT_MIU_BURST4);
			break;
		case 8:
			FCIE_RIU_16_ON(FCIE_MMA_PRI_REG, BIT_MIU_BURST8);
			break;
		default:
			printf("SdErr: wrong burst length parameter\n");
			break;
	}
}

#endif // #ifdef SD_USE_FCIE5



