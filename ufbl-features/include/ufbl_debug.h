/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

#ifndef __UFBL_DEBUG_H__
#define __UFBL_DEBUG_H__

#if defined(UFBL_PROJ_ABC)

#include "uefi_debug.h"

#else

#if defined(SUPPORT_UBOOT) || \
	defined(SUPPORT_KERNELFLINGER) || \
	defined(SUPPORT_BOLT) || \
	defined(UFBL_TESTS) || \
	defined(UFBL_SIG_TESTS) || \
	defined(SUPPORT_MTK_PRELOADER)

#define CRITICAL 0
#define ALWAYS 0
#define INFO 1
#define SPEW 2
#ifdef DEBUG
	#define DEBUGLEVEL 2
#else
	#define DEBUGLEVEL 1
#endif

#endif /* defined(SUPPORT_UBOOT) || defined(SUPPORT_KERNELFLINGER) || defined(SUPPORT_BOLT) || defined(UFBL_TESTS) || defined(UFBL_SIG_TESTS) || defined(SUPPORT_MTK_PRELOADER) */

#if defined(SUPPORT_UBOOT) || defined(SUPPORT_MTK_PRELOADER)

#if defined(SUPPORT_MTK_PRELOADER)
#include <typedefs.h> /* for printf support */
#endif

#if (defined CONFIG_SPL_BUILD) && !(defined CONFIG_SPL_LIBCOMMON_SUPPORT)
/* No printf support if libcommon not available.  Only print if DEBUG defined */
#define dprintf(level, x...) do { if ((level) <= DEBUGLEVEL) { debug(x); } } while (0)

#else /* (defined CONFIG_SPL_BUILD) && !(defined CONFIG_SPL_LIBCOMMON_SUPPORT) */

#define dprintf(level, x...) do { if ((level) <= DEBUGLEVEL) { printf(x); } } while (0)
#endif /* (defined CONFIG_SPL_BUILD) && !(defined CONFIG_SPL_LIBCOMMON_SUPPORT) */
#define UFBL_STR(x) (x)

#else /* defined(SUPPORT_UBOOT) || defined(SUPPORT_MTK_PRELOADER)*/

#if defined(SUPPORT_KERNELFLINGER)

#include <log.h> /* dprintf */
#define dprintf(level, x, ...) do { \
	if (level == CRITICAL) {                   \
		log(L ## x "\n", ##__VA_ARGS__);   \
	} else { \
		debug(L ## x "\n", ##__VA_ARGS__); \
	} \
} while(0)
CHAR16* amzn_char8_to_char16(const char* src);
#define UFBL_STR(x) amzn_char8_to_char16(x)
#else  /* defined(SUPPORT_KERNELFLINGER) */

#ifndef UFBL_TESTS
#define dprintf(level, x...) do { if ((level) <= DEBUGLEVEL) { printf(x); } } while (0)
#else /* UFBL_TESTS */
#define dprintf(level, x...) printf(x);
#endif /* UFBL_TESTS */
#define UFBL_STR(x) (x)

#endif /* defined(SUPPORT_KERNELFLINGER) */

#endif /* defined(SUPPORT_UBOOT) || defined(SUPPORT_MTK_PRELOADER)*/

#endif /* UFBL_PROJ_ABC */

#endif /*__UFBL_DEBUG_H__ */
