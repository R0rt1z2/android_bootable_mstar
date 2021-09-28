/*
 * Copyright (c) 2016 - 2019 Amazon.com, Inc. or its affiliates.  All rights reserved.
 */

#include <tomcrypt.h>
#ifndef SUPPORT_UBOOT
#include <debug.h>
#endif
#include "amzn_temp_unlock.h"
#include "ufbl_debug.h" /* for dprintf */
#include "amzn_ufbl_alloc.h" /* for plat_alloc */
#include "unlock_ltc.h"

/**
 * Verify temporary unlock certificate's signature using root public key
 * @param cert Pointer to certificate buffer
 * @param buf Certificate buffer length
 * @return 0 - success, fail otherwise
 */
static int amzn_verify_temp_unlock_cert(const temp_unlock_cert_t *cert, unsigned int cert_len)
{
	int ret = -ERR_TEMP_UNLOCK_GENERAL_FAIL;
	unsigned int len;

	uint8_t digest[sha256_desc.hashsize];
	unsigned long digest_len = sha256_desc.hashsize;
	int sha256_idx = find_hash("sha256");

	const unsigned char *pk_blacklist = NULL;
	unsigned int key_len = 0;
	const unsigned char *key = NULL;

	if (!cert || !cert_len) {
		ret = -ERR_TEMP_UNLOCK_GET_CERT_FAIL;
		goto done;
	}

	ret = amzn_get_temp_unlock_root_pubkey(&key, &key_len);
	if (ret || !key || !key_len) {
		ret = -ERR_TEMP_UNLOCK_BAD_KEY;
		goto done;
	}
	/* Verify the signature (last 256 bytes) */
	if (amzn_verify_code_internal((unsigned char *)cert,
			(cert_len - RSA_2048_SIG_LEN),
			cert->signature, RSA_2048_SIG_LEN, key, key_len)) {
		ret = -ERR_TEMP_UNLOCK_RSA_VERIFY_FAIL;
		goto done;
	}

	if (amzn_verify_device_meta(cert->device_meta, DEVICE_META_SIZE)) {
		ret = -ERR_TEMP_UNLOCK_DEVICE_META_VERIFY_FAIL;
		goto done;
	}

	/* Check if the public key inside the cert is in blacklist */
	register_hash(&sha256_desc);
	if (hash_memory(sha256_idx, cert->pubkey, cert->pk_len, digest,
			&digest_len)) {
		ret = -ERR_TEMP_UNLOCK_HASH_FAIL;
		goto done;
	}

	ret = amzn_get_temp_unlock_pk_blacklist(&pk_blacklist, &len);
	if (!ret && pk_blacklist) {
		for (unsigned i = 0; i < len; i++) {
			if (!memcmp((pk_blacklist + i * SHA256_DIGEST_SIZE),
					digest, SHA256_DIGEST_SIZE)) {
				ret = -ERR_TEMP_UNLOCK_IN_BLACKLIST;
				goto done;
			}
		}
	}
	ret = TEMP_UNLOCK_SUCCESS;
done:
	return ret;
}

/**
 * Verify temporary unlock signature using the public key in cert.
 * @param sig Pointer to signature buffer
 * @param sig_len Signature buffer length
 * @return 0 - success, otherwise fail
 */
int amzn_verify_temp_unlock_code(const unsigned char *sig, unsigned int sig_len)
{
	int ret = -ERR_TEMP_UNLOCK_GENERAL_FAIL;
	unsigned int i = 0;
	unsigned int reboot_cnt = 0;
	temp_unlock_cert_t *cert = NULL;
	unsigned int cert_len = sizeof(temp_unlock_cert_t);
	unsigned char *codes = NULL;

	if (!sig || sig_len != RSA_2048_SIG_LEN) {
		ret = -ERR_TEMP_UNLOCK_GENERAL_FAIL;
		goto done;
	}

	ret = amzn_get_temp_unlock_codes(&codes, &reboot_cnt);
	if (ret || !codes || !reboot_cnt) {
		ret = -ERR_TEMP_UNLOCK_GET_CODE_FAIL;
		goto done;
	}

	/* Get base64 decoded temporary unlock certificate from IDME */
	if (amzn_get_temp_unlock_idme_cert(&cert, &cert_len)) {
		dprintf(CRITICAL, "%s: Failed to get temp unlock cert\n", __FUNCTION__);
		ret = -ERR_TEMP_UNLOCK_GET_CERT_FAIL;
		goto done;
	}

	/* Verify temporary unlock certificate using root public key */
	if ((ret = amzn_verify_temp_unlock_cert(cert, cert_len))) {
		dprintf(CRITICAL,
			"%s: Verify temp unlock cert fail, ret = %d\n", __FUNCTION__, ret);
		/* Clear IDME fields in case an invalid cert or code is written */
		amzn_clear_temp_unlock_idme();
		ret = -ERR_TEMP_UNLOCK_CERT_VERITY_FAIL;
		goto done;
	}

	for (i = 0; i < reboot_cnt; i++) {
		/* Verify if the signed signature matches any of the current unlock codes */
		if (!amzn_verify_code_internal(&codes[i * TEMP_UNLOCK_CODE_LEN],
				TEMP_UNLOCK_CODE_LEN,
				sig, sig_len, cert->pubkey, cert->pk_len)) {
			dprintf(CRITICAL,
				"Device is temporarily unlocked, %d reboots remaining\n", i);
			ret = TEMP_UNLOCK_SUCCESS;
			goto done;
		}
	}
	if (i >= reboot_cnt) {
		/**
		 * Clear IDME fields in case of failure to skip verification
		 * for future boot up
		 */
		amzn_clear_temp_unlock_idme();
		ret = ERR_TEMP_UNLOCK_GENERAL_FAIL;
	}
done:
	return ret;
}

/**
 * Helper function for base64 encode.
 */
int amzn_temp_unlock_b64_encode(const unsigned char *in, size_t len,
	unsigned char *out, size_t *outlen)
{
	return base64_encode(in, len, out, outlen);
}

/**
 * Helper function for base64 decode.
 */
int amzn_temp_unlock_b64_decode(const unsigned char *b64_str, size_t len_in,
	unsigned char *b64_dec, size_t *len_out)
{
	return base64_decode(b64_str, len_in, b64_dec, len_out);
}
