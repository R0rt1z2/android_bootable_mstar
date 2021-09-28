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

#include <command.h>
#include <common.h>
#include <ShareType.h>
#include <drvGPIO.h>
#include <drvSAR.h>
#include <MsDebug.h>
#include <MsUboot.h>
#include <MsTypes.h>
#include <MsSysUtility.h>
#include <stdio.h>

MS_U8 ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(MS_RIU_MAP))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_U16 Read2Byte(MS_U32 u32RegAddr)
{
    MS_U16 u16Value=0;
    MS_U16 u8Value=0;    
    u16Value = (MS_U16)((volatile MS_U8*)(MS_RIU_MAP))[(u32RegAddr << 1) - (u32RegAddr & 1)];
    u32RegAddr++;
    u8Value = ((volatile MS_U8*)(MS_RIU_MAP))[(u32RegAddr << 1) - (u32RegAddr & 1)];
    u16Value = u16Value | (u8Value << 8);
    return u16Value;
}


MS_BOOL WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    ((volatile MS_U8*)(MS_RIU_MAP))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    WriteByte(u32RegAddr,(MS_U8)u16Val);
    WriteByte((u32RegAddr+1),(MS_U8)(u16Val>>8));    
    return TRUE;
}

MS_BOOL WriteRegBit(MS_U32 u32RegAddr, MS_U16 u16Bit, MS_BOOL bEnable)
{
    MS_U16 u16Val = 0;

    u16Val = Read2Byte(u32RegAddr);  
    u16Val = (bEnable) ? (u16Val | u16Bit) : (u16Val & ~u16Bit);
    Write2Byte(u32RegAddr,u16Val);
    return TRUE;
}

MS_BOOL WriteRegBitPos(MS_U32 u32RegAddr, MS_U8 u8Bit, MS_BOOL bEnable)
{
    MS_U16 u16Bit;
    
    if(u8Bit >15)
    {
        UBOOT_ERROR("Over the bank boundary!\n");
    }

    u16Bit= (1 << u8Bit);
    WriteRegBit( u32RegAddr,  u16Bit,  bEnable);

    return TRUE;
}

int do_riu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32 u32Target = 0;
    MS_U32 u32Value  = 0;
    MS_U8  u8Bit =0;
    
    if (argc < 3)
    {
        cmd_usage(cmdtp);
        return 0;
    }
     
    if(strncmp(argv[1], "wword", 2) == 0) 
    {    
        u32Target = simple_strtoul(argv[2],NULL,16);     
        u32Value = simple_strtoul(argv[3],NULL,16);       
        Write2Byte(u32Target,(MS_U16)u32Value);
        u32Target = simple_strtoul(argv[2],NULL,16);     
        UBOOT_INFO("[%x]:0x%x\n",(unsigned int)u32Target,(unsigned int)Read2Byte(u32Target));
        
    }    
    else if(strncmp(argv[1], "rword", 2) == 0) 
    {
        u32Target = simple_strtoul(argv[2],NULL,16);     
        UBOOT_INFO("[%x]:0x%x\n",(unsigned int)u32Target,(unsigned int)Read2Byte(u32Target));
    }
    else if(strncmp(argv[1], "wbyte", 2) == 0) 
    {
        u32Target = simple_strtoul(argv[2],NULL,16);     
        u32Value = simple_strtoul(argv[3],NULL,16);           
        WriteByte(u32Target,(MS_U8)u32Value);
        UBOOT_INFO("[%x]:0x%x\n",(unsigned int)u32Target,(unsigned int)ReadByte(u32Target));
    }    
    else if(strncmp(argv[1], "rbyte", 2) == 0) 
    {
        u32Target = simple_strtoul(argv[2],NULL,16);         
        UBOOT_INFO("[%x]:0x%x\n",(unsigned int)u32Target,(unsigned int)ReadByte(u32Target));
    }     
    else if(strncmp(argv[1], "bit", 2) == 0) 
    {
        u32Target = simple_strtoul(argv[2],NULL,16);         
        u8Bit = simple_strtoul(argv[3],NULL,10);        
        u32Value = simple_strtoul(argv[4],NULL,16);              
        WriteRegBitPos(u32Target,u8Bit,(MS_U8)u32Value);
        UBOOT_INFO("[%x]:0x%x\n",(unsigned int)u32Target,(unsigned int)ReadByte(u32Target));
    }             
    else
    {
      cmd_usage(cmdtp);
    }
    
    return 0;    
}

int do_sar( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    U8 ch;
    U8 sarValue;
    U16 percent;

    if(argc<2)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    UBOOT_DEBUG("MS_RIU_MAP is 0x%x\n",(U32)MS_RIU_MAP);    

    ch=(U8)atoi(argv[1]);  
 
	sarValue=MDrv_SAR_Adc_GetValue(ch);
    percent=1.0*sarValue*1000/0x3ff;
    UBOOT_INFO("sar(8bit) = %x\n", sarValue);

    UBOOT_TRACE("OK\n");    
    return 0;
}



int do_gpio( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    MS_GPIO_NUM gpio_num;
    MS_GPIO_NUM gpio_num_p1;

    if(argc<3)
    {
        if(strcmp(argv[1],"list")==NULL && argc==2)
        {
        }
        else
        {
            cmd_usage(cmdtp);
            return -1;
        }
    }

    gpio_num=(MS_GPIO_NUM)atoi(argv[2]);  
    gpio_num_p1=gpio_num+1;

    if((strcmp(argv[1],"get")==NULL) ||(strncmp(argv[1],"inp",3)==NULL))
    {
        mdrv_gpio_set_input(gpio_num_p1);      
        UBOOT_INFO("IN  ");
        UBOOT_INFO("pin=%d\n", mdrv_gpio_get_level(gpio_num_p1));
    }
    else if(strncmp(argv[1],"output",3)==NULL)
    {
        if(argv[3][0]=='1') mdrv_gpio_set_high(gpio_num_p1);
        else if(argv[3][0]=='0') mdrv_gpio_set_low(gpio_num_p1);
        
        UBOOT_INFO("gpio[%d] is %d\n", gpio_num, mdrv_gpio_get_level(gpio_num_p1));
    }
    else if(strncmp(argv[1],"toggle",3)==NULL)
    {
        if(mdrv_gpio_get_level(gpio_num_p1)) 
        {
            mdrv_gpio_set_low(gpio_num_p1);        
        }
        else 
        {    
            mdrv_gpio_set_high(gpio_num_p1);
        }
        UBOOT_INFO("gpio[%d] is %d\n", gpio_num, mdrv_gpio_get_level(gpio_num_p1));
    }
    else if(strncmp(argv[1],"check",3)==NULL)
    {
        int ret = 0;
        int value = atoi(argv[4]);
        char gpio_state_command[CMD_BUF];

        UBOOT_INFO("Check gpio %d %s pin=%d ", gpio_num, argv[3], value);
        if(mdrv_gpio_get_inout(gpio_num_p1))
        {
            if(strncmp(argv[3],"IN",2)!=NULL)
                ret = -1;
        }
        else
        {
            if(strncmp(argv[3],"OUT",3)!=NULL)
                ret = -1;
        }
        if (value != mdrv_gpio_get_level(gpio_num_p1))
            ret = -1;

        if (ret != -1)
            UBOOT_INFO("ok\n");
        else
            UBOOT_INFO("fail\n");
        snprintf(gpio_state_command, CMD_BUF, "gpio state %d", gpio_num);
        UBOOT_DEBUG("cmd=%s\n", gpio_state_command);
        run_command(gpio_state_command, 0);
        return ret;
    }
    else if(strncmp(argv[1],"state",3)==NULL)
    {
        if(mdrv_gpio_get_inout(gpio_num_p1)) 
        {
            UBOOT_INFO("IN  ");
        }
        else
        {
            UBOOT_INFO("OUT ");
        }
        UBOOT_INFO("pin=%d\n", mdrv_gpio_get_level(gpio_num_p1));
    }
    else if(strncmp(argv[1],"list",3)==NULL)
    {
        U8 i;
        U8 size;


        if(argc==2)
        {
            size=200;
        }
        else
        {
            size=(MS_GPIO_NUM)atoi(argv[2]);  
        }
        
        for(i=1;i<size;i++) // END_GPIO_NUM 213 in eiffel
        {
            UBOOT_INFO("GPIO %3d ", i);
            gpio_num_p1=i+1;
        
            if(mdrv_gpio_get_inout(gpio_num_p1)) 
            {
                UBOOT_INFO("IN  ");
            }
            else
            {
                UBOOT_INFO("Out ");
            }
            UBOOT_INFO("pin=%d\n", mdrv_gpio_get_level(gpio_num_p1));

            if(i%10==0) UBOOT_INFO("\n");
        
        }

    }
    else
    {
        cmd_usage(cmdtp);
        return -1;

    }


    UBOOT_TRACE("OK\n");    
    return 0;
}





