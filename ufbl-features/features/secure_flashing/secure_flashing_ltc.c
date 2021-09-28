/*
 * Copyright (C) 2016 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#include <tomcrypt.h>
#ifndef SUPPORT_UBOOT
#include <debug.h>
#endif
#include "amzn_secure_flashing.h"
#include "ufbl_debug.h" /* for dprintf */
#include "amzn_ufbl_alloc.h" /* for plat_alloc */
#include "unlock_ltc.h"

int amzn_verify_sec_flashing_cert(unsigned char do_verify)
{
	int ret = ERR_SECURE_FLASHING_SUCCESS;
	unsigned int len = 0;
	unsigned int i;

	const sec_flash_cert *cert = NULL;
	uint8_t digest[sha256_desc.hashsize];
	unsigned long digest_len = sha256_desc.hashsize;
	int sha256_idx = find_hash("sha256");
	const unsigned char *pk_blacklist = NULL;

	unsigned int key_len = 0;
	const unsigned char *key = NULL;

	static unsigned char sec_flashing_cert_verified = 0;

	if (!do_verify && sec_flashing_cert_verified)
		return -ERR_SECURE_FLASHING_SUCCESS;
	sec_flashing_cert_verified = 0;

	cert = amzn_get_sec_flashing_cert(&len);
	if(!cert || !len) {
		ret = -ERR_SECURE_FLASHING_GET_SEC_FLASH_CERT_FAIL;
		goto done;
	}

	key = amzn_get_sec_flashing_root_pubkey(&key_len);
	if (!key || !key_len) {
		ret = -ERR_SECURE_FLASHING_BAD_KEY;
		goto done;
	}
	if (amzn_verify_code_internal((unsigned char *)cert,
			(len - RSA_2048_SIG_LEN),
			cert->signature, RSA_2048_SIG_LEN, key, key_len)) {
		ret = -ERR_SECURE_FLASHING_RSA_VERIFY_FAIL;
		goto done;
	}

	if (amzn_verify_device_meta(cert->device_meta, DEVICE_META_SIZE)) {
		ret = -ERR_SECURE_FLASHING_DEVICE_META_VERIFY_FAIL;
		goto done;
	}

	register_hash(&sha256_desc);
	if (hash_memory(sha256_idx, cert->pubkey, cert->pk_len, digest,
			&digest_len)) {
		ret = -ERR_SECURE_FLASHING_HASH_FAIL;
		goto done;
	}
	pk_blacklist = amzn_get_sec_flashing_pk_blacklist(&len);
	if (pk_blacklist) {
		for (i = 0; i < len; i++) {
			if (!memcmp((pk_blacklist + i * SHA256_DIGEST_SIZE), digest, SHA256_DIGEST_SIZE)) {
				ret = -ERR_SECURE_FLASHING_IN_BLACKLIST;
				goto done;
			}
		}
	}
	sec_flashing_cert_verified = 1;
done:
	return ret;
}

int amzn_verify_sec_flashing_code(const unsigned char *sig, unsigned int sig_len)
{
	int ret = ERR_SECURE_FLASHING_SUCCESS;
	uint8_t code[SEC_FLASH_CODE_LEN];
	unsigned int code_len = sizeof(code);
	unsigned int len;

	const sec_flash_cert *cert = amzn_get_sec_flashing_cert(&len);
	if(!cert || !len) {
		dprintf(CRITICAL, "%s: Failed to get sec flash cert\n", __FUNCTION__);
		ret = -ERR_SECURE_FLASHING_GET_SEC_FLASH_CERT_FAIL;
		goto done;
	}
	if ((ret = amzn_verify_sec_flashing_cert(0))) {
		dprintf(CRITICAL, "%s: Verify sec flash cert fail\n", __FUNCTION__);
		goto done;
	}

	if (amzn_get_sec_flashing_code(code, &code_len)) {
		dprintf(CRITICAL, "%s: Failed to get sec flash code\n", __FUNCTION__);
		ret = -ERR_SECURE_FLASHING_GET_CODE_FAIL;
		goto done;
	}

	ret = amzn_verify_code_internal(code, code_len,
						sig, sig_len, cert->pubkey, cert->pk_len);
done:
	return ret;
}


int amzn_base64_decode(const unsigned char *b64_str, size_t len_in,
	unsigned char *b64_dec, size_t *len_out)
{
	return base64_decode(b64_str, len_in, b64_dec, len_out);
}
