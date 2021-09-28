#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

ifneq (,$(filter ABC, $(strip $(UFBL_PROJECT))))
INCLUDES += -I$(strip $(UFBL_HOST_PATH))/platform/mt8135/include/platform/
endif

# Define fastboot lockdown
CFLAGS += -DUFBL_FEATURE_FASTBOOT_LOCKDOWN

OBJS += \
    $(LOCAL_DIR)/amzn_fastboot_lockdown.o
