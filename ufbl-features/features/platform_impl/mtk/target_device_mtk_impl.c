/*
 * Copyright (C) 2018 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#include <amzn_secure_boot.h>
#include <platform/boot_mode.h>

extern BOOT_ARGUMENT *g_boot_arg;
#define ___FEATURE_MTK_IMPL_STR(x) #x
#define __FEATURE_MTK_IMPL_STR(x) ___FEATURE_MTK_IMPL_STR(x)

const char *amzn_target_device_name(void)
{
    return __FEATURE_MTK_IMPL_STR(UFBL_FEATURE_PLATFORM_IMPL_MTK_PRODUCT);
}

int amzn_target_device_type(void)
{
    if (g_boot_arg->prod_dev == 0) {
        return AMZN_ENGINEERING_DEVICE;
    }
    return AMZN_PRODUCTION_DEVICE;
}
