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

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define MIU_RIU_REG_BASE                   0x1F000000

#define CH0_RIU_DTOP                      0x1012
#define CH1_RIU_DTOP                      0x1006
#define CH2_RIU_DTOP                      0x1620
#define CH0_RIU_ARB                       0x1615
#define CH1_RIU_ARB                       0x1622
#define CH2_RIU_ARB                       0x1623
#define CH0_RIU_ARBB                      0x1520
#define CH1_RIU_ARBB                      0x1521
#define CH2_RIU_ARBB                      0x1522
#define CH0_RIU_ATOP                      0x110d
#define CH1_RIU_ATOP                      0x1616
#define CH2_RIU_ATOP                      0x1621
#define CH0_RIU_DTOP_E                    0x152b
#define CH1_RIU_DTOP_E                    0x152c
#define CH2_RIU_DTOP_E                    0x152d
#define CH0_RIU_ARB_F                     0x3106
#define CH1_RIU_ARB_F                     0x3107
#define CH2_RIU_ARB_F                     0x3108
#define CH0_RIU_ARB_E                     0x3102
#define CH1_RIU_ARB_E                     0x3103
#define CH2_RIU_ARB_E                     0x3104
#define CH0_RIU_ATOP_E                    0x1515
#define CH1_RIU_ATOP_E                    0x1516
#define CH2_RIU_ATOP_E                    0x1517

#define EFUSE_RIU_BANK                     0x0020
#define REG_ATOP_REGION                    0x2F
#define DEBUG
#define DQSM_TRAINING_EN
#define VREF_DQ_EN
#define MIU_ENABLE_AUTO_WRITE_PHASE
#define MIU_ENABLE_AUTO_READ_PHASE

#define MIU_CHIP_VER_U01                    0
#define MIU_CHIP_VER_U02                    1

typedef unsigned char   u8;
typedef unsigned int   u16;
typedef unsigned long  u32;

#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile u16* ) (MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)

#define MHal_EFUSE_ReadReg16(u32reg) *(( volatile u16* ) (MIU_RIU_REG_BASE + EFUSE_RIU_BANK*0x100*2 + ((u32reg) << 1)))
#define MHal_EFUSE_WritReg16(u32reg, u16val) *(( volatile u16* ) (MIU_RIU_REG_BASE + EFUSE_RIU_BANK*0x100*2 + ((u32reg) << 1))) = (u16val)



//-------------------------------------------------------------------------------------------------
//  Prototypes
//-------------------------------------------------------------------------------------------------
void putn_ddr( u8 n );
void putk_ddr( char c );
void delayus_ddr(u32 us);
void BootRom_MiuDdr3Dqs(u8 u8MiuDev);
void single_cmd(u16 u16RegMiuDtop, u16 mrx, u8 CmdVal);
u16 VrefDQ_train_loop(u16 sub0_u16RegMiuDtop,
                      u16 sub1_u16RegMiuDtop,
                      u16 sub0_u16RegMiuDtopE,
                      u16 sub1_u16RegMiuDtopE,
                      u16 sub0_u16RegMiuArb,
                      u16 sub1_u16RegMiuArb,
                      u8 Indx,
                      u8 step,
                      u8 max);
void BootRom_MiuDdrTrim(void);
u8 CalculateDrvStength(u8 DrvStength, u8 DefDrv);
void MHal_MIU_WritReg16_sub01(u32 sub0_u32bank, u32 sub1_u32bank, u32 u32reg, u16 u16val);
void RX_DQ_ph_update(u16 u16RegMiuAtop, u16 u16RegMiuAtop_E);
void RX_DQSM_ph_update(u16 u16RegMiuAtop, u16 u16RegMiuAtop_E, u16 sub0_u16RegMiuArb, u16 sub1_u16RegMiuArb);
//-------------------------------------------------------------------------------------------------
//  Local variables
//-------------------------------------------------------------------------------------------------
const u32 VREF_VAL[10] = {0x10, 0x12, 0x14, 0x16, 0x18,
                          0x1a, 0x1c, 0x1e, 0x20, 0x22};

//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void putn_ddr(u8 n)
{
    char c = '0' + n;

    *(volatile unsigned int*)(0x1F201300) = c;
}
//-------------------------------------------------------------------------------------------------
void putk_ddr(char c)
{
   *(volatile unsigned int*)(0x1F201300) = c;
}

void MHal_MIU_WritReg16_sub01(u32 sub0_u32bank, u32 sub1_u32bank, u32 u32reg, u16 u16val)
{
	MHal_MIU_WritReg16(sub0_u32bank, u32reg, u16val);
	MHal_MIU_WritReg16(sub1_u32bank, u32reg, u16val);
}
//
void MHal_MIU_WritReg16_tx_dq_offset_patch(u32 u32bank, u32 u32reg, u16 DQ0_sub_value, u16 DQ1_sub_value)
{
	
	/// For DQ - n function (20180302 by yung.chang)
  u16 R_TX_DQ_ph     = 0;
  u16 R_TX_DQ_ph_L   = 0;  
  u16 R_TX_DQ_ph_H   = 0;
  u16 R_TX_DQ_ph_DQ0 = 0;
  u16 R_TX_DQ_ph_DQ1 = 0;
  u16 R_TX_DQ_ph_new = 0;
   
	R_TX_DQ_ph = MHal_MIU_ReadReg16(u32bank, u32reg);
	
	R_TX_DQ_ph_DQ0 =  R_TX_DQ_ph & 0x003f;
	R_TX_DQ_ph_DQ1 = (R_TX_DQ_ph & 0x3f00) >> 8 ;
	
	//For bit [7:0]
	if(((R_TX_DQ_ph_DQ0) & 0x1f) >= DQ0_sub_value) {
    R_TX_DQ_ph_L = R_TX_DQ_ph_DQ0 - DQ0_sub_value;  	
  }
 	else  {
  	R_TX_DQ_ph_L = (R_TX_DQ_ph_DQ0 + 17) - DQ0_sub_value - 0x20;  	
  }
  
  //For bit [15:8]
	if(((R_TX_DQ_ph_DQ1) & 0x1f) >= DQ1_sub_value) {
    R_TX_DQ_ph_H = R_TX_DQ_ph_DQ1 - DQ1_sub_value;  	
  }
 	else  {
    R_TX_DQ_ph_H = (R_TX_DQ_ph_DQ1 + 17) - DQ1_sub_value - 0x20;  	
  }
  
  R_TX_DQ_ph_new = (R_TX_DQ_ph_H << 8) + R_TX_DQ_ph_L;
  
  MHal_MIU_WritReg16(u32bank, u32reg, R_TX_DQ_ph_new);
  
}

//-------------------------------------------------------------------------------------------------
void single_cmd(u16 u16RegMiuDtop_E, u16 mrx, u8 CmdVal)
{
	// For Fast Command Mode
    MHal_MIU_WritReg16(u16RegMiuDtop_E, 0x1c, mrx);
    MHal_MIU_WritReg16(u16RegMiuDtop_E, 0x1e, (CmdVal & 0x000f));   //[3:0] : reg_single_cmd
}
//-------------------------------------------------------------------------------------------------
//####################################################################################
//
//VrefDQ training loop procedure
//
//####################################################################################
u16 VrefDQ_train_loop(u16 sub0_u16RegMiuDtop,
                      u16 sub1_u16RegMiuDtop,
                      u16 sub0_u16RegMiuDtopE,
                      u16 sub1_u16RegMiuDtopE,
                      u16 sub0_u16RegMiuArb,
                      u16 sub1_u16RegMiuArb,
                      u8 Indx,
                      u8 step,
                      u8 max) {
    u16 loop;

    u16 sub0_u16temp1=0;
    u16 sub0_u16temp2=0;
    u16 sub0_u16data[3]={0,0,0};

    u16 sub1_u16temp1=0;
    u16 sub1_u16temp2=0;
    u16 sub1_u16data[3]={0,0,0};

    u8  sub0_u8temp;
    u8  sub0_u8temp1;
    u8  sub0_u8temp2;

    u8  sub1_u8temp;
    u8  sub1_u8temp1;
    u8  sub1_u8temp2;

    u16 return_value = 0;

    u8 loop_cnt=0;

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb, 0x7c, 0xf004); //SWITCH dpat deb_out = min. margin * 64 + all dq pass windows


    for (loop=Indx; loop<((max*step)+Indx); loop=loop+step)
    {
        if (loop > 10)
        {
            break;
        }
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtopE,sub1_u16RegMiuDtopE, 0x14, 0x0001);       //[0] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref

        if( (MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //Check DDR4 ?
        //Set MRx[6:0] = Choose VrefDQ value & Range
          single_cmd(sub0_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[loop] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE
          single_cmd(sub0_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[loop] | 0x80, 0x00); // (Enable VrefDQ training)
        }

        if( (MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //Check DDR4 ?
        //Set MRx[6:0] = Choose VrefDQ value & Range
          single_cmd(sub1_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[loop] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE
          single_cmd(sub1_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[loop] | 0x80, 0x00); // (Enable VrefDQ training)
        }

        delayus_ddr(1); //wait 150ns for tVREFDQE

        //Enter DQ Vref training Mode.
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtopE, sub1_u16RegMiuDtopE, 0x14, 0x0000);       //[0] : reg_auto_ref_off = 0

        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0x78, 0x0004);
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0x78, 0x0000);                                                //dpat_rst

        // Enable TX DQ deskew scan (by DPAT engine)
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0x76, 0x0002);
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0x78, 0x0013);

        delayus_ddr(1);
        while((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);

       #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
        while((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
       #endif

        //MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0xf004); //SWITCH dpat deb_out = min. margin * 64 + all dq pass windows
        delayus_ddr(1);

        //---- For sub0 Min. Margin normaize ------------------------------------------
        sub0_u16data[2]=sub0_u16data[1];
        sub0_u16data[1]=sub0_u16data[0];
        sub0_u16data[0]=MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x7e);
        sub0_u16temp1 = sub0_u16data[0] + (sub0_u16data[1] << 2) + sub0_u16data[2];
        if (sub0_u16temp1 >= sub0_u16temp2) {
            sub0_u16temp2=sub0_u16temp1;
            sub0_u8temp1=loop-1;
        }
        #ifdef DEBUG
          MHal_MIU_WritReg16(sub0_u16RegMiuDtop, (0x60 + (loop_cnt<<1) ), (sub0_u16data[0]) );
        #endif

        //-----------------------------------------------------------------------------

       #if !defined(CONFIG_MIU1_DRAM_NONE)
        //---- For sub0 Min. Margin normaize ------------------------------------------
        sub1_u16data[2]=sub1_u16data[1];
        sub1_u16data[1]=sub1_u16data[0];
        sub1_u16data[0]=MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x7e);
        sub1_u16temp1 = sub1_u16data[0] + (sub1_u16data[1] << 2) + sub1_u16data[2];
        if (sub1_u16temp1 >= sub1_u16temp2) {
            sub1_u16temp2=sub1_u16temp1;
            sub1_u8temp1=loop-1;
        }
        #ifdef DEBUG
          MHal_MIU_WritReg16(sub1_u16RegMiuDtop, (0x60 + (loop_cnt<<1) ), (sub1_u16data[0]) );
        #endif

        //-----------------------------------------------------------------------------
       #endif

       loop_cnt++;
    }
    //u8temp = ((u8temp1 + u8temp2 + 1) >> 1);
    return_value = sub0_u8temp1 + ((sub1_u8temp1 << 8) & 0xff00) ;
    return return_value;
}

//####################################################################################
//
//TX DQ phase update
//
//####################################################################################
void TX_DQ_ph_update (u16 u16RegMiuAtop, u16 u16RegMiuAtop_E) {

    u16 u16RegVal0;

	// wriu -w 0x110dc0  0x0004
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    delayus_ddr(1);


	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151520  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x20, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x151522  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x22, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x151524  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x24, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x151526  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x26, u16RegVal0);


	// wriu -w 0x110dc0  0x0005
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    delayus_ddr(1);

	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151528  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x28, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x15152a  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x2a, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x15152c  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x2c, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x15152e  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x2e, u16RegVal0);

	// wriu -w 0x110dc0  0x0006
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    delayus_ddr(1);
	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151530  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x30, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x151532  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x32, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x151534  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x34, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x151536  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x36, u16RegVal0);

	// wriu -w 0x110dc0  0x0007
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    delayus_ddr(1);
	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151538  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x38, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x15153a  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x3a, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x15153c  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x3c, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x15153e  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x3e, u16RegVal0);
}

//-------------------------------------------------------------------------------------------------

//####################################################################################
//
//RX DQ/DQS phase update
//
//####################################################################################
void RX_DQ_ph_update (u16 u16RegMiuAtop, u16 u16RegMiuAtop_E)
{
    u16 u16RegVal0;
	// wriu -w 0x110dc0  0x0000
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0000);		//dpat_sel-> DQ[7:0]
    delayus_ddr(1);

	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151580  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x80, u16RegVal0);	//dll_deskew1,0
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x151582  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x82, u16RegVal0);	//dll_deskew3,2
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x151584  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x84, u16RegVal0);	//dll_deskew5,4
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x151586  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x86, u16RegVal0);	//dll_deskew7,6

	// wriu -w 0x110dc0  0x0001
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0001);		//dpat_sel-> DQ[15:8]
    delayus_ddr(1);

	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151588  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x88, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x15158a  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x8a, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x15158c  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x8c, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x15158e  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x8e, u16RegVal0);


	// wriu -w 0x110dc0  0x0002
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0002);		//dpat_sel-> DQ[23:16]
    delayus_ddr(1);
	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151590  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x90, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x151592  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x92, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x151594  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x94, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x151596  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x96, u16RegVal0);

	// wriu -w 0x110dc0  0x0003
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0003);		//dpat_sel-> DQ[31:24]
    delayus_ddr(1);
	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x151598  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x98, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x15159a  0xreg_rdata0000
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x9a, u16RegVal0);
	// rriu -c 0x110dc6 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
	// wriu -w 0x15159c  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x9c, u16RegVal0);
	// rriu -c 0x110dc8 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc8);
	// wriu -w 0x15159e  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x9e, u16RegVal0);

	//wriu -w 0x110dc0  0x0011
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0011);		//hw_kcode_offset
    delayus_ddr(1);
	//rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	//wriu -w 0x110d94  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x94, u16RegVal0);
	//rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	//wriu -w 0x110d96  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x96, u16RegVal0);

	// wriu -w 0x110dc0  0x0013
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0013);		//hw_kcode_offset_2nd
    delayus_ddr(1);
	// rriu -c 0x110dc2 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
	// wriu -w 0x110da4  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa4, u16RegVal0);
	// rriu -c 0x110dc4 2 //reg_rdata
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
	// wriu -w 0x110da6  0xreg_rdata
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa6, u16RegVal0);

}
//####################################################################################
//
//RX DQ/DQS phase update
//
//####################################################################################
void RX_DQSM_ph_update(u16 u16RegMiuAtop, u16 u16RegMiuAtop_E, u16 sub0_u16RegMiuArb, u16 sub1_u16RegMiuArb)
{
	  u16 u16RegVal0;
	  
    //-----------------------------------------------------------
    //Read DQSM phase result (low bound + offset) & write to REGs
    //-----------------------------------------------------------
    u16 u16Dqsm0_rlt;
    u16 u16Dqsm1_rlt;
    u16 u16Dqsm2_rlt;
    u16 u16Dqsm3_rlt;

    u16RegVal0 = MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78);

    MHal_MIU_WritReg16(sub0_u16RegMiuArb, 0x7C, 0x9008);
    delayus_ddr(1);
    u16Dqsm0_rlt = MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x7E);  //reg_rdata = dqsm0_result

    MHal_MIU_WritReg16(sub0_u16RegMiuArb, 0x7C, 0x9009);
    delayus_ddr(1);
    u16Dqsm1_rlt = MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x7E);  //reg_rdata = dqsm1_result

    MHal_MIU_WritReg16(sub0_u16RegMiuArb, 0x7C, 0x900A);
    delayus_ddr(1);
    u16Dqsm2_rlt = MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x7E);  //reg_rdata = dqsm2_result

    MHal_MIU_WritReg16(sub0_u16RegMiuArb, 0x7C, 0x900B);
    delayus_ddr(1);
    u16Dqsm3_rlt = MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x7E);  //reg_rdata = dqsm3_result

    //---------------
    //Set DQSM phase
    //---------------
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, ((MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xFFF0) | ((u16Dqsm0_rlt & 0x000F))));   //[3:0]   = dqsm0_result[3:0]
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, ((MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xF0FF) | ((u16Dqsm1_rlt & 0x000F)<<8)));  //[11:8]  = dqsm1_result[3:0]
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, ((MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xFFF0) | ((u16Dqsm2_rlt & 0x000F))));   //[3:0]   = dqsm2_result[3:0]
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, ((MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xF0FF) | ((u16Dqsm3_rlt & 0x000F)<<8)));  //[11:8]  = dqsm3_result[3:0]

    //--------------
    //Set DQSM skew offset
    //--------------
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0xF0, 0x0000); //dqsm offset = 0x0000


    //--------------
    //Set DQSM skew
    //--------------
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xF0, ((MHal_MIU_ReadReg16(u16RegMiuAtop, 0xF0) & 0xFFF0) | ((u16Dqsm0_rlt & 0x00F0)>>4)));  //[3:0]   = dqsm0_result[7:4]
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xF0, ((MHal_MIU_ReadReg16(u16RegMiuAtop, 0xF0) & 0xFF0F) | (u16Dqsm1_rlt & 0x00F0)));       //[7:4]   = dqsm1_result[7:4]
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xF0, ((MHal_MIU_ReadReg16(u16RegMiuAtop, 0xF0) & 0xF0FF) | ((u16Dqsm2_rlt & 0x00F0)<<4)));  //[11:8]  = dqsm2_result[7:4]
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xF0, ((MHal_MIU_ReadReg16(u16RegMiuAtop, 0xF0) & 0x0FFF) | ((u16Dqsm3_rlt & 0x00F0)<<8)));  //[15:12] = dqsm3_result[7:4]

    //--------------
    //Set DQSM dly
    //--------------
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x0E, ((MHal_MIU_ReadReg16(u16RegMiuAtop, 0x0E) & 0xFFC1) | ((u16Dqsm0_rlt & 0x1F00)>>7)));   //[5:1]   = dqsm0_result[12:8]

}


void delayus_ddr(u32 us)
{
    u16 u16RegVal0;

    u16RegVal0 = ((us* 12) & 0xffff);
    MHal_MIU_WritReg16(0x30, 0x24, u16RegVal0);

    u16RegVal0 = ((us* 12) >> 16);
    MHal_MIU_WritReg16(0x30, 0x26, u16RegVal0);

    u16RegVal0 = 0x0002;
    MHal_MIU_WritReg16(0x30, 0x20, u16RegVal0);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(0x30, 0x22);
    }while((u16RegVal0 & 0x0001) == 0);
}

static void DQSM_train_MPR(u16 u16RegMiuAtop, u16 u16RegMiuAtop_E, u16 u16RegMiuDtop, u16 u16RegMiuDtop_E)
{
    u16 u16Org_dqsm_dly;
    u16 u16Org_dqsm_skew0;
    u16 u16Org_dqsm_skew1;
    u16 u16Org_dqsm_skew2;
    u16 u16Org_dqsm_skew3;
    u16 u16Org_dqsm_skew_offset0;
    u16 u16Org_dqsm_skew_offset1;
    u16 u16Org_dqsm_skew_offset2;
    u16 u16Org_dqsm_skew_offset3;    
    u16 u16Org_dqsm_ph0;
    u16 u16Org_dqsm_ph1;
    u16 u16Org_dqsm_ph2;
    u16 u16Org_dqsm_ph3;
    u16 u16Org_dqsm_ph_all0;
    u16 u16Org_dqsm_ph_all1;
    u16 u16Org_dqsm_ph_all2;
    u16 u16Org_dqsm_ph_all3;
    u16 u16RegVal_min;
    u16 u16RegVal_max; 
    u16 u16RegVal_pass_min0;
    u16 u16RegVal_pass_min1;
    u16 u16RegVal_pass_min2;
    u16 u16RegVal_pass_min3;
    u16 u16RegVal_pass_max0; 
    u16 u16RegVal_pass_max1;
    u16 u16RegVal_pass_max2;
    u16 u16RegVal_pass_max3;
    u16 u16RegVal_pass_center0; 
    u16 u16RegVal_pass_center1;
    u16 u16RegVal_pass_center2;
    u16 u16RegVal_pass_center3;
    u16 u16RegVal_min_determined0 = 0;
    u16 u16RegVal_min_determined1 = 0;
    u16 u16RegVal_min_determined2 = 0;
    u16 u16RegVal_min_determined3 = 0;
    u16 u16RegVal_pass_cont_cnt0 = 0;
    u16 u16RegVal_pass_cont_cnt1 = 0;
    u16 u16RegVal_pass_cont_cnt2 = 0;
    u16 u16RegVal_pass_cont_cnt3 = 0;    
    u16 u16RegVal; 
    u16 i = 0;
    u16 j = 0;

    u16 skew_num = 2;
    u16 ph_num = 10;
    //_PUTS("Traning DQSM\n");

    if((MHal_MIU_ReadReg16(u16RegMiuDtop, 0x02) & 0x0300) == 0x0200 ) // 4X mode
    {
        skew_num = 2;
        ph_num = 10;
    }
    else
    {
   	skew_num = 4;
        ph_num = 10;    	
    }

    u16Org_dqsm_dly = (MHal_MIU_ReadReg16(u16RegMiuAtop,0x0e)&0x003e)>>1;
    u16Org_dqsm_skew0 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x000f);
    u16Org_dqsm_skew1 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x00f0)>>4;
    u16Org_dqsm_skew2 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x0f00)>>8;
    u16Org_dqsm_skew3 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0xf000)>>12;
    u16Org_dqsm_skew_offset0 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x000f);
    u16Org_dqsm_skew_offset1 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x00f0)>>4;
    u16Org_dqsm_skew_offset2 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x0f00)>>8;
    u16Org_dqsm_skew_offset3 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0xf000)>>12;     
    u16Org_dqsm_ph0 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x60)&0x000f);     
    u16Org_dqsm_ph1 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x60)&0x0f00)>>8; 
    u16Org_dqsm_ph2 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x62)&0x000f); 
    u16Org_dqsm_ph3 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x62)&0x0f00)>>8;
	
    u16Org_dqsm_ph_all0 = (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew0 + u16Org_dqsm_skew_offset0)*ph_num + u16Org_dqsm_ph0;
    u16Org_dqsm_ph_all1	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew1 + u16Org_dqsm_skew_offset1)*ph_num + u16Org_dqsm_ph1;
    u16Org_dqsm_ph_all2	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew2 + u16Org_dqsm_skew_offset2)*ph_num + u16Org_dqsm_ph2;
    u16Org_dqsm_ph_all3	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew3 + u16Org_dqsm_skew_offset3)*ph_num + u16Org_dqsm_ph3;
	  
    u16RegVal_min = u16Org_dqsm_ph_all0;
    u16RegVal_max = u16Org_dqsm_ph_all0;
    if(u16RegVal_min >= u16Org_dqsm_ph_all1){u16RegVal_min = u16Org_dqsm_ph_all1;}
    if(u16RegVal_min >= u16Org_dqsm_ph_all2){u16RegVal_min = u16Org_dqsm_ph_all2;}		
    if(u16RegVal_min >= u16Org_dqsm_ph_all3){u16RegVal_min = u16Org_dqsm_ph_all3;}		

    if(u16RegVal_max <= u16Org_dqsm_ph_all1){u16RegVal_max = u16Org_dqsm_ph_all1;}
    if(u16RegVal_max <= u16Org_dqsm_ph_all2){u16RegVal_max = u16Org_dqsm_ph_all2;}		
    if(u16RegVal_max <= u16Org_dqsm_ph_all3){u16RegVal_max = u16Org_dqsm_ph_all3;}	  	
	  	
    u16RegVal_min_determined0	= 0;
    u16RegVal_min_determined1	= 0;
    u16RegVal_min_determined2	= 0;
    u16RegVal_min_determined3	= 0;
    u16RegVal_pass_min0 = u16RegVal_min-2*ph_num;        
    u16RegVal_pass_min1 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_min2 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_min3 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_max0 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max1 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max2 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max3 = u16RegVal_min-2*ph_num;

    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0xf0, 0x0000 ); //skew offset need to be 0
    
    MHal_MIU_WritReg16(u16RegMiuDtop_E, 0x14, (MHal_MIU_ReadReg16(u16RegMiuDtop_E, 0x14) | 0x0001)); //[00] : reg_auto_ref_off = 1	
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) | 0x1000)); //[12] : reg_cke_always_on = 1
    
    single_cmd(u16RegMiuDtop_E, 0x0400, 0x04); //Pre-charge all command
    delayus_ddr(1); //wait 1ms      
   
    single_cmd(u16RegMiuDtop_E, 0x7004, 0x00); //{3'b011,mpr_mode,11'h004}	
    delayus_ddr(1); //wait 1ms   
    
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xd6, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd6) | 0x0001) ); //[00] : enable MPR
    
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0100);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe2, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe4, 0x1001);
	  
    for(j = u16RegVal_min-2*ph_num; j <= u16RegVal_max+2*ph_num; j++)	
    {
			  
        MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0100);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x0e, (((j/skew_num/ph_num)<<1 )& 0x003e) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x0e) & 0xffc1) );   //set delay
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num) )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xfff0) );   //set skew0
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<4 )& 0x00f0) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xff0f) );   //set skew1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xf0ff) );   //set skew2
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<12)& 0xf000) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0x0fff) );   //set skew3
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((j%ph_num)    )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xfff0) );   //set ph0
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((j%ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xf0ff) );   //set ph1
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((j%ph_num)    )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xfff0) );   //set ph2
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((j%ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xf0ff) );   //set ph3
        delayus_ddr(1);
     
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x0002)); //reset rx fifo
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
     
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f); //push butt
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
        delayus_ddr(1);

        MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0101); //enable BIST 
			
        while ((MHal_MIU_ReadReg16(u16RegMiuDtop, 0xe0) & 0x8000) != 0x8000); // wait for BIST finish

        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa0) & 0x00ff) == 0x0000) //byte0 pass or not //pass
        {
            u16RegVal_pass_cont_cnt0 = u16RegVal_pass_cont_cnt0+1;
            if( (u16RegVal_pass_cont_cnt0 >=3) & (u16RegVal_min_determined0 ==0))   
            {
	        u16RegVal_min_determined0 = 1;
	        u16RegVal_pass_min0 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt0 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max0 = j-1;
            }
            u16RegVal_pass_cont_cnt0 = 0;
        }
        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa0) & 0xff00) == 0x0000) //byte1 pass or not //pass
        {
            u16RegVal_pass_cont_cnt1 = u16RegVal_pass_cont_cnt1+1;
            if( (u16RegVal_pass_cont_cnt1 >=3) & (u16RegVal_min_determined1 ==0))   
            {
                u16RegVal_min_determined1 = 1;
                u16RegVal_pass_min1 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt1 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max1 = j-1;
            }
            u16RegVal_pass_cont_cnt1 = 0;
        }

        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa2) & 0x00ff) == 0x0000) //byte2 pass or not //pass
        {
            u16RegVal_pass_cont_cnt2 = u16RegVal_pass_cont_cnt2+1;
            if( (u16RegVal_pass_cont_cnt2 >=3) & (u16RegVal_min_determined2 ==0))   
            {
                u16RegVal_min_determined2 = 1;
                u16RegVal_pass_min2 = j-2;
            }	
        }
        else //fail
        {	
            if(u16RegVal_pass_cont_cnt2 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max2 = j-1;
            }
            u16RegVal_pass_cont_cnt2 = 0;
        }
        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa2) & 0xff00) == 0x0000) //byte3 pass or not //pass
        {
            u16RegVal_pass_cont_cnt3 = u16RegVal_pass_cont_cnt3+1;
            if( (u16RegVal_pass_cont_cnt3 >=3) & (u16RegVal_min_determined3 ==0))   
            {
                u16RegVal_min_determined3 = 1;
                u16RegVal_pass_min3 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt3 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max3 = j-1;
            }
            u16RegVal_pass_cont_cnt3 = 0;
        }
    }
 
    single_cmd(u16RegMiuDtop_E, 0x6000, 0x00); //program MR3 by MRX (disable MPR)
    delayus_ddr(1); //wait 1ms   

		MHal_MIU_WritReg16(u16RegMiuDtop, 0xd6, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd6) & 0xfffe) ); //[00] : disable MPR   

    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0000); //disable BIST 
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x0002)); //reset rx fifo
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
     
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f); //push butt
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
    delayus_ddr(1);
		
    u16RegVal_pass_center0 = (u16RegVal_pass_min0 + u16RegVal_pass_max0)/2;
    u16RegVal_pass_center1 = (u16RegVal_pass_min1 + u16RegVal_pass_max1)/2;
    u16RegVal_pass_center2 = (u16RegVal_pass_min2 + u16RegVal_pass_max2)/2;
    u16RegVal_pass_center3 = (u16RegVal_pass_min3 + u16RegVal_pass_max3)/2;

    u16RegVal_min = u16RegVal_pass_center0;
    if(u16RegVal_min >= u16RegVal_pass_center1){u16RegVal_min = u16RegVal_pass_center1;}
    if(u16RegVal_min >= u16RegVal_pass_center2){u16RegVal_min = u16RegVal_pass_center2;}		
    if(u16RegVal_min >= u16RegVal_pass_center3){u16RegVal_min = u16RegVal_pass_center3;}			
	  			       
    u16RegVal = u16RegVal_min/skew_num/ph_num;       
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x0e, ((u16RegVal<<1)& 0x003e) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x0e) & 0xffc1) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center0 - u16RegVal*skew_num*ph_num)/ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center1 - u16RegVal*skew_num*ph_num)/ph_num)<< 4 )& 0x00f0) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xff0f) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center2 - u16RegVal*skew_num*ph_num)/ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xf0ff) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center3 - u16RegVal*skew_num*ph_num)/ph_num)<< 12)& 0xf000) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0x0fff) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((u16RegVal_pass_center0%ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((u16RegVal_pass_center1%ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xf0ff) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((u16RegVal_pass_center2%ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((u16RegVal_pass_center3%ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xf0ff) );	 	
}
static void DQSM_train_BIST(u16 u16RegMiuAtop, u16 u16RegMiuAtop_E, u16 u16RegMiuDtop, u16 u16RegMiuDtop_E)
{
    u16 u16Org_dqsm_dly;
    u16 u16Org_dqsm_skew0;
    u16 u16Org_dqsm_skew1;
    u16 u16Org_dqsm_skew2;
    u16 u16Org_dqsm_skew3;
    u16 u16Org_dqsm_skew_offset0;
    u16 u16Org_dqsm_skew_offset1;
    u16 u16Org_dqsm_skew_offset2;
    u16 u16Org_dqsm_skew_offset3;    
    u16 u16Org_dqsm_ph0;
    u16 u16Org_dqsm_ph1;
    u16 u16Org_dqsm_ph2;
    u16 u16Org_dqsm_ph3;
    u16 u16Org_dqsm_ph_all0;
    u16 u16Org_dqsm_ph_all1;
    u16 u16Org_dqsm_ph_all2;
    u16 u16Org_dqsm_ph_all3;
    u16 u16RegVal_min;
    u16 u16RegVal_max; 
    u16 u16RegVal_pass_min0;
    u16 u16RegVal_pass_min1;
    u16 u16RegVal_pass_min2;
    u16 u16RegVal_pass_min3;
    u16 u16RegVal_pass_max0; 
    u16 u16RegVal_pass_max1;
    u16 u16RegVal_pass_max2;
    u16 u16RegVal_pass_max3;
    u16 u16RegVal_pass_center0; 
    u16 u16RegVal_pass_center1;
    u16 u16RegVal_pass_center2;
    u16 u16RegVal_pass_center3;
    u16 u16RegVal_min_determined0 = 0;
    u16 u16RegVal_min_determined1 = 0;
    u16 u16RegVal_min_determined2 = 0;
    u16 u16RegVal_min_determined3 = 0;
    u16 u16RegVal_pass_cont_cnt0 = 0;
    u16 u16RegVal_pass_cont_cnt1 = 0;
    u16 u16RegVal_pass_cont_cnt2 = 0;
    u16 u16RegVal_pass_cont_cnt3 = 0;    
    u16 u16RegVal; 
    u16 i = 0;
    u16 j = 0;

    u16 skew_num = 2;
    u16 ph_num = 10;
    //_PUTS("Traning DQSM\n");

    if((MHal_MIU_ReadReg16(u16RegMiuDtop, 0x02) & 0x0300) == 0x0200 ) // 4X mode
    {
        skew_num = 2;
        ph_num = 10;
    }
    else
    {
   	skew_num = 4;
        ph_num = 10;    	
    }

    u16Org_dqsm_dly = (MHal_MIU_ReadReg16(u16RegMiuAtop,0x0e)&0x003e)>>1;
    u16Org_dqsm_skew0 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x000f);
    u16Org_dqsm_skew1 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x00f0)>>4;
    u16Org_dqsm_skew2 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0x0f00)>>8;
    u16Org_dqsm_skew3 = (MHal_MIU_ReadReg16(u16RegMiuAtop,0xf0)&0xf000)>>12;
    u16Org_dqsm_skew_offset0 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x000f);
    u16Org_dqsm_skew_offset1 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x00f0)>>4;
    u16Org_dqsm_skew_offset2 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0x0f00)>>8;
    u16Org_dqsm_skew_offset3 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0xf0)&0xf000)>>12;     
    u16Org_dqsm_ph0 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x60)&0x000f);     
    u16Org_dqsm_ph1 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x60)&0x0f00)>>8; 
    u16Org_dqsm_ph2 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x62)&0x000f); 
    u16Org_dqsm_ph3 = (MHal_MIU_ReadReg16(u16RegMiuAtop_E,0x62)&0x0f00)>>8;
	
    u16Org_dqsm_ph_all0 = (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew0 + u16Org_dqsm_skew_offset0)*ph_num + u16Org_dqsm_ph0;
    u16Org_dqsm_ph_all1	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew1 + u16Org_dqsm_skew_offset1)*ph_num + u16Org_dqsm_ph1;
    u16Org_dqsm_ph_all2	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew2 + u16Org_dqsm_skew_offset2)*ph_num + u16Org_dqsm_ph2;
    u16Org_dqsm_ph_all3	= (u16Org_dqsm_dly*skew_num + u16Org_dqsm_skew3 + u16Org_dqsm_skew_offset3)*ph_num + u16Org_dqsm_ph3;
	  
    u16RegVal_min = u16Org_dqsm_ph_all0;
    u16RegVal_max = u16Org_dqsm_ph_all0;
    if(u16RegVal_min >= u16Org_dqsm_ph_all1){u16RegVal_min = u16Org_dqsm_ph_all1;}
    if(u16RegVal_min >= u16Org_dqsm_ph_all2){u16RegVal_min = u16Org_dqsm_ph_all2;}		
    if(u16RegVal_min >= u16Org_dqsm_ph_all3){u16RegVal_min = u16Org_dqsm_ph_all3;}		

    if(u16RegVal_max <= u16Org_dqsm_ph_all1){u16RegVal_max = u16Org_dqsm_ph_all1;}
    if(u16RegVal_max <= u16Org_dqsm_ph_all2){u16RegVal_max = u16Org_dqsm_ph_all2;}		
    if(u16RegVal_max <= u16Org_dqsm_ph_all3){u16RegVal_max = u16Org_dqsm_ph_all3;}	  	
	  	
    u16RegVal_min_determined0	= 0;
    u16RegVal_min_determined1	= 0;
    u16RegVal_min_determined2	= 0;
    u16RegVal_min_determined3	= 0;
    u16RegVal_pass_min0 = u16RegVal_min-2*ph_num;        
    u16RegVal_pass_min1 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_min2 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_min3 = u16RegVal_min-2*ph_num;   
    u16RegVal_pass_max0 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max1 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max2 = u16RegVal_min-2*ph_num;
    u16RegVal_pass_max3 = u16RegVal_min-2*ph_num;

    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0xf0, 0x0000 ); //skew offset need to be 0
    
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0006);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe2, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe4, 0x2000);
	  
    for(j = u16RegVal_min-2*ph_num; j <= u16RegVal_max+2*ph_num; j++)	
    {
			  
        MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0006);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x0e, (((j/skew_num/ph_num)<<1 )& 0x003e) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x0e) & 0xffc1) );   //set delay
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num) )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xfff0) );   //set skew0
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<4 )& 0x00f0) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xff0f) );   //set skew1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xf0ff) );   //set skew2
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((j%(skew_num*ph_num))/ph_num)<<12)& 0xf000) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0x0fff) );   //set skew3
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((j%ph_num)    )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xfff0) );   //set ph0
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((j%ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xf0ff) );   //set ph1
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((j%ph_num)    )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xfff0) );   //set ph2
        MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((j%ph_num)<<8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xf0ff) );   //set ph3
        delayus_ddr(1);
     
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x0002)); //reset rx fifo
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
     
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f); //push butt
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
        delayus_ddr(1);

        MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0007); //enable BIST 
			
        while ((MHal_MIU_ReadReg16(u16RegMiuDtop, 0xe0) & 0x8000) != 0x8000); // wait for BIST finish

        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa0) & 0x00ff) == 0x0000) //byte0 pass or not //pass
        {
            u16RegVal_pass_cont_cnt0 = u16RegVal_pass_cont_cnt0+1;
            if( (u16RegVal_pass_cont_cnt0 >=3) & (u16RegVal_min_determined0 ==0))   
            {
	        u16RegVal_min_determined0 = 1;
	        u16RegVal_pass_min0 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt0 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max0 = j-1;
            }
            u16RegVal_pass_cont_cnt0 = 0;
        }
        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa0) & 0xff00) == 0x0000) //byte1 pass or not //pass
        {
            u16RegVal_pass_cont_cnt1 = u16RegVal_pass_cont_cnt1+1;
            if( (u16RegVal_pass_cont_cnt1 >=3) & (u16RegVal_min_determined1 ==0))   
            {
                u16RegVal_min_determined1 = 1;
                u16RegVal_pass_min1 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt1 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max1 = j-1;
            }
            u16RegVal_pass_cont_cnt1 = 0;
        }

        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa2) & 0x00ff) == 0x0000) //byte2 pass or not //pass
        {
            u16RegVal_pass_cont_cnt2 = u16RegVal_pass_cont_cnt2+1;
            if( (u16RegVal_pass_cont_cnt2 >=3) & (u16RegVal_min_determined2 ==0))   
            {
                u16RegVal_min_determined2 = 1;
                u16RegVal_pass_min2 = j-2;
            }	
        }
        else //fail
        {	
            if(u16RegVal_pass_cont_cnt2 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max2 = j-1;
            }
            u16RegVal_pass_cont_cnt2 = 0;
        }
        if ( (MHal_MIU_ReadReg16(u16RegMiuDtop, 0xa2) & 0xff00) == 0x0000) //byte3 pass or not //pass
        {
            u16RegVal_pass_cont_cnt3 = u16RegVal_pass_cont_cnt3+1;
            if( (u16RegVal_pass_cont_cnt3 >=3) & (u16RegVal_min_determined3 ==0))   
            {
                u16RegVal_min_determined3 = 1;
                u16RegVal_pass_min3 = j-2;
            }	
        }
        else //fail
        {
            if(u16RegVal_pass_cont_cnt3 >=5)  // pass window must >= 5 phase 
            {						
                u16RegVal_pass_max3 = j-1;
            }
            u16RegVal_pass_cont_cnt3 = 0;
        }
    }
 
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xe0, 0x0000); //disable BIST 
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x0002)); //reset rx fifo
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & 0xfffd)); //reset rx fifo
     
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f); //push butt
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005); //push butt
    delayus_ddr(1);
		
    u16RegVal_pass_center0 = (u16RegVal_pass_min0 + u16RegVal_pass_max0)/2;
    u16RegVal_pass_center1 = (u16RegVal_pass_min1 + u16RegVal_pass_max1)/2;
    u16RegVal_pass_center2 = (u16RegVal_pass_min2 + u16RegVal_pass_max2)/2;
    u16RegVal_pass_center3 = (u16RegVal_pass_min3 + u16RegVal_pass_max3)/2;

    u16RegVal_min = u16RegVal_pass_center0;
    if(u16RegVal_min >= u16RegVal_pass_center1){u16RegVal_min = u16RegVal_pass_center1;}
    if(u16RegVal_min >= u16RegVal_pass_center2){u16RegVal_min = u16RegVal_pass_center2;}		
    if(u16RegVal_min >= u16RegVal_pass_center3){u16RegVal_min = u16RegVal_pass_center3;}			
	  			       
    u16RegVal = u16RegVal_min/skew_num/ph_num;       
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x0e, ((u16RegVal<<1)& 0x003e) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x0e) & 0xffc1) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center0 - u16RegVal*skew_num*ph_num)/ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center1 - u16RegVal*skew_num*ph_num)/ph_num)<< 4 )& 0x00f0) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xff0f) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center2 - u16RegVal*skew_num*ph_num)/ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0xf0ff) );
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xf0, ((((u16RegVal_pass_center3 - u16RegVal*skew_num*ph_num)/ph_num)<< 12)& 0xf000) | (MHal_MIU_ReadReg16(u16RegMiuAtop, 0xf0) & 0x0fff) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((u16RegVal_pass_center0%ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x60, (((u16RegVal_pass_center1%ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x60) & 0xf0ff) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((u16RegVal_pass_center2%ph_num)     )& 0x000f) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xfff0) );
    MHal_MIU_WritReg16(u16RegMiuAtop_E, 0x62, (((u16RegVal_pass_center3%ph_num)<< 8 )& 0x0f00) | (MHal_MIU_ReadReg16(u16RegMiuAtop_E, 0x62) & 0xf0ff) );	 	
}
//-------------------------------------------------------------------------------------------------
//####################################################################################
//
//DDR3 & DDR4 Auto-phase procedure
//
//####################################################################################
void BootRom_MiuDdrDqs(u8 u8MiuDev)
{
    u16 u16RegVal0;
    u16 u16RegVal1;
    u16 u16RegVal2;

    u16 sub0_u16RegMiuArb;
    u16 sub0_u16RegMiuArbb;
    u16 sub0_u16RegMiuAtop;
    u16 sub0_u16RegMiuDtop;
    u16 sub0_u16RegMiuDtopE;
    u16 sub0_u16RegMiuArb_F;
    u16 sub0_u16RegMiuArb_E;
    u16 sub0_u16RegMiuAtop_E;
    u32 sub0_u32KcodeOffsetValue;
    u16 sub0_u16DQSMaxCenter[4];
    u16 sub0_u16KCodeOffset[4];
    u16 sub0_u16KCode;

    u16 sub1_u16RegMiuArb;
    u16 sub1_u16RegMiuArbb;
    u16 sub1_u16RegMiuAtop;
    u16 sub1_u16RegMiuDtop;
    u16 sub1_u16RegMiuDtopE;
    u16 sub1_u16RegMiuArb_F;
    u16 sub1_u16RegMiuArb_E;
    u16 sub1_u16RegMiuAtop_E;
    u32 sub1_u32KcodeOffsetValue;
    u16 sub1_u16DQSMaxCenter[4];
    u16 sub1_u16KCodeOffset[4];
    u16 sub1_u16KCode;

    u8 i = 0;

    u16 u16DQPos0 = 0;
    u16 u16DQPos1 = 0;
    u16 u16DQPos2 = 0;
    u16 u16DQPos3 = 0;
    u16 u16DQPosRes = 0;

    u16 u16RegVal;

    u16 sub0_u16Vref_indx_tmp = 0;
    u16 sub1_u16Vref_indx_tmp = 0;
    u16 sub0_u16Vref_indx = 0;
    u16 sub1_u16Vref_indx = 0;


    u16 TX_DQS0_ph = 0;
    u16 TX_DQS1_ph = 0;
    u16 TX_DQS2_ph = 0;
    u16 TX_DQS3_ph = 0;
    u16 TX_DQS01_ph = 0;
    u16 TX_DQS23_ph = 0;

    u16 MIU0_default_ph0 = 0;
    u16 MIU0_default_ph1 = 0;

    sub0_u16RegMiuArb    = CH0_RIU_ARB;
		sub0_u16RegMiuArbb   = CH0_RIU_ARBB;
    sub0_u16RegMiuAtop   = CH0_RIU_ATOP;
    sub0_u16RegMiuDtop   = CH0_RIU_DTOP;
    sub0_u16RegMiuDtopE  = CH0_RIU_DTOP_E;
    sub0_u16RegMiuArb_F  = CH0_RIU_ARB_F;
    sub0_u16RegMiuArb_E  = CH0_RIU_ARB_E;
    sub0_u16RegMiuAtop_E = CH0_RIU_ATOP_E;

    sub1_u16RegMiuArb    = CH1_RIU_ARB;
		sub1_u16RegMiuArbb   = CH1_RIU_ARBB;
    sub1_u16RegMiuAtop   = CH1_RIU_ATOP;
    sub1_u16RegMiuDtop   = CH1_RIU_DTOP;
    sub1_u16RegMiuDtopE  = CH1_RIU_DTOP_E;
    sub1_u16RegMiuArb_F  = CH1_RIU_ARB_F;
    sub1_u16RegMiuArb_E  = CH1_RIU_ARB_E;
    sub1_u16RegMiuAtop_E = CH1_RIU_ATOP_E;
    //----------------------------------------------------------------
    //
    //Un-mask BIST
    //
    //----------------------------------------------------------------
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0xe6, 0x7ffe);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop , sub1_u16RegMiuDtop , 0x1e, 0x8c08);

    MHal_MIU_WritReg16(sub0_u16RegMiuDtop , 0xfe, MHal_MIU_ReadReg16(sub0_u16RegMiuDtop, 0xfe) & ~0x0800); //set reg_miu_test_off = 0
    MHal_MIU_WritReg16(sub1_u16RegMiuDtop , 0xfe, MHal_MIU_ReadReg16(sub1_u16RegMiuDtop, 0xfe) & ~0x0800); //set reg_miu_test_off = 0

    //----------------------------------------------------------------
    //
    //Arbitration setting for auto-phase
    //
    //----------------------------------------------------------------
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArbb, sub1_u16RegMiuArbb, 0x1c, 0x0002); //enable miu_test limit mask (test_pre_arb4)
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0xe2, 0x0002); //disable ctrl_arb member limit

    MHal_MIU_WritReg16(sub0_u16RegMiuDtop, 0x38, MHal_MIU_ReadReg16(sub0_u16RegMiuDtop, 0x38) & ~0x0400 ); //[10] : reg_miu_cmd_cut_in_en = 0
    MHal_MIU_WritReg16(sub1_u16RegMiuDtop, 0x38, MHal_MIU_ReadReg16(sub1_u16RegMiuDtop, 0x38) & ~0x0400 ); //[10] : reg_miu_cmd_cut_in_en = 0

    MHal_MIU_WritReg16(sub0_u16RegMiuArb , 0x06, MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x06) & ~0x8000); // Trun off miu_test intra reorder	
    MHal_MIU_WritReg16(sub1_u16RegMiuArb , 0x06, MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x06) & ~0x8000); // Turn off miu_test intra reorder	
   
    //----------------------------------------------------------------
    //
    //MIU test pattern setting
    //
    //----------------------------------------------------------------
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop ,0xe0, 0x0006);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop ,0xe2, 0x0000); //base
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop ,0xe4, 0x2000); //length
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop ,0xe6, 0x0000); //length


    //----------------------------------------------------------------
    //
    //DPAT pre-setting
    //
    //----------------------------------------------------------------


    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x70, 0x0000); //[6:0]  reg_rx_ph_scan_st_offset
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x72, 0x2000); //[3:0]  reg_tx_ph_scan_st_offset
                                                                                 //[7:4]  reg_tx_ph_scan_ed_offset
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0xa8, 0x0913); //[5:0]  reg_max_tx_dq_phase
                                                                                 //[13:8] reg_max_tx_cmd_phase
    //MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub0_u16RegMiuArb,0xaa, 0x000d); //[5:0]  reg_max_rx_dqsm_phase
    MHal_MIU_WritReg16_sub01(sub1_u16RegMiuArb, sub1_u16RegMiuArb,0xaa, 0x0009); //[5:0]  reg_max_rx_dqsm_phase
                                                                                 //[15]   reg_rx_deskew_w_sel
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x78, 0x0004); //[2] : reg_dpat_sw_rst = 1
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x78, 0x0000); //[2] : reg_dpat_sw_rst = 0
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x7c, 0x8000); //[15:0] : reg_dpat_deb_sel
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb,0x74, 0x0d81); //[0] reg_live_test_en         = 0
                                                                                 //[1] reg_live_test_tx_enhance = 0



#ifdef DQSM_TRAINING_EN
//   if( (MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //DDR4?
//    DQSM_train_MPR(sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E, sub0_u16RegMiuDtop, sub0_u16RegMiuDtopE);
//   }else{ //DDR3
//    DQSM_train_BIST(sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E, sub0_u16RegMiuDtop, sub0_u16RegMiuDtopE);   	
//   }
//    
//   #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
//   if( (MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //DDR4?
//    DQSM_train_MPR(sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E, sub1_u16RegMiuDtop, sub1_u16RegMiuDtopE);
//   }else{ //DDR3
//    DQSM_train_BIST(sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E, sub1_u16RegMiuDtop, sub1_u16RegMiuDtopE);   	
//   }
//   #endif    
//====================================================================================
//
//
//Read DQSM training
//
//
//====================================================================================

    
    MIU0_default_ph0 = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop_E, 0x60);  
    MIU0_default_ph1 = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop_E, 0x62);  

    MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E , 0x60, 0x0909);
    MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E , 0x62, 0x0909);

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x70, 0x0202);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x7c, 0x800b);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0004);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x76, 0x0014);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0013);

    delayus_ddr(1);

    while((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);

    #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
    while((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
    #endif

    RX_DQSM_ph_update (sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E, sub0_u16RegMiuArb, sub1_u16RegMiuArb);
    //RX_DQSM_ph_update (sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E, sub0_u16RegMiuArb, sub1_u16RegMiuArb);
    
    
    MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E , 0x60, MIU0_default_ph0);
    MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E , 0x62, MIU0_default_ph1);

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub0_u16RegMiuArb,  0xaa, 0x000d); //[5:0]  reg_max_rx_dqsm_phase
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x70, 0x0202);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x7c, 0x800b);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0004);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x76, 0x0014);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0013);
    
    delayus_ddr(1);

    while((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);

    #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
    while((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
    #endif

    //RX_DQSM_ph_update (sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E, sub0_u16RegMiuArb, sub0_u16RegMiuArb);
    RX_DQSM_ph_update (sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E, sub0_u16RegMiuArb, sub0_u16RegMiuArb);
#endif     

#ifdef VREF_DQ_EN
    if(((MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04)||(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04)
    { //Case = DDR4
//====================================================================================
//
//
//VrefDQ training
//
//
//====================================================================================

        MHal_MIU_WritReg16(sub0_u16RegMiuDtopE, 0x1c, (MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x6c) & 0xff00) );       //Copy From DIG_E 0x36 [15:8] to DIG_E 0x0e [15:8]
        MHal_MIU_WritReg16(sub1_u16RegMiuDtopE, 0x1c, (MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x6c) & 0xff00) );       //Copy From DIG_E 0x36 [15:8] to DIG_E 0x0e [15:8]

        // Fine tune -->step :  +/- 2 codes twice
        u16RegVal = 4;//TDB
        u16RegVal = VrefDQ_train_loop(sub0_u16RegMiuDtop,
                                      sub1_u16RegMiuDtop,
                                      sub0_u16RegMiuDtopE,
                                      sub1_u16RegMiuDtopE,
                                      sub0_u16RegMiuArb,
                                      sub1_u16RegMiuArb,
                                      u16RegVal-4,          //Start Index
                                      1,                    //Step
                                      9);                   //Max
                                     

        sub0_u16Vref_indx_tmp =  (u16RegVal & 0x00ff)       ;//+ 4; //4 is VrefDQ offset (20180226 skydo mail update)
        sub1_u16Vref_indx_tmp = ((u16RegVal & 0xff00) >> 8) ;//+ 4; //4 is VrefDQ offset (20180226 skydo mail update)

        if(sub0_u16Vref_indx_tmp > 9) {
          sub0_u16Vref_indx = 9;
        } else {
        	sub0_u16Vref_indx = sub0_u16Vref_indx_tmp;
        }

        if(sub1_u16Vref_indx_tmp > 9) {
          sub1_u16Vref_indx = 9;
        } else {
        	sub1_u16Vref_indx = sub1_u16Vref_indx_tmp;
        }

        //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb  , sub1_u16RegMiuArb  , 0x78, 0x0000);
        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtopE, sub1_u16RegMiuDtopE, 0x14, 0x0001);       //[0] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref

        if( (MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //DDR4?
          //Set MRx[6:0] = Choose VrefDQ value & Range
          single_cmd(sub0_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[sub0_u16Vref_indx] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE

          //Enable & Change DQ Vref
          single_cmd(sub0_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[sub0_u16Vref_indx] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE

          //Exit DQ Vref training Mode
          single_cmd(sub0_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x1c) & ~0x0080) , 0x00); // (Disable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE
        }

        if( (MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) == 0x04 ) { //DDR4?
          //Set MRx[6:0] = Choose VrefDQ value & Range
          single_cmd(sub1_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[sub1_u16Vref_indx] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE

          //Enable & Change DQ Vref
          single_cmd(sub1_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x6c) & ~0x00ff) | VREF_VAL[sub1_u16Vref_indx] | 0x80, 0x00); // (Enable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE

          //Exit DQ Vref training Mode
          single_cmd(sub1_u16RegMiuDtopE,(MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x1c) & ~0x0080) , 0x00); // (Disable VrefDQ training)
          delayus_ddr(1); //wait 150ns for tVREFDQE
        }

        MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtopE, sub1_u16RegMiuDtopE, 0x14, 0x0000);       //[0] : reg_auto_ref_off = 0

    }
#endif

#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
//====================================================================================
//
//
//Write DQ phase training
//
//
//====================================================================================

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb, 0x78, 0x0004);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb, 0x76, 0x0002);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb, sub1_u16RegMiuArb, 0x78, 0x0013);

    while ((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);	//reg_r_track_finish
     delayus_ddr(1);

   #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
    while ((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
   #endif
    delayus_ddr(1);

    //if((MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03)
    //{ //Case = DDR4
    //    TX_DQ_ph_update(sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E);
    //}

    //if((MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03)
    //{ //Case = DDR4
        TX_DQ_ph_update(sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E);
    //}




//====================================================================================
//
//
//Write DQM phase training
//
//
//====================================================================================


    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop, 0xbe, 0x0001);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x40, 0x0010);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x78, 0x0004);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x78, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x76, 0x0002);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x78, 0x0013);

    delayus_ddr(1);

    while((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);

   #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
    while((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
   #endif

    // wriu -w 0x110dc0  0x0004
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0xc0, 0x0004);
    delayus_ddr(1);

    //For SUB0
    if((MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03) { //Case = DDR4
      u16RegVal0 = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop, 0xca);
      MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E, 0x14, u16RegVal0);

      u16RegVal0 = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop, 0xcc);
      MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E, 0x16, u16RegVal0);
    }

    //if((MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03) { //Case = DDR4
    //For SUB1
      u16RegVal0 = MHal_MIU_ReadReg16(sub1_u16RegMiuAtop, 0xca);
      MHal_MIU_WritReg16(sub1_u16RegMiuAtop_E, 0x14, u16RegVal0);

      u16RegVal0 = MHal_MIU_ReadReg16(sub1_u16RegMiuAtop, 0xcc);
      MHal_MIU_WritReg16(sub1_u16RegMiuAtop_E, 0x16, u16RegVal0);
    //}

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuDtop, sub1_u16RegMiuDtop, 0xbe, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x40, 0x0000);


////-----------------------------------------------------------------------
////Patch for Heavy loading phase shift issue
////DQS phase result, LW: +1, HW: +1
////-----------------------------------------------------------------------
//
//    if((MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03)
//    { //Case = DDR4
//    	//For LW
//      TX_DQS01_ph = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop_E, 0x10) +  1 + (1 << 8);
//      //For HW
//      TX_DQS23_ph = MHal_MIU_ReadReg16(sub0_u16RegMiuAtop_E, 0x12) +  1 + (1 << 8);
//      
//      MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E, 0x10, TX_DQS01_ph);
//      MHal_MIU_WritReg16(sub0_u16RegMiuAtop_E, 0x12, TX_DQS23_ph);
//    }
//
//    if((MHal_MIU_ReadReg16(sub1_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03)
//    { //Case = DDR4
//    	//For LW
//      TX_DQS01_ph = MHal_MIU_ReadReg16(sub1_u16RegMiuAtop_E, 0x10) +  1 + (1 << 8);
//      //For HW
//      TX_DQS23_ph = MHal_MIU_ReadReg16(sub1_u16RegMiuAtop_E, 0x12) +  1 + (1 << 8);
//      
//      MHal_MIU_WritReg16(sub1_u16RegMiuAtop_E, 0x10, TX_DQS01_ph);
//      MHal_MIU_WritReg16(sub1_u16RegMiuAtop_E, 0x12, TX_DQS23_ph);
//    }
//-----------------------------------------------------------------------
//Patch for Heavy loading phase shift issue
//DQ per bit phase result offset
//-----------------------------------------------------------------------
//    if((MHal_MIU_ReadReg16(sub0_u16RegMiuDtopE, 0x20) & 0x01f) != 0x03)
//    { //Case = DDR4
//    
//    //Low word	
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x14, 1, 1); //For DM    
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x20, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x22, 1, 1); 
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x24, 1, 1); 
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x26, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x28, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x2a, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x2c, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x2e, 1, 1);
//    
//    //High word
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x16, 1, 1); //For DM
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x30, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x32, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x34, 2, 2); //HW DQ4,DQ5
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x36, 2, 1); //HW DQ6
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x38, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x3a, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x3c, 1, 1);
//    MHal_MIU_WritReg16_tx_dq_offset_patch(sub0_u16RegMiuAtop_E, 0x3e, 1, 1);
//    }
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE
//====================================================================================
//
//
//Read DQ deskew training
//
//
//====================================================================================

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x7c, 0x800b);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0004);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0000);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x76, 0x0011);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb,  sub1_u16RegMiuArb,  0x78, 0x0013);

    delayus_ddr(1);

    while((MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x78) & 0x8000) == 0x0);

   #if !defined(CONFIG_UMA) && !defined(CONFIG_MIU1_DRAM_NONE)
    while((MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x78) & 0x8000) == 0x0);
   #endif

     RX_DQ_ph_update (sub0_u16RegMiuAtop, sub0_u16RegMiuAtop_E);
     RX_DQ_ph_update (sub1_u16RegMiuAtop, sub1_u16RegMiuAtop_E);

#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */
	// wait 1 // wait about 500ns for Dummy write only worst pattern BIST time
    delayus_ddr(1);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x70, 0x0000); //[11] reg_dline_gated_control = 0
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x90, 0xf0f3);
    delayus_ddr(1);
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x78, 0x0000); //[0] reg_dpat_en = 0
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0x74, 0x0d80); //[0] reg_live_test_en = 0
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuAtop, sub1_u16RegMiuAtop, 0x70, 0x0800); //[11] reg_dline_gated_control = 1

    //----------------------------------------------------------------
    //
    //Recovery settings
    //
    //----------------------------------------------------------------

    MHal_MIU_WritReg16(sub0_u16RegMiuArb , 0x06, MHal_MIU_ReadReg16(sub0_u16RegMiuArb, 0x06) | 0x8000); // Trun on miu_test intra reorder
    MHal_MIU_WritReg16(sub1_u16RegMiuArb , 0x06, MHal_MIU_ReadReg16(sub1_u16RegMiuArb, 0x06) | 0x8000); // Turn on miu_test intra reorder

  //MHal_MIU_WritReg16(sub0_u16RegMiuDtop, 0x38, MHal_MIU_ReadReg16(sub0_u16RegMiuDtop, 0x38) & ~0x0400 ); //[10] : reg_miu_cmd_cut_in_en = 0
  //MHal_MIU_WritReg16(sub1_u16RegMiuDtop, 0x38, MHal_MIU_ReadReg16(sub1_u16RegMiuDtop, 0x38) & ~0x0400 ); //[10] : reg_miu_cmd_cut_in_en = 0

    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArbb, sub1_u16RegMiuArbb, 0x1c, 0x0000); //enable miu_test limit mask (test_pre_arb4)
    MHal_MIU_WritReg16_sub01(sub0_u16RegMiuArb , sub1_u16RegMiuArb , 0xe2, 0x7ffe); //enable ctrl_arb member limit




    putk_ddr('M');
    putk_ddr('I');
    putk_ddr('U');
    putn_ddr(u8MiuDev);
    putk_ddr('_');
    putk_ddr('D');
    putk_ddr('Q');
    putk_ddr('S');
    putk_ddr('-');
    putk_ddr('O');
    putk_ddr('K');
    putk_ddr('\n');
    putk_ddr('\r');
}
