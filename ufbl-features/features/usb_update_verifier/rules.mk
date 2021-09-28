#
# Copyright (c) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved
#

LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/../include -I$(LOCAL_DIR)

# Define usb verifier
CFLAGS += -DUFBL_FEATURE_USB_FLASH

OBJS += \
	$(LOCAL_DIR)/usb_verifier.o

