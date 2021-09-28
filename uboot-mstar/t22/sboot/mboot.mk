####################################################################
#######                       mboot.mk                       #######
####################################################################
#
#
# mboot defination
#
#
-include .config


TOOL_PATH="/tools/script/ck"
PROJDIR=`pwd`
BLOCK_SIZE=2097152

BOOT_OUTPUT_PATH= $(O)/out
UBOOT_BIN	    = $(BOOT_OUTPUT_PATH)/$(CONFIG_APP_AS_2ND_LOADER_BIN_PATH)
UBOOT_MAP	    = $(BOOT_OUTPUT_PATH)/$(CONFIG_APP_AS_2ND_LOADER_MAP_PATH)
INFO_BIN        = $(BOOT_OUTPUT_PATH)/info.bin
CHUNK_HEADER    = $(BOOT_OUTPUT_PATH)/chunk_header.bin
SECURITY_BIN    = $(BOOT_OUTPUT_PATH)/secure_info.bin
SECURITY_INC    = $(BOOT_OUTPUT_PATH)/security.inc


TOOLDIR         =$(TOPDIR)/util
SBOOT_LDS=./src/$(CHIP_FOLDER)/sboot.lds.S
SBOOT_MAP=./out/sboot.elf.map
SECUREDIR = ./secure
SIGNATURE_PM=./secure_info_pm.bin
SIGNATURE_SBOOT=./secure_info_sboot.bin
SIGNATURE_UBOOT=./secure_info_uboot.bin


# KEY related
SIGNATURE_CUSTOMER_KEY_BANK=$(SECUREDIR)/secure_info_customer_key_bank.bin
RSA_BOOT_PRIVATE_KEY=$(SECUREDIR)/RSAboot_priv.txt
RSA_BOOT_PUBLIC_KEY=$(SECUREDIR)/RSAboot_pub.txt
RSA_BOOT_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAboot_pub.bin
RSA_EA_PRIVATE_KEY=$(SECUREDIR)/RSAupgrade_priv.txt
RSA_EA_PUBLIC_KEY=$(SECUREDIR)/RSAupgrade_pub.txt
RSA_EA_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAupgrade_pub.bin
RSA_UPGRADE_PUBLIC_KEY=$(SECUREDIR)/RSAupgrade_pub.txt
RSA_UPGRADE_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAupgrade_pub.bin
RSA_IMAGE_PRIVATE_KEY=$(SECUREDIR)/RSAimage_priv.txt
RSA_IMAGE_PUBLIC_KEY=$(SECUREDIR)/RSAimage_pub.txt
RSA_IMAGE_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAimage_pub.bin
ifeq ($(CONFIG_AESKEY_PATH), y)
AES_BOOT_KEY=$(SECUREDIR)/$(CONFIG_AESKEY_PATH_STRING)
else
AES_BOOT_KEY=$(SECUREDIR)/AESboot.bin
endif
AES_UPGRADE_KEY=$(SECUREDIR)/AESupgrade.bin
AES_RE_DRM_KEY=$(SECUREDIR)/AESdrm.bin
MAGIC_ID=$(SECUREDIR)/MagicID.bin
DDR_BACKUP_BIN=ddrBackup.bin
#TEE key bank
TEE_MSID=$(SECUREDIR)/TEE_MSID.bin
TEEKB_VERSION=$(SECUREDIR)/TEEKB_VERSION.bin
ifeq ($(CONFIG_AESKEY_PATH), y)
TEE_AES_KEY=$(SECUREDIR)/$(CONFIG_AESKEY_PATH_STRING)
else
TEE_AES_KEY=$(SECUREDIR)/AESboot.bin
endif
TEE_RSA_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAteeext_pub.bin
TEE_RSA_PRIVATE_KEY=$(SECUREDIR)/RSAteeext_priv.txt
TEE_RSA_PUBLIC_KEY=$(SECUREDIR)/RSAteeext_pub.txt
#REE key bank
REE_AES_KEY=$(SECUREDIR)/AESree.bin
REE_RSA_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAreeext_pub.bin
REE_RSA_PRIVATE_KEY=$(SECUREDIR)/RSAreeext_priv.txt
REE_RSA_PUBLIC_KEY=$(SECUREDIR)/RSAreeext_pub.txt
#Embedded TEE RSAKey
EMBEDDED_TEE_RSA_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAcus_pub.bin
EMBEDDED_TEE_RSA_PRIVATE_KEY=$(SECUREDIR)/RSAcus_priv.txt
EMBEDDED_TEE_RSA_PUBLIC_KEY=$(SECUREDIR)/RSAcus_pub.txt
#Embedded REE RSAKey,EmbeddedTL_A_REE_KEY
EMBEDDED_REE_RSA_PUBLIC_KEY_BIN=$(SECUREDIR)/RSAree_pub.bin
EMBEDDED_REE_RSA_PRIVATE_KEY=$(SECUREDIR)/RSAree_priv.txt
EMBEDDED_REE_RSA_PUBLIC_KEY=$(SECUREDIR)/RSAree_pub.txt
#Embedded AESUNIQUEKEYS,EmbeddedTL_D_REE_KEY
EMBEDDED_TEE_AESKEY=$(SECUREDIR)/AES_TEE_Uniform.bin
EMBEDDED_REE_AESKEY=$(SECUREDIR)/AES_REE_Uniform.bin

AWK		= awk

define ALIGN
$$(($(1) + (($(2) - ($(1) % $(2))) % $(2))))
endef


ifdef CONFIG_STB_ECOS_BOOT
LOGO_SRC = ../MstarCustomer/$(CUSTOMER)/sboot/bin/logo/HB_logo.JPG
else
LOGO_SRC = ../MstarCustomer/$(CUSTOMER)/sboot/bin/logo/boot0.jpg
endif

EA_UBOOTBIN = $(BOOT_OUTPUT_PATH)/ea-u-boot.bin
EA_BIN = $(BOOT_OUTPUT_PATH)/EA.bin
