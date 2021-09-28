#Copyright (C) 2016 Amazon.com Inc. or its affiliates.  All Rights Reserved
LOCAL_DIR := $(GET_LOCAL_DIR)

# List of features for this product
#FEATURE_FASTBOOT_EXTENSIONS := true
FEATURE_IDME := true

FEATURE_COMMON_OPENSSL := false
FEATURE_SECURE_BOOT := false
FEATURE_LIFE_CYCLE_REASONS := false
FEATURE_LK_RAMDUMP := false
FEATURE_UNLOCK := false
FEATURE_FASTBOOT_LOCKDOWN := false

# This is required for fastboot extensions
DEFINES += UFBL_PLATFORM_MTK
DEFINES += UFBL_TESTS
DEFINES += IDME_NUM_OF_EMMC_BLOCKS=30
DEFINES += IDME_UPDATE_TABLE=1
#DEFINES += MTK_MMC_HACK


# After defining list of features, include
# ufbl common definitions
include $(LOCAL_DIR)/ufbl_common.mk

