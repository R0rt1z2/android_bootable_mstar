#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
#UFBL setting
ENABLE_UFBL_FEATURES := 1
ifeq ($(ENABLE_UFBL_FEATURES),1)
UFBL_FEATURES_ROOT := $(UFBL_PATH)
endif

ifeq ($(ENABLE_UFBL_FEATURES),1)
INCLUDES += -I$(INCLUDES) -I$(UFBL_FEATURES_ROOT)/include
CFLAGS   += -DCONFIG_UFBL
endif

ifeq ($(ENABLE_UFBL_FEATURES),1)
include $(UFBL_FEATURES_ROOT)/project/$(UFBL_PROJECT).mk
endif
