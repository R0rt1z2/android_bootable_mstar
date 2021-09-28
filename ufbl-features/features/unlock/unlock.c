/*
 * Copyright (C) 2015 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#include <stddef.h>
#include <amzn_unlock.h>
#if defined(UFBL_FEATURE_IDME)
#include <idme.h>
#endif

#ifdef SUPPORT_BOLT
#include <platform/bcm_platform.h>
#endif


/* Compiler may already define this.
 * If it doesn't, try this other compiler specific setting.
 * __attribute__((weak)) is specific to the GNU toolchain.
 */
#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

__WEAK int amzn_get_unlock_code(unsigned char *code, unsigned int *len)
{
	(void)code; /* avoid unused variable warning */
	(void)len;

	return -1;
}

/* Default implementation just returns the standard unlock code */
__WEAK int amzn_get_limited_unlock_code(unsigned char *code, unsigned int *len,
					unsigned int ctr)
{
	(void)ctr;
	return amzn_get_unlock_code(code, len);
}

__WEAK const unsigned char *amzn_get_unlock_key(unsigned int *key_len)
{
	(void)key_len; /* avoid unused variable warning */

	return NULL;
}

#if defined(UFBL_FEATURE_UNLOCK)
__WEAK int amzn_target_is_unlocked(void)
{
	char signed_code[SIGNED_UNLOCK_CODE_LEN];

	memset(signed_code, 0, SIGNED_UNLOCK_CODE_LEN);

	if (!idme_get_var_external("unlock_code", signed_code,
				   sizeof(signed_code)) &&
		(!(amzn_verify_unlock(signed_code,
			sizeof(signed_code))))) {
		return 1;
	} else {
		return 0;
	}
}
#endif
