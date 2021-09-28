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
#include <MsProjectID.h>
#include <drv_eMMC.h>
#include <eMMC_err_codes.h>
#include <MsDebug.h>
#include <CusConfig.h>


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static mstar_project_ID_t ProjectID;
static MS_BOOL bIsInit = FALSE; 

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
int do_Multi2Optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	  char *partitionNamePtr = NULL;
	  const MultiImageHeader_t* multiOpteeHeaderPtr;
	  char cmdBuffer[256];
	  int ret = -1;
	  unsigned int filesize = 0;
    if (argc < 4)
        return cmd_usage(cmdtp);
    
    UBOOT_TRACE("IN\n");
    multiOpteeHeaderPtr = simple_strtoul(argv[1], NULL, 16);
    partitionNamePtr = argv[2];
    filesize = simple_strtoul(argv[3], NULL, 16);
    
    if(!strncmp(multiOpteeHeaderPtr->magic,MULTIBIN_MAG,strlen(MULTIBIN_MAG))) //multiOptee image
    {
    	  uint16_t totalnumber=multiOpteeHeaderPtr->number;
    	  mstar_project_ID_t projectID = GetProjectID();
    	  int i = 0;
    	  MS_BOOL bIsFound = FALSE;
        UBOOT_TRACE("MultiOptee image detected.\n");
        for (i=0;i<totalnumber;i++)
        {
            if(multiOpteeHeaderPtr->info[i].id == projectID.Model_ID)
            {
                bIsFound = TRUE;
                break;
            }
        }
        
        if(bIsFound == TRUE)
        {
            //mmc erase partition
            sprintf(cmdBuffer,"mmc erase.p %s",partitionNamePtr);
            UBOOT_DEBUG("%s\n",cmdBuffer);
            ret=run_command(cmdBuffer,0);
            if(ret==-1)
            {
                UBOOT_ERROR("command failed:%s\n",cmdBuffer);
                return ret;
            }
            //mmc write partition
            UBOOT_DEBUG("ImageInfo matched,i=%d,offset=0x%x,size=0x%x.\n",i,multiOpteeHeaderPtr->info[i].offset,multiOpteeHeaderPtr->info[i].size);
            sprintf(cmdBuffer,"mmc write.p 0x%x %s 0x%x",(unsigned int)multiOpteeHeaderPtr+multiOpteeHeaderPtr->info[i].offset,partitionNamePtr,multiOpteeHeaderPtr->info[i].size);
            UBOOT_DEBUG("%s\n",cmdBuffer);
            ret=run_command(cmdBuffer,0);
            if(ret==-1)
            {
                UBOOT_ERROR("command failed:%s\n",cmdBuffer);
                return ret;
            }
        }
        else
        {
            UBOOT_ERROR("No ImageInfo_t ID matches the ModelID:%02d\n",projectID.Model_ID);
            return ret;
        }
        
    }
    else//Not the multiOptee image
    {
        UBOOT_TRACE("non-MultiOptee image detected.\n");
        //mmc erase partition
        sprintf(cmdBuffer,"mmc erase.p %s",partitionNamePtr);
        UBOOT_DEBUG("%s\n",cmdBuffer);
        ret=run_command(cmdBuffer,0);
        if(ret==-1)
        {
            UBOOT_ERROR("command failed:%s\n",cmdBuffer);
            return ret;
        }
        //mmc write partition
        sprintf(cmdBuffer,"mmc write.p 0x%x %s 0x%x",multiOpteeHeaderPtr,partitionNamePtr,filesize);
        UBOOT_DEBUG("%s\n",cmdBuffer);
        ret=run_command(cmdBuffer,0);
        if(ret==-1)
        {
            UBOOT_ERROR("command failed:%s\n",cmdBuffer);
            return ret;
        }
    }
    UBOOT_TRACE("OK\n");
	  return 0;
}

int ReadProjectIdFromBoot()
{
    unsigned char temp [256*1024];
    char* pTconPanelEnv = NULL;

    UBOOT_TRACE("IN\n");

    memset(&ProjectID,0,sizeof(ProjectID));
#if defined(CONFIG_ONEBIN_GPIO_ENABLE) // read model ID from GPIO and CHIP info
    ProjectID.Model_ID = MsDrv_OneSN_Get_model_ID_by_gpio();
#else // read model ID from Model_ID.bin in boot1 partition
    if(eMMC_ST_SUCCESS != eMMC_ReadBootPart(temp, 256*1024, 0, 1))
    {
        UBOOT_ERROR("eMMC_ReadBootPart read fail\n");
        return -1;
    }
    UBOOT_DUMP(temp+PROJECTID_OFFSET,0x20);
    memcpy(&ProjectID,temp+PROJECTID_OFFSET,sizeof(ProjectID));
#endif
    bIsInit = TRUE;

    if (((pTconPanelEnv = getenv("enable_tcon_panel")) != NULL) && (strcmp(pTconPanelEnv,"1") == 0))
    {
        ProjectID.Model_ID += 8;  // tcon panel case, model_id + 8
    }

    sprintf(temp,"%d\0",ProjectID.Model_ID);
    setenv("modelID", temp);
    sprintf(temp,"/%s/config/onebin/model_%d/Customer_1.ini\0",CONFIG,ProjectID.Model_ID);
    setenv("customer_ini_path", temp);
    saveenv();

    UBOOT_DEBUG("ProjectID.Model_ID=0X%X\n",ProjectID.Model_ID);
    UBOOT_DEBUG("ProjectID.Board_ID=0X%X\n",ProjectID.Board_ID);
    UBOOT_DEBUG("ProjectID.Package_ID=0X%X\n",ProjectID.Package_ID);
    UBOOT_DEBUG("ProjectID.DDR0_ID=0X%X\n",ProjectID.DDR0_ID);
    UBOOT_DEBUG("ProjectID.DDR0_Size=0X%X\n",ProjectID.DDR0_Size);
    UBOOT_DEBUG("ProjectID.DDR1_ID=0X%X\n",ProjectID.DDR1_ID);
    UBOOT_DEBUG("ProjectID.DDR1_Size=0X%X\n",ProjectID.DDR1_Size);
    UBOOT_DEBUG("ProjectID.DDR2_ID=0X%X\n",ProjectID.DDR2_ID);
    UBOOT_DEBUG("ProjectID.DDR2_Size=0X%X\n",ProjectID.DDR2_Size);
    UBOOT_DEBUG("ProjectID.Mmap_ID=0X%X\n",ProjectID.Mmap_ID);
    UBOOT_DEBUG("ProjectID.Keypad_ID=0X%X\n",ProjectID.Keypad_ID);
    UBOOT_TRACE("OK\n");
    return 0;
}

mstar_project_ID_t GetProjectID()
{
    if (bIsInit == FALSE)
    {
        UBOOT_ERROR("ProjectID has not been initialized yet!!!\n");
    }
    return ProjectID;
}
