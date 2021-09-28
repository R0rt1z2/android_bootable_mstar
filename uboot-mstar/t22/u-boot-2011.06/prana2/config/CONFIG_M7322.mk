CC_TVOPTS += -DCHIP_FAMILY_TYPE=CHIP_FAMILY_M7322

# FRC:  0(SEC-R2), 1(FRC-R2)
FRC ?= 0

ifeq ($(FRC),1)
CC_TVOPTS += -DFRC_ENABLED=1
endif

ifeq ($(PLATFORM),MBoot)
CC_TVOPTS += -DMBOOT
endif

PRANA_SRC += \
	$(ROOT)/driver/core/device.c                    \
	$(ROOT)/driver/riu/drvGlobal.c

INC_DIR   += \
	-I$(ROOT)/driver

