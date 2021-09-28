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

LIB	=  $(obj)libmscustomer_src.a

#AOBJS	=

COBJS	=
#MstarCustomer:src
COBJS += ./MsCustomerRegister.o
COBJS += ./CusSystem.o
COBJS += ./CusUpgrade.o
COBJS += ./CusUpgradeUtility.o
COBJS += ./CusOsd.o
COBJS += ./CusPM.o
COBJS += ./CusLED.o
COBJS += ./CusCB.o
ifeq ($(CONFIG_URSA_8),y)
COBJS += ./CusUrsa8Init.o
endif
ifeq ($(CONFIG_MSTARI2C),y)
COBJS += ./CusI2C.o
endif
ifeq ($(CONFIG_DISPLAY_PICTURE_UPDATE),y)
COBJS += ./CusCastTV.o
endif
ifeq ($(CONFIG_MTK_BT_USB),y)
COBJS += ./mtk-bt/LD_btmtk_usb.o
COBJS += ./mtk-bt/LD_usbbt.o
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
