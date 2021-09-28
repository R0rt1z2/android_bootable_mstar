/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/

#ifndef __CMD_MS_Utility__
#define __CMD_MS_Utility__
#include <command.h>
#include <ShareType.h>

struct smc_param {
	U32 a0;
	U32 a1;
	U32 a2;
	U32 a3;
	U32 a4;
	U32 a5;
	U32 a6;
	U32 a7;
};

int do_mscompress7 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mdelay(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_showVersion(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

#if ((CONFIG_TV_CHIP == 1) || (ENABLE_RESERVED_CHUNK_HEADER == 1))
int do_showBoard(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_MMC)
int get_mmc_part_number(const char* partition, int *partNum);
#endif

//-------------------------------------------------------------------------------------------------
/// Check TEE binary before programming it
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_MSTAR_TOOL_CHECK_TEE)
int do_checkTEE(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_FAT==1))
int do_spi2usb ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
int do_usb2spi ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
#endif

#if ((ENABLE_MODULE_USB == 1)&&(ENABLE_MODULE_EEPROM==1))
int do_usb2eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
int do_eeprom2usb ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
int do_nandprogramforrom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_CMDLINE)
int do_skipnandprogramforrom ( cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[]);
#endif

#if (ENABLE_UTEST == 1)
int appInitUsbDisk_Single(char idx);
#endif

//-------------------------------------------------------------------------------------------------
/// Jump to uboot's console from anywhere
//-------------------------------------------------------------------------------------------------
void jump_to_console(void);


#if (ENABLE_MODULE_USB == 1)
//-------------------------------------------------------------------------------------------------
/// Init usb storage in application layer
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int appInitUsbDisk(void);
#endif

//-------------------------------------------------------------------------------------------------
/// get next line in script, and the input data will be modified in this function
/// @return  char*                            \b OUT: a string in the script
//-------------------------------------------------------------------------------------------------
char *get_script_next_line(char **line_buf_ptr);

//-------------------------------------------------------------------------------------------------
/// load entire script to return buffer.
/// @return  char*                            \b OUT: script content.
/// remember free the memory when use vfsfile_loadscript load some script
/// ex:  char *script = vfsfile_loadscript("/config/set_env");
///      free(script);
//-------------------------------------------------------------------------------------------------
char* loadscript(char *filedir,U32 *filesize);

//-------------------------------------------------------------------------------------------------
/// load entire script to "get next line in script"
/// @return  void                            \b OUT: NULL
//-------------------------------------------------------------------------------------------------
void runscript_linebyline(char *scriptdir);


//-------------------------------------------------------------------------------------------------
/// searc the specific pattern in a specific buffer
/// @param buf                                  \b IN: buffer addres
/// @param bus_size                           \b IN: The size of input buffer
/// @param pattern                             \b IN: pattern that you want to find out
/// @return char *                              \b OUT: NULL:FAIL, non-null:Sucess
//-------------------------------------------------------------------------------------------------
char *pattern_search(char *buf, unsigned int buf_size, char *pattern);

void tee_smc_call(struct smc_param *param);

// temp for usb secure upgrade used
void Reset_System(void);

#if defined(CONFIG_MCU_ARM)
void get_temp_used_addr(unsigned int *temp_addr);
#endif

//-------------------------------------------------------------------------------------------------
/// check if RPMB is programed
//-------------------------------------------------------------------------------------------------
int anti_rb_enabled(void);

//-------------------------------------------------------------------------------------------------
/// check if the device is in lockdown
//-------------------------------------------------------------------------------------------------
int is_lockdown(void);


//-------------------------------------------------------------------------------------------------
/// check if a command is supported in lockdown
//-------------------------------------------------------------------------------------------------
int chk_cmd_lockdown(const char* command);

#endif

