/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

#ifndef UFBL_FEATURES_H
#define UFBL_FEATURES_H

/**
 *  Include the IDME API which will be used for the bootloader **/
#include "idme.h"
/**
 *  Include RAMDump APIs **/
#include "ramdump.h"

/*  Include the LCR APIS **/
#include "life_cycle_reasons.h"

/* Include Secure Boot APIs */
#include "amzn_secure_boot.h"

/* Include Unlock APIs */
#include "amzn_unlock.h"

/**
 *  Fastboot Extension API will register the following fastboot command
 *  -- dump   used to dump any of the EMMC partitions 
 *  -- verify used to verify the correctness of flashed image **/
__WEAK void ufbl_fastboot_features_init(void);

#endif
