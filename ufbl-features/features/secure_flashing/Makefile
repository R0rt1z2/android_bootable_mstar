# Copyright (c) 2016-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved

#
# Common list of objects files we want to include
#
SECURE_FLASHING_OBJECTS = \
    secure_flashing.o \
    secure_flashing_ltc.o

# Common set of CFLAGS we need
SECURE_FLASHING_CFLAGS = \
    -DSUPPORT_UBOOT \
    -std=c99 \
    -ffunction-sections -fdata-sections \
    -DUFBL_FEATURE_SECURE_FLASHING \
    -DUFBL_FEATURE_SECURE_FLASHING_LTC \
    -DLTC_SOURCE \
    -DLTC_NO_FILE \
    -DUSE_LTM -DLTM_DESC \
    -D_WCHAR_T -DLTC_CLEAN_STACK

SECURE_FLASHING_HEADERS = \
    -I$(UFBL_PATH)/features/libtomcrypt/ \
    -I$(UFBL_PATH)/features/libtomcrypt/src/headers

# Begin rules for kbuild U-Boot
ifeq (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))

ccflags-y    += $(SECURE_FLASHING_CFLAGS) $(SECURE_FLASHING_HEADERS)
obj-y        += $(SECURE_FLASHING_OBJECTS)

else # default rules

include $(TOPDIR)/config.mk

SECURE_FLASHING_LIB = $(obj)libsecure_flashing.o

# Default CFLAGS
CFLAGS += \
    $(SECURE_FLASHING_CFLAGS) \
    $(SECURE_FLASHING_HEADERS)

COBJS := $(SECURE_FLASHING_OBJECTS)
SRCS  := $(COBJS:.o=.c)
OBJS  := $(addprefix $(obj),$(COBJS))

$(SECURE_FLASHING_LIB): $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

endif # (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))
