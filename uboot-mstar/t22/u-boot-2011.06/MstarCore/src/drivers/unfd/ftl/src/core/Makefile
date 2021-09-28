include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
CONFIG_CROSS_COMPILER_PREFIX := $(subst ",,$(CONFIG_CROSS_COMPILER_PREFIX))

LIB = $(obj)libmstarftlcore.a

all:    $(LIB)

$(LIB):
	@cp $(CONFIG_CROSS_COMPILER_PREFIX)mstarftlcore.z libmstarftlcore.a

#########################################################################

include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
