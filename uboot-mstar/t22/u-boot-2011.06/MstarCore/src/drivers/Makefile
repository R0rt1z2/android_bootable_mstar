#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
#CONFIG_MSTAR_CHIP_NAME := amber5
#CFLAGS += -DET_DEBUG -DDEBUG

LIB	= $(obj)libdrivers.a

COBJS =

#ifeq ($(ENABLE_POWER_MUSIC),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/audio/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/audio/
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/audio/MsHalAudio.o
COBJS += ./audio/MsDrvAudio.o
COBJS += ./audio/Audio_amplifier.o
#endif

ifeq ($(CONFIG_BOOTVIDEO),y)

# DEMUX
COBJS += ./demux/MsDrvDemux.o

# VIDEO
COBJS += ./video/MsDrvVideo.o
endif

#ifeq ($(ENABLE_GOP),y)
COBJS += ./gop/MsDrvGop.o
#endif

#ifeq ($(ENABLE_JPD),y)
COBJS += ./jpd/MsDrvJpd.o
#endif
# SYS ###
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/mhal_sys.o
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/


ifeq ($(CONFIG_SD),y)
CFLAGS += -I./sd/
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/
CPPFLAGS += -I./sd/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/

COBJS += ./sd/mmc.o
COBJS += ./sd/fcie_mmc.o
COBJS += ./sd/DrvSd.o
COBJS += ./sd/DrvSdioIPVerify.o
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/hal_sdio.o
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/hal_sdio_platform.o
endif

ifeq ($(CONFIG_MSTAR_FTL),y)
CFLAGS += -I./unfd/inc/common/
CFLAGS += -I./unfd/inc/api/
CFLAGS += -I./unfd/inc/config/
CPPFLAGS += -I./unfd/inc/common/
CPPFLAGS += -I./unfd/inc/api/
CPPFLAGS += -I./unfd/inc/config/

CFLAGS += -I./unfd/ftl/inc/
CPPFLAGS += -I./unfd/ftl/inc/

COBJS += ./unfd/ftl/ftl.o
COBJS += ./unfd/ftl/src/ftl_api.o
COBJS += ./unfd/ftl/src/ftl_hal.o
COBJS += ./unfd/ftl/src/ftl_utl.o
endif

ifeq ($(CONFIG_NAND),y)
CFLAGS += -I./unfd/inc/api/
CFLAGS += -I./unfd/inc/common/
CFLAGS += -I./unfd/inc/config/
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/
CPPFLAGS += -I./unfd/inc/api/
CPPFLAGS += -I./unfd/inc/common/
CPPFLAGS += -I./unfd/inc/config/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/

COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/drvNAND_platform.o
COBJS += ./unfd/src/common/drvNAND_hal.o
COBJS += ./unfd/src/common/drvNAND_ReadRetry.o
COBJS += ./unfd/nand.o
COBJS += ./unfd/src/common/drvNAND_utl.o
COBJS += ./unfd/src/common/drvNAND_ip_verify.o
COBJS += ./unfd/src/common/drvNAND_hal_v5.o
COBJS += ./unfd/src/common/drvNAND_ftl.o
COBJS += ./unfd/src/common/drvNAND_task.o
COBJS += ./unfd/src/common/drvNAND_ReadRetry_v5.o
COBJS += ./unfd/src/api/drvNAND_prg.o
endif

ifeq ($(CONFIG_SPI_NAND),y)
CFLAGS += -I./spinand/inc/common/
CFLAGS += -I./spinand/inc/config/
CPPFLAGS += -I./spinand/inc/common/
CPPFLAGS += -I./spinand/inc/config/
COBJS += ./spinand/src/config/$(CONFIG_MSTAR_CHIP_NAME)/drvSPINAND_hal.o
COBJS += ./spinand/src/common/drvSPINAND.o
COBJS += ./spinand/spinand.o
endif

ifeq ($(CONFIG_FRONT_END),y)
COBJS += ./tuner/Tuner.o
COBJS += ./cofdm/COFDM_Demodulator.o
endif

ifeq ($(CONFIG_OAD_IN_MBOOT),y)
COBJS += ./oad/msAPI_oad_biop.o
COBJS += ./oad/msAPI_oad_parser.o
COBJS += ./oad/msAPI_oad_process.o
COBJS += ./oad/msAPI_demux.o
COBJS += ./oad/cus_oad/msAPI_oad_scan.o
COBJS += ./oad/cus_oad/apiRfChannel.o
COBJS += ./oad/msAPI_Memory.o
COBJS += ./oad/oad_main.o
COBJS += ./iic/drvIIC.o
endif

ifeq ($(CONFIG_EMAC),y)
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/emac/mhal_emac.o
COBJS += ./emac/mdrv_emac.o
endif

ifeq ($(CONFIG_GMAC),y)
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/mhal_gmac.o
COBJS += ./gmac/mdrv_gmac.o
endif

ifeq ($(CONFIG_KEYPAD),y)

ifeq ($(CONFIG_Customer),y)
CFLAGS += -I$(TOPDIR)/MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
CPPFLAGS += -I$(TOPDIR)/MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
else
CFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
CPPFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
endif

COBJS += ./keypad/msKeypad.o
endif

ifeq ($(CONFIG_IR),y)
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/ir/msIR.o
else
ifeq ($(CONFIG_DYNAMIC_IR),y)
COBJS += ./swir/drvSWIR.o
endif
endif

ifeq ($(CONFIG_ENABLE_DEMURA),y)
COBJS += ./panel/MsDrvDemura.o
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/panel/halDemura.o
CPPFLAGS += -I./panel/
CFLAGS += -I./panel/
endif

ifeq ($(CONFIG_INX_NOVA_VB1),y)
COBJS += ./panel/panel_INX_NOVA_vb1.o
endif

ifeq ($(CONFIG_INX_VB1),y)
COBJS += ./panel/panel_INX_vb1.o
endif

ifeq ($(CONFIG_URSA_6M30),y)
COBJS += ./ursa/ursa_6m30.o
endif

ifeq ($(CONFIG_URSA_6M40),y)
COBJS += ./ursa/ursa_6m40.o
endif

ifeq ($(CONFIG_URSA_6M60),y)
COBJS += ./ursa/ursa_6m60.o
endif

ifeq ($(CONFIG_URSA6_VB1),y)
COBJS += ./ursa/ursa_6m38.o
endif

ifeq ($(CONFIG_NOVA_KS2),y)
COBJS += ./ursa/ursa_6m38.o
endif

ifeq ($(CONFIG_URSA7_VB1),y)
COBJS += ./ursa/ursa_7.o
endif

ifeq ($(CONFIG_URSA_8),y)
COBJS += ./ursa/ursa_8.o
endif

ifeq ($(CONFIG_URSA9_VB1),y)
COBJS += ./ursa/ursa_9.o
endif

ifeq ($(CONFIG_URSA_10),y)
COBJS += ./ursa/ursa_10.o
endif

ifeq ($(CONFIG_URSA20_VB1),y)
COBJS += ./ursa/ursa_common.o
COBJS += ./ursa/ursa_20.o
endif

ifeq ($(CONFIG_PNL_OLED_LG_SUPPORT),y)
COBJS += ./panel/panel_OLED_LG.o
endif

ifeq ($(CONFIG_URSA_UNION),y)
COBJS += ./ursa/ursa_common.o
COBJS += ./ursa/ursa_6m38.o
COBJS += ./ursa/ursa_9.o
COBJS += ./ursa/ursa_20.o
endif

ifeq ($(CONFIG_USB),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
endif

ifeq ($(CONFIG_USB_FASTBOOT_ENABLE),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb_udc/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb_udc/
CFLAGS += -I./usb_udc/inc
CPPFLAGS += -I./usb_udc/inc
COBJS += ./usb_udc/hal_usb_otg.o
COBJS += ./usb_udc/hal_usb_util.o
COBJS += ./usb_udc/hal_usb_function.o
COBJS += ./usb_udc/hal_usb_dma.o
COBJS += ./usb_udc/hal_usb_drc.o
COBJS += ./usb_udc/ubootglue_udc.o
endif

ifeq ($(CONFIG_OTG),y)
COBJS += ./otg/src/drvUSBDwld.o
COBJS += ./otg/src/Miscdata.o
COBJS += ./otg/src/ms_dma.o
COBJS += ./otg/src/ms_drc.o
COBJS += ./otg/src/ms_function.o
COBJS += ./otg/src/ms_msd_global.o
COBJS += ./otg/src/ms_otg.o
COBJS += ./otg/src/ms_usb.o
COBJS += ./otg/src/msd_fn_scsi.o
COBJS += ./otg/src/msd_fn_xfer.o
endif

ifeq ($(CONFIG_USB_XHCI),y)
COBJS += ./usb30/xhci-mstar.o
COBJS += ./usb30/xhci.o
COBJS += ./usb30/xhci-dbg.o
COBJS += ./usb30/xhci-mem.o
COBJS += ./usb30/xhci-ring.o
COBJS += ./usb30/xhci-lib.o
COBJS += ./usb30/xhci-parse.o
endif

ifeq ($(CONFIG_HDMI_TX),y)

CPPFLAGS += -I ../hal/$(CONFIG_MSTAR_CHIP_NAME)/hdmi/
CFLAGS += -I ../hal/$(CONFIG_MSTAR_CHIP_NAME)/hdmi/

ifeq ($(CONFIG_HDMITX_MSTAR_ROCKET),y)
COBJS +=  ./hdmitx/mstar/rocket/MsDrvRocket.o
endif
ifeq ($(CONFIG_HDMITX_MSTAR_ROCKET2),y)
COBJS +=  ./hdmitx/mstar/rocket2/MsDrvRocket.o
endif

ifeq ($(CONFIG_HDMITX_MSTAR),y)
COBJS +=  ./hdmitx/mstar/MsDrvHdmi.o
else ifeq ($(CONFIG_TV_HDMITX_MSTAR),y)
COBJS +=  ./hdmitx/mstar/MsDrvTvHdmi.o
else ifeq ($(CONFIG_HDMITX_NXP_TDA19988),y)
# CONFIG_HDMITX_NXP_TDA19988
COBJS +=  ./hdmitx/sde2/inc/M_supply.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_Edid.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_HDCP.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_InOut.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_local.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_Misc.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_State.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmicec/src/tmdlHdmiCEC.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmicec/src/tmdlHdmiCEC_local.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/cfg/TDA9989/tmdlHdmiTx_cfg.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/src/tmdlHdmiTx.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/src/tmdlHdmiTx_local.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/tst/tmdlHdmiTx_ExampleApplication/src/tmdlHdmiTx_ExampleApplicationArm7_NO_RTOS.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/tst/tmdlHdmiTx_ExampleApplication/src/tmdlHdmiTx_IW_RTX.o
endif
endif
COBJS += ./MDrv_Sys.o
COBJS += ./MsDrvCache.o

# MIU
COBJS += ./miu/MsDrvMiu.o
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/miu/mhal_miu.o
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/miu/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/miu/

# STR
ifeq ($(CONFIG_MSTAR_STR_ENABLE),y)
COBJS += ./str/MsDrvStr.o
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/str/mhal_str.o
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/str/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/str/
endif

ifeq ($(CONFIG_EMAC),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/emac/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/emac/
endif

ifeq ($(CONFIG_CVBS_OUT),y)
COBJS +=  ./ve/MsDrvVE.o
endif

ifeq ($(CONFIG_XC_Init),y)
COBJS +=  ./xc/MsDrvXC.o
endif

ifeq ($(CONFIG_GMAC),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/
endif

ifeq ($(CONFIG_UART_RX),y)
COBJS += ./uart/MsDrvUART.o
endif

ifeq ($(CONFIG_SECURITY_BOOT),y)
CSRCS= $(TOPDIR)/../MstarCore/src/drivers/efuse_key/$(CONFIG_MSTAR_CHIP_NAME)/
REPLACE_SRC_PATH=$(shell test -d $(CSRCS) && echo 1 )
ifeq ($(REPLACE_SRC_PATH),1)
#ifeq ($(CONFIG_MSTAR_KERES),y)
COBJS += ./efuse_key/$(CONFIG_MSTAR_CHIP_NAME)/TEE_SetKey.o
else
COBJS += ./efuse_key/default/TEE_SetKey.o
endif
endif

CFLAGS += -I./iic/
CFLAGS += -I ../hal/$(CONFIG_MSTAR_CHIP_NAME)/board/
CPPFLAGS += -I./iic/
CPPFLAGS += -I ../hal/$(CONFIG_MSTAR_CHIP_NAME)/board/
all:	$(LIB) $(AOBJS)

SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
