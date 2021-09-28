# Copyright (c) 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved

#
# Common list of objects files we want to include
#
FASTBOOT_LOCKDOWN_OBJECTS = amzn_fastboot_lockdown.o

# Common set of CFLAGS we need
FASTBOOT_LOCKDOWN_CFLAGS = \
    -std=c99 -ffunction-sections -fdata-sections \
    -DUFBL_FEATURE_FASTBOOT_LOCKDOWN

# Begin rules for kbuild U-Boot
ifeq (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))
ccflags-y    += $(FASTBOOT_LOCKDOWN_CFLAGS)
obj-y        += $(FASTBOOT_LOCKDOWN_OBJECTS)

else # default rules

# TBD: Support non-kbuild U-Boot

endif
