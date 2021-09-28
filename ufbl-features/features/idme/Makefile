# Copyright (c) 2015-2020 Amazon.com, Inc. or its affiliates. All Rights Reserved

#
# Common list of objects files we want to include
#
IDME_OBJECTS += \
    cmd_idme.o \
    cmd_idme_v2_0.o \
    idme_utilities.o

IDME_CFLAGS += -DSUPPORT_UBOOT -DUFBL_FEATURE_IDME

IDME_HEADERS += \
    -I$(UFBL_PATH)/include \
    -I$(UFBL_PATH)/features/idme/include \

ifeq (yes,$(strip $(UFBL_PROJ_ABC)))
IDME_OBJECTS += idme_platform_ABC.o
else ifeq (yes,$(findstring yes,$(UFBL_PROJ_ABC) $(UFBL_PROJ_ABC) $(UFBL_PROJ_ABC) \
    $(UFBL_PROJ_LAGUNAB) $(UFBL_PROJ_HARMONY)))
IDME_OBJECTS += idme_platform_ABC.o
else ifeq (yes,$(findstring yes,$(UFBL_PROJ_ABC) $(UFBL_PROJ_ABC) $(UFBL_PROJ_ABC) \
    $(UFBL_PROJ_SOPHIA) $(UFBL_PROJ_ABC) $(UFBL_PLAT_MTK_T31) $(UFBL_PROJ_MONTANA)))
IDME_OBJECTS += idme_platform_mstar.o
else ifeq (yes, $(findstring yes, $(UFBL_PROJ_ABC) $(UFBL_PROJ_ABC) $(UFBL_PROJ_LAGUNAF) \
    $(UFBL_PROJ_HOYA)))
IDME_OBJECTS += idme_platform_amlogic.o
else ifeq (yes, $(findstring yes, $(UFBL_PROJ_ABC)))
IDME_OBJECTS += idme_platform_ABC.o
else
IDME_OBJECTS += idme_platform_bcrm.o
endif

# Begin rules for kbuild U-Boot
ifeq (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))

ccflags-y += $(IDME_CFLAGS) $(IDME_HEADERS)
obj-y += $(IDME_OBJECTS)

else # default rules

include $(TOPDIR)/config.mk

IDME_LIB = $(obj)libidme.o

CFLAGS += \
    $(IDME_CFLAGS) \
    $(IDME_HEADERS)

COBJS  := $(IDME_OBJECTS)
SRCS   := $(COBJS:.o=.c)
OBJS   := $(addprefix $(obj),$(COBJS))

$(IDME_LIB): $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

endif # (yes,$(strip $(UFBL_UBOOT_IS_KBUILD)))
