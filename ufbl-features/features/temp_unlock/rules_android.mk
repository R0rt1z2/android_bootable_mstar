#
# Copyright (c) 2016 - 2018 Amazon.com, Inc. or its affiliates.  All rights reserved.
#
# PROPRIETARY/CONFIDENTIAL.  USE IS SUBJECT TO LICENSE TERMS.
#
UFBL_CFLAGS += -DUFBL_FEATURE_TEMP_UNLOCK

UFBL_SOURCE_FILES += \
    features/temp_unlock/temp_unlock_ltc.c \
    features/temp_unlock/temp_unlock.c
