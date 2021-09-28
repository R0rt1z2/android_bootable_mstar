#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define secure_flashing
CFLAGS += -DUFBL_FEATURE_SECURE_FLASHING

OBJS += \
    $(LOCAL_DIR)/secure_flashing.o

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
CFLAGS += -DUFBL_FEATURE_SECURE_FLASHING_LTC
OBJS += \
    $(LOCAL_DIR)/secure_flashing_ltc.o
endif
