#
# Copyright (c) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved
#
UFBL_CFLAGS += -DUFBL_FEATURE_USB_UPDATE_VERIFIER

UFBL_SOURCE_FILES += \
	features/usb_update_verifier/usb_verifier.c
