#
# Copyright 2019-2020 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../../include -I$(LOCAL_DIR)

ifeq ($(UFBL_PROJECT),)
$(error UFBL_PROJECT should be defined and passed in LK makefile from BOARD_UFBL_PROJECT)
endif

# Give clients a way to override the UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT in
# case UFBL_PROJECT points to a family of products
ifneq ($(FEATURE_PLATFORM_IMPL_MTK_PRODUCT_OVERRIDE),)
    UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT=$(FEATURE_PLATFORM_IMPL_MTK_PRODUCT_OVERRIDE)
else
    UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT=$(UFBL_PROJECT)
endif

# Define fastboot lockdown
CFLAGS += \
    -DUFBL_FEATURE_PLATFORM_IMPL_MTK \
    -DUFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT=$(UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT) \
    -DUFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT_UPCASE=$(shell echo $(UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT) | tr [:lower:] [:upper:])

ifeq (true,$(strip $(FEATURE_PLATFORM_IMPL_MTK_TARGET_DEVICE)))
ifneq ($(FEATURE_SECURE_BOOT),true)
$(error You should enable FEATURE_SECURE_BOOT before FEATURE_PLATFORM_IMPL_MTK_TARGET_DEVICE)
endif
CFLAGS += -DUFBL_FEATURE_PLATFORM_IMPL_MTK_TARGET_DEVICE
OBJS += \
    $(LOCAL_DIR)/target_device_mtk_impl.o
endif

ifeq (true,$(strip $(FEATURE_PLATFORM_IMPL_MTK_UNLOCK)))
ifneq ($(FEATURE_UNLOCK),true)
$(error You should enable FEATURE_UNLOCK before FEATURE_PLATFORM_IMPL_MTK_UNLOCK)
endif
CFLAGS += -include $(LOCAL_DIR)/unlock_key/$(UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT).h
CFLAGS += -DUFBL_FEATURE_PLATFORM_IMPL_MTK_UNLOCK
OBJS += \
    $(LOCAL_DIR)/unlock_mtk_impl.o
endif

