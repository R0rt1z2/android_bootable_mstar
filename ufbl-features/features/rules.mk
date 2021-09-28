#
# Copyright (c) 2016 - 2021 Amazon.com, Inc. or its affiliates.  All rights reserved.
#

# Include rules for each feature
FEATURE_DIR := $(GET_LOCAL_DIR)


ifeq (true,$(strip $(FEATURE_FASTBOOT_EXTENSIONS)))
include $(FEATURE_DIR)/fastboot_extensions/rules.mk
endif

ifeq (true,$(strip $(FEATURE_IDME)))
include $(FEATURE_DIR)/idme/rules.mk
endif

ifeq (true,$(strip $(FEATURE_COMMON_OPENSSL)))
include $(FEATURE_DIR)/common_openssl/rules.mk
endif

ifeq (true,$(strip $(FEATURE_SECURE_BOOT)))
include $(FEATURE_DIR)/secure_boot/rules.mk
endif

ifeq (true,$(strip $(FEATURE_LK_RAMDUMP)))
include $(FEATURE_DIR)/ramdump/rules.mk
endif

ifeq (true,$(strip $(FEATURE_LIFE_CYCLE_REASONS)))
include $(FEATURE_DIR)/life_cycle_reasons/rules.mk
endif

ifeq (true,$(strip $(FEATURE_UNLOCK)))
include $(FEATURE_DIR)/unlock/rules.mk
endif

ifeq (true,$(strip $(FEATURE_SECURE_FLASHING)))
include $(FEATURE_DIR)/secure_flashing/rules.mk
endif

ifeq (true,$(strip $(FEATURE_TEMP_UNLOCK)))
include $(FEATURE_DIR)/temp_unlock/rules.mk
endif

ifeq (true,$(strip $(FEATURE_ONETIME_UNLOCK)))
include $(FEATURE_DIR)/onetime_unlock/rules.mk
endif

ifeq (true,$(strip $(FEATURE_LIBTOMMATH)))
include $(FEATURE_DIR)/libtommath/rules.mk
endif

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
include $(FEATURE_DIR)/libtomcrypt/rules.mk
endif

ifeq (true,$(strip $(FEATURE_BCB)))
include $(FEATURE_DIR)/bcb/rules.mk
endif

ifeq (true,$(strip $(FEATURE_FASTBOOT_LOCKDOWN)))
include $(FEATURE_DIR)/fastboot_lockdown/rules.mk
endif

ifeq (true,$(strip $(FEATURE_FOS_FLAGS)))
include $(FEATURE_DIR)/fos_flags/rules.mk
endif

ifeq (true,$(strip $(FEATURE_PLATFORM_IMPL_MTK)))
include $(FEATURE_DIR)/platform_impl/mtk/rules.mk
endif

ifeq (true,$(strip $(FEATURE_USB_UPDATE_VERIFIER)))
include $(FEATURE_DIR)/usb_update_verifier/rules.mk
endif
