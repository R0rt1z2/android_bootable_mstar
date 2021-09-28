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
#include "../inc/type.h"
#include "../inc/Globalvar.h"
#include "../inc/sysusbdrv.h"
#include "../inc/drvUSBDwld.h"
#include "../inc/ms_drc.h"
#include "../inc/ms_msd_fn_scsi.h"


//USB_VAR		usbDrvVar;
//vendorCmdStruct     scsiVendorCmdVar;
//static U8 isUsbInit = 0;

//extern u8   gu8ucDmaBuf[32*1024];

//extern void DelayUS(int us);



u8 readb(const volatile void *addr)
{
	return *(const volatile u8 *) addr;
}

void writeb(u8 b, volatile void *addr)
{
	*(volatile u8 *) addr = b;
}

u16 readw(const volatile void *addr)
{
	return *(const volatile u16 *) addr;
}

void writew(u16 b, volatile void *addr)
{
	*(volatile u16 *) addr = b;
}

void USB_DelayUS(int us)
{
#define RIUBASE_TIMER0    (0x1f000000 + (0x1810 << 2))
#define     REG_TIMER0_EN     (RIUBASE_TIMER0   + (0x0000 << 2))
#define     REG_TIMER0_HIT    (RIUBASE_TIMER0   + (0x0001 << 2))
#define     REG_TIMER0_MAX_LO (RIUBASE_TIMER0   + (0x0002 << 2))
#define     REG_TIMER0_MAX_HI (RIUBASE_TIMER0   + (0x0003 << 2))
#define C_WREG32(addr, value) do { *(volatile u32 *)(addr) = (value); asm volatile ("" : : : "memory"); } while(0)
#define C_RREG32(addr)        (*(volatile u32 *)(addr))

    // PIU timer0
    C_WREG32(REG_TIMER0_EN, 0);
    C_WREG32(REG_TIMER0_MAX_LO, (us * 12) & 0x0000FFFF);
    C_WREG32(REG_TIMER0_MAX_HI, (us * 12) >> 16);
    C_WREG32(REG_TIMER0_EN, 2);
    while(C_RREG32(REG_TIMER0_HIT) == 0)
        ;
}

u8 usb_dongle_detect(void)
{
	#define UTMI_BASE	0x3a80
	#define UHC_BASE	0x2400
	#define USBC_BASE	0x0700
	#define USBBC_BASE	0x13700
	#define HW_BASE		0x1f200000

	#define BIT1	0x02
	#define BIT3	0x08
	#define BIT4	0x10
	#define BIT6	0x40
	#define BIT7	0x80

	u16 lisnestatus=0, org_regs;
	u8 result=0, i=0, temp;

	result=1;
#if 0
	//disable BC
	writeb(readb((void*)HW_BASE+(USBC_BASE+0x02)*2) | 0x01, (void*) (HW_BASE+(USBC_BASE+0x02)*2));
	writeb(readb((void*)HW_BASE+(UHC_BASE+0x40)*2) & ~0x10, (void*) (HW_BASE+(UHC_BASE+0x40)*2));// 0: VBUS On.

	writeb(readb((void*)HW_BASE+(USBBC_BASE+0x43)*2-1) & ~0x40, (void*) (HW_BASE+(USBBC_BASE+0x43)*2-1));
	writeb(readb((void*)HW_BASE+(USBBC_BASE+0x4c)*2) & ~0x40, (void*) (HW_BASE+(USBBC_BASE+0x4c)*2));
#endif

	//restore orginal register
	org_regs=readw((void*)(HW_BASE+(UTMI_BASE+0x00)*2));

	/*********check DP, DM status***********/
	//enable UTMI
	writeb(readb((void*)(HW_BASE+(UTMI_BASE+0x08)*2)) & ~BIT7, (void*) (HW_BASE+(UTMI_BASE+0x08)*2));
	writew(0x0001, (void*) (HW_BASE+(UTMI_BASE+0x00)*2));

	//enable overwrite mode
	writeb(readb((void*)(HW_BASE+(UTMI_BASE+0x00)*2)) | BIT1, (void*) (HW_BASE+(UTMI_BASE+0x00)*2));

	for(i=0;i<3;i++)
	{
		//case1 dp(0), dm(0)
		temp = ((((readb((void*)(HW_BASE+(UTMI_BASE+0x00)*2)) & ~BIT3) & ~BIT4) | BIT6) | BIT7);
		writeb(temp, (void*)(HW_BASE+(UTMI_BASE+0x00)*2));
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~BIT3 & ~BIT4, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear dp, dm pull up
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) | BIT6 | BIT7, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//pull down dp, dm
		USB_DelayUS(5000);
		lisnestatus= (readw((void*)(HW_BASE+(UTMI_BASE+0x30)*2)) >> 12) & 0x3;
		//printf("[DP(0),DM(0)]lisnestatus:0x%x\n",lisnestatus);
		if(lisnestatus!=0x00)
		{
			result=0;
			//printf("You are not download dongle, OUT!!\n");
			break;
		}

		//case2 dp(0), dm(1)
		temp = ((readb((void*)(HW_BASE+(UTMI_BASE+0x00)*2)) & ~BIT7) | BIT4);
		writeb(temp, (void*) (HW_BASE+(UTMI_BASE+0x00)*2));
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~BIT7, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear dm pull down
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) | BIT4, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//pull up dm
		USB_DelayUS(5000);
		lisnestatus= (readw((void*)(HW_BASE+(UTMI_BASE+0x30)*2)) >> 12) & 0x3;
		//printf("[DP(0),DM(1)]lisnestatus:0x%x\n",lisnestatus);
		if((lisnestatus)!=0x03)
		{
			result=0;
			//printf("You are not download dongle, OUT!!\n");
			break;
		}

		//case3 dp(1), dm(0)
		temp = ((((readb((void*)(HW_BASE+(UTMI_BASE+0x00)*2)) & ~BIT6) & ~BIT4) | BIT3) | BIT7);
		writeb(temp, (void*)(HW_BASE+(UTMI_BASE+0x00)*2));
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~BIT6 & ~BIT4, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear dp pull down
		//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) | BIT3 | BIT7, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//pull up dp
		USB_DelayUS(5000);
		lisnestatus= (readw((void*)(HW_BASE+(UTMI_BASE+0x30)*2)) >> 12) & 0x3;
		//printf("[DP(1),DM(0)]lisnestatus:0x%x\n",lisnestatus);
		if((lisnestatus)!=0x03)
		{
			result=0;
			//printf("You are not download dongle, OUT!!\n");
			break;
		}
	}

	//if(result==1)
	//	printf("You are download dongle, GO!!\n");

	//Recovery all
	//temp = readb((void*)HW_BASE+(UTMI_BASE+0x00)*2) & ~BIT3 & ~BIT4 & ~BIT6 & ~BIT7 & ~BIT1;
	writeb(org_regs, (void*) (HW_BASE+(UTMI_BASE+0x00)*2));
	//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~BIT3 & ~BIT4, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear dp, dm pull up
	//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~BIT6 & ~BIT7, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear dp, dm pull down
	//writeb(readb((void*)HW_BASE+(UTMI2_BASE+0x00)*2) & ~0x02, (void*) (HW_BASE+(UTMI2_BASE+0x00)*2));//clear overwrite mode

	return result;	//result =1 means dongle
}

void *otg_memcpy(void *dest, const void *src, unsigned int n)
{
    char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}

S32 MDrv_USB_Read(U8 *buf, U32 len, USB_VAR *gUSBStruct)
{
    S32     rdLen;
    vendorCmdStruct *pCmdStruct = (vendorCmdStruct *)gUSBStruct->ptrVCmdStruct;
    //if (!isUsbInit)
    //    return 0;
    if (pCmdStruct->currDwldBytes - pCmdStruct->accReadBytes > len)
        rdLen = len;
    else
    {
        rdLen = pCmdStruct->currDwldBytes - pCmdStruct->accReadBytes;
        pCmdStruct->currDwldBytes = 0;
    }
    if (rdLen)
    {
	//gprintf("rdLen:0x%x\n",rdLen);
        otg_memcpy(buf, pCmdStruct->ucptrDataBuf + pCmdStruct->accReadBytes, rdLen);
        if (!pCmdStruct->currDwldBytes)
            pCmdStruct->accReadBytes = 0;
        else
            pCmdStruct->accReadBytes += rdLen;
    }
    return  rdLen;
}


S32 MDrv_USB_Write(U8 *buf, U32 len, USB_VAR *gUSBStruct)
{
    vendorCmdStruct *pCmdStruct = (vendorCmdStruct *)gUSBStruct->ptrVCmdStruct;
    S32     wrLen;
    U32     u32offset = pCmdStruct->currUpldOffset;
	//TONY
	int n;

    //if (!isUsbInit)
    //    return 0;
    /* Flush USB buffer for writing first (USB R/W is a simplex channel) */
    pCmdStruct->currDwldBytes = 0;
    pCmdStruct->accReadBytes = 0;
    if (u32offset)
    {
        if (pCmdStruct->currUpldBytes)
            otg_memcpy(pCmdStruct->ucptrDataBuf, pCmdStruct->ucptrDataBuf + u32offset,
                   pCmdStruct->currUpldBytes);
        /* Reset currUpldOffset member */
        pCmdStruct->currUpldOffset = 0;
    }
    u32offset = pCmdStruct->currUpldBytes;
    if (u32offset + len <= (16*1024))
        wrLen = len;
    else
        wrLen = (16*1024) - u32offset;


    //memcpy(pCmdStruct->ptrPPB2Buf/* + u32offset*/, buf, wrLen);
    for(n=0; n<wrLen; n++)
    {
		pCmdStruct->ptrPPB2Buf[n] = buf[n];
    }
    pCmdStruct->currUpldBytes += wrLen;
    return  wrLen;
}


void UTMI_Init(void)
{
	u32 UTMI_base;
	//printk("+UTMI\n");

	UTMI_base=UTMIBaseAddress;

	writeb((u8)((readb((void*)(UTMI_base+0x06*2)) & ~BIT5) | BIT6), (void*)(UTMI_base+0x06*2)); // reg_tx_force_hs_current_enable
	writeb((u8)((readb((void*)(UTMI_base+0x03*2-1)) & ~BIT4) | (BIT3 | BIT5)), (void*)(UTMI_base+0x03*2-1)); // Disconnect window select
	writeb(readb((void*)(UTMI_base+0x04*2)) | BIT6, (void*)(UTMI_base+0x04*2));
	writeb(readb((void*)(UTMI_base+0x07*2-1)) & (u8)(~BIT1), (void*)(UTMI_base+0x07*2-1)); // Disable improved CDR

	//ENABLE_UTMI_240_AS_120_PHASE_ECO
	/* bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron */
	writeb(readb((void*)(UTMI_base+0x08*2)) | BIT3, (void*)(UTMI_base+0x08*2));
	writeb(readb((void*)(UTMI_base+0x09*2-1)) | (BIT0 | BIT7), (void*)(UTMI_base+0x09*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement

	writeb(readb((void*)(UTMI_base+0x0b*2-1)) | BIT7, (void*)(UTMI_base+0x0b*2-1)); // TX timing select latch path
	writeb(readb((void*)(UTMI_base+0x15*2-1)) | BIT5, (void*)(UTMI_base+0x15*2-1)); // Chirp signal source select

#if defined(ENABLE_UTMI_55_INTERFACE)
	writeb(readb((void*)(UTMI_base+0x15*2-1)) | BIT6, (void*)(UTMI_base+0x15*2-1)); // change to 55 interface
#endif

	/* Init UTMI disconnect level setting */
	writeb(0x0, (void*)(UTMI_base+0x2a*2));

	/* Init UTMI eye diagram parameter setting */
	writeb(readb((void*)(UTMI_base+0x2c*2)) | 0x10, (void*)(UTMI_base+0x2c*2));
	writeb(readb((void*)(UTMI_base+0x2d*2-1)) | 0x02, (void*)(UTMI_base+0x2d*2-1));
	writeb(readb((void*)(UTMI_base+0x2e*2)) | 0x00, (void*)(UTMI_base+0x2e*2));
	writeb(readb((void*)(UTMI_base+0x2f*2-1)) | 0x81, (void*)(UTMI_base+0x2f*2-1));

	//printk("-UTMI\n");
}

void MDrv_USB_Init(void)
{
	//int ii=0;

	//printf("MDrv_USB_Init++\n");
	/***	create irq
	hal_interrupt_mmp_create( CYGNUM_HAL_INTERRUPT_OTG, 0, (isr_entry)usbIsr, NULL);
	***/
#if 1
	USBC_REG_WRITE8(0x02*2, (USBC_REG_READ8(0x02*2)& ~(BIT0|BIT1)) | (BIT1));
	UTMI_REG_WRITE8(0x3C*2, UTMI_REG_READ8(0x3C*2) | 0x1); // set CA_START as 1
	USB_DelayUS(1000);
	UTMI_REG_WRITE8(0x3C*2, UTMI_REG_READ8(0x3C*2) & ~0x01); // release CA_START
	while ((UTMI_REG_READ8(0x3C*2) & 0x02) == 0);        // polling bit <1> (CA_END)

	// Reset OTG controllers
	USBC_REG_WRITE8(0, 0xC);

	// Unlock Register R/W functions  (RST_CTRL[6] = 1)
	// Enter suspend  (RST_CTRL[3] = 1)
	USBC_REG_WRITE8(0, 0x48);

	UTMI_Init();

	// 2'b10: OTG enable
	USBC_REG_WRITE8(0x02*2, (USBC_REG_READ8(0x02*2)& ~(BIT0|BIT1)) | (BIT1));

	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100)&0xFE); // Reset OTG
	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100)|0x01);
	USB_REG_WRITE16(0x100, USB_REG_READ16(0x100)|0x8000); /* Disable DM pull-down */

	// Set FAddr to 0
	USB_REG_WRITE8(M_REG_FADDR, 0);
	// Set Index to 0
	USB_REG_WRITE8(M_REG_INDEX, 0);
	USB_REG_WRITE8(M_REG_CFG6_H, USB_REG_READ8(M_REG_CFG6_H) | 0x08);
	USB_REG_WRITE8(M_REG_CFG6_H, USB_REG_READ8(M_REG_CFG6_H) | 0x40);

	//Pull up D+
	//printk("HIGH SPEED\n");
	USB_REG_WRITE8(M_REG_POWER, (USB_REG_READ8(M_REG_POWER) & ~M_POWER_ENSUSPEND) | M_POWER_SOFTCONN | M_POWER_HSENAB);

	USB_REG_WRITE8(M_REG_DEVCTL,0);

	// Flush the next packet to be transmitted/ read from the endpoint 0 FIFO
	USB_REG_WRITE16(M_REG_CSR0, USB_REG_READ16(M_REG_CSR0) | M_CSR0_FLUSHFIFO);
	// Flush the latest packet from the endpoint Tx FIFO
	USB_REG_WRITE8(M_REG_INDEX, 1);
	USB_REG_WRITE16(M_REG_TXCSR, USB_REG_READ16(M_REG_TXCSR) | M_TXCSR_FLUSHFIFO);

	// Flush the next packet to be read from the endpoint Rx FIFO
	USB_REG_WRITE8(M_REG_INDEX, 2);
	USB_REG_WRITE16(M_REG_RXCSR, USB_REG_READ16(M_REG_RXCSR) | M_RXCSR_FLUSHFIFO);

	USB_REG_WRITE8(M_REG_INDEX, 0);

	// Clear all control/status registers
	USB_REG_WRITE16(M_REG_CSR0, 0);
	USB_REG_WRITE8(M_REG_INDEX, 1);
	USB_REG_WRITE16(M_REG_TXCSR, 0);
	USB_REG_WRITE8(M_REG_INDEX, 2);
	USB_REG_WRITE16(M_REG_RXCSR, 0);

	USB_REG_WRITE8(M_REG_INDEX, 0);

	// Enable all endpoint interrupts
	USB_Set_DRC_Interrupts();
#else
	UTMI_REG_WRITE16(0x14, (UTMI_REG_READ16(0x14) | 0x8000));
    UTMI_REG_WRITE16(0x10, 0x1403); // <12> Enable MPLL, bit <7> pd_bg_current_dft=0
    UTMI_REG_WRITE16(0x04, 0x2084); // Enable CLK12_SEL bit <2> for select low voltage crystal clock, <2>: 0: low V, 1: high V
	UTMI_REG_WRITE16(0x40, 0x8051); // PLL_TEST[15:0], for 24MHz
	UTMI_REG_WRITE16(0x44, 0x2088); // PLL_TEST[31:16] for 24MHz
    UTMI_REG_WRITE16(0x00, 0x6BC3); // Turn on reference voltage and regulator, reg_pdn: bit<15>, bit <2> ref_pdn
    UTMI_REG_WRITE16(0x00, 0x69C3); // Turn on UPLL, reg_pdn: bit<9>
    UTMI_REG_WRITE16(0x00, 0x0001); // Turn all (including hs_current) use override mode
	USBC_REG_WRITE16(0x00, 0x0068);
    UTMI_REG_WRITE16(0x14, (UTMI_REG_READ16(0x14) | 0x8000)); // 2010.02.04 David added
    UTMI_REG_WRITE16(0x10, 0x1403); // <12> Enable MPLL, bit <7> pd_bg_current_dft=0
    UTMI_REG_WRITE16(0x04, 0x2084); // Enable CLK12_SEL bit <2> for select low voltage crystal clock, <2>: 0: low V, 1: high V
    UTMI_REG_WRITE16(0x40, 0x0003); // Enable PLL_TEST[0] for select low voltage crystal clock, Set PLL_TEST[1] for PLL multiplier 20X, for 24MHz
    UTMI_REG_WRITE16(0x00, 0x6BC3); // Turn on reference voltage and regulator, reg_pdn: bit<15>, bit <2> ref_pdn
    UTMI_REG_WRITE16(0x00, 0x69C3); // Turn on UPLL, reg_pdn: bit<9>
    UTMI_REG_WRITE16(0x00, 0x0001); // Turn all (including hs_current) use override mode
    USBC_REG_WRITE8(0x00, 0x68);
    UTMI_REG_WRITE16(0x04, 0x2004); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    UTMI_REG_WRITE16(0x40, 0x0003); //Enable PLL_TEST[0] for select low voltage crystal clock
    UTMI_REG_WRITE16(0x00, 0x6BC3); //Turn on reference voltage and regulator
    UTMI_REG_WRITE16(0x00, 0x69C3); //Turn on UPLL, reg_pdn: bit<9>
    UTMI_REG_WRITE16(0x00, 0x0001); //Turn all (including hs_current) use override mode
	USBC_REG_WRITE8(0x00, 0x68);
	USBC_REG_WRITE16(0x04, 0x0002);
	UTMI_REG_WRITE16(0x28, UTMI_REG_READ16(0x28)|0x4000);
    UTMI_REG_WRITE16(0x24, 0x01FF);	// `TV.riu_write(`REG_UTMI_BASE+16'h0009, 16'h01FF);
    UTMI_REG_WRITE16(0x24, 0x00FF);	// `TV.riu_write(`REG_UTMI_BASE+16'h0009, 16'h00FF);
    UTMI_REG_WRITE16(0x78, 0x0001); //Set CA_START as 1
    UTMI_REG_WRITE16(0x78, 0x0000); //Release CA_START
    while((UTMI_REG_READ16(0x78)&0x0002)==0) //Polling bit <1> (CA_END)
    {
        if (ii++>10000)
        {
            //printf("Poll USB port0 CA_END timeout\n");
            break;
        }
    }

	UTMI_REG_WRITE16(0x0C, 0xB043); //Set bit <1:0> TX/RX reset
	UTMI_REG_WRITE16(0x0C, 0xB040); //Release bit <1:0> TX/RX reset

	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100)&0xFE);		// Reset OTG
	USB_REG_WRITE8(0x100, USB_REG_READ8(0x100)|0x01);

	USB_REG_WRITE16(0x100, USB_REG_READ16(0x100)|0x8000);

	/*enable otg interrup*/
	USB_Set_DRC_Interrupts();

	USB_REG_WRITE16(M_REG_CFG1_L, 0);
	USB_REG_WRITE16(M_REG_CFG2_L, 0);
#endif

	USB_REG_WRITE8(M_REG_FADDR, 0);
	USB_REG_WRITE8(M_REG_CFG6_H, USB_REG_READ8(M_REG_CFG6_H) | 0x08);
	USB_REG_WRITE8(M_REG_POWER, M_POWER_SOFTCONN | M_POWER_HSENAB);

	USB_REG_WRITE8(M_REG_DEVCTL, 0);
	//printf("MDrv_USB_Init--\n");
//	isUsbInit = 1; // for MDrv_USB_Read and MDrv_USB_Write error handling
}


//void function_test(void)
//{
//	if (fpUsbCmdLoop)
//		fpUsbCmdLoop(ptrusbStruct);
//}

void MDrv_USB_CmdLoop(void *pUsbStruct)
{
    if (otgIsUSBConfiged((USB_VAR *)pUsbStruct))
    {
        MSDFNCMDloop((USB_VAR *)pUsbStruct);
    }
}


void usbIsr(void)
//__irq void usbIsr(void)
{
	//USB_DRC_Interrupt(&usbDrvVar);
}

void ebable_UPLL(void)
{
	int i=0,j=0;
	//MIU128PLL Power on
	*(volatile unsigned int *)(0x1f2212d0) = (0x8211);
	// pd 63[0]
	*(volatile unsigned int *)(0x1f2212c4) = (0x0001);
	//clk_miu_p
	*(volatile unsigned int *)(0x1f20167c) = (0x0008);


	*(volatile unsigned int *)((0x1f200000)+(0x000800*2)) = (0x00c0);//enable UPLL0 clock
	*(volatile unsigned int *)((0x1f200000)+(0x00080e*2)) = (0x0001);//enable 384MHz clk from UPLL0
	*(volatile unsigned int *)((0x1f200000)+(0x000804*2)) = (0x0110);//UPLL0
	//initial UPLL1
	*(volatile unsigned int *)((0x1f200000)+(0x000820*2)) = (0x00c0);//enable UPLL1 clock
	*(volatile unsigned int *)((0x1f200000)+(0x000824*2)) = (0x0110);//UPLL1

	*(volatile unsigned int *)((0x1f200000)+(0x003A88*2)) = (0x040F);// UTMI0
	*(volatile unsigned int *)((0x1f200000)+(0x003A80*2)) = (0x7F05);

	*(volatile unsigned int *)((0x1f200000)+(0x003A08*2)) = (0x040F);// UTMI1
	*(volatile unsigned int *)((0x1f200000)+(0x003A00*2)) = (0x7F05);

	*(volatile unsigned int *)((0x1f200000)+(0x003908*2)) = (0x040F);// UTMI2
	*(volatile unsigned int *)((0x1f200000)+(0x003900*2)) = (0x7F05);
	//Module: usb0 (0x100700)
	*(volatile unsigned int *)((0x1f200000)+(0x000700*2)) = (0x0a);	//Disable MAC initial suspend, Reset UHC
	*(volatile unsigned int *)((0x1f200000)+(0x000700*2)) = (0x28);	//Release UHC reset, enable UHC and OTG XIU function
	//Module: utmi0 (0x103A80)
	*(volatile unsigned int *)((0x1f200000)+(0x003AA2*2)) = (0x2088);//PLL_TEST[30:28]: 3'b101 for IBIAS current select
	*(volatile unsigned int *)((0x1f200000)+(0x003AA0*2)) = (0x8051);//PLL_TEST[15]: Bypass 480MHz clock divider
	*(volatile unsigned int *)((0x1f200000)+(0x003A82*2)) = (0x2084);//Enable CLK12_SEL bit <2> for select low voltage crystal clock
	*(volatile unsigned int *)((0x1f200000)+(0x003A88*2)) = (0x0426);//bit<7>: Power down UTMI port-0 bandgap current

	*(volatile unsigned int *)((0x1f200000)+(0x003A80*2)) = (0x6bc3);//reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
	for(i=0;i<=1000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003A80*2)) = (0x69c3);//Turn on UPLL, reg_pdn: bit<9>
	for(i=0;i<=2000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003A80*2)) = (0x0001);//Turn all (including hs_current) use override mode
	//Module: usb1 (0x100780)
	*(volatile unsigned int *)((0x1f200000)+(0x000780*2)) = (0x0a);	//Disable MAC initial suspend, Reset UHC
	*(volatile unsigned int *)((0x1f200000)+(0x000780*2)) = (0x28);	//Release UHC reset, enable UHC and OTG XIU function
	//Module: utmi1 (0x103A00)
	*(volatile unsigned int *)((0x1f200000)+(0x003A22*2)) = (0x2088);//PLL_TEST[30:28]: 3'b101 for IBIAS current select
	*(volatile unsigned int *)((0x1f200000)+(0x003A20*2)) = (0x8051);//PLL_TEST[15]: Bypass 480MHz clock divider
	*(volatile unsigned int *)((0x1f200000)+(0x003A02*2)) = (0x2084);//Enable CLK12_SEL bit <2> for select low voltage crystal clock
	*(volatile unsigned int *)((0x1f200000)+(0x003A08*2)) = (0x0426);//bit<7>: Power down UTMI port-0 bandgap current

	*(volatile unsigned int *)((0x1f200000)+(0x003A00*2)) = (0x6bc3);//reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
	for(i=0;i<=1000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003A00*2)) = (0x69c3);//Turn on UPLL, reg_pdn: bit<9>
	for(i=0;i<=2000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003A00*2)) = (0x0001);//Turn all (including hs_current) use override mode

	//Module: usb2 (0x110200)
	*(volatile unsigned int *)((0x1f200000)+(0x010200*2)) = (0x0a);//Disable MAC initial suspend, Reset UHC
	*(volatile unsigned int *)((0x1f200000)+(0x010200*2)) = (0x28);//Release UHC reset, enable UHC and OTG XIU function
	//Module: utmi2 (0x103900)
	*(volatile unsigned int *)((0x1f200000)+(0x003922*2)) = (0x2088);//PLL_TEST[30:28]: 3'b101 for IBIAS current select
	*(volatile unsigned int *)((0x1f200000)+(0x003920*2)) = (0x8051);//PLL_TEST[15]: Bypass 480MHz clock divider
	*(volatile unsigned int *)((0x1f200000)+(0x003902*2)) = (0x2084);//Enable CLK12_SEL bit <2> for select low voltage crystal clock
	*(volatile unsigned int *)((0x1f200000)+(0x003908*2)) = (0x0426);//bit<7>: Power down UTMI port-0 bandgap current

	*(volatile unsigned int *)((0x1f200000)+(0x003900*2)) = (0x6bc3);//reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
	for(i=0;i<=1000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003900*2)) = (0x69c3);//Turn on UPLL, reg_pdn: bit<9>
	for(i=0;i<=2000;i++)
	{
		//do nothing
		j++;
	}
	*(volatile unsigned int *)((0x1f200000)+(0x003900*2)) = (0x0001);//Turn all (including hs_current) use override mode
}

#if defined(ENABLE_PC_UPGRADE_EMI_SWITCH)
void USB_EMI_Switch(void)
{
	//printf("EMI/IMI switch\n");
	usb_emi_switch();
}
#endif

#if 1
void usb_function_start(void)
{
	void	*ptrusbStruct;
	USB_VAR		usbDrvVar;
	vendorCmdStruct     scsiVendorCmdVar;
	u8  gu8ucDmaBuf[2*1024];

	ebable_UPLL();

	/* Init USB global struct variable */
	ptrusbStruct = &usbDrvVar;
	/* Init. USB driver function table */
	//fpUsbCmdLoop = MDrv_USB_CmdLoop;
	//fpSCSI_vendor = FnVendor_Cmds;
	//fpdisk_rom = FnDisk_Cmds_Rom;
	//fpdisk_ram = FnDisk_Cmds_Ram;
	//fpSCSI_disk = fpdisk_rom;

	/* Init. SCSI vendor command struct */
	Init_vendorCmd_Var(&scsiVendorCmdVar);
	Init_gUSB_Var(&usbDrvVar, &scsiVendorCmdVar);
	/* Init. Mass storage class buffer */
	otgInitMassBuffer(&usbDrvVar);

	MDrv_USB_Init();

	usbDrvVar.DeviceConnect = 1;
	usbDrvVar.ucptrDataBuf = (U8 *)(gu8ucDmaBuf);
	scsiVendorCmdVar.ucptrDataBuf = (U8 *)(&gu8ucDmaBuf[8]);

	while(1)
	{
		USB_DRC_Interrupt(&usbDrvVar);
		//if (fpUsbCmdLoop)
		MDrv_USB_CmdLoop(ptrusbStruct);
#if 0
		///////////////////test
		len=MDrv_USB_Read(Buf, 0x1000, ptrusbStruct);
		if(len)
		{
			printf("receive len:0x%x\n",len);
		}
		//////////////////////
#endif
	}
}
#endif
