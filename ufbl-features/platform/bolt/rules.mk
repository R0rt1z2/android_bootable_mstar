#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)

OBJS += \
    $(LOCAL_DIR)/ufbl_bolt_compat.o
