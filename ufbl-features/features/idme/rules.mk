#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../../include
INCLUDES += -I$(LOCAL_DIR)/include

ifneq ($(wildcard $(strip $(UFBL_HOST_PATH))/app/mt_boot),)
INCLUDES += -I$(strip $(UFBL_HOST_PATH))/app/mt_boot
endif

ifneq ($(wildcard $(strip $(UFBL_HOST_PATH))/lib/libfdt/include/),)
INCLUDES += -I$(strip $(UFBL_HOST_PATH))/lib/libfdt/include/
endif

# Define IDME
CFLAGS += -DUFBL_FEATURE_IDME

ifeq ($(FEATURE_IDME_ACE_FFS), true)
CFLAGS += -DUFBL_FEATURE_IDME_ACE_FFS
endif

OBJS += \
    $(LOCAL_DIR)/cmd_idme.o \
    $(LOCAL_DIR)/cmd_idme_v2_0.o \
    $(LOCAL_DIR)/idme_platform.o \
    $(LOCAL_DIR)/idme_utilities.o
