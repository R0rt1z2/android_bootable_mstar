/*
 * Copyright (C) 2015 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */

#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <debug.h>
#include <amzn_unlock.h>
#include "ufbl_debug.h"  /* for dprintf */
#include "amzn_ufbl_alloc.h" /* for plat_alloc */

#define RSA2048_BYTE_LEN	(256)
#define SHA256_BYTE_LEN		(32)

int amzn_verify_limited_unlock(void *data, unsigned int size, unsigned int ctr_min, unsigned int ctr_max)
{
	int rc = -1;
	int ret = -1;
	RSA *rsa_key = NULL;
	unsigned int key_len = 0;
	const unsigned char *key = amzn_get_unlock_key(&key_len);
	EVP_add_digest(EVP_sha256());
	unsigned char *digest = NULL, *decrypted = NULL;
	unsigned char code[UNLOCK_CODE_LEN];
	unsigned int code_len = sizeof(code);
	unsigned int ctr;

	if (!(digest = amzn_plat_alloc(SHA256_BYTE_LEN)) ||
			!(decrypted = amzn_plat_alloc(RSA2048_BYTE_LEN))) {
		dprintf(CRITICAL, "%s: Insufficient memory\n", UFBL_STR(__FUNCTION__));
		goto done;
	}

	if ((rsa_key = d2i_RSA_PUBKEY(NULL, &key, key_len)) == NULL) {
		dprintf(CRITICAL, "%s: Failed to convert RSA key\n",
			UFBL_STR(__FUNCTION__));
		goto done;
	}

	/* Decrypt the RSA signature */
	if (size < RSA2048_BYTE_LEN) {
		dprintf(CRITICAL, "%s: Invalid signature size: %d\n",
			UFBL_STR(__FUNCTION__), size);
		goto done;
	}

	if (RSA_public_decrypt(RSA2048_BYTE_LEN, data, decrypted,
			rsa_key, RSA_NO_PADDING) != RSA2048_BYTE_LEN) {
		dprintf(CRITICAL, "%s: RSA decryption failed\n",
			UFBL_STR(__FUNCTION__));
		goto done;
	}

	for (ctr = ctr_min; ctr <= ctr_max; ctr++) {
		/* Retrieve unlock code and compute the digest */
		if (ctr_min != ctr_max)
			ret = amzn_get_limited_unlock_code(code, &code_len, ctr);
		else
			ret = amzn_get_unlock_code(code, &code_len);
		if (ret) {
			dprintf(CRITICAL, "%s: Failed to get unlock code\n",
				UFBL_STR(__FUNCTION__));
			goto done;
		}

		SHA256(code, code_len, digest);

		/* PSS-Verify */
		if (RSA_verify_PKCS1_PSS(rsa_key, digest,
					 EVP_sha256(), decrypted, -1) == 1) {
			dprintf(CRITICAL, "%s: Unlock code verified\n",
				UFBL_STR(__FUNCTION__));
#if !defined(SUPPORT_KERNELFLINGER)
			fastboot_publish("unlock_status", "true");
#endif
			rc = 0;
			break;
		}
	}
	if (rc != 0) {
		dprintf(CRITICAL, "%s: Invalid unlock code\n",
			UFBL_STR(__FUNCTION__));
	}
done:
	if (decrypted != NULL)
		amzn_plat_free(decrypted);

	if (digest != NULL)
		amzn_plat_free(digest);

	if (rsa_key != NULL)
		RSA_free(rsa_key);

	return rc;
}

int amzn_verify_unlock(void *data, unsigned int size)
{
	return amzn_verify_limited_unlock(data, size, 0, 0);
}
