#
# Copyright (c) 2016 - 2021 Amazon.com, Inc. or its affiliates.  All rights reserved.
#


# Include rules for each feature
FEATURE_DIR = $(UFBL_LIB_PATH)
UFBL_LIBS =

ifeq (true,$(strip $(FEATURE_IDME)))
UFBL_LIBS += $(FEATURE_DIR)/idme/libidme.o
endif

ifeq (true,$(strip $(FEATURE_COMMON_OPENSSL)))
UFBL_LIBS += $(FEATURE_DIR)/common_openssl/libopenssl.o
endif

ifeq (true,$(strip $(FEATURE_SECURE_BOOT)))
UFBL_LIBS += $(FEATURE_DIR)/secure_boot/libsecure_boot.o
endif

ifeq (true,$(strip $(FEATURE_LIBTOMMATH)))
UFBL_LIBS += $(FEATURE_DIR)/libtommath/libtommath.o
endif

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
UFBL_LIBS += $(FEATURE_DIR)/libtomcrypt/libtomcrypt.o
endif

ifeq (true,$(strip $(FEATURE_UNLOCK)))
UFBL_LIBS += $(FEATURE_DIR)/unlock/libunlock.o
endif

ifeq (true,$(strip $(FEATURE_SECURE_FLASHING)))
UFBL_LIBS += $(FEATURE_DIR)/secure_flashing/libsecure_flashing.o
endif

ifeq (true,$(strip $(FEATURE_TEMP_UNLOCK)))
UFBL_LIBS += $(FEATURE_DIR)/temp_unlock/libtemp_unlock.o
endif

ifeq (true,$(strip $(FEATURE_ONETIME_UNLOCK)))
UFBL_LIBS += $(FEATURE_DIR)/onetime_unlock/libonetime_unlock.o
endif

ifeq (true,$(strip $(FEATURE_FASTBOOT_LOCKDOWN)))
UFBL_LIBS += $(FEATURE_DIR)/fastboot_lockdown/libfastboot_lockdown.o
endif

ifeq (true,$(strip $(FEATURE_USB_UPDATE_VERIFIER)))
UFBL_LIBS += $(FEATURE_DIR)/usb_update_verifier/libusbverifier.o
endif
