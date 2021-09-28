#
# Copyright (c) 2016 - 2018 Amazon.com, Inc. or its affiliates.  All rights reserved.
#
# PROPRIETARY/CONFIDENTIAL.  USE IS SUBJECT TO LICENSE TERMS.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define temp_unlock
CFLAGS += -DUFBL_FEATURE_TEMP_UNLOCK

OBJS += \
    $(LOCAL_DIR)/temp_unlock.o

ifeq (true,$(strip $(FEATURE_LIBTOMCRYPT)))
CFLAGS += -DUFBL_FEATURE_TEMP_UNLOCK_LTC
OBJS += \
    $(LOCAL_DIR)/temp_unlock_ltc.o
endif
