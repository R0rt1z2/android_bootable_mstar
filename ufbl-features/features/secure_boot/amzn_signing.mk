# Copyright 2016-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
#
# This function signs the Android boot image according
# to the UFBL common kernel signing scheme
#
# $(1): UFBL product name
# $(2): Raw Android boot image (must end with .raw)
define sign-amzn-image
$(if $(filter-out .raw,$(suffix $(2))),\
  $(eval $(error $(2) must end in .raw)))
$(hide) $(AMZN_KERNEL_SIGNER_CMD) \
  -c $(AMZN_KERNEL_DEV_CERT).pem \
  -k $(AMZN_KERNEL_DEV_CERT).key \
  -f $(AMZN_KERNEL_SIGNER_PATH)/$(1).conf \
  $(2)
$(hide) mv -f $(2).signed $(basename $(2))
endef
