/*
 * Copyright (C) 2016 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#if defined(UFBL_PLATFORM_MTK)
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#else
#include "common.h"
#endif

#include "fastboot.h"
#include "fos_flags.h"

bool fastboot_oem_flags(char *arg, /* out */ char *response) {
	/* 'arg' is a null-terminated string that is the full set of arguments, less 'oem '. */
	arg += strlen("flags ");
	bool success = false;

	snprintf(response, FASTBOOT_RESPONSE_LEN, "INFO");
	success = oem_flags(arg, response + 4, FASTBOOT_RESPONSE_LEN - 4);
	return success;
}

#if defined(UFBL_PLATFORM_MTK)
void cmd_oem_flags(char *arg, void *data, unsigned sz) {
    char response[FASTBOOT_RESPONSE_LEN] = {0};
    bool success = false;

    success = oem_flags(arg, response, FASTBOOT_RESPONSE_LEN);
    if (success) {
        fastboot_info(response);
    } else {
        fastboot_fail(response);
    }
    fastboot_okay("");
}
#endif
