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

LIB	= $(obj)libursa.a

COBJS =

COBJS += ./ursa_vb1.o

ifeq ($(CONFIG_URSA_6M30),y)
COBJS += ./ursa_6m30.o
endif

ifeq ($(CONFIG_URSA_6M40),y)
COBJS += ./ursa_6m40.o
endif

ifeq ($(CONFIG_URSA6_VB1),y)
COBJS += ./ursa_6m38.o
endif

ifeq ($(CONFIG_NOVA_KS2),y)
COBJS += ./ursa_6m38.o
endif

ifeq ($(CONFIG_URSA7_VB1),y)
COBJS += ./ursa_7.o
endif

ifeq ($(CONFIG_URSA_8),y)
COBJS += ./ursa_8.o
endif

ifeq ($(CONFIG_URSA9_VB1),y)
COBJS += ./ursa_9.o
endif

ifeq ($(CONFIG_URSA_10),y)
COBJS += ./ursa_10.o
endif

ifeq ($(CONFIG_URSA20_VB1),y)
COBJS += ./ursa_20.o
endif

ifeq ($(CONFIG_URSA_UNION),y)
COBJS += ./ursa_common.o
COBJS += ./ursa_6m38.o
COBJS += ./ursa_9.o
COBJS += ./ursa_20.o
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
