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

#include <common.h>
#include <command.h>
#include <MsApiMspi.h>
#include "drvMMIO.h"
#include "drvMSPI.h"


static MS_BOOL MSPI_Flash_Read_MX25L3236F(MS_U8* dataBuffer, MS_U32 flashAddr, MS_U32 readSize)
{
    unsigned char read_flash_command[4];
    MSPI_ErrorNo mspiErr;

    // command setup
    read_flash_command[0] = 0x03; // Read Data Bytes (READ) (Command 03h)
    read_flash_command[1] = (flashAddr>>16)&0xFF; // flash address from MSB
    read_flash_command[2] = (flashAddr>>8)&0xFF;
    read_flash_command[3] = (flashAddr>>0)&0xFF;

    MDrv_MSPI_SlaveEnable(TRUE); // CS# goes low
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(read_flash_command, sizeof(read_flash_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Read(dataBuffer, readSize)) )
    {
        printf("MDrv_MSPI_Read error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }
    MDrv_MSPI_SlaveEnable(FALSE); // CS# goes high
    return TRUE;
}

static MS_BOOL MSPI_Flash_EraseSector_MX25L3236F(MS_U32 flashAddr)
{
#define BIT_WEL_MX25L3236F (BIT(1))
#define BIT_WIP_MX25L3236F (BIT(0))
    unsigned char write_enable_command[] = {0x06}; // Write Enable (WREN) (Command 06h)
    unsigned char read_status_command[] = {0x05}; // Read Status Register (RDSR) (Command 05h)
    unsigned char erase_sector_command[4];
    unsigned char mspiStatus = 0;
    MSPI_ErrorNo mspiErr;

    erase_sector_command[0] = 0x20; // Sector Erase (SE) (Command 20h)
    erase_sector_command[1] = (flashAddr>>16)&0xFF; // flash address from MSB
    erase_sector_command[2] = (flashAddr>>8)&0xFF;
    erase_sector_command[3] = (flashAddr>>0)&0xFF;

    // write enable
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(write_enable_command, sizeof(write_enable_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }
    MDrv_MSPI_SlaveEnable(FALSE);

    // read status, check WEL is 1
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(read_status_command, sizeof(read_status_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    do {
        if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Read(&mspiStatus, sizeof(mspiStatus))) )
        {
            printf("MDrv_MSPI_Read error: 0x%X\n", (unsigned int)mspiErr);
            return FALSE;
        }
    } while( (mspiStatus & BIT_WEL_MX25L3236F) == 0 ) ;
    MDrv_MSPI_SlaveEnable(FALSE);

    // erase sector
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(erase_sector_command, sizeof(erase_sector_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }
    MDrv_MSPI_SlaveEnable(FALSE);

    // read status, check WIP is 0
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(read_status_command, sizeof(read_status_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    do {
        if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Read(&mspiStatus, sizeof(mspiStatus))) )
        {
            printf("MDrv_MSPI_Read error: 0x%X\n", (unsigned int)mspiErr);
            return FALSE;
        }
    } while( (mspiStatus & BIT_WIP_MX25L3236F) == BIT_WIP_MX25L3236F ) ;
    MDrv_MSPI_SlaveEnable(FALSE);

    return TRUE;
}

// erase operation is performed on each 4K-sector covered by the specified addresses
static MS_BOOL MSPI_Flash_Erase_MX25L3236F(MS_U32 flashStartAddr, MS_U32 flashEndAddr)
{
#define SECTOR_SIZE_MX25L3236F (0x1000)
    MS_U32 sectorStart = flashStartAddr / SECTOR_SIZE_MX25L3236F;
    MS_U32 sectorEnd = (flashEndAddr-1) / SECTOR_SIZE_MX25L3236F;
    MS_U32 sectorCount;

    for (sectorCount=sectorStart; sectorCount<=sectorEnd; sectorCount++)
    {
        if (FALSE == MSPI_Flash_EraseSector_MX25L3236F(sectorCount*SECTOR_SIZE_MX25L3236F))
        {
            printf("MSPI_Flash_EraseSector_MX25L3236F error\n");
            return FALSE;
        }
    }

    return TRUE;
}

static MS_BOOL MSPI_Flash_ProgramPage_MX25L3236F(MS_U8* dataBuffer, MS_U32 flashAddr, MS_U32 writeSize)
{
    unsigned char write_enable_command[] = {0x06}; // Write Enable (WREN) (Command 06h)
    unsigned char read_status_command[] = {0x05}; // Read Status Register (RDSR) (Command 05h)
    unsigned char page_program_command[4] = {0x02, 0x00, 0x00, 0x00};
    unsigned char mspiStatus = 0;
    MSPI_ErrorNo mspiErr;

    page_program_command[0] = 0x02; // Page Program (PP) (Command 02h)
    page_program_command[1] = (flashAddr>>16)&0xFF; // flash address from MSB
    page_program_command[2] = (flashAddr>>8)&0xFF;
    page_program_command[3] = (flashAddr>>0)&0xFF;

    // write enable
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(write_enable_command, sizeof(write_enable_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }
    MDrv_MSPI_SlaveEnable(FALSE);

    // read status, check WEL is 1
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(read_status_command, sizeof(read_status_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    do {
        if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Read(&mspiStatus, sizeof(mspiStatus))) )
        {
            printf("MDrv_MSPI_Read error: 0x%X\n", (unsigned int)mspiErr);
            return FALSE;
        }
    } while( (mspiStatus & BIT_WEL_MX25L3236F) == 0 ) ;
    MDrv_MSPI_SlaveEnable(FALSE);

    // page program
    MDrv_MSPI_SlaveEnable(TRUE);

    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(page_program_command, sizeof(page_program_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(dataBuffer, writeSize)) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    MDrv_MSPI_SlaveEnable(FALSE);

    // read status, check WIP is 0
    MDrv_MSPI_SlaveEnable(TRUE);
    if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Write(read_status_command, sizeof(read_status_command))) )
    {
        printf("MDrv_MSPI_Write error: 0x%X\n", (unsigned int)mspiErr);
        return FALSE;
    }

    do {
        if (E_MSPI_OK != (mspiErr = MDrv_MSPI_Read(&mspiStatus, sizeof(mspiStatus))) )
        {
            printf("MDrv_MSPI_Read error: 0x%X\n", (unsigned int)mspiErr);
            return FALSE;
        }
    } while( (mspiStatus & BIT_WIP_MX25L3236F) == BIT_WIP_MX25L3236F ) ;
    MDrv_MSPI_SlaveEnable(FALSE);

    return TRUE;
}

static MS_BOOL MSPI_Flash_Write_MX25L3236F(MS_U8* dataBuffer, MS_U32 flashAddr, MS_U32 writeSize)
{
#define PAGE_SIZE_MX25L3236F (0x100UL)
    MS_U32 firstPageSize = PAGE_SIZE_MX25L3236F - (flashAddr % PAGE_SIZE_MX25L3236F);
    MS_U32 programSize = writeSize > firstPageSize ? firstPageSize : writeSize;
    MS_U32 counter = 0;

    while (writeSize > 0)
    {
        if ( FALSE == MSPI_Flash_ProgramPage_MX25L3236F(dataBuffer + counter, flashAddr + counter, programSize) )
        {
            printf("MSPI_Flash_ProgramPage_MX25L3236F error\n");
            return FALSE;
        }

        writeSize -= programSize;
        counter += programSize;
        programSize = writeSize > PAGE_SIZE_MX25L3236F ? PAGE_SIZE_MX25L3236F : writeSize;
    }

    return TRUE;
}

static MS_BOOL MSPI_Flash_Init_MX25L3236F(void)
{
#if defined(CONFIG_MSTAR_BD_MST117B_10A_MOONEY) && defined(CONFIG_ENABLE_DEMURA)
    // For mooney 117B demura external flash padmux configuration
    *(unsigned volatile short*)(0x1F000000 + (0x101e<<9) + (0x4e<<2)) |= (1<<4); // 0x101e_h4e[4] = 1, reg_ld_spi3_config, select  use pad : PAD_GPIO3, PAD_GPIO5, PAD_GPIO4, PAD_GPIO2 for MSPI
#endif

    if (FALSE == MDrv_MMIO_Init())
    {
        printf("MDrv_MMIO_Init error\n");
        return FALSE;
    }

    if (E_MSPI_OK != MDrv_MSPI_Init_Ext(0)) // use MSPI0 to control GPIO2 ~ GPIO5
    {
        printf("MDrv_MSPI_Init_Ext error\n");
        return FALSE;
    }

    return TRUE;
}


// function pointers
static MS_BOOL (*mspi_flash_read)(MS_U8*, MS_U32, MS_U32) = 0;
static MS_BOOL (*mspi_flash_write)(MS_U8*, MS_U32, MS_U32) = 0;
static MS_BOOL (*mspi_flash_erase)(MS_U32, MS_U32) = 0;
static MS_BOOL (*mspi_flash_init)(void) = 0;


MS_BOOL MSPI_Flash_Read(MS_U8* dataBuffer, MS_U32 flashAddr, MS_U32 readSize)
{
    return mspi_flash_read(dataBuffer, flashAddr, readSize);
}


MS_BOOL MSPI_Flash_Erase(MS_U32 flashStartAddr, MS_U32 flashEndAddr)
{
    return mspi_flash_erase(flashStartAddr, flashEndAddr);
}


MS_BOOL MSPI_Flash_Write(MS_U8* dataBuffer, MS_U32 flashAddr, MS_U32 writeSize)
{
    return mspi_flash_write(dataBuffer, flashAddr, writeSize);
}

MS_BOOL MSPI_Flash_Init(char* flashName)
{
    if (strcmp(flashName, "MX25L3236F") == 0)
    {
        mspi_flash_init = MSPI_Flash_Init_MX25L3236F;
        mspi_flash_read = MSPI_Flash_Read_MX25L3236F;
        mspi_flash_write = MSPI_Flash_Write_MX25L3236F;
        mspi_flash_erase = MSPI_Flash_Erase_MX25L3236F;
    }
    else
    {
        printf("unknown flash name\n");
        return FALSE;
    }

    if (FALSE == mspi_flash_init())
    {
        printf("mspi_flash_init error\n");
        return FALSE;
    }

    return TRUE;
}


#define FLASH_TEST_ADDR (0x0)
#define FLASH_TEST_SIZE (0x80)
static unsigned char data_buffer[FLASH_TEST_SIZE] __attribute__((aligned(32)));

void MSPI_Flash_Test(void)
{
    // MSPI
    if (FALSE == MSPI_Flash_Init("MX25L3236F"))
    {
        printf("MSPI_Flash_Init failed\n");
    }

    // read flash
    {
        int i;
        if ( FALSE == MSPI_Flash_Read(data_buffer, FLASH_TEST_ADDR, FLASH_TEST_SIZE))
        {
            printf("MSPI_Flash_Read failed\n");
        }

        printf("data_buffer:");
        for (i=0; i<FLASH_TEST_SIZE; i++)
        {
            if ( (i%32) == 0) printf("\n");
            printf("%02X ", data_buffer[i]);
        }
        printf("\n\n");
    }

    // erase flash
    if ( FALSE == MSPI_Flash_Erase(FLASH_TEST_ADDR, FLASH_TEST_ADDR + FLASH_TEST_SIZE))
    {
        printf("MSPI_Flash_Erase failed\n");
    }

    // read flash
    {
        int i;
        if ( FALSE == MSPI_Flash_Read(data_buffer, FLASH_TEST_ADDR, FLASH_TEST_SIZE))
        {
            printf("MSPI_Flash_Read failed\n");
        }

        printf("data_buffer:");
        for (i=0; i<FLASH_TEST_SIZE; i++)
        {
            if ( (i%32) == 0) printf("\n");
            printf("%02X ", data_buffer[i]);
        }
        printf("\n\n");
    }

    // write flash
    {
        char input_data[FLASH_TEST_SIZE];
        int i;
        for (i=0; i<FLASH_TEST_SIZE; i++) input_data[i] = i%32;

        if (FALSE == MSPI_Flash_Write(input_data, FLASH_TEST_ADDR, FLASH_TEST_SIZE) )
        {
            printf("MSPI_Flash_Write failed\n");
        }
    }

    // read flash
    {
        int i;
        if ( FALSE == MSPI_Flash_Read(data_buffer, FLASH_TEST_ADDR, FLASH_TEST_SIZE))
        {
            printf("MSPI_Flash_Read failed\n");
        }

        printf("data_buffer:");
        for (i=0; i<FLASH_TEST_SIZE; i++)
        {
            if ( (i%32) == 0) printf("\n");
            printf("%02X ", data_buffer[i]);
        }
        printf("\n\n");
    }
}
