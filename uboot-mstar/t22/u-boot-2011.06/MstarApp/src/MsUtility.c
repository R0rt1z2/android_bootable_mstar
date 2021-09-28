/*
*===================================================================================================================================================================================================
** Copyright (c) 2006-2011 MStar Semiconductor, Inc.
*This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
*This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*===================================================================================================================================================================================================
*/

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <exports.h>
#include <lzmadec.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <MsDebug.h>
#include <MsUtility.h>
#include <MsOS.h>
#include <MsSystem.h>
#include <version.h>
#include <MsMmc.h>
#include <MsRawIO.h>
#include <mmc.h>

#if (ENABLE_MODULE_USB == 1)
#include <usb.h>
#endif

#include <amzn_unlock.h>

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
#include <drvNAND_uboot.h>
#endif
#if defined(CONFIG_SECARM_OPTEE)
#include <mstar_mbx_head_general.h>
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SCRIPT_FILE_COMMENT         '#'                 // symbol for comment
#define SCRIPT_FILE_END             '%'                 // symbol for file end
#define IS_COMMENT(x)       (SCRIPT_FILE_COMMENT == (x))
#define IS_FILE_END(x)      (SCRIPT_FILE_END == (x))
#define IS_LINE_END(x)      ('\r' == (x)|| '\n' == (x))
#define IS_IGNORED_CHAR(x)  ('\r' == (x)|| '\n' == (x) || '\t' == (x) || ' ' == (x))

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------

#if defined(CONFIG_MMC)
int get_mmc_part_number(const char* partition, int *partNum)
{
    block_dev_desc_t *mmc_dev;
    struct mmc *mmc = NULL;
    u32 curr_partnum = 1;
    int curr_device = 0;
    disk_partition_t dpt;

    mmc = find_mmc_device(curr_device);

    if (!mmc) {
        printf("no mmc device at slot %x\n", curr_device);
        return -1;
    }

    if(!mmc->has_init){
        printf("Do mmc init first!\n");
        mmc_init(mmc);
        mmc->has_init = 1;
    }

    mmc_dev = mmc_get_dev(curr_device);
    if ((mmc_dev == NULL) ||
            (mmc_dev->type == DEV_TYPE_UNKNOWN))
    {
        printf("no mmc device found!\n");
        return -1;
    }

    for(;;)
    {
        if(get_partition_info_emmc(mmc_dev, curr_partnum, &dpt))
            break;
        if(!strcmp(partition, (const char *)dpt.name)){
            *partNum = curr_partnum;
            return 0;
        }
        curr_partnum++;
    }
    return -1;
}
#endif // #if defined(CONFIG_MMC)

#if defined(CONFIG_MCU_ARM)
// MAX_TEMP_DEFINED_SIZE is 200MB, MAX_TEMP_USED_SIZE needs to less than MAX_TEMP_DEFINED_SIZE
// We use MAX_TEMP_DEFINED_SIZE to get download address
// and use MAX_TEMP_USED_SIZE to control transmission size
#define MAX_TEMP_DEFINED_SIZE  (200*1024*1024)  // 200M
#define MAX_TEMP_USED_SIZE (100*1024*1024)  // 100M
#if defined(CONFIG_ANDROID_BOOT)
void get_temp_used_addr(unsigned int *temp_addr)
{
    // Just for initialize, these values may be change after calculate
    U32 download_start_address = CONFIG_SYS_MIU0_BUS + 0x200000;
    U32 download_end_address = CONFIG_SYS_MIU0_BUS + 0x200000;

    const U32 MIU_OFFSET = CONFIG_SYS_MIU0_BUS;
    U32 mboot_address = (CONFIG_DRAM_SIZE_MB_RELOCATE*1024*1024) + MIU_OFFSET;

    // check if the mboot address < 256MB, ex. 137MB
    // use 256MB~512MB
    if( mboot_address < (MAX_TEMP_DEFINED_SIZE + MIU_OFFSET) )
    {
        download_start_address = 0x10000000+MIU_OFFSET;
        download_end_address = download_start_address + MAX_TEMP_USED_SIZE - 1;
    }
    else
    {
        // if the mboot address > 256MB, ex. 305MB
        // use 28MB~256MB (reserve 0x20280000 + 20MB for booting kernel)
        download_start_address = 0x1C00000+MIU_OFFSET;
        download_end_address = download_start_address + MAX_TEMP_USED_SIZE - 1;
    }

    // set address into global download_addr var.
    //*temp_addr = download_start_address;
    // temp to use BOOT_MODE_TMP_BUF_ADDR until mmap setting is ready
    *temp_addr = BOOT_MODE_TMP_BUF_ADDR;
}
#else
void get_temp_used_addr(unsigned int *temp_addr)
{
    char* env_value = NULL;
    const U32 MIU_OFFSET = CONFIG_SYS_MIU0_BUS;

    // check E_LX_MEM2_ADR
    env_value = getenv("E_LX_MEM2_ADR");
    if(env_value != NULL)
    {
        *temp_addr = CONFIG_SYS_MIU0_BUS + simple_strtoul(env_value, NULL, 16);
        return;
    }

    // if uboot has no E_LX_MEM2_ADR, try MI_KERNEL_POOL2_ADR
    env_value = getenv("MI_KERNEL_POOL2_ADR");
    if(env_value != NULL)
    {
        *temp_addr = CONFIG_SYS_MIU0_BUS + simple_strtoul(env_value, NULL, 16);
        return;
    }

    // if both E_LX_MEM2_ADR and MI_KERNEL_POOL2_ADR do not exist, use default address
    *temp_addr = BOOT_MODE_TMP_BUF_ADDR;
}
#endif // defined(CONFIG_ANDROID_BOOT)
#endif // defined(CONFIG_MCU_ARM)

void *SzAlloc(void *p, size_t size) { p = p; return malloc(size);}
void SzFree(void *p, void *address) { p = p; free(address); }
ISzAlloc g_Alloc = { SzAlloc, SzFree };

int do_mdelay(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
  int delaytime = 0;
  delaytime = simple_strtol(argv[1],NULL,10);
  MsOS_DelayTask(delaytime);
  return 0;
}

U8 MsDecompress7(U8 *pSrc, U8 *pDst, U32 srclen)
{
    U32 OriginalFileLength = 0;
    int result = SZ_OK;
    U8 i;
    ELzmaStatus estatus = NULL;
    SizeT destLen, srcLen;
    char buf [12], filesize[12];

    #define LZMA_PROPS_SIZE 5

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    srclen -= (LZMA_PROPS_SIZE + 8);

    // It seems that in our system, we never have a chance to have a compressed data which needs to use U64 integer to store its file size
    // So we use just U32 here to instead of U64
    /* Read and parse header */
    for (i = 0; i < 8; i++)
        OriginalFileLength += ( (U32)pSrc[LZMA_PROPS_SIZE + i] << (i << 3) );

    destLen = OriginalFileLength;
    srcLen = srclen;

    result = LzmaDecode(pDst, &destLen, pSrc + (LZMA_PROPS_SIZE + 8), &srcLen, pSrc, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &estatus, &g_Alloc);

    switch( estatus )
    {
    case LZMA_STATUS_FINISHED_WITH_MARK:
    case LZMA_STATUS_NOT_FINISHED:
    case LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK:
        break;
    default:
    //LZMA_STATUS_NOT_SPECIFIED,               /* use main error code instead */
    //LZMA_STATUS_NEEDS_MORE_INPUT,            /* you must provide more input bytes */
        //printf("ELzmaStatus = %d\n", estatus);
        //Decompression fail!
        return FALSE;
    }

    if (result != SZ_OK)
    {
    //  SZ_ERROR_DATA - Data error
    //  SZ_ERROR_MEM  - Memory allocation error
    //  SZ_ERROR_UNSUPPORTED - Unsupported properties
    //  SZ_ERROR_INPUT_EOF - It needs more bytes in input buffer (src).
    //#define SZ_ERROR_DATA 1
    //#define SZ_ERROR_MEM 2
    //#define SZ_ERROR_CRC 3
    //#define SZ_ERROR_UNSUPPORTED 4
    //#define SZ_ERROR_PARAM 5
    //#define SZ_ERROR_INPUT_EOF 6
    //#define SZ_ERROR_OUTPUT_EOF 7
    //#define SZ_ERROR_READ 8
    //#define SZ_ERROR_WRITE 9
    //#define SZ_ERROR_PROGRESS 10
    //#define SZ_ERROR_FAIL 11
    //#define SZ_ERROR_THREAD 12
    //#define SZ_ERROR_ARCHIVE 16
    //#define SZ_ERROR_NO_ARCHIVE 17
        //printf("error code:%d\n", result);
        //Decompression fail!
        return FALSE;
    }

    if ( destLen != OriginalFileLength )
    {
        //Decompression fail!
        return FALSE;
    }

    if ( srcLen != srclen )
    {
        //Decompression fail!
// Skip this check temporarily      return FALSE;
    }

    memset(buf, 0 , sizeof(buf));
    memset(filesize, 0 , sizeof(filesize));
    snprintf(buf, sizeof(buf), "%lX", destLen);
    strcpy(filesize, "filesize");
    setenv(filesize, buf);

#if((CONFIG_TV_CHIP == 0) && (CONFIG_MIPS32 == 1))
    // flush cache after read
    flush_cache((ulong)pDst, destLen);

    //Decompression OK!
    printf("Decompression OK!\n");
#endif
    return TRUE;
}


int do_mscompress7 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int i;
    long AddrSrc=0, LengthSrc=0, AddrDst=0;

    if (argc < 6) {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    for (i=1; i<argc; ++i) {
        switch(i)
        {
            case 1:
                if(argv[i][0] != 'd')
                {
                    return 1;
                }
                break;
            case 2:
                if(simple_strtol(argv[i], NULL, 10) != 0)
                {
                    return 1;
                }
                break;
            case 3:
                AddrSrc = simple_strtol(argv[i], NULL, 16);
                break;
            case 4:
                LengthSrc = simple_strtol(argv[i], NULL, 16);
                break;
            case 5:
                AddrDst = simple_strtol(argv[i], NULL, 16);
                break;
            default:
                break;
        }
    }
    if(TRUE != MsDecompress7((U8*)AddrSrc, (U8*)AddrDst, LengthSrc))
    {
        printf("%s: MsDecompress7() failed, at %d\n", __func__, __LINE__);
        return 1;
    }
    return 0;
}

#if (ENABLE_MINIUBOOT == 0)
int do_showVersion(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    printf("MstarCore:%s\nMstarApp:%s\nMstarCustomer:%s\nTool:%s\nUboot:%s\nSboot:%s\n",
		MSTARCORECOMMIT,MSTARAPPCOMMIT,MSTARCUSTOMERCOMMIT,TOOLCOMMIT,UBOOTCOMMIT,SBOOTCOMMIT);
    return 0;
}

#if ((CONFIG_TV_CHIP == 1) || (ENABLE_RESERVED_CHUNK_HEADER == 1))
int do_showBoard(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buf[1024] = {0,};

    memset(buf, 0, sizeof(buf));
    if (MsApi_GetBoardInfo(buf, sizeof(buf)) != TRUE)
    {
         UBOOT_ERROR("MsApi_GetBoardInfo error\n");
         return -1;
    }
    buf[sizeof(buf)-1] = '\0';

    printf("==================\n");
    printf("%s", buf);
    return 0;
}
#endif
#endif
#if defined(CONFIG_MSTAR_TOOL_CHECK_TEE)
int do_checkTEE(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_TEE_CHIP_ADDR
    uint32_t u32TEE_MagicNumber[CONFIG_TEE_MAGIC_LEN] = {0x4B22DB99, 0x354E582B, 0x42FDB1A0, 0x4F97B547};
    uint32_t u32BINAddr = simple_strtoul(argv[1], NULL, 16);
    uint32_t u32TEE_BIN_MagicNumber = 0;
    uint16_t u16Value               = 0;
    uint8_t  u8Index                = 0;
    uint8_t  u8GetChar              = 0;

    u16Value  = *(volatile unsigned short*) (CONFIG_TEE_CHIP_ADDR);
    u16Value &= 0xF00; // BIT8~BIT11
    u16Value  = u16Value >> 8;

    //read bfe_boot.bin tee magic number
    for(u8Index = 0; u8Index < CONFIG_TEE_MAGIC_LEN; u8Index++)
    {
        u32TEE_BIN_MagicNumber  = *((volatile unsigned int*) (u32BINAddr + CONFIG_TEE_MAIGC_OFFSET) + u8Index);

        if(u32TEE_BIN_MagicNumber != u32TEE_MagicNumber[u8Index])
        {
            printf("\nDetected Non-TEE binary\n");
            return 0;
        }
    }

    printf("\nDetected TEE binary\n");

    if(u16Value == 0)
    {
        printf("\033[0;31mWarning! The chip will be programmed from non-TEE to TEE!\033[0m\n");
        printf("\033[0;31mWarning! This is unrecoverable!\033[0m\n");

        while(1)
        {
            printf("\nDo you really want to program it?\n");
            printf("y: to continue program \n");
            printf("n: will reset to avoid following accidentally programming commands (y/n):");
            u8GetChar = getc();

            if (u8GetChar == 'y' || u8GetChar == 'Y')
            {
                return 0;
            }
            else if (u8GetChar == 'n' || u8GetChar == 'N')
            {
                run_command("reset", 0);
                return -1;
            }
        }
    }

#endif
    return 0;
}
#endif

extern int readline (const char *const prompt);
extern char console_buffer[CONFIG_SYS_CBSIZE + 1];
void jump_to_console(void)
{

    static char lastcommand[CONFIG_SYS_CBSIZE +1] = { 0, };//Fix Coverity CID:148928
    int len;
    int rc = 1;
    int aflag;
    char *pEnv=NULL;
    MS_U32 u32WDTisEnable =0;

    pEnv=getenv("WDT_ENABLE");
    if(pEnv)
    {
        u32WDTisEnable=simple_strtoul(pEnv, 0, 16);
        if(u32WDTisEnable==1)
        {
            printf("MBoot will reset within 10s, if you don't want to reset, please run wdt_enable 0!!\n");
            run_command("wdt_enable 10",0);
         }
    }
    else
    {
        run_command("wdt_enable 0",0);
    }
    // for case : close console log
    open_console_log();
    printf("jump_to_console start!!\n");
    for (;;)
    {
        len = readline (CONFIG_SYS_PROMPT);

        aflag = 0;   /* assume no special flags for now */
        if (len > 0)
            strcpy (lastcommand, console_buffer);
        else if (len == 0)
            aflag |= CMD_FLAG_REPEAT;

        if (len == -1)
            puts ("<INTERRUPT>\n");
        else if (!chk_cmd_lockdown(lastcommand))
        {
            puts ("command not supported in lockdown\n");
            rc = 0;
        }
        else
            rc = run_command (lastcommand, aflag);

        if (rc <= 0) {
            /* invalid command or not repeatable, forget it */
            lastcommand[0] = 0;
        }
    }
     printf("jump_to_console end!!\n");
}

#if (ENABLE_MODULE_USB == 1)
int appInitUsbDisk(void)
{
    char idx=0;

#if   defined(ENABLE_FIFTH_EHC)
    const char u8UsbPortCount = 5;
#elif defined(ENABLE_FOURTH_EHC)
    const char u8UsbPortCount = 4;
#elif defined(ENABLE_THIRD_EHC)
    const char u8UsbPortCount = 3;
#elif defined(ENABLE_SECOND_EHC)
    const char u8UsbPortCount = 2;
#else
    const char u8UsbPortCount = 1;
#endif
    UBOOT_TRACE("IN\n");

    for(idx=0; idx<u8UsbPortCount; idx++)
    {
#if defined (CONFIG_USB_PREINIT)
        usb_stop(idx);
        if (usb_post_init(idx) == 0)
#else
        if (usb_init(idx) == 0)
#endif
        {
            if (usb_stor_scan(1) == 0)
            {
                UBOOT_TRACE("OK\n");
                return 0 ;
            }
        }
    }
    UBOOT_TRACE("OK\n");
    return -1;
}

#if (ENABLE_UTEST == 1)
int appInitUsbDisk_Single(char idx)
{
    UBOOT_TRACE("IN\n");

#if defined (CONFIG_USB_PREINIT)
    usb_stop(idx);
    if (usb_post_init(idx) == 0)
#else
    if (usb_init(idx) == 0)
#endif
    {
        if (usb_stor_scan(1) == 0)
        {
            UBOOT_TRACE("OK\n");
            return 0 ;
        }
    }
    UBOOT_TRACE("OK\n");
    return -1;
}
#endif

#endif



#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_FAT==1))
int do_spi2usb ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    ulong u32SPIAddr=0;
    ulong u32Len=0;
    char *fileName;
    int ret=0;
    char buffer[CMD_BUF] = "\0";
    U8 *temp=NULL;

    UBOOT_TRACE("IN\n");
    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(appInitUsbDisk()!=0)
    {
        UBOOT_ERROR("no usb disk plug-in \n");
        return -1;
    }


    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"spi init");
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("spi init fail\n");
        free(temp);
        return -1;
    }


    u32SPIAddr= simple_strtol(argv[1], NULL, 16);
    u32Len= simple_strtol(argv[2], NULL, 16);
    fileName=argv[3];

    temp=(U8 *)malloc(u32Len);
    if(temp==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(temp,0,u32Len);
    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"spi rdc %x %x %x",(unsigned int)temp,(unsigned int)u32SPIAddr,(unsigned int)(u32Len));
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("spi2usb fail\n");
        free(temp);
        return -1;
    }

    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"fatwrite usb 0 0x%x %s %x",(unsigned int)temp,fileName,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("spi2usb fail\n");
        free(temp);
        return -1;
    }
    free(temp);

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_usb2spi ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32 u32SPIAddr=0;
    MS_U32 u32Len=0;
    char *fileName;
    int ret=0;
    char buffer[CMD_BUF] = "\0";
    MS_U8 *temp=NULL;
    char *pfileSize=NULL;
    MS_U32 u32FileSize=0;

    UBOOT_TRACE("IN\n");

    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(appInitUsbDisk()!=0)
    {
        UBOOT_ERROR("no usb disk plug-in \n");
        return -1;
    }


    u32SPIAddr= simple_strtol(argv[1], NULL, 16);
    u32Len= simple_strtol(argv[2], NULL, 16);
    fileName=argv[3];

    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"fatfilesize usb 0 %s",fileName);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("get file size fail\n");
        return -1;
    }

    pfileSize=getenv("filesize");
    if(pfileSize==NULL)
    {
        UBOOT_ERROR("get file size fail\n");
        return -1;
    }
    u32FileSize=simple_strtoul(pfileSize, 0, 16);

    if(u32FileSize<u32Len)
    {
        UBOOT_ERROR("The file size is smaller than input length, file size=0x%x(bytes), lenght=0x%x(bytes)\n",(unsigned int)u32FileSize,(unsigned int)u32Len);
        return -1;
    }

    temp=(MS_U8 *)malloc(u32Len);
    if(temp==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }

    memset(temp,0,u32Len);
    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"fatload usb 0 0x%x %s %x",(unsigned int)temp,fileName,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("fatload usb fail\n");
        free(temp);
        return -1;
    }

    memset(buffer, 0, CMD_BUF);
    snprintf(buffer,CMD_BUF,"spi wrc %x %x %x",(unsigned int)temp,u32SPIAddr,(unsigned int)(u32Len));
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer,0);
    if(ret==-1)
    {
        UBOOT_ERROR("spi wrc fail \n");
        free(temp);
        return -1;
    }

    free(temp);

    UBOOT_TRACE("OK\n");
    return 0;
}

#endif

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_EEPROM==1))
int do_eeprom2usb ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    ulong u32Off=0;
    ulong u32Len=0;
    char *fileName;
    int ret=0;
    char cmd[CMD_BUF];
    U8 *temp=NULL;
    UBOOT_TRACE("IN\n");
    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(appInitUsbDisk()!=0)
    {
        UBOOT_ERROR("no usb disk plug-in \n");
        return -1;
    }


    u32Off= simple_strtol(argv[1], NULL, 16);
    u32Len= simple_strtol(argv[2], NULL, 16);
    fileName=argv[3];



    temp=(U8 *)malloc(u32Len);
    if(temp==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(temp,0,u32Len);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd,sizeof(cmd),"eeprom read %x %x %x",(unsigned int)temp,(unsigned int)u32Off,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    ret=run_command(cmd,0);
    if(ret!=0)
    {
        UBOOT_ERROR("read from eeprom fail\n");
        free(temp);
        return -1;
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd,sizeof(cmd),"fatwrite usb 0 0x%x %s %x",(unsigned int)temp,fileName,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    ret=run_command(cmd,0);
    if(ret!=0)
    {
        UBOOT_ERROR("write to usb disk fail\n");
        free(temp);
        return -1;
    }
    free(temp);
    UBOOT_TRACE("OK\n");
    return 0;
}


int do_usb2eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    ulong u32Off=0;
    ulong u32Len=0;
    char *fileName;
    int ret=0;
    char cmd[CMD_BUF];
    U8 *temp=NULL;
    UBOOT_TRACE("IN\n");

    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(appInitUsbDisk()!=0)
    {
        UBOOT_ERROR("no usb disk plug-in \n");
        return -1;
    }


    u32Off= simple_strtol(argv[1], NULL, 16);
    u32Len= simple_strtol(argv[2], NULL, 16);
    fileName=argv[3];

    temp=(U8 *)malloc(u32Len);
    if(temp==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }



    memset(temp,0,u32Len);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd,sizeof(cmd),"fatload usb 0 0x%x %s %x",(unsigned int)temp,fileName,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    ret=run_command(cmd,0);
    if(ret!=0)
    {
        UBOOT_ERROR("read from usb disk fail\n");
        free(temp);
        return -1;
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd,sizeof(cmd),"eeprom write %x %x %x",(unsigned int)temp,(unsigned int)u32Off,(unsigned int)u32Len);
    UBOOT_DEBUG("cmd=%s\n",cmd);
    ret=run_command(cmd,0);
    if(ret!=0)
    {
        UBOOT_ERROR("write to eeprom fail\n");
        free(temp);
        return -1;
    }
    free(temp);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

char *get_script_next_line(char **line_buf_ptr)
{
    char *line_buf;
    char *next_line;
    int i;
    UBOOT_TRACE("IN\n");

    line_buf = (*line_buf_ptr);

    // strip '\r', '\n' and comment
    while (1)
    {
        // strip '\r' & '\n' & ' ' & '\t'
        if (IS_IGNORED_CHAR(line_buf[0]))
        {
            line_buf++;
        }
        // strip comment
        else if (IS_COMMENT(line_buf[0]))
        {
            for (i = 0; !IS_LINE_END(line_buf[0]) && i <= MAX_LINE_SIZE; i++)
            {
                line_buf++;
            }

            if (i > MAX_LINE_SIZE)
            {
                line_buf[0] = SCRIPT_FILE_END;

                UBOOT_ERROR("Error: the max size of one line is %d!!!\n", MAX_LINE_SIZE); // <-@@@

                break;
            }
        }
        else
        {
            break;
        }
    }

    // get next line
    if (IS_FILE_END(line_buf[0]))
    {
        next_line = NULL;
    }
    else
    {
        next_line = line_buf;

        for (i = 0; !IS_LINE_END(line_buf[0]) && i <= MAX_LINE_SIZE; i++)
        {
            line_buf++;
        }

        if (i > MAX_LINE_SIZE)
        {
            next_line = NULL;
            UBOOT_ERROR("Error: the max size of one line is %d!!!\n", MAX_LINE_SIZE); // <-@@@
        }
        else
        {
            line_buf[0] = '\0';
            *line_buf_ptr = line_buf + 1;
        }
    }
    UBOOT_TRACE("OK\n");

    return next_line;
}

char* loadscript(char *filedir,U32 *filesize)
{
    char *script = NULL;
    UBOOT_TRACE("IN\n");

    *filesize = vfs_getsize(filedir);
    if(*filesize == 0)
    {
        UBOOT_ERROR("No enough buffer or path fail(%s) :\n[%s] [%s]\n",filedir,__FILE__,__FUNCTION__);
        return NULL;
    }

    script = (char *)malloc(*filesize);
    if(script == NULL)
    {
        UBOOT_ERROR("No enough buffer %s %s\n",__FILE__,__FUNCTION__);
        return NULL;
    }
    if(vfs_read((void *)script,filedir,0,(unsigned int)(*filesize))==-1)
    {
        free(script);
        UBOOT_ERROR("Error: vfs_read Fail \n");
        return NULL;
    }
    UBOOT_TRACE("OK\n");
    return script;
}

void runscript_linebyline(char *scriptdir)
{
    char *script = NULL;
    char *TmpScrtip = NULL;
    char *next_line;
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
    script = loadscript(scriptdir,&filesize);
    TmpScrtip = script;
    if(script != NULL)
    {
        while ((next_line = get_script_next_line(&script)) != NULL)
        {
            UBOOT_INFO("\n>> %s \n", next_line);
            run_command(next_line, 0);
        }
    }
    if(TmpScrtip != NULL)
    {
        free(TmpScrtip);
    }
    UBOOT_TRACE("OK\n");
}

char *pattern_search(char *buf, unsigned int buf_size, char *pattern)
{
    char *ptr=NULL;
    UBOOT_TRACE("IN\n");
    if((buf==NULL)||(pattern==NULL))
    {
        UBOOT_ERROR("One of the parameters is null pointer\n");
        return NULL;
    }
    if(buf_size==0)
    {
        UBOOT_ERROR("The buf_size is 0\n");
        return NULL;
    }

    ptr=buf;
    UBOOT_DEBUG("target=%s,len=0x%x\n",pattern,strlen(pattern));
    while(buf_size)
    {
        if(memcmp(ptr,pattern,strlen(pattern))==0)
        {
            break;
        }
        ptr++;

        if(buf_size==strlen(pattern))
        {
            buf_size=0;
        }
        else
        {
            buf_size--;
        }
    }
    if(buf_size==0)
    {
        UBOOT_DEBUG("Can't find the %s \n",pattern);
        return NULL;
    }
    else
    {
        UBOOT_TRACE("OK\n");
        return ptr;
    }
}

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
int do_nandprogramforrom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    char ciscmd[128];
    int filelength = 0;
    U32 dbgAddress=0;
    //console_disable();
    //write register status Begin

    #if defined(CONFIG_ARM)
    (*(volatile unsigned short*)(0x1F206700)) = 0x2454;
    (*(volatile unsigned short*)(0x1F206704)) = 0x3697;
    (*(volatile unsigned short*)(0x1F206700)) = 0x0000;
    (*(volatile unsigned short*)(0x1F206704)) = 0x0000;
    dbgAddress=0x1F002140;
    #elif defined(CONFIG_MIPS32)
    (*(volatile unsigned short*)(0xBF206700)) = 0x2454;
    (*(volatile unsigned short*)(0xBF206704)) = 0x3697;
    (*(volatile unsigned short*)(0xBF206700)) = 0x0000;
    (*(volatile unsigned short*)(0xBF206704)) = 0x0000;
    dbgAddress=0xBF206720;
    #endif

    (*(volatile unsigned short*)(dbgAddress)) = 0x003A;// jh.huang

    #if defined(CONFIG_ENV_IS_IN_NANDUBI)
    run_command("nand erase.force" , 0);
    #else
    run_command("nand erase.chip" , 0);
    #endif
    //get file length for register
    (*(volatile unsigned short*)(dbgAddress)) = 0x003b;// jh.huang

    #if defined(CONFIG_ARM)
    filelength = *(volatile unsigned short*)(0x1F206710);
    filelength |= (*(volatile unsigned short*)(0x1F20670C)) << 16;
    printf("file length = %d\n", filelength);
    filelength -= 65536*3;
    #ifndef HASH0_SIZE
    sprintf(ciscmd,"ncisbl 0x24000000 0x24010000 0x24020000 0x24030000 0x%X", filelength);
    #else
    filelength -= HASH0_SIZE;
    sprintf(ciscmd,"ncishash 0x24000000 0x24010000 0x24020000 0x24030000 0x%X 0x%X 0x%X", HASH0_SIZE, (0x24030000+HASH0_SIZE), filelength);
    #endif
    #elif defined(CONFIG_MIPS32)
    filelength = *(volatile unsigned short*)(0xBF206710);
    filelength |= (*(volatile unsigned short*)(0xBF20670C)) << 16;
    printf("file length = %d\n", filelength);
    filelength -= 65536*3;
    #ifndef HASH0_SIZE
    sprintf(ciscmd,"ncisbl 0x82000000 0x82010000 0x82020000 0x82030000 0x%X", filelength);
    #else
    filelength -= HASH0_SIZE;
    sprintf(ciscmd,"ncishash 0x84000000 0x84010000 0x84020000 0x84030000 0x%X 0x%X 0x%X", HASH0_SIZE, (0x84030000+HASH0_SIZE), filelength);
    #endif
    #endif
    (*(volatile unsigned short*)(dbgAddress)) = 0x003c;// jh.huang
    printf("CIS Cmd = %s\n", ciscmd);
    (*(volatile unsigned short*)(dbgAddress)) = 0x003d;// jh.huang
    run_command(ciscmd, 0);
    (*(volatile unsigned short*)(dbgAddress)) = 0x003f;// jh.huang
    printf("done\n");
    #if defined(CONFIG_ARM)
    while((*(volatile unsigned short*)(0x1F206700)) != 0x0000);
    #elif defined(CONFIG_MIPS32)
    while((*(volatile unsigned short*)(0xBF206700)) != 0x0000);
    #endif
    (*(volatile unsigned short*)(dbgAddress)) = 0x0040;// jh.huang
    run_command("reset", 0);
    return 0;
}
#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_CMDLINE)
int do_skipnandprogramforrom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    console_disable();

    run_command("setprogramok", 0);
    udelay(5000000);

    console_init_r();
    return 0;
}
#endif

void tee_smc_call(struct smc_param *param)
{
#if defined(CONFIG_SECARM_OPTEE)
	struct thread_smc_args args;

	args.a0 = param->a0;
	args.a1 = param->a1;
	args.a2 = param->a2;
	args.a3 = param->a3;
	args.a4 = param->a4;
	args.a5 = param->a5;
	args.a6 = param->a6;
	args.a7 = param->a7;
	send_mbx(&args, CMD_R2T_GENERAL, 0);
#elif defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_OPTEE_TEELOADER)
	__asm__ volatile(
		"mov	r8, r0\n\t"
		"ldm	r8, {r0-r7}\n\t"
		"smc #0\n\t"
		"stm	r8, {r0-r7}\n\t"
		:
		:"r"(param)
		:"r0","r1","r2","r3","r4","r5","r6","r7","r8","memory"
	);
#endif
}

void Reset_System()
{
    // clean up the upgrade releated env
    run_command("setenv upgrade_mode", 0);
    run_command("setenv MstarUpgrade_complete 0", 0);
    run_command("setenv upgrade_status", 0);
    run_command("setenv force_upgrade", 0);
    saveenv();
    MsSystemReset();
}

#define ANTIROLLBACK_REG_ADDR (0x1f000c08)
#define ANTIROLLBACK_NOT_ENABLED_MAGIC (0xEBAB)
int anti_rb_enabled()
{
   // the dummy register will be set to the magic number in sboot
   // only when anti-rollback is not enabled.
   if (*(volatile unsigned short *)ANTIROLLBACK_REG_ADDR == ANTIROLLBACK_NOT_ENABLED_MAGIC)
      return 0;
   else
      return 1;
}

int is_lockdown()
{
    char* __bootmodestr = getenv ("bootmode");
    int ibootmode = atoi(__bootmodestr);
#if UFBL_FEATURE_SECURE_BOOT
   /* locked production device in normal mode */
   if ( (target_is_production()) &&
        (!amzn_device_is_unlocked()) &&
        (ibootmode == 1) )
      return 1;
   else
      return 0;
 #else
      return 0;
 #endif
}

int chk_cmd_lockdown(const char* command)
{
   if ( (is_lockdown()) &&
        (strncmp("fastboot", command, strlen("fastboot"))) &&
        (strncmp("reset", command, strlen("reset"))) &&
        (strncmp("usb", command, strlen("usb"))) &&
        (strncmp("estar", command, strlen("estar"))) &&
        (strncmp("onetimeunlock", command, strlen("onetimeunlock"))) )
      return 0;
   else
      return 1;
}
