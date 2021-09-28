#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved. 
#
ifndef CONFIG_SPL_BUILD
obj-$(CONFIG_ENABLE_FOS_FLAGS) += ../../../../ufbl-features/features/fos_flags/fos_flags.o
obj-$(CONFIG_ENABLE_FOS_FLAGS) += ../../../../ufbl-features/features/fos_flags/fastboot_fos_flags.o
endif
