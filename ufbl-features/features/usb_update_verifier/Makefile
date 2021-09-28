# Copyright (c) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved

#
# Common list of objects files we want to include
#
USB_FLASH_OBJECTS = \
    usb_verifier.o

# Common set of CFLAGS we need
USB_CFLAGS = \
    -DSUPPORT_UBOOT \
    -std=c99 \
    -ffunction-sections -fdata-sections \
    -DUFBL_FEATURE_USB_UPDATE_VERIFIER \
    -DLTC_SOURCE \
    -DLTC_NO_FILE \
    -DUSE_LTM -DLTM_DESC \
    -D_WCHAR_T -DLTC_CLEAN_STACK

USB_HEADERS = \
    -I$(UFBL_PATH)/features/libtomcrypt/ \
    -I$(UFBL_PATH)/features/libtomcrypt/src/headers

# Begin rules for kbuild U-Boot
ifeq (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))

ccflags-y    += $(USB_CFLAGS) $(USB_HEADERS)
obj-y        += $(USB_FLASH_OBJECTS)

else # default rules

include $(TOPDIR)/config.mk

USB_LIB = $(obj)libusbverifier.o

# Default CFLAGS
CFLAGS += \
    $(USB_CFLAGS) \
    $(USB_HEADERS)

COBJS := $(USB_FLASH_OBJECTS)
SRCS  := $(COBJS:.o=.c)
OBJS  := $(addprefix $(obj),$(COBJS))

$(USB_LIB): $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

endif # (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))
