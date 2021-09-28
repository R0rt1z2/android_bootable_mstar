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

LIB	=  $(obj)libmsapp_cmd.o

#AOBJS	=

COBJS	=
#MstarApp:cmd
COBJS += ./cmd_MsBootLogo.o
ifeq ($(CONFIG_BOOTVIDEO),y)
COBJS += ./cmd_MsBootVideo.o
endif
COBJS += ./cmd_MsUpgrade.o
COBJS += ./cmd_MsUpgradeUtility.o
COBJS += ./cmd_MsBoot.o
COBJS += ./cmd_MsTest.o
COBJS += ./cmd_MsUtility.o
COBJS += ./cmd_MsEnvironment.o
COBJS += ./cmd_MsOSD.o
COBJS += ./cmd_MsFat.o
COBJS += ./cmd_MsMiu.o
ifeq ($(CONFIG_SECURITY_BOOT),y)
COBJS += ./cmd_MsSecure.o
endif
COBJS += ./cmd_MsDebug.o
COBJS += ./cmd_MsRawIO.o
COBJS += ./cmd_MsMmap.o

ifeq ($(CONFIG_MSTAR_FASTBOOT_ENABLE),y)
COBJS += ./cmd_MsFastboot.o
endif
SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

#CPPFLAGS += -I..

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
