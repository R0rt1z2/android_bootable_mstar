#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define secure boot
CFLAGS += -DUFBL_FEATURE_SECURE_BOOT

OBJS += \
    $(LOCAL_DIR)/image_verify.o

ifeq (true,$(strip $(FEATURE_COMMON_OPENSSL)))
CFLAGS += -DUFBL_FEATURE_SECURE_BOOT_OPENSSL
OBJS += \
	$(LOCAL_DIR)/image_verify_openssl.o
endif

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
CFLAGS += -DUFBL_FEATURE_SECURE_BOOT_LTC
OBJS += \
	$(LOCAL_DIR)/image_verify_ltc.o
endif
