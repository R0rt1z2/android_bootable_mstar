#
# Copyright (c) 2016 - 2018 Amazon.com, Inc. or its affiliates.  All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define onetime_unlock
CFLAGS += -DUFBL_FEATURE_ONETIME_UNLOCK

OBJS += \
    $(LOCAL_DIR)/onetime_unlock.o

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
CFLAGS += -DUFBL_FEATURE_ONETIME_UNLOCK_LTC
OBJS += \
    $(LOCAL_DIR)/onetime_unlock_ltc.o
endif
