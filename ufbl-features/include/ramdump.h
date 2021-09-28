/*
 * ramdump.h
 *
 * Copyright 2011-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

/*!
 * @file ramdump.h
 * @brief This file contains functions for ramdump related interfaces and data
 *  structures
 *
 */

#ifndef __RAMDUMP_H__
#define __RAMDUMP_H__

void ramdump_init(void);

int check_ramdump(void);

int ramdump_to_eMMC(void);

int ramdump_in_ram_compress(void);


#endif /* __RAMDUMP_H__ */

