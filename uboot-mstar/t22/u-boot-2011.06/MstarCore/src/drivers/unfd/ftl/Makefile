include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

LIB	= $(obj)libmstarftl.a

CFLAGS += -I../unfd/inc/common/
CFLAGS += -I../unfd/inc/api/
CFLAGS += -I../unfd/inc/config/
CPPFLAGS += -I../unfd/inc/common/
CPPFLAGS += -I../unfd/inc/api/
CPPFLAGS += -I../unfd/inc/config/

CFLAGS += -I./inc/
CPPFLAGS += -I./inc/

COBJS += ./ftl.o
COBJS += ./src/ftl_api.o
COBJS += ./src/ftl_dbg.o
COBJS += ./src/ftl_err.o
COBJS += ./src/ftl_hal.o
COBJS += ./src/ftl_hal_plane.o
COBJS += ./src/ftl_test.o
COBJS += ./src/ftl_utl.o

all:	$(LIB) $(AOBJS)

SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################

