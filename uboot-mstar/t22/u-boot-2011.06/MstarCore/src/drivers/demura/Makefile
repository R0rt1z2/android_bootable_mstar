include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config
include board_check.mk


COBJS		:=
UDV			:=
VENDOR		:= Mstar
LIB			:= $(obj)libdemura.a

CHIP_NAME	:= $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
LOCAL_PATH	:= $(TOPDIR)/../MstarCore/src/drivers/demura
HAL_PATH	:= $(TOPDIR)/../MstarCore/src/hal/$(CHIP_NAME)/demura
URSA_DIR	:= U$(URSA_TYPE)


###########  Base library  ###########
COBJS-y		+= $(LOCAL_PATH)/base/dmalloc.o
COBJS-y		+= $(LOCAL_PATH)/base/bits_stream_reader.o
COBJS-y		+= $(LOCAL_PATH)/base/read_text.o
COBJS-y		+= $(LOCAL_PATH)/base/ms_array.o
COBJS-y		+= $(LOCAL_PATH)/base/ms_vector.o
COBJS-y		+= $(LOCAL_PATH)/base/ms_utils.o
COBJS-y		+= $(LOCAL_PATH)/base/crc_libs.o
CFLAGS		+= -I$(LOCAL_PATH)/
CPPFLAGS	+= -I$(LOCAL_PATH)/
CFLAGS		+= -I$(LOCAL_PATH)/base/
CPPFLAGS	+= -I$(LOCAL_PATH)/base/
CFLAGS		+= -I$(HAL_PATH)
CPPFLAGS	+= -I$(HAL_PATH)

COBJS-y		+= $(LOCAL_PATH)/MsApiDemura.o
COBJS-y		+= $(HAL_PATH)/halRegOp.o

###########  Demura IP  ###########
ifeq ($(CONFIG_DEMURA_FCIC),y)
    DRV_PATH	:= $(LOCAL_PATH)/fcic
    COBJS-y		+= $(DRV_PATH)/Init_IP.o
    COBJS-y		+= $(DRV_PATH)/MDrvFCIC.o
    COBJS-y		+= $(HAL_PATH)/halFCIC.o
    COBJS-y		:= $(COBJS-y)    # Unpack the macro right now
    CFLAGS		+= -I$(LOCAL_PATH)/mstar
    CPPFLAGS	+= -I$(LOCAL_PATH)/mstar
endif

ifeq ($(CONFIG_DEMURA_MSTAR),y)
    DRV_PATH	:= $(LOCAL_PATH)/mstar
    COBJS-y		+= $(DRV_PATH)/Init_IP.o
    COBJS-y		+= $(HAL_PATH)/halDemura.o
    COBJS-y		+= $(DRV_PATH)/MDrvDemura.o
    COBJS-y		+= $(DRV_PATH)/convert_entry.o
    CFLAGS		+= -I$(DRV_PATH)
    CPPFLAGS	+= -I$(DRV_PATH)

    ###########  Panel Vendor  ###########
    ifneq ($(CONFIG_DEMURA_VENDOR_MSTAR),y)
        UDV			:= y
        ifeq ($(CONFIG_DEMURA_VENDOR_CSOT),y)
            VENDOR	:= csot
        else ifeq ($(CONFIG_DEMURA_VENDOR_AUO),y)
            VENDOR	:= auo
            COBJS-y	+= $(DRV_PATH)/vendor/$(VENDOR)/parse_header.o
        else ifeq ($(CONFIG_DEMURA_VENDOR_INNOLUX),y)
            VENDOR	:= innolux
            COBJS-y	+= $(DRV_PATH)/vendor/$(VENDOR)/decompress_8206.o
            COBJS-y	+= $(DRV_PATH)/vendor/$(VENDOR)/parse_header.o
        else ifeq ($(CONFIG_DEMURA_VENDOR_LGD),y)
            VENDOR	:= lgd
            COBJS-y	+= $(DRV_PATH)/vendor/$(VENDOR)/parse_fcic.o
        endif
    endif
    
    COBJS-$(UDV)	+= $(DRV_PATH)/URSA/$(URSA_DIR)/demura.o
    COBJS-$(UDV)	+= $(DRV_PATH)/URSA/$(URSA_DIR)/demura_common.o
    COBJS-$(UDV)	+= $(DRV_PATH)/URSA/$(URSA_DIR)/demuraDLL.o
    COBJS-$(UDV)	+= $(DRV_PATH)/vendor/$(VENDOR)/decoder_input.o
    CFLAGS			+= -I$(DRV_PATH)/URSA/
    CFLAGS			+= -I$(DRV_PATH)/URSA/$(URSA_DIR)/
    CFLAGS			+= -I$(DRV_PATH)/vendor/
    CPPFLAGS		+= -I$(DRV_PATH)/URSA/
    CPPFLAGS		+= -I$(DRV_PATH)/URSA/$(URSA_DIR)/
    CPPFLAGS		+= -I$(DRV_PATH)/vendor/
endif


###########  Pre Define  ###########
ifeq ($(URSA_TYPE),11)
CFLAGS		+= -DCONFIG_DEMURA_URSA11
CPPFLAGS	+= -DCONFIG_DEMURA_URSA11
else ifeq ($(URSA_TYPE),13)
CFLAGS		+= -DCONFIG_DEMURA_URSA13
CPPFLAGS	+= -DCONFIG_DEMURA_URSA13
endif
CFLAGS	+= -DCONFIG_DEMURA_URSA_STRING=\"$(URSA_TYPE)\"
CFLAGS	+= -DCONFIG_DEMURA_VENDOR_STRING=\"$(VENDOR)\"
CFLAGS	+= -include $(TOPDIR)/include/config.h


###########  Check Rule  ###########
CFLAGS	+= -Wextra
# Our function return type maybe int, double or float ..., While C default return type is "int" !
CFLAGS	+= -Werror=implicit-function-declaration

###########  # Complie rule  ###########
COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
