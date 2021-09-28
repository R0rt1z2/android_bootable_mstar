/*
 * Copyright (C) 2016 - 2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#include <tomcrypt.h>
#ifndef SUPPORT_UBOOT
#include <debug.h>
#endif
#include "amzn_unlock.h"
#include "unlock_ltc.h"
#include "ufbl_debug.h" /* for dprintf */
#include "amzn_ufbl_alloc.h" /* for plat_alloc */

int amzn_verify_code_internal(const unsigned char *data, unsigned int data_len,
	const unsigned char *sig, unsigned int sig_len,
	const unsigned char *key, unsigned int key_len)
{
	int ret = -ERR_UNLOCK_RSA_VERIFY_FAIL;
	int status = -1;
	rsa_key *rsa_pk = NULL;
	uint8_t digest[sha256_desc.hashsize];
	unsigned long digest_len = sha256_desc.hashsize;

	if (!data || !data_len ||
			!sig || !sig_len
			|| !key || !key_len) {
		dprintf(CRITICAL, "%s: Invalid arg\n", __FUNCTION__);
		ret = -ERR_UNLOCK_BAD_ARGUMENT;
		goto done;
	}

	if (!(rsa_pk = (rsa_key *)amzn_plat_alloc(sizeof(*rsa_pk)))) {
		dprintf(CRITICAL, "%s: Cannot malloc key\n", __FUNCTION__);
		ret = -ERR_UNLOCK_NO_MEMORY;
		goto done;
	}

#ifdef USE_LTM
	/*
	 * Compiler inserts memcpy for large structure initialization
	 * some bootloaders use symbol definition (e.g. BOLT) which results
	 * into linking error. Explicit use of memcpy() resolves this problem.
	 */
	memcpy(&ltc_mp, &ltm_desc, sizeof(ltc_mp));
#elif defined(USE_TFM)
	ltc_mp = tfm_desc;
#elif defined(USE_GMP)
	ltc_mp = gmp_desc;
#else
	extern ltc_math_descriptor EXT_MATH_LIB;
	ltc_mp = EXT_MATH_LIB;
#endif

	/* Parse the unlock key */
	if (rsa_import(key, key_len, rsa_pk) != CRYPT_OK) {
		dprintf(CRITICAL, "%s: Cannot parse key\n", __FUNCTION__);
		ret = -ERR_UNLOCK_BAD_KEY;
		goto done;
	}

	register_hash(&sha256_desc);
	int sha256_idx = find_hash("sha256");
	hash_memory(sha256_idx, data, data_len, digest,
			&digest_len);

	/* PSS-Verify */
	ret = rsa_verify_hash(sig, sig_len, digest, digest_len,
				  sha256_idx, sha256_desc.hashsize,
				  &status, rsa_pk);

	if (ret == CRYPT_OK && status == 1) {
		ret = ERR_UNLOCK_SUCCESS;
	} else {
		ret = -ERR_UNLOCK_RSA_VERIFY_FAIL;
	}

	/* need to use rsa_free only if rsa_import is success */
	rsa_free(rsa_pk);

done:
	amzn_plat_free(rsa_pk);

	return ret;
}


int amzn_verify_limited_unlock(const unsigned char *sig, unsigned int sig_len,
	unsigned int ctr_min, unsigned int ctr_max)
{
	int ret = -ERR_UNLOCK_SUCCESS;
	unsigned int ctr;
	uint8_t code[UNLOCK_CODE_LEN];
	unsigned int code_len = sizeof(code);

	unsigned int key_len = 0;
	const uint8_t *key = amzn_get_unlock_key(&key_len);
	if (!key || !key_len) {
		dprintf(CRITICAL, "%s: Failed to get unlock key\n", __FUNCTION__);
		ret = -ERR_UNLOCK_BAD_KEY;
		goto done;
	}

	for (ctr = ctr_min; ctr <= ctr_max; ctr++) {
		ret = amzn_get_limited_unlock_code(code, &code_len, ctr);
		if (ret != 0) {
			dprintf(CRITICAL, "%s: Failed to get unlock code\n",
				__FUNCTION__);
			ret = -ERR_UNLOCK_GET_CODE_FAIL;
			goto done;
		}
		ret = amzn_verify_code_internal(code, code_len,
							sig, sig_len, key, key_len);
		if (!ret)
			break;
	}
done:
	return ret;

}

int amzn_verify_unlock(const unsigned char *sig, unsigned int sig_len)
{
	int ret = ERR_UNLOCK_SUCCESS;
	uint8_t code[UNLOCK_CODE_LEN];
	unsigned int code_len = sizeof(code);

	unsigned int key_len = 0;
	const uint8_t *key = NULL;

	key = amzn_get_unlock_key(&key_len);
	if (!key || !key_len) {
		dprintf(CRITICAL, "%s: Failed to get unlock key\n", __FUNCTION__);
		ret = -ERR_UNLOCK_BAD_KEY;
		goto done;
	}
	if (amzn_get_unlock_code(code, &code_len)) {
		dprintf(CRITICAL, "%s: Failed to get unlock code\n", __FUNCTION__);
		ret = -ERR_UNLOCK_GET_CODE_FAIL;
		goto done;
	}

	ret = amzn_verify_code_internal(code, code_len,
						sig, sig_len, key, key_len);
done:
	return ret;
}
