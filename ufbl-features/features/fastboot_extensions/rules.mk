#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../../include
ifneq (,$(filter ABC ABC, $(strip $(UFBL_PROJECT))))
INCLUDES += -I$(strip $(UFBL_HOST_PATH))/app/mt_boot
INCLUDES += -I$(strip $(UFBL_HOST_PATH))/platform/mt8135/include/platform/
endif


OBJS += \
    $(LOCAL_DIR)/fastboot_extensions.o
