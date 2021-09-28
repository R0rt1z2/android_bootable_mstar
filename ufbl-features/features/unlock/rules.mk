#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#

LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define unlock
CFLAGS += -DUFBL_FEATURE_UNLOCK

OBJS += \
	$(LOCAL_DIR)/unlock.o

ifeq (true,$(strip $(FEATURE_COMMON_OPENSSL)))
CFLAGS += -DUFBL_FEATURE_UNLOCK_OPENSSL
OBJS += \
	$(LOCAL_DIR)/unlock_openssl.o
endif

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
CFLAGS += -DUFBL_FEATURE_UNLOCK_LTC
OBJS += \
	$(LOCAL_DIR)/unlock_ltc.o
endif
