#
# Copyright (C) 2007-2008 Industrie Dial Face S.p.A.
# Luigi 'Comio' Mantellini (luigi.mantellini@idf-hit.com)
#
# (C) Copyright 2003-2006
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)libavb.o

CFLAGS += -DAVB_COMPILATION
CPPFLAGS += -DAVB_COMPILATION

SOBJS =

COBJS-libavb += \
    avb_sysdeps_posix.o \
    avb_chain_partition_descriptor.o \
    avb_crc32.o \
    avb_crypto.o \
    avb_descriptor.o \
    avb_footer.o \
    avb_hash_descriptor.o \
    avb_hashtree_descriptor.o \
    avb_kernel_cmdline_descriptor.o \
    avb_property_descriptor.o \
    avb_rsa.o \
    avb_sha512.o \
    avb_slot_verify.o \
    avb_util.o \
    avb_vbmeta_image.o \
    avb_version.o

#ifdef CONFIG_SHA256
COBJS-libavb += avb_sha256.o
#else
#COBJS-libavb += avb_gcpu_sha256.o
#endif

# COBJS-$(CONFIG_OF_LIBUFDT) += $(COBJS-libufdt)
COBJS-y += $(COBJS-libavb)

COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
