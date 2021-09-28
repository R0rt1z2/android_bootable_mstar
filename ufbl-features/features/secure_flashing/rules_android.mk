#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
UFBL_CFLAGS += -DUFBL_FEATURE_SECURE_FLASHING

UFBL_SOURCE_FILES += \
    features/secure_flashing/secure_flashing_ltc.c \
    features/secure_flashing/secure_flashing.c
