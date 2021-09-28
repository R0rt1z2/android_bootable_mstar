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

LIB	=  $(obj)libmscore_src.a

#AOBJS	=

COBJS	=
#MstarCore:src

# SYS ###
CFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/
CPPFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/
COBJS += ./MsInit.o
COBJS += ./apiIR.o
COBJS += ./msAPI_Power.o
COBJS += ./MsTemplate.o
COBJS += ./MsSystem.o
COBJS += ./MsSysUtility.o
COBJS += ./MsCoreRegister.o
COBJS += ./MsSetupTee.o
COBJS += ./msAPI_string.o

ifdef CONFIG_POINTS_HANDLER_ENABLE
COBJS += ./MsApiChkPoints.o
endif

ifeq ($(CONFIG_FRONT_END),y)
CPPFLAGS += -I./cofdm/
CFLAGS += -I./cofdm/
CPPFLAGS += -I./tuner/
CFLAGS += -I./tuner/
COBJS += ./msAPI_Tuner.o
endif

ifeq ($(CONFIG_KEYPAD),y)
ifeq ($(CONFIG_Customer),y)
CFLAGS += -I$(TOPDIR)/../MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
CPPFLAGS += -I$(TOPDIR)/../MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
else
CFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
CPPFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
endif
endif

#ifeq ($(ENABLE_POWER_MUSIC),y)
COBJS += ./MsApiAudio.o
#endif
ifeq ($(CONFIG_UARTCOMM),y)
COBJS += ./MsUartComm.o
endif
ifeq ($(CONFIG_CVBS_OUT),y)
COBJS += ./MsApiVE.o
endif
ifeq ($(CONFIG_XC_Init),y)
COBJS += ./MsApiXC.o
endif
ifeq ($(CONFIG_DOLBY),y)
COBJS += ./MsApiMspi.o
endif
COBJS += ./MsApiPanel.o
COBJS += ./MsApiGEGOP.o
COBJS += ./MsApiJpd.o
COBJS += ./MsApiPM.o
COBJS += ./MsApiSpi.o
COBJS += ./MsMemPerformance.o
ifdef CONFIG_CMD_MMC
COBJS += ./MsMmc.o
endif
ifeq ($(CONFIG_MSTAR_STR_ENABLE),y)
COBJS += ./MsStr.o
endif
ifdef CONFIG_MSTAR_FTL
COBJS += ./MsFtl.o
endif
ifeq ($(CONFIG_HDMI_TX),y)
COBJS += ./MsApiHdmi.o
endif
ifeq ($(CONFIG_EEPROM),y)
COBJS += ./MsEeprom.o
endif
ifeq ($(CONFIG_UART_RX),y)
COBJS += ./MsApiUart.o
endif
COBJS += ./MsApiUrsa.o

ifdef CONFIG_SD
COBJS += ./MsSd.o
endif

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
