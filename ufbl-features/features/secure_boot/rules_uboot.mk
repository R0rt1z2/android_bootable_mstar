#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
secure-boot-ccflags-$(CONFIG_LIBTOMCRYPT) += -DUFBL_FEATURE_SECURE_BOOT_LTC \
-I$(srctree)/../../ufbl-features/features/libtomcrypt/src/headers \
-I$(srctree)/../../ufbl-features/features/libtomcrypt \
-I$(srctree)/../../ufbl-features/features/libtomcrypt/uboot \
-DLTC_SOURCE \
-DLTC_NO_FILE \
-DUSE_LTM -DLTM_DESC \
-D_WCHAR_T -DLTC_CLEAN_STACK \

# Leave line above blank
secure-boot-ccflags-$(CONFIG_LIBTOMCRYPT_SPL) += -DUFBL_FEATURE_SECURE_BOOT_LTC \
-I$(srctree)/../../ufbl-features/features/libtomcrypt/src/headers \
-I$(srctree)/../../ufbl-features/features/libtomcrypt \
-I$(srctree)/../../ufbl-features/features/libtomcrypt/uboot \
-DLTC_SOURCE \
-DLTC_NO_FILE \
-DUSE_LTM -DLTM_DESC \
-D_WCHAR_T -DLTC_CLEAN_STACK \

# Leave line above blank
subdir-ccflags-$(CONFIG_SECURE_BOOT) += -DUFBL_FEATURE_SECURE_BOOT \
$(secure-boot-ccflags-y) \

# Leave line above blank
secure-boot-obj-$(CONFIG_LIBTOMCRYPT) += \
../../../../ufbl-features/features/secure_boot/image_verify_ltc.o
secure-boot-obj-$(CONFIG_LIBPOLARSSL) += \
../../../../ufbl-features/features/secure_boot/image_verify_openssl.o
secure-boot-obj-$(CONFIG_LIBTOMCRYPT_SPL) += \
../../../../ufbl-features/features/secure_boot/image_verify_ltc.o
secure-boot-obj-$(CONFIG_LIBPOLARSSL_SPL) += \
../../../../ufbl-features/features/secure_boot/image_verify_openssl.o
obj-$(CONFIG_SECURE_BOOT) += \
../../../../ufbl-features/features/secure_boot/image_verify.o \
$(secure-boot-obj-y)
