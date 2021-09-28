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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------

#include <common.h>
#include <command.h>
#if defined(CONFIG_CMD_EEPROM) && (CONFIG_MSTARI2C == 1)
#include <exports.h>
#include <apiSWI2C.h>
#include <CusConfig.h>
#include <CusDevice.h>
#include <MsOS.h>
#include <MsDebug.h>
#include <ShareType.h>
#include <MsEeprom.h>
#include <MsUboot.h>

#define PROJECT_ID "projectID"
#define PROJECT_ID_ADDRESS "Address"


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HIBYTE(value)     ((MS_U8)((value) >> 8))
#define LOBYTE(value)     ((MS_U8)(value))

typedef enum
{
    EEPROM_24C01,
    EEPROM_24C02,
    EEPROM_24C04,
    EEPROM_24C08,
    EEPROM_24C16,
    EEPROM_24C32,
    EEPROM_24C64,
    EEPROM_24C128,
    EEPROM_24C256,
    EEPROM_24C512,
    EEPROM_24C1024,
} EEPROM_FAMILY;


typedef struct
{
    MS_U16 u16DeviceAddr;
    EEPROM_FAMILY type;        // 24c02~24c512, page size is used in burst write
} ST_EEPROM_ID;


typedef struct
{
    MS_U16 u16SlaveIDAddr;
    MS_U8  u8AddrCnt;
    MS_U8  pu8Offset[2];
    MS_U16 u8PageSize; // max 2 bytes for many years !!
} ST_EEPROM_PARA;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
const char *const fmt = "\nEEPROM @0x%lX(%d) %s: addr %08lx  off %04lx  count %ld ... \n";
static MS_U8 EepromID = 0 ; // default is 0
ST_EEPROM_ID stEepromArray[EEPROM_REGISTER_NUM]=
{
    EEPROM_REGISTER_INFO
};

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static char* getEepromName(EEPROM_FAMILY type);
static void showDevInfo(void);
static ST_EEPROM_PARA getEepeomParameter(ST_EEPROM_ID EepromType,MS_U16 offset);

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
static ST_EEPROM_PARA getEepeomParameter(ST_EEPROM_ID EepromType,MS_U16 offset)
{
    ST_EEPROM_PARA stEtype;
    u16 u16Tmp = 0;

    switch(EepromType.type)
    {
        case EEPROM_24C01:
        case EEPROM_24C02:
            stEtype.u16SlaveIDAddr=EepromType.u16DeviceAddr;
            stEtype.u8AddrCnt=1;
            stEtype.pu8Offset[0]=LOBYTE(offset);
            stEtype.u8PageSize=8;
            break;
        case EEPROM_24C04:
            u16Tmp=((HIBYTE(offset)&0x1)<<1);
            stEtype.u16SlaveIDAddr=(EepromType.u16DeviceAddr|u16Tmp);
            stEtype.u8AddrCnt=1;
            stEtype.pu8Offset[0]=LOBYTE(offset);
            stEtype.u8PageSize=16;
            break;
        case EEPROM_24C08:
            u16Tmp=((HIBYTE(offset)&0x3)<<1);
            stEtype.u16SlaveIDAddr=(EepromType.u16DeviceAddr|u16Tmp);
            stEtype.u8AddrCnt=1;
            stEtype.pu8Offset[0]=LOBYTE(offset);
            stEtype.u8PageSize=16;
            break;
        case EEPROM_24C16:
            u16Tmp=((HIBYTE(offset)&0x7)<<1);
            stEtype.u16SlaveIDAddr=(EepromType.u16DeviceAddr|u16Tmp);
            stEtype.u8AddrCnt=1;
            stEtype.pu8Offset[0]=LOBYTE(offset);
            stEtype.u8PageSize=16;
            break;
        case EEPROM_24C32:
        case EEPROM_24C64:
            stEtype.u8PageSize=32;
            stEtype.u16SlaveIDAddr=EepromType.u16DeviceAddr;
            stEtype.u8AddrCnt=2;
            stEtype.pu8Offset[0]=HIBYTE(offset);
            stEtype.pu8Offset[1]=LOBYTE(offset);
            break;
        case EEPROM_24C128:
        case EEPROM_24C256:
            stEtype.u8PageSize=64;
            stEtype.u16SlaveIDAddr=EepromType.u16DeviceAddr;
            stEtype.u8AddrCnt=2;
            stEtype.pu8Offset[0]=HIBYTE(offset);
            stEtype.pu8Offset[1]=LOBYTE(offset);
            break;
        case EEPROM_24C512:
            stEtype.u8PageSize=128;
            stEtype.u16SlaveIDAddr=EepromType.u16DeviceAddr;
            stEtype.u8AddrCnt=2;
            stEtype.pu8Offset[0]=HIBYTE(offset);
            stEtype.pu8Offset[1]=LOBYTE(offset);
        case EEPROM_24C1024:
            stEtype.u8PageSize=256;
            stEtype.u16SlaveIDAddr=EepromType.u16DeviceAddr;
            stEtype.u8AddrCnt=2;
            stEtype.pu8Offset[0]=HIBYTE(offset);
            stEtype.pu8Offset[1]=LOBYTE(offset);
            break;
        default:
            UBOOT_ERROR("Wrong Eeprom Type!!\n");
            break;
    }
    return stEtype;
}



int eeprom_PageRead(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen)
{
     ST_EEPROM_PARA stEtype;
     MS_U16 u16writeSize=0;

     if(pBuf==NULL)
     {
        UBOOT_ERROR("input parameter 'pBuf' is a null pointer\n");
        return -1;
     }

     while(u16BufLen>0)
     {
        stEtype = getEepeomParameter(stEepromArray[EepromID],offset);
        UBOOT_DEBUG(fmt, stEtype.u16SlaveIDAddr, stEtype.u8AddrCnt,"read", pBuf,stEtype.pu8Offset, u16BufLen);
        if(u16BufLen >= stEtype.u8PageSize)
        {
            u16writeSize = stEtype.u8PageSize;
            u16BufLen = u16BufLen - stEtype.u8PageSize;
        }
        else
        {
            u16writeSize = u16BufLen;
            u16BufLen = 0;
        }
        if(MApi_SWI2C_ReadBytes(stEtype.u16SlaveIDAddr, stEtype.u8AddrCnt,stEtype.pu8Offset,u16writeSize, (MS_U8*)(pBuf)) != TRUE)
        {
            UBOOT_ERROR("MApi_SWI2C_ReadBytes FAIL!!\n");
            return -1;
        }

        pBuf+=u16writeSize;
        offset+=u16writeSize;
    }
    return 0;
}

int eeprom_PageWrite(MS_U16 offset, MS_U8 *pBuf, MS_U16 u16BufLen)
{
    ST_EEPROM_PARA stEtype;
    MS_U16 u16writeSize=0;

    if(pBuf==NULL)
    {
       UBOOT_ERROR("input parameter 'pBuf' is a null pointer\n");
       return -1;
    }

    while(u16BufLen>0)
    {
       stEtype = getEepeomParameter(stEepromArray[EepromID],offset);
       UBOOT_DEBUG(fmt, stEtype.u16SlaveIDAddr, stEtype.u8AddrCnt,"write", pBuf,stEtype.pu8Offset, u16BufLen);
       if(u16BufLen >= stEtype.u8PageSize)
       {
           u16writeSize = stEtype.u8PageSize;
           u16BufLen = u16BufLen - stEtype.u8PageSize;
       }
       else
       {
           u16writeSize = u16BufLen;
           u16BufLen = 0;
       }
       if(MApi_SWI2C_WriteBytes(stEtype.u16SlaveIDAddr, stEtype.u8AddrCnt,stEtype.pu8Offset,u16writeSize, (MS_U8*)(pBuf)) != TRUE)
       {
           UBOOT_ERROR("MApi_SWI2C_WriteBytes FAIL!!\n");
           return -1;
       }

       MsOS_DelayTask(10);
       pBuf+=u16writeSize;
       offset+=u16writeSize;
   }
    return 0;
}

MS_U8 eeprom_ReadByte(MS_U16 offset)
{
    MS_U8 u8Value = 0;
    eeprom_PageRead(offset,(MS_U8 *)&u8Value,1);
    return u8Value;
}

int eeprom_WriteByte(MS_U16 offset,MS_U8 u8Value)
{
    return eeprom_PageWrite(offset, (MS_U8 *)&u8Value, 1);
}

static char* getEepromName(EEPROM_FAMILY type)
{
    char *EepromName = NULL;
    switch(type)
    {
        case EEPROM_24C01:
            EepromName="AT24C01";
            break;
        case EEPROM_24C02:
            EepromName="AT24C02";
            break;
        case EEPROM_24C04:
            EepromName="AT24C04";
            break;
        case EEPROM_24C08:
            EepromName="AT24C08";
            break;
        case EEPROM_24C16:
            EepromName="AT24C16";
            break;
        case EEPROM_24C32:
            EepromName="AT24C32";
            break;
        case EEPROM_24C64:
            EepromName="AT24C64";
            break;
        case EEPROM_24C128:
            EepromName="AT24C128";
            break;
        case EEPROM_24C256:
            EepromName="AT24C256";
            break;
        case EEPROM_24C512:
            EepromName="AT24C512";
        case EEPROM_24C1024:
            EepromName="AT24C1024";
            break;
        default:
            EepromName="UnKnowName\n";
            break;
    }
    return EepromName;
}

static void showDevInfo(void)
{
    int i=0;
    UBOOT_INFO("Num\tDeviceID\tDeviceType\n");
    for(i=0;i<EEPROM_REGISTER_NUM;i++)
    {
        UBOOT_INFO(" %d\t 0x%.4x\t\t %s\n",(i+1),stEepromArray[i].u16DeviceAddr,getEepromName(stEepromArray[i].type));
    }
}

void eeprom_switch(int i)   // base 0
{
    EepromID = i;
    UBOOT_INFO("EepromNum : %d\n",EepromID+1);
}

MS_U16 getaddress()
{
	char Pathbuff [BUFFER_SIZE]="\0";
	char addressbuff[BUFFER_SIZE]="\0";
	char *script = NULL;
	U32 filesize = 0;

	vfs_mount(CONFIG);
    snprintf(Pathbuff,sizeof(Pathbuff),"%s/eeprom/eeprom_key_setting_cus.ini",CONFIG_PATH);
    script = loadscript(Pathbuff,&filesize);
    if(script == NULL)
    {
        UBOOT_INFO("eeprom_key_setting_cus.ini not exist, please check it again!\n");
        return -1;
    }
	UBOOT_INFO("sizeof( eeprom_key_setting_cus.ini ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

	if(Profile_GetString(PROJECT_ID, PROJECT_ID_ADDRESS,"", addressbuff, (sizeof(addressbuff) / sizeof((addressbuff)[0]))))
	{
		char buff2[BUFFER_SIZE]="\0";
		sscanf(addressbuff, "0X%s", &buff2);
		MS_U16 buff3=strtol(buff2,NULL,16);

		return buff3;
	}
}

int eepromWrite( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	MS_U32 value = 0;
    MS_U32 off = 0;
	int ret = 0;

	MDrv_WriteByte(0x121016,MDrv_ReadByte(0x121016)&~BIT7);
	MDrv_WriteByte(0x121014,MDrv_ReadByte(0x121014)&~BIT7);//eep unproduct

	off=simple_strtoul (argv[1], NULL, 16);
	value=simple_strtoul (argv[2], NULL, 16);
	ret = eeprom_WriteByte(off + getaddress(),value);

	UBOOT_INFO("write 0x%x\n",(unsigned int)value);

	MDrv_WriteByte(0x121014,MDrv_ReadByte(0x121014)&BIT7);//eep product

	return 0;
}

int eepromRead( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	MS_U32 value = 0;
    MS_U32 off = 0;

	off = simple_strtoul (argv[1], NULL, 16);
	value = eeprom_ReadByte(off+getaddress());

	UBOOT_INFO("read 0x%x\n",(unsigned int)value);

	return 0;
}

int do_eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32 addr = 0;
    MS_U32 off = 0;
    MS_U16 cnt = 0;
    char * data = NULL;
    int ret = 0;
    char buffer[CMD_BUF]="\0";

    if (argc < 2)
    {
        return cmd_usage(cmdtp);
    }
    if (strncmp (argv[1], "wb", 2) == 0)
    {
         off=simple_strtoul (argv[2], NULL, 16);
         addr=simple_strtoul (argv[3], NULL, 16);
         ret = eeprom_WriteByte(off,addr);

         addr=(MS_U32)eeprom_ReadByte(off);
         UBOOT_INFO("0x%x\n",(unsigned int)addr);

    }
    else if (strncmp (argv[1], "rb", 2) == 0)
    {
		off  = simple_strtoul (argv[2], NULL, 16);
        addr=(MS_U32)eeprom_ReadByte(off);
        UBOOT_INFO("0x%x\n",(unsigned int)addr);
    }
    else if (strncmp (argv[1], "ws", 2) == 0)
    {
		off = simple_strtoul (argv[2], NULL, 16);
		data = (char *)argv[3];
        ret = eeprom_PageWrite(off,(MS_U8*)data,strlen(data));
    }
	else if (strncmp (argv[1], "dump", 4) == 0)
    {
 		off  = simple_strtoul (argv[2], NULL, 16);
		cnt  = simple_strtoul (argv[3], NULL, 16);
        data=(char *)malloc(cnt);
        if(data==NULL)
        {
            UBOOT_ERROR("memory allocate fail\n");
            return -1;
        }
        eeprom_PageRead(off,(MS_U8 *)data,cnt);
        snprintf(buffer,CMD_BUF,"md.b %x %x",data,cnt);
        run_command(buffer,0);
    }
	else if (strncmp (argv[1], "read", 4) == 0)
    {
		addr = simple_strtoul (argv[2], NULL, 16);
		off  = simple_strtoul (argv[3], NULL, 16);
		cnt  = simple_strtoul (argv[4], NULL, 16);
        ret = eeprom_PageRead(off,(MS_U8*)addr,cnt);
    }
    else if (strncmp (argv[1], "write", 5) == 0)
    {
		addr = simple_strtoul (argv[2], NULL, 16);
		off  = simple_strtoul (argv[3], NULL, 16);
		cnt  = simple_strtoul (argv[4], NULL, 16);
		ret = eeprom_PageWrite(off,(MS_U8*)addr,cnt);
	}
    else if (strncmp (argv[1], "erase", 5) == 0)
    {
		cnt  = simple_strtoul (argv[2], NULL, 16);
        data=(char *)malloc(cnt);
        memset((void *)data,0xFF,cnt);
        ret = eeprom_PageWrite(off,(MS_U8*)data,cnt);
        free(data);
    }
    else if (strncmp (argv[1], "switch", 5) == 0)
    {
        EepromID = simple_strtoul (argv[2], NULL, 16);
        EepromID--;
        UBOOT_INFO("EepromNum : %d\n",EepromID+1);

    }
    else if (strncmp (argv[1], "list", 4) == 0)
    {
        UBOOT_INFO("------------------------------------\n");
        UBOOT_INFO("    Current Eeprom Number : %d \n",(EepromID+1));
        UBOOT_INFO("------------------------------------\n");
        showDevInfo();
        UBOOT_INFO("------------------------------------\n");
    }
    else
    {
        cmd_usage(cmdtp);
    }

	return ret;
}



#endif
