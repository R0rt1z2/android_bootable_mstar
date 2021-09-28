#
# (C) Copyright 2004-2006
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

LIB	=  $(obj)libmscore_cmd.a

#AOBJS	=

COBJS	=
ifeq ($(CONFIG_UARTCOMM),y)
COBJS += ./cmd_MsUartComm.o
endif
ifeq ($(CONFIG_CVBS_OUT),y)
COBJS += ./cmd_MsVE.o
endif
ifeq ($(CONFIG_XC_Init),y)
COBJS += ./cmd_MsXC.o
endif
ifeq ($(CONFIG_DOLBY),y)
COBJS += ./cmd_MsMspi.o
endif
ifeq ($(CONFIG_MSTAR_STR_ENABLE),y)
COBJS += ./cmd_Str.o
endif

#MstarCore:cmd
ifeq ($(CONFIG_EMAC),y)
COBJS += ./cmd_emac.o
endif
ifeq ($(CONFIG_GMAC),y)
COBJS += ./cmd_emac.o
endif
COBJS += ./cmd_spi.o
COBJS += ./cmd_MsMemPerformance.o
COBJS += ./cmd_MsTemplate.o
COBJS += ./cmd_MsSystem.o
COBJS += ./cmd_MsSysUtility.o
COBJS += ./cmd_MsPanel.o

ifdef CONFIG_CMD_MMC
COBJS += ./cmd_MsMmc.o
endif
ifdef CONFIG_MSTAR_FTL
COBJS += ./cmd_MsFtl.o
endif
ifdef CONFIG_SD
COBJS += ./cmd_MsSd.o
endif
ifeq ($(CONFIG_FRONT_END),y)
COBJS += ./cmd_tuner.o
endif
ifeq ($(CONFIG_OAD_IN_MBOOT),y)
COBJS += ./cmd_dmx.o
endif
ifeq ($(CONFIG_HDMI_TX),y)
COBJS += ./cmd_MsHdmi.o
endif
ifeq ($(CONFIG_EEPROM),y)
COBJS += ./cmd_MsEeprom.o
endif
ifeq ($(CONFIG_UART_RX),y)
COBJS += ./cmd_MsUart.o
endif
COBJS += ./cmd_MsUrsa.o

SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

#CPPFLAGS += -I..

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
