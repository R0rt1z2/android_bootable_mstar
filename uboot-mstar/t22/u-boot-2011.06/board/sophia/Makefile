include $(TOPDIR)/config.mk

LIB = $(obj)libboard.o

COBJS-y += setup.o

ifeq ($(FEATURE_SECURE_BOOT),true)
COBJS-y += secure_boot_cfg.o
endif

ifeq ($(BOARD_KERNEL_SEPARATED_DT),true)
CFLAGS += -DBOARD_KERNEL_SEPARATED_DT=1
endif

COBJS	= $(COBJS-y)
SRCS 	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend
