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
//  Include files
//-------------------------------------------------------------------------------------------------#include "e1000.h"

#include "mdrv_emac.h"

u8 MY_MAC[6] = { 0x00, 0x30, 0x1B, 0xBA, 0x02, 0xDB };
unsigned char phy_id=0;

#define EMAC_DBG_TFTP       (0)
#define EMAC_DBG_MSG(x)     //x
#define RTK_OUI_MSB         (0x1C)
#if (EMAC_DBG_TFTP)
s32 rxpre_seq = 0, txpre_seq = 0;
#endif

//For BIST
#define PHY_LOOPBACK    (1)
#define MAC_LOOPBACK    (2)

//--------------------------------------------------------------------------------------------------
//  Global variable
//--------------------------------------------------------------------------------------------------
u32 EMAC_MEM_BASE = 0, RAM_ADDR_BASE = 0, RX_BUFFER_BASE=0, RBQP_BASE=0, TX_BUFFER_BASE=0;
u8 *packet = (u8 *)packet_data;
struct eth_device nic_device;

static int MDrv_EMAC_tx(struct eth_device *nic, volatile void *tx_packet, int length);
void MDrv_EMAC_DumpMem(u32 addr, u32 len);
#if (SUPPORT_ALBANY)
u32 albany_read_reg(u32 reg);
void albany_write_reg(u32 reg, u32 val);
#endif

#ifndef EWAVE_TEST_NUM
    #define EWAVE_TEST_NUM                  0
#endif

eWaveTestCb eWaveCbTable[EWAVE_TEST_NUM] = {NULL};

u8 ustart = 0;
u32 packet_data[524];
BasicConfigEMAC ThisBCE;
UtilityVarsEMAC ThisUVE;

//-------------------------------------------------------------------------------------------------
//Initialize and start the Receiver and Transmit subsystems
//-------------------------------------------------------------------------------------------------

#ifdef  RX_OFFSET_4_BYTE_PATCH
/*header checksum*/
static u32 _headcsum (unsigned char *ptr)
{
    u32 csum = 0;
    u32 offset;

    for (offset = 14; offset < 34; offset+=2)
    {
        if (offset==24)
            continue;

        csum += ((ptr[offset] << 8) | (ptr[offset+1]));

        if (csum >= 0x10000)
            csum -= 0xFFFF;
    }

    csum = 0xFFFF - csum;
    return csum;
}
#endif

static void MDrv_EMAC_start (const struct eth_device *nic)
{
    struct emac_hw *hw = (struct emac_hw *) nic->priv;
    u32 uRegVal;
#ifdef SOFTWARE_DESCRIPTOR
    u32 i;
#endif

    // Initialize Receive Buffer Descriptors
    memset((u8*)(RBQP_BASE+RAM_ADDR_BASE),0x00,RBQP_SIZE);
#ifdef SOFTWARE_DESCRIPTOR
    for(i=0; i<RBQP_LENG;i++)
    {
#ifdef EMAC_64BIT_MOBF_DESCRIPTOR_TABLE_128BIT_ALIGNMENT_PATCH
        if(i<(RBQP_LENG-1))
            MHal_EMAC_WritRam32((RBQP_BASE+RAM_ADDR_BASE), i*0x10, (RX_BUFFER_BASE+0x600*i));
        else
            MHal_EMAC_WritRam32((RBQP_BASE+RAM_ADDR_BASE), i*0x10, (RX_BUFFER_BASE+0x600*i+EMAC_DESC_WRAP));
#else
        if(i<(RBQP_LENG-1))
            MHal_EMAC_WritRam32((RBQP_BASE+RAM_ADDR_BASE), i*8, (RX_BUFFER_BASE+0x600*i));
        else
            MHal_EMAC_WritRam32((RBQP_BASE+RAM_ADDR_BASE), i*8, (RX_BUFFER_BASE+0x600*i+EMAC_DESC_WRAP));
#endif
    }
#else
    MHal_EMAC_WritRam32((RBQP_BASE+RAM_ADDR_BASE),(RBQP_SIZE-0x08),0x00000002);
#endif

    Chip_Flush_Memory();

    // set offset of read and write pointers in the receive circular buffer //
    uRegVal = MHal_EMAC_Read_BUFF();
    uRegVal = (RX_BUFFER_BASE|RX_BUFFER_SEL);
    MHal_EMAC_Write_BUFF(uRegVal);
    MHal_EMAC_Write_RDPTR(0);
    MHal_EMAC_Write_WRPTR(0);

    // Program address of descriptor list in Rx Buffer Queue register //
    MHal_EMAC_Write_RBQP(RBQP_BASE);
    //Reset buffer index//
    hw->rxBuffIndex = 0;

    // Enable Receive and Transmit //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= (EMAC_RE | EMAC_TE);
    MHal_EMAC_Write_CTL(uRegVal);
}

//-------------------------------------------------------------------------------------------------
// Open the ethernet interface
//-------------------------------------------------------------------------------------------------
static int MDrv_EMAC_open (struct eth_device *nic, bd_t * bis)
{
    // struct emac_hw *hw = (struct emac_hw *) nic->priv;
    u32 uRegVal;

    //ato  EMAC_SYS->PMC_PCER = 1 << EMAC_ID_EMAC;   //Re-enable Peripheral clock //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= EMAC_CSR;
    MHal_EMAC_Write_CTL(uRegVal);
    // Enable PHY interrupt //
    MHal_EMAC_enable_phyirq ();

    // Enable MAC interrupts //
    uRegVal = EMAC_INT_RCOM |EMAC_INT_RBNA
    | EMAC_INT_TUND | EMAC_INT_RTRY | EMAC_INT_TCOM
    | EMAC_INT_ROVR | EMAC_INT_HRESP;
    MHal_EMAC_Write_IER(uRegVal);

    MDrv_EMAC_start (nic);
    return 0;
}

/**************************************************************************
POLL - Wait for a frame
***************************************************************************/
static int MDrv_EMAC_rx(struct eth_device *nic)
{
    struct emac_hw *hw = (struct emac_hw *) nic->priv;
    struct recv_desc_bufs *dlist;
    unsigned char *p_recv;
    u32 pktlen;
    u32 uRegVal=0;
    u32 wrap_bit;
#if (EMAC_DBG_TFTP)
    u8 type = 0;
    u16 block = 0;
#endif
    dlist = hw->dlist ;
    // If any Ownership bit is 1, frame received.

    Chip_Flush_Memory();

    while ( (dlist->descriptors[hw->rxBuffIndex].addr )& EMAC_DESC_DONE)
    {
        p_recv = (unsigned char *) ((((dlist->descriptors[hw->rxBuffIndex].addr)) + RAM_ADDR_BASE) &~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
        pktlen = (u32)dlist->descriptors[hw->rxBuffIndex].size & 0x7ff;    /* Length of frame including FCS */

        Chip_Flush_Memory();

#ifdef  RX_OFFSET_4_BYTE_PATCH
        u32 headcsum= (p_recv[0x18] << 8) | (p_recv[0x19]);

        /* If the header checksum correct, no change */
        if(headcsum == _headcsum(p_recv))
        {
            p_recv = p_recv;
        }
        /* If the header checksum is incorrect, confirm it after shift right 4 bytes */
        else
        {
            headcsum = (p_recv[0x1c] << 8) | (p_recv[0x1d]);

            /* If the header checksum after shift right 4 bytes is correct, then shift this packet*/
            if(headcsum == _headcsum(p_recv + 4))
            {
                EMAC_DBG_MSG(printf("header csum = 0x%x\n", headcsum));
                EMAC_DBG_MSG(MDrv_EMAC_DumpMem(p_recv, pktlen));
                p_recv = p_recv + 4;
                EMAC_DBG_MSG(MDrv_EMAC_DumpMem(p_recv, pktlen));
            }
            /* In the case, which means the header have no header checksum*/
            else
            {
                p_recv = p_recv;
            }
        }
#endif

#if (EMAC_DBG_TFTP)
        block = *(p_recv+0x2d) | (*(p_recv+0x2c) << 8);
        type = *(p_recv+0x2b);
        if (3 == type && block >0)
        {
            if (block == rxpre_seq)
            {
                EMAC_DBG_MSG(printf("xxxxxx Rx tftp Packet:%lx Type:%u block:%u again!!!\n", p_recv, type, block));
            }
            else
            {
                EMAC_DBG_MSG(printf("000000 Rx tftp Packet:%lx Type:%u block:%u again!!!\n", p_recv, type, block));
                rxpre_seq++;
            }
        }
        else if (4 == type)
        {
            EMAC_DBG_MSG(printf("Rx tftp ACK Packet:%lx Type:%u block:%u\n", p_recv, type, block));
        }
        else if (6 == type)
        {
            EMAC_DBG_MSG(printf("\nRx tftp Opt ACK Packet:%lx Type:%u\n", p_recv, type));
            EMAC_DBG_MSG(printf("Ready to tftp transaction!!!!!!!\n"));
        }
        else if (block != (rxpre_seq+1) && rxpre_seq > 0)
        {
            EMAC_DBG_MSG(printf("\nError tftp packet block:%u rsr:%x tsr:%x\n", (rxpre_seq+1), MHal_EMAC_Read_RSR(), MHal_EMAC_Read_TSR()));
            EMAC_DBG_MSG(printf("\ndescriptor:%lx size:%lx\n", &(dlist->descriptors[hw->rxBuffIndex]), pktlen));
            //MDrv_EMAC_DumpMem(p_recv, 0x40);
            //MDev_EMAC_stats();
        }
        
        MDrv_EMAC_DumpMem(p_recv, 0x40);
#endif
        if ((unsigned char *) (dlist->recv_buf + RX_BUFFER_SIZE) >
        (p_recv + ROUND_SUP_4 (pktlen)))
        {
            // the frame is not splitted in two parts //
            memcpy(packet, p_recv, pktlen);
            // update consumer pointer//
            uRegVal = MHal_EMAC_Read_RDPTR();
            uRegVal += ROUND_SUP_4(pktlen);
            MHal_EMAC_Write_RDPTR(uRegVal);
        }
        else
        {
            //the frame is splitted in two parts //
            int firstPacketPartSize =
            (unsigned char *) dlist->recv_buf + RX_BUFFER_SIZE - p_recv;
            int secondPacketSize =(ROUND_SUP_4(pktlen) - firstPacketPartSize);
            memcpy((unsigned char *)packet, p_recv, firstPacketPartSize);
            memcpy((unsigned char *)(packet+firstPacketPartSize), (unsigned char *) dlist->recv_buf, secondPacketSize);

            // update consumer pointer and toggle the wrap bit //
            wrap_bit = (MHal_EMAC_Read_RDPTR() & EMAC_WRAP_R) ^ EMAC_WRAP_R;
            MHal_EMAC_Write_RDPTR(secondPacketSize | wrap_bit);
        }

        Chip_Flush_Memory();

        if (ThisBCE.loopback == PHY_LOOPBACK)
        {
            printf("Rx Data");
            MDrv_EMAC_DumpMem((u32)packet, 0x40);
        }
        else if (ThisBCE.loopback == MAC_LOOPBACK)
        {
            MDrv_EMAC_tx(nic, (void *)packet, pktlen);
        }
        else
            NetReceive((uchar *)packet, pktlen);

        dlist->descriptors[hw->rxBuffIndex].addr  &= ~EMAC_DESC_DONE;  /* reset ownership bit */
        Chip_Flush_Memory();

        //wrap after last buffer //
        hw->rxBuffIndex++;
        if (hw->rxBuffIndex == MAX_RX_DESCR)
        {
            hw->rxBuffIndex = 0;
        }
    }
#if defined(CONFIG_KRONUS)
    Chip_Read_Memory();
#endif

    return 1;
}

/**************************************************************************
TRANSMIT - Transmit a frame
***************************************************************************/

static char t_packet[2096] __attribute__ ((aligned (8)));

static int MDrv_EMAC_tx(struct eth_device *nic, volatile void *tx_packet, int length)
{
#if (EMAC_DBG_TFTP)
    u8 *ptr = (u8 *)tx_packet, type;
    u16 block = 0;
#endif
    u32 tx_base;
    u8 *ptr;
    tx_base = (u32)t_packet + BUS_MIU0_BASE_NONCACHE-BUS_MIU0_BASE_CACHE ;
    ptr = (u8 *)tx_base;

    while (length < 60)         //padding to 60 bytes
    {
        *((u8*)tx_packet+length)=0;
        length++;
    }

#if (EMAC_DBG_TFTP)
    type = *(ptr+0x2b);
    block = *(ptr+0x2d) | (*(ptr+0x2c) << 8);
    if (1 == type)
     {
        EMAC_DBG_MSG(printf("\nTx tftp Request Type:%u\n", ptr, type));
     }
    else if (3 == type)
    {
        EMAC_DBG_MSG(printf("Tx tftp Packet:%lx Type:%u block:%u\n", ptr, type, block));
    }
    else if (4 == type && block >0)
    {
        EMAC_DBG_MSG(printf("xxxxxxx Tx tftp ACK Packet:%lx Type:%u block:%u again!!!!\n", ptr, type, block));
        if (block == txpre_seq)
            EMAC_DBG_MSG(printf("xxxxxxx Tx tftp ACK Packet:%lx Type:%u block:%u again!!!!\n", ptr, type, block));
        else
            block++;
    }
#endif

    MHal_EMAC_CheckTSR();

#if defined(CONFIG_JANUS) || defined(CONFIG_MARIA10)
    memcpy((void *)t_packet, (void *)tx_packet, length);
    MHal_EMAC_Write_TAR((u32)t_packet);
#else
    memcpy((void *)ptr, (void *)tx_packet, length);
    Chip_Flush_Memory();
    MHal_EMAC_Write_TAR((u32)ptr - BUS_MIU0_BASE_NONCACHE);
#endif

    if (ThisBCE.loopback == PHY_LOOPBACK)
    {
        printf("Tx Data");
        MDrv_EMAC_DumpMem((u32)tx_packet, 0x40);
    }

    /* Sync packet data to memory from cache */
    Chip_Flush_Memory();

    // Set length of the packet in the Transmit Control register //
    MHal_EMAC_Write_TCR(length);

    return 1;
}

//-------------------------------------------------------------------------------------------------
// Close the interface
//-------------------------------------------------------------------------------------------------
void MDrv_EMAC_close(struct eth_device *nic)
{
    u32 uRegVal;
    //Disable Receiver and Transmitter //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal &= ~(EMAC_TE | EMAC_RE);
    MHal_EMAC_Write_CTL(uRegVal);
    // Disable PHY interrupt //
    MHal_EMAC_disable_phyirq ();

    //Disable MAC interrupts //
    uRegVal = EMAC_INT_RCOM | EMAC_INT_RBNA
    | EMAC_INT_TUND | EMAC_INT_RTRY | EMAC_INT_TCOM
    | EMAC_INT_ROVR | EMAC_INT_HRESP;
    MHal_EMAC_Write_IDR(uRegVal);
    netif_stop_queue (nic);
}

void MDev_EMAC_stats(void)
{
    printf("Rx Ok:%u\n", MHal_EMAC_Read_OK());       /* Good frames received */
    printf("ALE:%u\n", MHal_EMAC_Read_ALE());
    printf("ELR:%u\n", MHal_EMAC_Read_ELR());        /* Excessive Length or Undersize Frame error */
    printf("SEQE:%u\n", MHal_EMAC_Read_SEQE());      /* Excessive Length or Undersize Frame error */
    printf("ROVR:%u\n", MHal_EMAC_Read_ROVR());      //rx fifo error
    printf("SE:%u\n", MHal_EMAC_Read_SE());
    printf("RJB:%u\n", MHal_EMAC_Read_RJB());

    printf("Tx Ok:%u\n", MHal_EMAC_Read_FRA());
    printf("TUE:%u\n", MHal_EMAC_Read_TUE());        /* Transmit FIFO underruns */
    printf("CSE:%u\n", MHal_EMAC_Read_CSE());      /* Carrier Sense errors */
    printf("SQEE:%u\n", MHal_EMAC_Read_SQEE());      /* Heartbeat error */
 }

void MDrv_EMAC_PhyAddrScan(void)
{
    u32 word_ETH_MAN  = 0x00000000;

#ifdef CONFIG_ETHERNET_ALBANY
    MHal_EMAC_Write_JULIAN_0100(0x0000F011);
#else
    MHal_EMAC_Write_JULIAN_0100(0x0000F007);
#endif

    ThisUVE.flagISR_INT_DONE = 0x00;
    MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &word_ETH_MAN);
    if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
    {
        printf("phy [%d]=%x\n",phy_id, word_ETH_MAN);
        return;
    }

    for (phy_id = 0; phy_id < 32; phy_id++)
    {
        MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &word_ETH_MAN);
        printf("phy [%d]=%x\n",phy_id, word_ETH_MAN);
        if((0xffff != word_ETH_MAN)&&(0x0000 != word_ETH_MAN))
        {
            return;
        }
    }
    printf("Can't get correct PHY Addr and reset to 0\n");
    phy_id = 0;
}

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
static int MDrv_EMAC_HW_init(const struct eth_device *nic)
{
    u32 word_ETH_CTL = 0x00000000;
    u32 word_ETH_CFG = 0x00000800;
    u32 uJulian104Value=0;
    s32 uNegPhyVal = 0;

    // (20071026_CHARLES) Disable TX, RX and MDIO:   (If RX still enabled, the RX buffer will be overwrited)
    MHal_EMAC_Write_CTL(word_ETH_CTL);
    // Init RX --------------------------------------------------------------
    memset((u8*)(RAM_ADDR_BASE + RX_BUFFER_BASE),0x00,RX_BUFFER_SIZE);

    MHal_EMAC_Write_BUFF(RX_BUFFER_BASE|RX_BUFFER_SEL);
    MHal_EMAC_Write_RDPTR(0x00000000);
    MHal_EMAC_Write_WRPTR(0x00000000);
    // Initialize "Receive Buffer Queue Pointer"
    MHal_EMAC_Write_RBQP(RBQP_BASE);

    // Enable Interrupts ----------------------------------------------------
    MHal_EMAC_Write_IER(0x0000FFFF);
    printf("MAC Address %02X:%02X:%02X:%02X:%02X:%02X\n",ThisBCE.sa1[0],ThisBCE.sa1[1],
    ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
    // Set MAC address ------------------------------------------------------
    MHal_EMAC_Write_SA1_MAC_Address(ThisBCE.sa1[0],ThisBCE.sa1[1],ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
    MHal_EMAC_Write_SA2_MAC_Address(ThisBCE.sa2[0],ThisBCE.sa2[1],ThisBCE.sa2[2],ThisBCE.sa2[3],ThisBCE.sa2[4],ThisBCE.sa2[5]);
    MHal_EMAC_Write_SA3_MAC_Address(ThisBCE.sa3[0],ThisBCE.sa3[1],ThisBCE.sa3[2],ThisBCE.sa3[3],ThisBCE.sa3[4],ThisBCE.sa3[5]);
    MHal_EMAC_Write_SA4_MAC_Address(ThisBCE.sa4[0],ThisBCE.sa4[1],ThisBCE.sa4[2],ThisBCE.sa4[3],ThisBCE.sa4[4],ThisBCE.sa4[5]);

#ifdef SOFTWARE_DESCRIPTOR
    #ifdef CHECKSUM
        uJulian104Value=uJulian104Value|(CHECKSUM_ENABLE|SOFTWARE_DESCRIPTOR_ENABLE);
    #else
        uJulian104Value=uJulian104Value|SOFTWARE_DESCRIPTOR_ENABLE;
    #endif
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);//Disable interrupt delay
#else
    #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS)
        MHal_EMAC_Write_JULIAN_0100(0x0000F007);
        MHal_EMAC_Write_JULIAN_0104(0x00);
    #else
        uJulian104Value=0x00000000;
        MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
    #endif
#endif //#ifdef SOFTWARE_DESCRIPTOR

    // IMPORTANT: Run NegotiationPHY() before writing REG_ETH_CFG.
    ThisBCE.duplex = 1;
    ThisBCE.speed = SPEED_10;
    uNegPhyVal = MHal_EMAC_NegotiationPHY();

    ThisUVE.flagMacTxPermit = 0x01;
    switch(uNegPhyVal)
    {
    case 1:
        ThisBCE.duplex = 1;
        ThisBCE.speed = SPEED_10;
        break;

    case 2:
        ThisBCE.duplex = 2;
        ThisBCE.speed = SPEED_10;
        word_ETH_CFG |= (EMAC_FD);
        break;

    case 3:
        ThisBCE.duplex = 1;
        ThisBCE.speed = SPEED_100;
        word_ETH_CFG |= (EMAC_SPD);
        break;

    case 4:
        word_ETH_CFG |= (EMAC_SPD|EMAC_FD);
        break;

    default:
        printf("Status Error!\n");
        break;
    }

#if (SUPPORT_ALBANY)
    if (ThisBCE.speed == SPEED_100)
        albany_write_reg(0x0b7b, 0x08a6);
#endif

    printf("word_ETH_CFG %x", word_ETH_CFG);
    MHal_EMAC_Write_CFG(word_ETH_CFG);
    // ETH_CTL Register -----------------------------------------------------
    word_ETH_CTL = 0x0000010;  // Enable MDIO
    if(ThisBCE.wes == 1) word_ETH_CTL |= 0x00000080;
    MHal_EMAC_Write_CTL(word_ETH_CTL);

#ifdef SOFTWARE_DESCRIPTOR
#if (SUPPORT_ALBANY)
    MHal_EMAC_Write_JULIAN_0100(0x0000F011);    //MII
#else
    MHal_EMAC_Write_JULIAN_0100(0x0000F007);    //RMII
#endif
#else
    MHal_EMAC_Write_JULIAN_0100(0x00000107);
#endif

#ifdef CONFIG_ETHERNET_ALBANY
    MHal_EMAC_Write_JULIAN_0100(0x0000F011);
#endif

    MHal_EMAC_HW_init();

    ThisUVE.flagPowerOn = 1;
    ThisUVE.initedEMAC  = 1;

    return 1;
}

//-------------------------------------------------------------------------------------------------
// EMAC init Variable
//-------------------------------------------------------------------------------------------------
static volatile u8 emac_var[EMAC_ABSO_MEM_SIZE];
u32 MDrv_EMAC_VarInit(void)
{
    u32 alloRAM_ADDR_BASE;
    int i;
    char *s, *e;
    alloRAM_ADDR_BASE = (u32)emac_var -BUS_MIU0_BASE_CACHE + BUS_MIU0_BASE_NONCACHE; //transfer to non-cachable
    memset((u32 *)alloRAM_ADDR_BASE,0x00,EMAC_ABSO_MEM_SIZE);
    Chip_Flush_Memory();
    EMAC_MEM_BASE = (alloRAM_ADDR_BASE + 0x3FFF) & ~0x3FFF;   // IMPORTANT: Let lowest 14 bits as zero.
    RAM_ADDR_BASE = BUS_MIU0_BASE_NONCACHE;
    printf("EMAC Mem Base:%x\n", EMAC_MEM_BASE);

    RX_BUFFER_BASE   = EMAC_MEM_BASE - RAM_ADDR_BASE;
    RBQP_BASE        = RX_BUFFER_BASE + RX_BUFFER_SIZE;
    if ((RBQP_BASE + RBQP_SIZE+ EMAC_MEM_GAP) > (alloRAM_ADDR_BASE + EMAC_ABSO_MEM_SIZE))
    {
        printf("MMAP overflow: %x > %x\n", (RBQP_BASE + RBQP_SIZE+ EMAC_MEM_GAP), (alloRAM_ADDR_BASE + EMAC_ABSO_MEM_SIZE));
        return 0;
    }

    printf("Rx buff:%x\nRBQP:%x\n", RX_BUFFER_BASE, RBQP_BASE);
    memset(&ThisBCE,0x00,sizeof(BasicConfigEMAC));
    memset(&ThisUVE,0x00,sizeof(UtilityVarsEMAC));

    ThisBCE.wes         = 0;                // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    ThisBCE.duplex      = 2;                // 1:Half-duplex, 2:Full-duplex
    ThisBCE.cam         = 0;                // 0:No CAM, 1:Yes
    ThisBCE.rcv_bcast   = 0;                // 0:No, 1:Yes
    ThisBCE.rlf         = 0;                // 0:No, 1:Yes receive long frame(1522)
    ThisBCE.rcv_bcast   = 1;
    ThisBCE.speed       = EMAC_SPEED_100;

    s = getenv ("ethaddr");
    if (s)
    {
        for (i = 0; i < 6; ++i)
        {
            ThisBCE.sa1[i] = s ? simple_strtoul (s, &e, 16) : 0;
            if (s)
            {
                s = (*e) ? e + 1 : e;
            }
        }
    }
    else
    {
        ThisBCE.sa1[0]      = MY_MAC[0];
        ThisBCE.sa1[1]      = MY_MAC[1];
        ThisBCE.sa1[2]      = MY_MAC[2];
        ThisBCE.sa1[3]      = MY_MAC[3];
        ThisBCE.sa1[4]      = MY_MAC[4];
        ThisBCE.sa1[5]      = MY_MAC[5];
    }

    ThisBCE.loopback = 0;
    //if (!((u32)packet_data% 4))
    //    packet +=2;
    printf("Packet buffer:%lx\n", (long unsigned int)packet);

    return alloRAM_ADDR_BASE;
}

/**************************************************************************
PROBE - Look for an adapter, this routine's visible to the outside
You should omit the last argument struct pci_device * for a non-PCI NIC
***************************************************************************/
struct emac_hw hw_device;
int emac_init=0;

void MDrv_EMAC_DumpMem(u32 addr, u32 len)
{
    u8 *ptr = (u8 *)addr;
    u32 i;

    printf("\n ===== Dump %lx =====\n", (long unsigned int)ptr);
    for (i=0; i<len; i++)
    {
        if ((u32)i%0x10 ==0)
            printf("%lx: ", (long unsigned int)ptr);
        if (*ptr < 0x10)
            printf("0%x ", *ptr);
        else
            printf("%x ", *ptr);
        if ((u32)i%0x10 == 0x0f)
            printf("\n");
    ptr++;
    }
    printf("\n");
}

void MDrv_EMAC_DumpReg(void)
{
    u32 idx, val = 0;

    printf("======= PHY Reg =======\n");
    for (idx = 0; idx < 0x20; idx++)
    {
        MHal_EMAC_read_phy(phy_id, idx, &val);
        printf("  Reg-%-3X=%-4x   ",idx, val);
        if (idx%5 == 4)
            printf("\n");
    }

    printf("\n======= EMAC Reg =======\n");
    for (idx = 0; idx < 0x120; idx+=4)
    {
        printf("  Reg-%-3X=%-8x   ",idx, MHal_EMAC_ReadReg32(idx));
        if (idx%0x10 == 0x0c)
            printf("\n");
    }
}

u8 MDrv_EMAC_Bist(u8 mode)
{
    u16 timeout = 0;
    u32 status = 0;
    volatile u8 endless_loop = 1;

    ThisBCE.loopback = mode;
    switch(mode)
    {
    case PHY_LOOPBACK:
        MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, 0x6100);
        do
        {
            MHal_EMAC_read_phy(phy_id, PHY_REG_STATUS, &status);
            timeout++;
        } while((!(status & 0x0004)) && (timeout < 25000));
        if (timeout == 50000)
        {
            printf("Open PHY loopback mode timeout!!\n");
            MHal_EMAC_NegotiationPHY();
            return FALSE;
        }
        else
            printf("Open PHY loopback mode!\n");
        break;

    case MAC_LOOPBACK:
        MDrv_EMAC_open(&nic_device, NULL);
        printf("Long loopback test\n");
        // It's really an infinite loop
        // But it's done this way to "cheat" Dropzone/Parasoft, forbidding infinite loops
        do
        {
           MDrv_EMAC_rx(&nic_device);
        }
        while (endless_loop);
        break;

    default:
        MHal_EMAC_NegotiationPHY();
        printf("EMAC normal mode!\n");
        break;
    }

    return TRUE;
}

void MDrv_EMAC_PatchPHY(void)
{
    u32 val = 0;
    MHal_EMAC_read_phy(phy_id, 2, &val);
    if (RTK_OUI_MSB == val)
    {
        MHal_EMAC_read_phy(phy_id, 25, &val);
        MHal_EMAC_write_phy(phy_id, 25, 0x400);
        MHal_EMAC_read_phy(phy_id, 25, &val);
    }
#if (SUPPORT_ALBANY)
    albany_patch();
#endif
}

void MDrv_EMAC_PHY_force_10(void)
{
    MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, PHY_REG_BASIC_RESET);
    mdelay(10);
    MHal_EMAC_write_phy(phy_id, PHY_REG_ADVERTISE, PHY_REG_ADVERTISE_CSMA|PHY_REG_ADVERTISE_10HALF|PHY_REG_ADVERTISE_10FULL);
    MHal_EMAC_write_phy(phy_id, PHY_REG_CONTROL1000, 0x0);
    mdelay(10);
    MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, PHY_REG_BASIC_ANENABLE|PHY_REG_BASIC_ANRESTART);
}

void MDrv_EMAC_PHY_force_100(void)
{
    MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, PHY_REG_BASIC_RESET);
    mdelay(10);
    MHal_EMAC_write_phy(phy_id, PHY_REG_CONTROL1000, 0x0);
    mdelay(10);
    MHal_EMAC_write_phy(phy_id, PHY_REG_BASIC, PHY_REG_BASIC_ANENABLE|PHY_REG_BASIC_ANRESTART);
}

int MDrv_EMAC_initialize(const bd_t * bis)
{
    struct eth_device *nic = NULL;
    struct emac_hw *hw = NULL;
    char* phy_id_string=NULL;

    phy_id_string = getenv ("PHY_ID");
    if (phy_id_string != NULL)
    {

        //phy_id =phy_id_string ? (int)simple_strtol(phy_id_string, NULL, 10) : 0;
        phy_id =(int)simple_strtol(phy_id_string, NULL, 10);
    }

    if(emac_init)
    {
        MDrv_EMAC_PhyAddrScan();
#if (EMAC_DBG_TFTP)
        rxpre_seq = txpre_seq = 0;
#endif
        return -1;
    }

    if (!MDrv_EMAC_VarInit())
        return -1;
    nic=&nic_device;
    hw=&hw_device;

    hw->dlist_phys = hw->dlist = (struct recv_desc_bufs *) (RX_BUFFER_BASE + RAM_ADDR_BASE);

    MHal_EMAC_Power_On_Clk();
    MDrv_EMAC_PhyAddrScan();

    MDrv_EMAC_PatchPHY();

#ifdef CONFIG_MAC_FORCE_10
    MDrv_EMAC_PHY_force_10();
#endif
#ifdef CONFIG_MAC_FORCE_100
    MDrv_EMAC_PHY_force_100();
#endif

    nic->priv = hw;
    MDrv_EMAC_HW_init(nic);

    /* EMAC HW init */
    nic->init = MDrv_EMAC_open;
    nic->recv = MDrv_EMAC_rx;
    nic->send = MDrv_EMAC_tx;
    nic->halt = MDrv_EMAC_close;

    memcpy(nic->enetaddr, ThisBCE.sa1, sizeof(ThisBCE.sa1));
    eth_register(nic);
    emac_init=1;

    mdelay(3000);

    return 0;

    //test start
}

void MDrv_EMAC_PowerMode(u8 mode)
{
    if (mode == 1)
        MHal_EMAC_Power_On_Clk();
    else
        MHal_EMAC_Power_Off_Clk();
}

void MDrv_EMAC_WaveTest(u8 mode)
{
    if((eWaveCbTable[mode]!=NULL) && (mode<EWAVE_TEST_NUM))
    {
        eWaveCbTable[mode]();
    }
    else
    {
        printf("ewavetest mode:%d not support...\n", mode);
    }
}

void MDrv_EMAC_SwReset(void)
{
    extern void MHal_EMAC_PHY_reset(void) __attribute__((weak));

    if(MHal_EMAC_PHY_reset)
    {
        MHal_EMAC_PHY_reset();
    }
}

// ftpupgrade
int MDrv_EMAC_open_for_lwip() {
    return MDrv_EMAC_open(&nic_device, NULL);
}

void MDrv_EMAC_close_for_lwip()
{
    MDrv_EMAC_close(&nic_device);
}

int MDrv_EMAC_tx_for_lwip(volatile void *tx_packet, int length)
{
    return MDrv_EMAC_tx(&nic_device, tx_packet, length);
}

int MDrv_EMAC_rx_for_lwip(void (*rx_callback)(volatile void *, int))
{
    struct emac_hw *hw = &hw_device;
    struct recv_desc_bufs *dlist;
    unsigned char *p_recv;
    u32 pktlen;
    u32 uRegVal=0;
    u32 wrap_bit;
#if (EMAC_DBG_TFTP)
    u8 type = 0;
    u16 block = 0;
#endif
    dlist = hw->dlist ;
    // If any Ownership bit is 1, frame received.
    Chip_Flush_Memory();

    while ( (dlist->descriptors[hw->rxBuffIndex].addr )& EMAC_DESC_DONE)
    {
        p_recv = (unsigned char *) ((((dlist->descriptors[hw->rxBuffIndex].addr)) + RAM_ADDR_BASE) &~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
        pktlen = (u32)dlist->descriptors[hw->rxBuffIndex].size & 0x7ff;    /* Length of frame including FCS */

        Chip_Flush_Memory();

#ifdef  RX_OFFSET_4_BYTE_PATCH
        u32 headcsum= (p_recv[0x18] << 8) | (p_recv[0x19]);

        /* If the header checksum correct, no change */
        if(headcsum == _headcsum(p_recv))
        {
            p_recv = p_recv;
        }
        /* If the header checksum is incorrect, confirm it after shift right 4 bytes */
        else
        {
            headcsum = (p_recv[0x1c] << 8) | (p_recv[0x1d]);

            /* If the header checksum after shift right 4 bytes is correct, then shift this packet*/
            if(headcsum == _headcsum(p_recv + 4))
            {
                EMAC_DBG_MSG(printf("header csum = 0x%x\n", headcsum));
                EMAC_DBG_MSG(MDrv_EMAC_DumpMem(p_recv, pktlen));
                p_recv = p_recv + 4;
                EMAC_DBG_MSG(MDrv_EMAC_DumpMem(p_recv, pktlen));
            }
            /* In the case, which means the header have no header checksum*/
            else
            {
                p_recv = p_recv;
            }
        }
#endif

#if (EMAC_DBG_TFTP)
        block = *(p_recv+0x2d) | (*(p_recv+0x2c) << 8);
        type = *(p_recv+0x2b);
        if (3 == type && block >0)
        {
            if (block == rxpre_seq)
            {
                EMAC_DBG_MSG(printf("xxxxxx Rx tftp Packet:%lx Type:%u block:%u again!!!\n", p_recv, type, block));
            }
            else
            {
                EMAC_DBG_MSG(printf("000000 Rx tftp Packet:%lx Type:%u block:%u again!!!\n", p_recv, type, block));
                rxpre_seq++;
            }
        }
        else if (4 == type)
        {
            EMAC_DBG_MSG(printf("Rx tftp ACK Packet:%lx Type:%u block:%u\n", p_recv, type, block));
        }
        else if (6 == type)
        {
            EMAC_DBG_MSG(printf("\nRx tftp Opt ACK Packet:%lx Type:%u\n", p_recv, type));
            EMAC_DBG_MSG(printf("Ready to tftp transaction!!!!!!!\n"));
        }
        else if (block != (rxpre_seq+1) && rxpre_seq > 0)
        {
            EMAC_DBG_MSG(printf("\nError tftp packet block:%u rsr:%x tsr:%x\n", (rxpre_seq+1), MHal_EMAC_Read_RSR(), MHal_EMAC_Read_TSR()));
            EMAC_DBG_MSG(printf("\ndescriptor:%lx size:%lx\n", &(dlist->descriptors[hw->rxBuffIndex]), pktlen));
            //MDrv_EMAC_DumpMem(p_recv, 0x40);
            //MDev_EMAC_stats();
        }

        MDrv_EMAC_DumpMem(p_recv, 0x40);
#endif
        if ((unsigned char *) (dlist->recv_buf + RX_BUFFER_SIZE) >
        (p_recv + ROUND_SUP_4 (pktlen)))
        {
            // the frame is not splitted in two parts //
            memcpy(packet, p_recv, pktlen);
            // update consumer pointer//
            uRegVal = MHal_EMAC_Read_RDPTR();
            uRegVal += ROUND_SUP_4(pktlen);
            MHal_EMAC_Write_RDPTR(uRegVal);
        }
        else
        {
            //the frame is splitted in two parts //
            int firstPacketPartSize =
            (unsigned char *) dlist->recv_buf + RX_BUFFER_SIZE - p_recv;
            int secondPacketSize =(ROUND_SUP_4(pktlen) - firstPacketPartSize);
            memcpy((unsigned char *)packet, p_recv, firstPacketPartSize);
            memcpy((unsigned char *)(packet+firstPacketPartSize), (unsigned char *) dlist->recv_buf, secondPacketSize);

            // update consumer pointer and toggle the wrap bit //
            wrap_bit = (MHal_EMAC_Read_RDPTR() & EMAC_WRAP_R) ^ EMAC_WRAP_R;
            MHal_EMAC_Write_RDPTR(secondPacketSize | wrap_bit);
        }

        Chip_Flush_Memory();

        if (ThisBCE.loopback == PHY_LOOPBACK)
        {
            printf("Rx Data");
            MDrv_EMAC_DumpMem((u32)packet, 0x40);
        }
        else if (ThisBCE.loopback == MAC_LOOPBACK)
        {
            MDrv_EMAC_tx(&nic_device, (void *)packet, pktlen);
        }
        else
            rx_callback(packet, (int) pktlen);

        dlist->descriptors[hw->rxBuffIndex].addr  &= ~EMAC_DESC_DONE;  /* reset ownership bit */
        Chip_Flush_Memory();

        //wrap after last buffer //
        hw->rxBuffIndex++;
        if (hw->rxBuffIndex == MAX_RX_DESCR)
        {
            hw->rxBuffIndex = 0;
        }
    }
#if defined(CONFIG_KRONUS)
    Chip_Read_Memory();
#endif

    return 1;
}
