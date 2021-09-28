#
# Copyright (c) 2016 - 2018 Amazon.com, Inc. or its affiliates.  All rights reserved.
#
UFBL_CFLAGS += -DUFBL_FEATURE_ONETIME_UNLOCK

UFBL_SOURCE_FILES += \
    features/onetime_unlock/onetime_unlock_ltc.c \
    features/onetime_unlock/onetime_unlock.c
