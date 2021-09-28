#Copyright (C) 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved
LOCAL_DIR := $(GET_LOCAL_DIR)

# List of features for this product
#FEATURE_FASTBOOT_EXTENSIONS := true
#FEATURE_IDME := true
FEATURE_BCB := true

FEATURE_COMMON_OPENSSL := false
FEATURE_SECURE_BOOT := false
FEATURE_LIFE_CYCLE_REASONS := false
FEATURE_LK_RAMDUMP := false
FEATURE_UNLOCK := false
FEATURE_FASTBOOT_LOCKDOWN := false

DEFINES += UFBL_TESTS


# After defining list of features, include
# ufbl common definitions
include $(LOCAL_DIR)/ufbl_common.mk

