#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../../include
INCLUDES += -I$(LOCAL_DIR)/include

CFLAGS += -DUFBL_FEATURE_BCB

OBJS += \
    $(LOCAL_DIR)/bcb.o \

