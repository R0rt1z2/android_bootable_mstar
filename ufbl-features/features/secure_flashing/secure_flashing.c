/*
 * Copyright (C) 2015-2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
*/
#include "amzn_secure_flashing.h"

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

#if defined(UFBL_FEATURE_SECURE_FLASHING)

static sec_flash_cert sec_cert = {
	{0},
	0,
	{0},
	{0}
};

__WEAK int amzn_get_sec_flashing_code(unsigned char *code, unsigned int *len)
{
	(void)code;
	(void)len;
	return -1;
}

__WEAK const unsigned char * amzn_get_sec_flashing_root_pubkey(unsigned int *key_len)
{
	(void)key_len;
	return NULL;
}

__WEAK int amzn_verify_device_meta(const unsigned char *data, unsigned int len)
{
	(void)data;
	(void)len;
	return 0;
}

__WEAK const unsigned char * amzn_get_sec_flashing_pk_blacklist(unsigned int *len)
{
	(void)len;
	return NULL;
}

const sec_flash_cert * amzn_get_sec_flashing_cert(unsigned int *len)
{
	if (!len)
		return NULL;
	*len = sizeof(sec_cert);
	return &sec_cert;
}

int amzn_set_sec_flashing_cert(unsigned char *buf, unsigned int len)
{
	if (!buf || (len != sizeof(sec_cert)))
		return -1;
	memcpy((unsigned char *)&sec_cert, buf, len);
	return 0;
}

void amzn_set_sec_flashing_signed_code(const unsigned char *signed_code, unsigned int len)
{
	sec_flash_cert *cert = &sec_cert;
	if(!signed_code || len != RSA_2048_SIG_LEN || !cert || !cert->signature)
		return;
	memcpy(cert->signature, signed_code, RSA_2048_SIG_LEN);
}

int amzn_device_is_sec_flashing_enabled(void)
{
	sec_flash_cert *cert = &sec_cert;
	if(!cert || !cert->signature)
		return 0;
	if (!amzn_verify_sec_flashing_code((void*)cert->signature, RSA_2048_SIG_LEN)) {
		return 1;
	}
	return 0;
}
#endif
