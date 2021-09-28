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

LIB	= $(obj)libusb20.a

COBJS =

ifeq ($(CONFIG_USB),y)
CFLAGS += -I../../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
CPPFLAGS += -I../../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
ifeq ($(CONFIG_MSTAR_MONACO),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MIAMI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MUJI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_CELTICS),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MONET),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MESSI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MUNICH),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MULAN),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_KRATOS),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_KIWI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MANHATTAN),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MASERATI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_KANO),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_CURRY),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MASERATI),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MACAN),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_KAYLA),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
else ifeq ($(CONFIG_MSTAR_MAXIM),y)
COBJS += ./drvhostlib.o
COBJS += ./ubootglue.o
COBJS += ./usb_lan.o
COBJS += ./ax88772a.o
COBJS += ./rtl8150.o
COBJS += ./pegasus.o
endif
endif

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
