/*
 * Copyright (C) 2015 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#ifndef __CMD_BCB_H
#define __CMD_BCB_H

#ifdef SUPPORT_UBOOT
#ifdef FASTBOOT_RESPONSE_LEN
/* Only fastboot users need to know about this */
int fastboot_bcb(char response[FASTBOOT_RESPONSE_LEN - 4], char *cmd);
#endif
#endif //SUPPORT_UBOOT
#endif

