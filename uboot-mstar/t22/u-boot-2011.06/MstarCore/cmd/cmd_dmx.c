/*
 * (C) Copyright 2002
 * Detlev Zundel, DENX Software Engineering, dzu@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * DMX handling routines
 */

#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <drvMMIO.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <apiDMX.h>

#if 1//(CONFIG_COMMANDS & CFG_CMD_DMX)
#if (CONFIG_BOOTVIDEO == 1)
#include <demux/MsDrvDemux.h>
#else
#define TSP_FW_SIZE 0x4000
static MS_U8 u8TSFwBuf[TSP_FW_SIZE + 128];

#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KENYA)||defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KAYLA) ||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_K5TN)||defined(CONFIG_K1C)||defined(CONFIG_K5AP)
    // set VQ for TSP, set FW/VQ for TSP2
#define TSP_VQ_SIZE 0x1900
static MS_U8 u8TSVQBuf[TSP_FW_SIZE + 16];
#endif
MS_U8 u8TSPFirmware[] = {
    #include <fwTSP.dat>
};
#endif

/*
 * Subroutine:  do_tuner
 *
 * Description: Handler for 'tuner' command..
 *
 * Inputs:	argv[1] contains the subcommand
 *
 * Return:      None
 *
 */
int do_dmx_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_PHYADDR      phyFW;
    MS_U8*          pu8FW;
    DMX_FLOW_INPUT inSrc;
    if (argc<1)
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    inSrc = (DMX_FLOW_INPUT)simple_strtoul(argv[1], NULL, 10);
#if (CONFIG_BOOTVIDEO == 1)
    MsDrv_DmxInit(inSrc);
#else
    MDrv_MMIO_Init(); // must init MMI before init DMX
#if defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KAYLA)||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_K5TN)||defined(CONFIG_K1C)||defined(CONFIG_K5AP)
    pu8FW = (MS_U8*)MsOS_PA2KSEG1((MS_U32)0x2000000);
#else
    pu8FW = (MS_U8*)(((int)u8TSFwBuf + 0x7F) & (~0x7F));
    pu8FW = (MS_U8*)MsOS_VA2PA((MS_U32)pu8FW);
    pu8FW = (MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8FW);
#endif
    memcpy(pu8FW, u8TSPFirmware, sizeof(u8TSPFirmware));
    MsOS_FlushMemory();

    phyFW = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8FW);
    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_SetFW(phyFW, sizeof(u8TSPFirmware)))
    {
        printf("DeMux>>ERROR! SetFW fail\n");
        return -1;
    }
    MApi_DMX_SetHK(TRUE);
    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Init())
    {
        printf("DeMux>>ERROR! TSP init fail\n");
        return -1;
    }

#if defined(CONFIG_URANUS4)||defined(CONFIG_KRONUS)||defined(CONFIG_KAISERIN)||defined(CONFIG_KENYA)||defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KAYLA)||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_K5TN)||defined(CONFIG_K1C)||defined(CONFIG_K5AP)
    // set VQ for TSP, set FW/VQ for TSP2
    MS_U8*          pu8VQ;
    MS_PHYADDR      phyVQ;
    DMX_TSPParam    param;

    memset(&param,0,sizeof(param));
    pu8VQ = (MS_U8*)(((int)u8TSVQBuf + 0x7) & (~0x7));
    phyVQ = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8VQ);

    // printf("[%s][%d] (fw, vq) = (0x%08x 0x%08x)\n", __FUNCTION__, __LINE__,
    // phyFW, phyVQ);
    param.phyFWAddr = (MS_PHYADDR)phyFW;
    param.u32FWSize = TSP_FW_SIZE;
#if defined(CONFIG_KERES)||defined(CONFIG_KIRIN)||defined(CONFIG_KRIS)||defined(CONFIG_KAYLA)||defined(CONFIG_KRATOS)||defined(CONFIG_KIWI)||defined(CONFIG_K5TN)||defined(CONFIG_K1C)||defined(CONFIG_K5AP)
    param.phyVQAddr = (MS_PHYADDR)0x3000000;
#else
    param.phyVQAddr = (MS_PHYADDR)phyVQ;
#endif
    param.u32VQSize = TSP_VQ_SIZE;//(200*16 + 200*8 + 200*8);

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_TSPInit(&param))
    {
        printf("DeMux>>ERROR! TSP set fw/vq fail\n");
        return -1;
    }
#endif
#ifdef HB_CUS
	if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc, FALSE, TRUE, TS_PARALLEL_OUTPUT))
#else
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc, TS_CLK_INV, TRUE, TS_PARALLEL_OUTPUT))
#endif
	{
        printf("DeMux>>ERROR! switch to demod fail\n");
        return -1;
    }
#ifdef HB_CUS
    MApi_DMX_Parl_Invert(DMX_FLOW_PLAYBACK, FALSE);
#endif
#endif  // #if (CONFIG_BOOTVIDEO == 1)
    return 0;
}
U_BOOT_CMD(
	dmx_init,	2,	1,	do_dmx_init,
	"dmx_init     - initialize the demux setting\n",
	"<arg[1]>: dmx flow input source => 0:demodulator; 1:memory; 2:ext input0; 3:ext input1\n"
);


#endif /* (CONFIG_COMMANDS) */
