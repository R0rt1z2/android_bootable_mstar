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
#include "mhal_noe.h"

#include <malloc.h>
#include <net.h>
#include <miiphy.h>
#include "mdrv_noe.h"
#include "mii_mgr.h"

#if !defined(TRUE) && !defined(FALSE)
#define TRUE                1
#define FALSE               0
#endif

#undef DEBUG
#define BIT(x)              ((1 << x))

/* bits range: for example BITS(16,23) = 0xFF0000
 *   ==>  (BIT(m)-1)   = 0x0000FFFF     ~(BIT(m)-1)   => 0xFFFF0000
 *   ==>  (BIT(n+1)-1) = 0x00FFFFFF
 */
#define BITS(m,n)   (~(BIT(m)-1) & ((BIT(n) - 1) | BIT(n)))

#define INTERNAL_LOOPBACK_ENABLE    1
#define INTERNAL_LOOPBACK_DISABLE   0
#define NOE_AUTO_NEGOTIATION_TIMEOUT    10

#define TOUT_LOOP   1000
#define ENABLE      1
#define DISABLE     0

#ifndef CONFIG_SYS_CACHELINE_SIZE
#define CONFIG_SYS_CACHELINE_SIZE   64
#endif  /* CONFIG_SYS_CACHELINE_SIZE */

#define NOE_NUM_TX_DESC     8
#define NOE_NUM_RX_DESC     CONFIG_SYS_RX_ETH_BUFFER

#define phys_to_bus(a) (a)

#define MACADDR_ENV "macaddr"

#define NOE_USE_AUTO_NEGOTIATION    0x1
#define NOE_FORCE_SPEED_1000        0x2
#define NOE_FORCE_SPEED_100         0x3
#define NOE_FORCE_SPEED_10          0x4

#define NOE_CONFIG_PHY_SPEED        NOE_USE_AUTO_NEGOTIATION

/*  Phy Vender ID list */
// Atheros
#define EV_ATHEROS_PHY_ID0  0x004d
#define EV_ATHEROS_PHY_ID1  0xd072
// Marvell
#define EV_MARVELL_PHY_ID0  0x0141
#define EV_MARVELL_PHY_ID1  0x0CC2
// Vtss
#define EV_VTSS_PHY_ID0     0x0007
#define EV_VTSS_PHY_ID1     0x0421
// ICPLUS
#define EV_ICPLUS_PHY_ID0   0x0243
#define EV_ICPLUS_PHY_ID1   0x0D90
// Realtek
#define EV_RTK_PHY_ID0      0x001C
#define EV_RTK_PHY_ID1      0xC915
// Mstar
#define EV_MSTAR_PHY_ID0    0x03A2
#define EV_MSTAR_PHY_ID1    0x9441

typedef enum drv_phy_speed_e {
    DRV_PHY_SPEED_10 = 1,
    DRV_PHY_SPEED_100 = 2,
    DRV_PHY_SPEED_1000 = 4,
} drv_phy_speed_t;

typedef struct _BUFFER_ELEM_    BUFFER_ELEM;

struct _BUFFER_ELEM_
{
    int tx_idx;
    unsigned char *pbuf;
    BUFFER_ELEM       *next;
};

typedef struct _VALID_BUFFER_STRUCT_    VALID_BUFFER_STRUCT;

struct _VALID_BUFFER_STRUCT_
{
    BUFFER_ELEM    *head;
    BUFFER_ELEM    *tail;
};

VALID_BUFFER_STRUCT  noe_free_buf_list;
VALID_BUFFER_STRUCT  noe_busing_buf_list;
static BUFFER_ELEM   noe_free_buf[PKTBUFSRX];

/*=========================================
      PDMA RX Descriptor Format define
=========================================*/

//-------------------------------------------------
typedef struct _PDMA_RXD_INFO1_  PDMA_RXD_INFO1_T;

struct _PDMA_RXD_INFO1_
{
    unsigned int    PDP0;
};
//-------------------------------------------------
typedef struct _PDMA_RXD_INFO2_    PDMA_RXD_INFO2_T;

struct _PDMA_RXD_INFO2_
{
    unsigned int    PLEN1                   : 14;
    unsigned int    LS1                     : 1;
    unsigned int    UN_USED                 : 1;
    unsigned int    PLEN0                   : 14;
    unsigned int    LS0                     : 1;
    unsigned int    DDONE_bit               : 1;
};
//-------------------------------------------------
typedef struct _PDMA_RXD_INFO3_  PDMA_RXD_INFO3_T;

struct _PDMA_RXD_INFO3_
{
    unsigned int    PDP1;
};
//-------------------------------------------------
typedef struct _PDMA_RXD_INFO4_    PDMA_RXD_INFO4_T;

struct _PDMA_RXD_INFO4_
{
    unsigned int    FOE_Entry               : 14;
    unsigned int    CRSN                    : 5;
    unsigned int    SP                      : 3;
    unsigned int    L4F                     : 1;
    unsigned int    L4VLD                   : 1;
    unsigned int    TACK                    : 1;
    unsigned int    IP4F                    : 1;
    unsigned int    IP4                     : 1;
    unsigned int    IP6                     : 1;
    unsigned int    UN_USE1                 : 4;
};

struct PDMA_rxdesc {
    PDMA_RXD_INFO1_T rxd_info1;
    PDMA_RXD_INFO2_T rxd_info2;
    PDMA_RXD_INFO3_T rxd_info3;
    PDMA_RXD_INFO4_T rxd_info4;
};
/*=========================================
      PDMA TX Descriptor Format define
=========================================*/
//-------------------------------------------------
typedef struct _PDMA_TXD_INFO1_  PDMA_TXD_INFO1_T;

struct _PDMA_TXD_INFO1_
{
    unsigned int    SDP0;
};
//-------------------------------------------------
typedef struct _PDMA_TXD_INFO2_    PDMA_TXD_INFO2_T;

struct _PDMA_TXD_INFO2_
{
    unsigned int    SDL1                  : 14;
    unsigned int    LS1_bit               : 1;
    unsigned int    BURST_bit             : 1;
    unsigned int    SDL0                  : 14;
    unsigned int    LS0_bit               : 1;
    unsigned int    DDONE_bit             : 1;
};
//-------------------------------------------------
typedef struct _PDMA_TXD_INFO3_  PDMA_TXD_INFO3_T;

struct _PDMA_TXD_INFO3_
{
    unsigned int    SDP1;
};
//-------------------------------------------------
typedef struct _PDMA_TXD_INFO4_    PDMA_TXD_INFO4_T;

struct _PDMA_TXD_INFO4_
{
    unsigned int    VLAN_TAG            :16;
    unsigned int    INS                 : 1;
    unsigned int    RESV                : 2;
    unsigned int    UDF                 : 6;
    unsigned int    FPORT               : 3;
    unsigned int    TSO                 : 1;
    unsigned int    TUI_CO              : 3;
};

struct PDMA_txdesc {
    PDMA_TXD_INFO1_T txd_info1;
    PDMA_TXD_INFO2_T txd_info2;
    PDMA_TXD_INFO3_T txd_info3;
    PDMA_TXD_INFO4_T txd_info4;
};

typedef struct noe_basic_config_s
{
    u8  loopback;
} noe_basic_config_t;

typedef enum {
    E_DRV_NOE_CMD_READ = 0,
    E_DRV_NOE_CMD_WRITE,
} EN_DRV_NOE_CMD_TYPE;

typedef enum {
    E_NOE_BIST_MODE_NORMAL = 0,
    E_NOE_BIST_MODE_PHY_LOOPBACK,
    E_NOE_BIST_MODE_MAC_LOOPBACK,
} EN_NOE_BIST_MODE;

/* function declaration */
static int   MDrv_NOE_Eth_Init(struct eth_device* dev, bd_t* bis);
static int   MDrv_NOE_Eth_Send(struct eth_device* dev, volatile void *packet, int length);
static int   MDrv_NOE_Eth_Recv(struct eth_device* dev);
static void  MDrv_NOE_Eth_Halt(struct eth_device* dev);
static int   MDrv_NOE_Eth_Setup(struct eth_device* dev);
static int   MDrv_NOE_Eth_Initialize(const bd_t *bis);
static void  MDrv_NOE_Eth_Init_MAC_PHY(void);
static void  MDrv_NOE_Eth_Init_FrameEngine(void);

#if defined(MSTAR_NOE_USE_FPGA_ENV)
static void  MDrv_NOE_Setup_FPGA_Environment(u32 eport, u32 phyaddr);
#else
static void MDrv_NOE_Enable_Auto_Polling(u32 eport, u32 phyaddr);
static int _mdrv_noe_is_atheros_gigaphy(u32 phyaddr);
static int _mdrv_noe_is_marvell_gigaphy(u32 phyaddr);
static int _mdrv_noe_is_vtss_gigaphy(u32 phyaddr);
static int _mdrv_noe_is_icplus_gigaphy(u32 phyaddr);
static int _mdrv_noe_is_realtek_gigaphy(u32 phyaddr);
static int _mdrv_noe_is_mstar_gigaphy(u32 phyaddr);
static void _mdrv_noe_phyctrl_speed_set(u32 phyaddr, u32 speed);
static void _mdrv_noe_phyctrl_1000BASE_T_set(u32 phyaddr, u32 enable);
static int _mdrv_noe_wait_an_complete(void);
#endif
static void _mdrv_noe_print_packet(u8 *buf, int length);
static int _mdrv_noe_is_DMA_busy(struct eth_device* dev);
static int _mdrv_noe_eth_recv(u8 isLwip, void (*rx_callback)(volatile void *, int));
/* End of function declaration */

static  struct PDMA_txdesc tx_ring0_cache[NOE_NUM_TX_DESC] __attribute__ ((aligned(CONFIG_SYS_CACHELINE_SIZE)));    /* TX descriptor ring */
static  struct PDMA_rxdesc rx_ring_cache[NOE_NUM_RX_DESC] __attribute__ ((aligned(CONFIG_SYS_CACHELINE_SIZE)));     /* RX descriptor ring */

static int rx_dma_owner_idx0;                             /* Point to the next RXD DMA wants to use in RXD Ring#0.  */
static int rx_wants_alloc_idx0;                           /* Point to the next RXD CPU wants to allocate to RXD Ring #0. */
static int tx_cpu_owner_idx0;                             /* Point to the next TXD in TXD_Ring0 CPU wants to use */
static volatile struct PDMA_rxdesc *rx_ring;
static volatile struct PDMA_txdesc *tx_ring0;

static int rx_desc_cnt;
static int rx_desc_threshold;

u8 MY_MAC[6] = { 0x00, 0x30, 0x1B, 0xBA, 0x02, 0xDB };
u8 eth_sa1[6];             // Specific Addr 1 (MAC Address)

struct eth_device noe_dev;

volatile uchar  NoeRxPktBuf[PKTBUFSRX+1][PKTSIZE_ALIGN];
volatile uchar  *NoeNetRxPackets[PKTBUFSRX];

noe_basic_config_t noe_basic_cfg;

static char loopback_buf[128] = {0};
static uchar loopback_failed;

void MDrv_NOE_Start(struct eth_device *dev )
{
    s32 omr;
    omr = NOE_REG(PDMA_GLO_CFG);
    udelay(100);
    omr |= TX_WB_DDONE | RX_DMA_EN | TX_DMA_EN ;
    NOE_REG(PDMA_GLO_CFG) = omr;
    udelay(500);
}

void MDrv_NOE_Stop(struct eth_device *dev)
{
    s32 omr;
    omr = NOE_REG(PDMA_GLO_CFG);
    udelay(100);
    omr &= ~(TX_WB_DDONE | RX_DMA_EN | TX_DMA_EN) ;
    NOE_REG(PDMA_GLO_CFG) = omr;
    udelay(500);
}


BUFFER_ELEM *_mdrv_noe_free_buf_entry_dequeue(VALID_BUFFER_STRUCT *hdr)
{
    int     zero = 0;           /* causes most compilers to place this */
    /* value in a register only once */
    BUFFER_ELEM  *node;

    /* Make sure we were not passed a null pointer. */
    if (!hdr) {
        return (NULL);
    }

    /* If there is a node in the list we want to remove it. */
    if (hdr->head) {
        /* Get the node to be removed */
        node = hdr->head;

        /* Make the hdr point the second node in the list */
        hdr->head = node->next;

        /* If this is the last node the headers tail pointer needs to be nulled
           We do not need to clear the node's next since it is already null */
        if (!(hdr->head)) {
            hdr->tail = (BUFFER_ELEM *)zero;
        }

        node->next = (BUFFER_ELEM *)zero;

    }
    else {
        node = NULL;
        return (node);
    }

    /*  Restore the previous interrupt lockout level.  */

    /* Return a pointer to the removed node */

    //shnat_validation_flow_table_entry[node->index].state = SHNAT_FLOW_TABLE_NODE_USED;
    return (node);
}

static BUFFER_ELEM *_mdrv_noe_free_buf_entry_enqueue(VALID_BUFFER_STRUCT *hdr, BUFFER_ELEM *item)
{
    int zero =0;

    if (!hdr) {
        return (NULL);
    }

    if (item != NULL)
    {
        /* Temporarily lockout interrupts to protect global buffer variables. */
        // Sys_Interrupt_Disable_Save_Flags(&cpsr_flags);

        /* Set node's next to point at NULL */
        item->next = (BUFFER_ELEM *)zero;

        /*  If there is currently a node in the linked list, we want to add the
            new node to the end. */
        if (hdr->head) {
            /* Make the last node's next point to the new node. */
            hdr->tail->next = item;

            /* Make the roots tail point to the new node */
            hdr->tail = item;
        }
        else {
            /* If the linked list was empty, we want both the root's head and
               tial to point to the new node. */
            hdr->head = item;
            hdr->tail = item;
        }

        /*  Restore the previous interrupt lockout level.  */

    }
    else
    {
        printf("\n shnat_flow_table_free_entry_enqueue is called,item== NULL \n");
    }

    return(item);

} /* MEM_Buffer_Enqueue */


static void MDrv_NOE_Eth_Init_FrameEngine(void)
{
    int i;
    u32 regValue, pinmux_eport;
    uchar *temp;
    unsigned long addr, len;

    pinmux_eport = MHal_NOE_Get_PinMux_Eport();

    rx_ring = (struct PDMA_rxdesc *)((ulong)&rx_ring_cache[0]);
    tx_ring0 = (struct PDMA_txdesc *)((ulong)&tx_ring0_cache[0]);

    noe_free_buf_list.head = NULL;
    noe_free_buf_list.tail = NULL;

    noe_busing_buf_list.head = NULL;
    noe_busing_buf_list.tail = NULL;

    /*
     *  Setup packet buffers, aligned correctly.
     */
    noe_free_buf[0].pbuf = (unsigned char *)&NoeRxPktBuf[0];
    noe_free_buf[0].pbuf += PKTALIGN - 1;
    noe_free_buf[0].pbuf -= (ulong)noe_free_buf[0].pbuf % PKTALIGN;
    noe_free_buf[0].next = NULL;

    _mdrv_noe_free_buf_entry_enqueue(&noe_free_buf_list,&noe_free_buf[0]);

#ifdef DEBUG
    printf("\n noe_free_buf[0].pbuf = 0x%08X \n",noe_free_buf[0].pbuf);
#endif
    for (i = 1; i < PKTBUFSRX; i++) {
        noe_free_buf[i].pbuf = noe_free_buf[0].pbuf + (i)*PKTSIZE_ALIGN;
        noe_free_buf[i].next = NULL;
#ifdef DEBUG
        printf("\n noe_free_buf[%d].pbuf = 0x%08X\n",i,noe_free_buf[i].pbuf);
#endif
        _mdrv_noe_free_buf_entry_enqueue(&noe_free_buf_list,&noe_free_buf[i]);
    }

    for (i = 0; i < PKTBUFSRX; i++)
    {
        noe_free_buf[i].tx_idx = NOE_NUM_TX_DESC;
#ifdef DEBUG
        printf("\n noe_free_buf[%d] = 0x%08X,noe_free_buf[%d].next=0x%08X \n",i,&noe_free_buf[i],i,noe_free_buf[i].next);
#endif
    }

    for (i = 0; i < NOE_NUM_RX_DESC; i++) {
        temp = memset((void *)&rx_ring[i],0,16);
        rx_ring[i].rxd_info2.DDONE_bit = 0;

        {
            BUFFER_ELEM *buf;
            buf = _mdrv_noe_free_buf_entry_dequeue(&noe_free_buf_list);
            NoeNetRxPackets[i] = buf->pbuf;
            rx_ring[i].rxd_info2.LS0= 0;
            rx_ring[i].rxd_info2.PLEN0= PKTSIZE_ALIGN;
            rx_ring[i].rxd_info1.PDP0 = cpu_to_le32(phys_to_bus((u32)NoeNetRxPackets[i]));
        }
    }

    for (i=0; i < NOE_NUM_TX_DESC; i++) {
        temp = memset((void *)&tx_ring0[i],0,16);
        tx_ring0[i].txd_info2.LS0_bit = 1;
        tx_ring0[i].txd_info2.DDONE_bit = 1;
        /* PN:
         *  0:CPU
         *  1:GE1
         *  2:GE2 (for RT2883)
         *  6:PPE
         *  7:Discard
         */
        if (pinmux_eport == E_HAL_NOE_EPORT_GE1) {
            tx_ring0[i].txd_info4.FPORT = 1;
        }
        else {
            tx_ring0[i].txd_info4.FPORT = 2;
        }
    }

    rx_dma_owner_idx0 = 0;
    rx_wants_alloc_idx0 = (NOE_NUM_RX_DESC - 1);
    tx_cpu_owner_idx0 = 0;

    rx_desc_cnt = 0;
    rx_desc_threshold = CONFIG_SYS_CACHELINE_SIZE / sizeof(struct PDMA_rxdesc);

    regValue = NOE_REG(PDMA_GLO_CFG);
    udelay(100);

    {
        regValue &= 0x0000FFFF;

        NOE_REG(PDMA_GLO_CFG) = regValue;
        udelay(500);
        regValue = NOE_REG(PDMA_GLO_CFG);
    }

    /* Tell the adapter where the TX/RX rings are located. */
    addr = (unsigned long)&rx_ring_cache[0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    len = (NOE_NUM_RX_DESC * sizeof(rx_ring_cache[0]) + 2 * CONFIG_SYS_CACHELINE_SIZE) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    flush_cache(addr, len);
    NOE_REG(RX_BASE_PTR0) = phys_to_bus((u32)&rx_ring[0]);
    //printf("\n rx_ring=%08X ,RX_BASE_PTR0 = %08X \n",&rx_ring[0],NOE_REG(RX_BASE_PTR0));

    addr = (unsigned long)&tx_ring0_cache[0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    len = (NOE_NUM_TX_DESC * sizeof(tx_ring0_cache[0]) + 2 * CONFIG_SYS_CACHELINE_SIZE) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    flush_cache(addr, len);
    NOE_REG(TX_BASE_PTR0) = phys_to_bus((u32)&tx_ring0[0]);
    //printf("\n tx_ring0=%08X, TX_BASE_PTR0 = %08X \n",&tx_ring0[0],NOE_REG(TX_BASE_PTR0));

    NOE_REG(RX_MAX_CNT0) = cpu_to_le32((u32)NOE_NUM_RX_DESC);
    NOE_REG(TX_MAX_CNT0) = cpu_to_le32((u32)NOE_NUM_TX_DESC);

    NOE_REG(TX_CTX_IDX0) = cpu_to_le32((u32)tx_cpu_owner_idx0);
    NOE_REG(PDMA_RST_IDX) = cpu_to_le32((u32)RST_DTX_IDX0);

    NOE_REG(RX_CALC_IDX0) = cpu_to_le32((u32)(NOE_NUM_RX_DESC - 1));
    NOE_REG(PDMA_RST_IDX) = cpu_to_le32((u32)RST_DRX_IDX0);
}

int MDrv_NOE_Eth_Initialize(const bd_t *bis)
{
    struct eth_device *dev = &noe_dev;

    if (MHal_NOE_Is_Initialized() == 0) {
        memset(dev, 0, sizeof(*dev));
        memset(&noe_basic_cfg, 0, sizeof(noe_basic_config_t));
        MHal_NOE_Init_PinMux_Config();

        sprintf(dev->name, "Eth0");

        dev->iobase = NOE_FRAME_ENGINE_BASE;
        dev->init   = MDrv_NOE_Eth_Init;
        dev->halt   = MDrv_NOE_Eth_Halt;
        dev->send   = MDrv_NOE_Eth_Send;
        dev->recv   = MDrv_NOE_Eth_Recv;

        memcpy(dev->enetaddr, eth_sa1, sizeof(eth_sa1));

        eth_register(dev);

    #if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
        miiphy_register(dev->name, noe_mii_read, noe_mii_write);
    #endif

    }

    MDrv_NOE_Eth_Init(dev, (bd_t *)bis);

    return 1;
}


static int MDrv_NOE_Eth_Init(struct eth_device* dev, bd_t* bis)
{
    u32 eport, connect;

    eport = MHal_NOE_Get_PinMux_Eport();
    connect = MHal_NOE_Get_PinMux_Connect();

    if(MHal_NOE_Is_Initialized() == 0)
    {
        MHal_NOE_Init_System();
        MHal_NOE_Set_PinMux(eport, connect);
        udelay(500);
        MDrv_NOE_Eth_Setup(dev);
    }
    else
    {
        MDrv_NOE_Start(dev);
    }

    return (1);
}


int _mdrv_noe_is_DMA_busy(struct eth_device* dev)
{
    u32 reg;

    reg = NOE_REG(PDMA_GLO_CFG);

    if((reg & RX_DMA_BUSY)){
        return 1;
    }

    if((reg & TX_DMA_BUSY)){
        printf("\n  TX_DMA_BUSY !!! ");
        return 1;
    }
    return 0;
}

#if defined(MSTAR_NOE_USE_FPGA_ENV)
void MDrv_NOE_Setup_FPGA_Environment(u32 eport, u32 phyaddr)
{
    u32 regValue;

    if (eport == E_HAL_NOE_EPORT_GE1) {
        printf("GMAC1: Force mode, Link Up, 100Mbps, Full-Duplex, FC ON\n");
        NOE_REG(NOE_MAC_P1_MCR) = 0x2105e337;
        MHal_NOE_Set_GDM_Forward_Port(eport);
        MHal_NOE_Switch_MDIO(eport);
    }
    else {
        printf("GMAC2: Force mode, Link Up, 100Mbps, Full-Duplex, FC ON\n");
        NOE_REG(NOE_MAC_P2_MCR) = 0x2105e337;
        MHal_NOE_Set_GDM_Forward_Port(eport);
        MHal_NOE_Switch_MDIO(eport);
    }

    mii_mgr_read(phyaddr, MII_CTRL1000, &regValue);
    // turn off 1000Base-T Advertisement  (9.9=1000Full, 9.8=1000Half)
    regValue &= ~(ADVERTISE_1000HALF | ADVERTISE_1000FULL);
    mii_mgr_write(phyaddr, MII_CTRL1000, regValue);

    /*10Mbps, debug*/
    regValue = ADVERTISE_PAUSE_CAP | ADVERTISE_10FULL |\
                ADVERTISE_10HALF | ADVERTISE_CSMA;
    mii_mgr_write(phyaddr, MII_ADVERTISE, regValue);

    mii_mgr_read(phyaddr, MII_BMCR, &regValue);
    printf("address 0:[%04x]\n",regValue);
    regValue &= ~(BMCR_LOOPBACK);
    regValue |= BMCR_ANENABLE | BMCR_ANRESTART; //restart AN
    mii_mgr_write(phyaddr, MII_BMCR, regValue);
}
#else
static void MDrv_NOE_Enable_Auto_Polling(u32 eport, u32 phyaddr)
{
    u32 regValue;

    regValue = le32_to_cpu(NOE_REG(NOE_MAC_PPSC));
    regValue |= (1<<31);        /* phy auto-polling enable */
    regValue &= ~(0x1f);        /* reset phy start addr */
    regValue &= ~(0x1f<<8);     /* reset phy end addr */

    if (eport == E_HAL_NOE_EPORT_GE1) {
        regValue |= (phyaddr << 0);//setup PHY address for auto polling (Start Addr).
        regValue |= (phyaddr << 8);// setup PHY address for auto polling (End Addr).
    }
    else {
        regValue |= ((phyaddr-1)&0x1f << 0);//setup PHY address for auto polling (Start Addr).
        regValue |= (phyaddr << 8);// setup PHY address for auto polling (End Addr).
    }
    /*kurtis: AN is strange*/
    NOE_REG(NOE_MAC_PPSC) = regValue;
}

static void _mdrv_noe_phyctrl_1000BASE_T_set(u32 phyaddr, u32 enable)
{
    u32 regValue = 0;
    mii_mgr_read(phyaddr, MII_CTRL1000, &regValue);
    if (enable) {
        regValue |= (ADVERTISE_1000HALF | ADVERTISE_1000FULL);
    }
    else {
        regValue &= ~(ADVERTISE_1000HALF | ADVERTISE_1000FULL);
    }
    mii_mgr_write(phyaddr, MII_CTRL1000, regValue);
}


static void _mdrv_noe_phyctrl_speed_set(u32 phyaddr, u32 speed)
{
    u32 regValue = 0;

    if (speed & DRV_PHY_SPEED_10) {
        regValue |= ADVERTISE_PAUSE_CAP | ADVERTISE_10FULL |\
                        ADVERTISE_10HALF | ADVERTISE_CSMA;
    }

    if (speed & DRV_PHY_SPEED_100) {
        regValue |= ADVERTISE_PAUSE_CAP | ADVERTISE_100HALF |\
                        ADVERTISE_100FULL | ADVERTISE_CSMA;
    }

    if (speed & DRV_PHY_SPEED_1000) {
        _mdrv_noe_phyctrl_1000BASE_T_set(phyaddr, TRUE);
        regValue |= ADVERTISE_PAUSE_CAP | ADVERTISE_1000XHALF |\
                        ADVERTISE_1000XFULL | ADVERTISE_CSMA;
    }
    else {
        _mdrv_noe_phyctrl_1000BASE_T_set(phyaddr, FALSE);
    }

    mii_mgr_write(phyaddr, MII_ADVERTISE, regValue);
}

static int _mdrv_noe_is_atheros_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if (!mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)) {
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }
    if (!mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)) {
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if ((phy_id0 == EV_ATHEROS_PHY_ID0) && (phy_id1 == EV_ATHEROS_PHY_ID1)) {
        return 1;
    }
    return 0;
}

static int _mdrv_noe_is_marvell_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if( ! mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)){
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }

    if( ! mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)){
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if((phy_id0 == EV_MARVELL_PHY_ID0) && (phy_id1 == EV_MARVELL_PHY_ID1)) {
        return 1;
    }

    return 0;
}

static int _mdrv_noe_is_vtss_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if( ! mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)){
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }

    if( ! mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)){
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if((phy_id0 == EV_VTSS_PHY_ID0) && (phy_id1 == EV_VTSS_PHY_ID1)) {
        return 1;
    }

    return 0;
}

static int _mdrv_noe_is_icplus_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if( ! mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)){
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }

    if( ! mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)){
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if((phy_id0 == EV_ICPLUS_PHY_ID0) && ((phy_id1 & 0xfff0)== EV_ICPLUS_PHY_ID1)) {
        return 1;
    }

    return 0;
}


static int _mdrv_noe_is_realtek_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if( ! mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)){
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }

    if( ! mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)){
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if((phy_id0 == EV_RTK_PHY_ID0) && (phy_id1 == EV_RTK_PHY_ID1)) {
        return 1;
    }

    return 0;
}

static int _mdrv_noe_is_mstar_gigaphy(u32 phyaddr)
{
    u32 phy_id0 = 0, phy_id1 = 0;

    if( ! mii_mgr_read(phyaddr, MII_PHYSID1, &phy_id0)){
        printf("\n Read PhyID 0 is Fail!!\n");
        phy_id0 = 0;
    }

    if( ! mii_mgr_read(phyaddr, MII_PHYSID2, &phy_id1)){
        printf("\n Read PhyID 1 is Fail!!\n");
        phy_id1 = 0;
    }

    if((phy_id0 == EV_MSTAR_PHY_ID0) && (phy_id1 == EV_MSTAR_PHY_ID1)) {
        return 1;
    }

    return 0;
}

static int _mdrv_noe_wait_an_complete(void)
{
    u16 timeout = 0;
    u32 status = 0;
    u32 phyaddr;

    printf("Wait for Auto-Negotiation complete ...\n");
    phyaddr = MHal_NOE_Get_PinMux_Phyaddr();
    do {
        mii_mgr_read(phyaddr, MII_BMSR, &status);
        timeout++;
        mdelay(1000);
    } while((!(status & BMSR_ANEGCOMPLETE)) && (timeout < NOE_AUTO_NEGOTIATION_TIMEOUT));

    if (timeout == 10) {
        printf("Auto-Negotiation failed!!\n");
        return FALSE;
    }
    else {
        return TRUE;
    }
}
#endif

static void MDrv_NOE_Eth_Init_MAC_PHY(void)
{
    u32 pinmux_eport, phyaddr;

    pinmux_eport = MHal_NOE_Get_PinMux_Eport();
    phyaddr      = MHal_NOE_Get_PinMux_Phyaddr();

#if defined(MSTAR_NOE_USE_FPGA_ENV)
    MDrv_NOE_Setup_FPGA_Environment(pinmux_eport, phyaddr);
#else   // K7U_ASIC
    u32 regValue = 0;
    u32 speed = 0;
#if (NOE_CONFIG_PHY_SPEED == NOE_USE_AUTO_NEGOTIATION)
#if defined(MSTAR_INTERNAL_GPHY_TO_EPHY)
    speed = DRV_PHY_SPEED_10 | DRV_PHY_SPEED_100;
#else
    speed = DRV_PHY_SPEED_10 | DRV_PHY_SPEED_100 | DRV_PHY_SPEED_1000;
#endif
#elif (NOE_CONFIG_PHY_SPEED == NOE_FORCE_SPEED_1000)
    speed = DRV_PHY_SPEED_1000;
#elif (NOE_CONFIG_PHY_SPEED == NOE_FORCE_SPEED_100)
    speed = DRV_PHY_SPEED_100;
#elif (NOE_CONFIG_PHY_SPEED == NOE_FORCE_SPEED_10)
    speed = DRV_PHY_SPEED_10;
#endif

    _mdrv_noe_phyctrl_speed_set(phyaddr, speed);

    if (_mdrv_noe_is_realtek_gigaphy(phyaddr)) {
        /* Add delay to RX_CLK for RXD Outputs */
        mii_mgr_write(phyaddr, 0x1f, 0x0007);
        mii_mgr_write(phyaddr, 0x1e, 0x00a4);
        mii_mgr_write(phyaddr, 0x1c, 0xad91);
        mii_mgr_write(phyaddr, 0x1f, 0x0000);
    }
    else if (_mdrv_noe_is_marvell_gigaphy(phyaddr)) {
        mii_mgr_read(phyaddr, 20, &regValue);
        regValue |= 1<<7; //Add delay to RX_CLK for RXD Outputs
        mii_mgr_write(phyaddr, 20, regValue);

        mii_mgr_read(phyaddr, 0, &regValue);
        regValue |= 1<<15; //PHY Software Reset
        mii_mgr_write(phyaddr, 0, regValue);
    }
    else if (_mdrv_noe_is_vtss_gigaphy(phyaddr)) {
        /* Add delay to RX_CLK for RXD Outputs */
        mii_mgr_write(phyaddr, 31, 0x0001); //extended page
        mii_mgr_read(phyaddr, 28, &regValue);
        printf("Vitesse phy skew: %x --> ", regValue);
        regValue |= (0x3<<12); // RGMII RX skew compensation= 2.0 ns
        regValue &= ~(0x3<<14);// RGMII TX skew compensation= 0 ns
        mii_mgr_write(phyaddr, 28, regValue);
        mii_mgr_write(phyaddr, 31, 0x0000); //main registers
    }
    else if (_mdrv_noe_is_atheros_gigaphy(phyaddr)) {
        /* Add delay to RX_CLK for RXD Outputs */
    }
    else if (_mdrv_noe_is_icplus_gigaphy(phyaddr)) {
        /* Add delay to RX_CLK for RXD Outputs */
    }
    else if (_mdrv_noe_is_mstar_gigaphy(phyaddr)) {
        printf("isMstarGigaPHY\n");
    }
    else {
        printf("Unknown GigaPHY!!\n");
    }

    /* Disable pause ability */
    printf("Disable pause ability\n");
    mii_mgr_read(phyaddr, MII_ADVERTISE, &regValue);
    regValue &= ~(ADVERTISE_PAUSE_CAP); //disable pause ability
    mii_mgr_write(phyaddr, MII_ADVERTISE, regValue);

    mii_mgr_read(phyaddr, MII_BMCR, &regValue);
    regValue &= ~(BMCR_LOOPBACK);
    regValue |= BMCR_ANENABLE | BMCR_ANRESTART; //restart AN
    mii_mgr_write(phyaddr, MII_BMCR, regValue);

    MDrv_NOE_Enable_Auto_Polling(pinmux_eport, phyaddr);

    if (pinmux_eport == E_HAL_NOE_EPORT_GE1) {
        NOE_REG(NOE_MAC_P1_MCR) = 0x21056300;//(P0, Auto mode)
        NOE_REG(NOE_MAC_P2_MCR) = 0x00008000;//(P1, Down)
        MHal_NOE_Set_GDM_Forward_Port(pinmux_eport);
    }
    else {
        NOE_REG(NOE_MAC_P2_MCR) = 0x21056300;//(P1, Auto mode)
        NOE_REG(NOE_MAC_P1_MCR) = 0x00008000;//(P0, Down)
        MHal_NOE_Set_GDM_Forward_Port(pinmux_eport);
    }
#endif
}

static int MDrv_NOE_Eth_Setup(struct eth_device* dev)
{
#ifndef MSTAR_NOE_USE_FPGA_ENV
    u32 eport, connect, regValue;
    eport = MHal_NOE_Get_PinMux_Eport();
    connect = MHal_NOE_Get_PinMux_Connect();
#endif

    printf("\n Waitting for RX_DMA_BUSY status Start... ");
    while(1)
        if(!_mdrv_noe_is_DMA_busy(dev))
            break;
    printf("done\n\n");

    MDrv_NOE_Eth_Init_MAC_PHY();

    MHal_EMAC_Write_SA1_MAC_Address(dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],\
        dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);

    MDrv_NOE_Eth_Init_FrameEngine();

#ifndef MSTAR_NOE_USE_FPGA_ENV
    if (_mdrv_noe_wait_an_complete()) {
        if (eport == E_HAL_NOE_EPORT_GE1 && connect == E_HAL_NOE_GMAC_TO_INTERNAL_GPHY) {
            regValue = (NOE_REG(NOE_MAC_P1_SR)) >> 2;
            if (((regValue & 0x3) == NOE_MAC_SPD_STATUS_10M) || ((regValue & 0x3) == NOE_MAC_SPD_STATUS_100M)) {
                MHal_NOE_Set_IGPHY_MII();
            }
        }
    }
#endif

    MDrv_NOE_Start(dev);
    return 1;
}

static int MDrv_NOE_Eth_Send(struct eth_device* dev, volatile void *packet, int length)
{
    int     status = -1;
    int     i;
    int     retry_count = 0, temp;
    unsigned long   addr, len;
Retry:
    if (retry_count > 10) {
        return (status);
    }

    if (length <= 0) {
        printf("%s: bad packet size: %d\n", dev->name, length);
        return (status);
    }

    addr = (unsigned long)&tx_ring0_cache[0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    len = (NOE_NUM_TX_DESC * sizeof(tx_ring0_cache[0]) + 2 * CONFIG_SYS_CACHELINE_SIZE) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    invalidate_dcache_range(addr, len);

    Chip_Flush_Memory();

    for(i = 0; tx_ring0[tx_cpu_owner_idx0].txd_info2.DDONE_bit == 0 ; i++)
    {
        if (i >= TOUT_LOOP) {
            printf("TX DDONE_bit is abnormal!!\n");
            goto Done;
        }
    }

    temp = NOE_REG(TX_DTX_IDX0);

    if(temp == (tx_cpu_owner_idx0+1) % NOE_NUM_TX_DESC) {
        puts(" @ ");
        goto Done;
    }

    tx_ring0[tx_cpu_owner_idx0].txd_info1.SDP0 = cpu_to_le32(phys_to_bus((u32)packet));
    tx_ring0[tx_cpu_owner_idx0].txd_info2.SDL0 = length;

#if 0
    printf("==========TX==========(CTX=%d)\n",tx_cpu_owner_idx0);
    printf("tx_ring0[tx_cpu_owner_idx0].txd_info1 =%x\n",tx_ring0[tx_cpu_owner_idx0].txd_info1);
    printf("tx_ring0[tx_cpu_owner_idx0].txd_info2 =%x\n",tx_ring0[tx_cpu_owner_idx0].txd_info2);
    printf("tx_ring0[tx_cpu_owner_idx0].txd_info3 =%x\n",tx_ring0[tx_cpu_owner_idx0].txd_info3);
    printf("tx_ring0[tx_cpu_owner_idx0].txd_info4 =%x\n",tx_ring0[tx_cpu_owner_idx0].txd_info4);
#endif

    tx_ring0[tx_cpu_owner_idx0].txd_info2.DDONE_bit = 0;
    status = length;

    addr = (unsigned long)packet & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    len = ((unsigned long)length + 2 * CONFIG_SYS_CACHELINE_SIZE ) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    flush_cache(addr, len);

    addr = (unsigned long)&tx_ring0_cache[0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    len = (NOE_NUM_TX_DESC * sizeof(tx_ring0_cache[0]) + 2 * CONFIG_SYS_CACHELINE_SIZE) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
    flush_cache(addr, len);

    tx_cpu_owner_idx0 = (tx_cpu_owner_idx0+1) % NOE_NUM_TX_DESC;
    NOE_REG(TX_CTX_IDX0) = cpu_to_le32((u32)tx_cpu_owner_idx0);

    if (noe_basic_cfg.loopback == E_NOE_BIST_MODE_PHY_LOOPBACK) {
        printf("\nTX Data:\n");
        memcpy(loopback_buf, (char *)packet, length);
        _mdrv_noe_print_packet((u8 *)packet, length);
    }

    return status;
Done:
    udelay(500);
    retry_count++;
    goto Retry;
}


static int MDrv_NOE_Eth_Recv(struct eth_device* dev)
{
    return _mdrv_noe_eth_recv(FALSE, NULL);
}

static int _mdrv_noe_eth_recv(u8 isLwip, void (*rx_callback)(volatile void *, int))
{
    int length = 0;
    int inter_loopback_cnt = 0;
    u32 *rxd_info;
    unsigned long   addr, len;
    int i, rx_dma_owner_idx_tmp = 0;

    for (; ; ) {
        addr = (unsigned long)&rx_ring_cache[0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
        len = (NOE_NUM_RX_DESC * sizeof(rx_ring_cache[0]) + 2 * CONFIG_SYS_CACHELINE_SIZE) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
        invalidate_dcache_range(addr, len);

        Chip_Flush_Memory();

        rxd_info = (u32 *)(&rx_ring[rx_dma_owner_idx0].rxd_info2);

        if ( (*rxd_info & BIT(31)) == 0 )
        {
            break;
        }

        udelay(1);
        length = rx_ring[rx_dma_owner_idx0].rxd_info2.PLEN0;

        if(length == 0)
        {
            printf("\n Warring!! Packet Length has error !!,In normal mode !\n");
        }

        if(rx_ring[rx_dma_owner_idx0].rxd_info4.SP == 6)
        {// Packet received from CPU port
            printf("\n Normal Mode,Packet received from CPU port,plen=%d \n",length);
            inter_loopback_cnt++;
            length = inter_loopback_cnt;//for return
        }
        else {
            addr = (unsigned long)NoeNetRxPackets[rx_dma_owner_idx0] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
            len = (length + 2 * CONFIG_SYS_CACHELINE_SIZE ) & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
            invalidate_dcache_range(addr, len);
            Chip_Flush_Memory();

            if (noe_basic_cfg.loopback == E_NOE_BIST_MODE_PHY_LOOPBACK) {
                printf("\nRX Data:\n");
                _mdrv_noe_print_packet((u8 *)NoeNetRxPackets[rx_dma_owner_idx0], length);

                if (memcmp(loopback_buf, (char *)NoeNetRxPackets[rx_dma_owner_idx0], length)) {
                    loopback_failed = 1;
                    printf(">>> RX Got the wrong content <<<\n");
                }
                else {
                    printf(">>> RX content is the same as TX <<<\n");
                }
            }
            else {
                if (isLwip)
                    rx_callback((void *)(NoeNetRxPackets[rx_dma_owner_idx0]), length);
                else
                    NetReceive((void *)(NoeNetRxPackets[rx_dma_owner_idx0]), length);
            }
        }

        rx_desc_cnt++;
        if (rx_desc_cnt == rx_desc_threshold) {
            for (i = rx_desc_threshold; i > 0; i--) {
                rx_dma_owner_idx_tmp = rx_dma_owner_idx0 - rx_desc_threshold + i;
                rx_ring[rx_dma_owner_idx_tmp].rxd_info2.DDONE_bit = 0;
                rx_ring[rx_dma_owner_idx_tmp].rxd_info2.LS0 = 0;
                rx_ring[rx_dma_owner_idx_tmp].rxd_info2.PLEN0 = PKTSIZE_ALIGN;

#if 0
        printf("=====RX=======(CALC=%d LEN=%d)\n",rx_dma_owner_idx0, length);
        printf("rx_ring[rx_dma_owner_idx0].rxd_info1 =%x\n",rx_ring[rx_dma_owner_idx0].rxd_info1);
        printf("rx_ring[rx_dma_owner_idx0].rxd_info2 =%x\n",rx_ring[rx_dma_owner_idx0].rxd_info2);
        printf("rx_ring[rx_dma_owner_idx0].rxd_info3 =%x\n",rx_ring[rx_dma_owner_idx0].rxd_info3);
        printf("rx_ring[rx_dma_owner_idx0].rxd_info4 =%x\n",rx_ring[rx_dma_owner_idx0].rxd_info4);
#endif
            }

            /* because hw will access the rx desc, sw flush the desc after updating it */
            addr = (unsigned long)&rx_ring_cache[rx_dma_owner_idx_tmp] & ~(CONFIG_SYS_CACHELINE_SIZE - 1);
            len = CONFIG_SYS_CACHELINE_SIZE;
            flush_cache(addr, len);

            /*  Move point to next RXD which wants to alloc*/
            NOE_REG(RX_CALC_IDX0) = cpu_to_le32((u32)rx_dma_owner_idx0);

            rx_desc_cnt = 0;
        }

        /* Update to Next packet point that was received.
         */
        rx_dma_owner_idx0 = (rx_dma_owner_idx0 + 1) % NOE_NUM_RX_DESC;

        //printf("\n ************************************************* \n");
        //printf("\n RX_CALC_IDX0=%d \n", NOE_REG(RX_CALC_IDX0));
        //printf("\n RX_DRX_IDX0 = %d \n",NOE_REG(RX_DRX_IDX0));
        //printf("\n ************************************************* \n");
    }
    return length;
}

static void MDrv_NOE_Eth_Halt(struct eth_device* dev)
{
     MDrv_NOE_Stop(dev);
}

static void _mdrv_noe_print_packet(u8 *buf, int length)
{
    int i;
    int remainder;
    int lines;
    printf("Packet of length %d \n", length );
    lines = length / 16;
    remainder = length % 16;

    for ( i = 0; i < lines ; i ++ ) {
        int cur;

        for ( cur = 0; cur < 8; cur ++ ) {
            u8 a, b;

            a = *(buf ++ );
            b = *(buf ++ );
            printf("%02X %02X ", a, b );
        }
        printf("\n");
    }
    for ( i = 0; i < remainder/2 ; i++ ) {
        u8 a, b;

        a = *(buf ++ );
        b = *(buf ++ );
        printf("%02X %02X ", a, b );
    }
    if (remainder % 2) {
        printf("%02X ", *buf);
    }
    printf("\n");
}


u8 MDrv_NOE_Bist(u8 mode)
{
    u16 timeout = 0;
    u32 status = 0;
    u32 regValue = 0;
    u32 phyaddr;

    noe_basic_cfg.loopback = mode;
    phyaddr = MHal_NOE_Get_PinMux_Phyaddr();

    switch (mode) {
        case E_NOE_BIST_MODE_PHY_LOOPBACK:
            loopback_failed = 0;
            regValue = BMCR_LOOPBACK | BMCR_SPEED100 | BMCR_FULLDPLX;
            mii_mgr_write(phyaddr, MII_BMCR, regValue);
            do {
                mii_mgr_read(phyaddr, MII_BMSR, &status);
                timeout++;
            } while((!(status & BMSR_LSTATUS)) && (timeout < 25000));
            if (timeout == 50000) {
                printf("Open PHY loopback mode timeout!!\n");
                MDrv_NOE_Eth_Init_MAC_PHY();
                return FALSE;
            }
            else {
                printf("Open PHY loopback mode!\n");
            }
            break;
        default:
            MDrv_NOE_Eth_Init_MAC_PHY();
            printf("NOE normal mode!\n");
            break;
    }
    return TRUE;
}

int MDrv_NOE_Initialize(const bd_t * bis)
{
    int i;
    char *s = NULL, *e = NULL;
    s = getenv(MACADDR_ENV);
    if (s)
    {
        for (i = 0; i < 6; ++i)
        {
            eth_sa1[i] = s ? simple_strtoul (s, &e, 16) : 0;
            if (s)
            {
                s = (*e) ? e + 1 : e;
            }
        }
    }
    else
    {
        eth_sa1[0] = MY_MAC[0];
        eth_sa1[1] = MY_MAC[1];
        eth_sa1[2] = MY_MAC[2];
        eth_sa1[3] = MY_MAC[3];
        eth_sa1[4] = MY_MAC[4];
        eth_sa1[5] = MY_MAC[5];
    }

    MDrv_NOE_Eth_Initialize(bis);
    return 0;
}

void MDrv_NOE_Dump_Register(void)
{
    int i;

    printf("\n|         <<GDMA statistics>>             |\n");
    printf("| GDMA1_RX_GBCNT  : %010u (Rx Good Bytes)   |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2400));
    printf("| GDMA1_RX_GPCNT  : %010u (Rx Good Pkts)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2408));
    printf("| GDMA1_RX_OERCNT : %010u (overflow error)  |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2410));
    printf("| GDMA1_RX_FERCNT : %010u (FCS error)       |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2414));
    printf("| GDMA1_RX_SERCNT : %010u (too short)       |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2418));
    printf("| GDMA1_RX_LERCNT : %010u (too long)        |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x241C));
    printf("| GDMA1_RX_CERCNT : %010u (checksum error)  |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2420));
    printf("| GDMA1_RX_FCCNT  : %010u (flow control)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2424));
    printf("| GDMA1_TX_SKIPCNT: %010u (about count) |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2428));
    printf("| GDMA1_TX_COLCNT : %010u (collision count) |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x242C));
    printf("| GDMA1_TX_GBCNT  : %010u (Tx Good Bytes)   |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2430));
    printf("| GDMA1_TX_GPCNT  : %010u (Tx Good Pkts)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2438));
    printf("|                       |\n");
    printf("| GDMA2_RX_GBCNT  : %010u (Rx Good Bytes)   |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2440));
    printf("| GDMA2_RX_GPCNT  : %010u (Rx Good Pkts)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2448));
    printf("| GDMA2_RX_OERCNT : %010u (overflow error)  |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2450));
    printf("| GDMA2_RX_FERCNT : %010u (FCS error)       |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2454));
    printf("| GDMA2_RX_SERCNT : %010u (too short)       |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2458));
    printf("| GDMA2_RX_LERCNT : %010u (too long)        |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x245C));
    printf("| GDMA2_RX_CERCNT : %010u (checksum error)  |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2460));
    printf("| GDMA2_RX_FCCNT  : %010u (flow control)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2464));
    printf("| GDMA2_TX_SKIPCNT: %010u (skip)            |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x2468));
    printf("| GDMA2_TX_COLCNT : %010u (collision)       |\n",       NOE_REG(NOE_FRAME_ENGINE_BASE+0x246C));
    printf("| GDMA2_TX_GBCNT  : %010u (Tx Good Bytes)   |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2470));
    printf("| GDMA2_TX_GPCNT  : %010u (Tx Good Pkts)    |\n",   NOE_REG(NOE_FRAME_ENGINE_BASE+0x2478));
    printf("+-----------------------------------------------+\n");
    printf("|         <<GDMA status>>                   |\n");
    printf("| GDMA_MAC_P1_SR  : 0x%x                    |\n",   NOE_REG(NOE_MAC_P1_SR));
    printf("| GDMA_MAC_P2_SR  : 0x%x                    |\n",   NOE_REG(NOE_MAC_P2_SR));
    printf("\n");

    printf("+-----------------------------------------------+\n");

    printf("\n|         <<TX/RX Ring Info>>             |\n");

    printf("TX TX_CTX_IDX0 = %d\n", NOE_REG(TX_CTX_IDX0));
    printf("TX TX_DTX_IDX0 = %d\n", NOE_REG(TX_DTX_IDX0));

    for (i=0; i<NOE_NUM_TX_DESC; i++) {
        printf("tx_ring0[%d].DDONE_bit: (%d)\n", i, tx_ring0[i].txd_info2.DDONE_bit);
    }

    printf("+-----------------------------------------------+\n");

    printf("RX RX_CALC_IDX0 = %d\n", NOE_REG(RX_CALC_IDX0));
    printf("RX RX_DRX_IDX0  = %d\n", NOE_REG(RX_DRX_IDX0));

    for ( i=0; i<NOE_NUM_RX_DESC; i++) {
        printf("rx_ring[%d].DDONE_bit: (%d)\n", i, rx_ring[i].rxd_info2.DDONE_bit);
    }

}

void MDrv_EMAC_SwReset(void)
{
    /* TFTP TBD */
}

int MDrv_EMAC_open_for_lwip() {
    return MDrv_NOE_Eth_Init(&noe_dev, NULL);
}

void MDrv_EMAC_close_for_lwip()
{
    MDrv_NOE_Eth_Halt(&noe_dev);
}

int MDrv_EMAC_tx_for_lwip(volatile void *tx_packet, int length)
{
    return MDrv_NOE_Eth_Send(&noe_dev, tx_packet, length);
}

int MDrv_EMAC_rx_for_lwip(void (*rx_callback)(volatile void *, int))
{
    return _mdrv_noe_eth_recv(TRUE, rx_callback);
}


int MDrv_NOE_eWave(int mode, int subitem)
{
    return MHal_NOE_eWave(mode, subitem);
}

int MDrv_NOE_Loopback_Is_Failed(void)
{
    return loopback_failed;
}

