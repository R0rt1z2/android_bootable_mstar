ifeq ($(CONFIG_MSTAR_M7221),y)
    URSA_TYPE	:= 13
    FCIC_IP		:= true
else ifeq ($(CONFIG_MSTAR_M3822),y)
    URSA_TYPE	:= 11
    FCIC_IP		:= false
else ifeq ($(CONFIG_MSTAR_M7622),y)
    URSA_TYPE	:= 13
    FCIC_IP		:= true
else ifeq ($(CONFIG_MSTAR_MOONEY),y)
    URSA_TYPE	:= 13
    FCIC_IP		:= false
else ifeq ($(CONFIG_MSTAR_MAXIM),y)
    URSA_TYPE	:= 11
    FCIC_IP		:= false
else ifeq ($(CONFIG_MSTAR_MACAN),y)
    URSA_TYPE	:= 11
    FCIC_IP		:= false
else ifeq ($(CONFIG_MSTAR_MAZDA),y)
    URSA_TYPE	:= 11
    FCIC_IP		:= false
else ifeq ($(CONFIG_MSTAR_MASERATI),y)
    URSA_TYPE	:= 11
    FCIC_IP		:= false
else
    URSA_TYPE	:= invalid_num
    FCIC_IP		:= false
endif


# Check rule
ifeq ($(URSA_TYPE),invalid_num)
    $(info Please set URSA_TYPE value)
    $(info Exit Building ...)
    $(info )
    exit -1
endif

ifeq ($(FCIC_IP),false)
    ifeq ($(CONFIG_DEMURA_FCIC),y)
        $(info $(CONFIG_MSTAR_CHIP_NAME) do not support FCIC, Please unset it ! )
        $(info Exit Building ...)
        $(info )
        exit -1
    endif
endif

