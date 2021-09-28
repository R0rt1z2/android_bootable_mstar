/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#ifndef __AMZN_FASTBOOT_LOCKDOWN_H
#define __AMZN_FASTBOOT_LOCKDOWN_H

int is_restricted_command_on_locked_hw(const unsigned char* const fastboot_command_buffer);

#endif
