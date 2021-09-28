/*
 * Copyright (C) 2018 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

/*!
 * @file idme_utilities.h
 * @brief This file contains helper functions for interacting with variables
 *  in the userstore partition
 */

#ifndef __IDME_UTILITIES_H__
#define __IDME_UTILITIES_H__

/**
 * idme_is_flag_set
 *
 * @param flagtype the 'name' of the idme key-value pair to query.  The value
 *                 associated with flagtype should correspond to a hex number,
 *                 for example, "dev_flags", "fos_flags" or "usr_flags"
 * @param flags the bitwise flag to check
 * @returns 1 (true) if the flagtype flag in IDME is set, and zero (false) otherwise
 */
int idme_is_flag_set(const char* flagtype, unsigned long long flags);

/**
 * idme_is_fos_flag_set
 *
 * @param flags the bitwise flag to check
 * @returns 1 (true) if the "fos_flags" flags in IDME is set, and zero (false) otherwise
 */
int idme_is_fos_flag_set(unsigned long long flags);

#endif /* __IDME_UTILITIES_H__ */
