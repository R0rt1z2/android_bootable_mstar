/*
 * Copyright (C) 2016 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#ifndef UFBL_FEATURES_FOS_FLAGS_H
#define UFBL_FEATURES_FOS_FLAGS_H

/**
 * Performs the requested fos_flags operation.
 *
 * The format of 'arg' is as follows:
 * '<type:> <modifiers> <value1[|value2|value3|...]>'
 *
 * type:
 * fos_flags:, fos:, f: (default if not specified)
 * dev_flags:, dev:, d:
 * usr_flags:, usr:, u:
 *
 * modifiers:
 * + means to OR value with the existing value
 *    (flags = flags | value)
 * - means to AND the inverse of value with the existing value
 *    (flags = flags & ~value)
 * = means to set the value (flags = value). If = is not set, it is assumed
 *
 * value:
 * If begins with 0x, it is treated as a hexadecimal value and set directly
 * If the value field is a number, it is converted from decimal to hex and set
 * Multiple values may be ORd together as in value1|value2|value3
 * Symbolic names are explicitly NOT supported.
 *
 * If no type, modifiers or value is specified, print out the current value.
 * Also, accept the special keyword 'print'
 *
 * @param arg Input argument buffer. Must be nul-terminated.
 * @param response Output buffer. Will be nul-terminated
 * @param response_length Length of output buffer.
 * @return TRUE on success, FALSE on errors. {@param response} will be filled with details.
 */
bool oem_flags(char *arg, /* out */char *response, const size_t response_length);

/**
 * Wrapper for Fastboot exposing the fos_flags functionality.
 *
 * @param arg flags command arguments
 * @param response nul-terminated string output from the oem_flags command.
 * @return TRUE if flags command successful, FALSE if error
 */
bool fastboot_oem_flags(char *arg, /* out */char *response);

#if !defined(FASTBOOT_RESPONSE_LEN)
#define FASTBOOT_RESPONSE_LEN 64
#endif

#endif //UFBL_FEATURES_FOS_FLAGS_H
