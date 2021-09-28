/*
 * Copyright (c) 2015 - 2019 Amazon.com, Inc. or its affiliates.  All rights reserved.
 */
#include "amzn_temp_unlock.h"
#include "ufbl_debug.h" /* for dprintf */
#ifdef SUPPORT_BOLT
#include <platform/bcm_platform.h>
#endif
#include "amzn_ufbl_alloc.h" /* for plat_alloc */
#include "idme.h"

/* Compiler may already define this.
 * If it doesn't, try this other compiler specific setting.
 * __attribute__((weak)) is specific to the GNU toolchain.
 */
#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

#if defined(UFBL_FEATURE_TEMP_UNLOCK)

/* Magic string at the begining of IDME field*/
#define AMZN_TEMP_UNLOCK_IDME_MAGIC "AZTU"
#define AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE 4

/**
 * Get temporary unlock codes.
 * @param codes Double Pointer to unlock codes buffer
 * @param reboot_cnt Pointer to reboot_cnt, to be filled
 * @return 0 - success, fail otherwise
 */
__WEAK int amzn_get_temp_unlock_codes(unsigned char **codes, unsigned int *reboot_cnt)
{
	(void)(codes);
	(void)(reboot_cnt);
	return -1;
}

/**
 * Get temporary unlock root public key (per product key).
 * @param key Double Pointer to root public key buffer
 * @param key_len Pointer to key length
 * @return 0 - success, fail otherwise
 */
__WEAK int amzn_get_temp_unlock_root_pubkey(unsigned char **key, unsigned int *key_len)
{
	(void)(key);
	(void)(key_len);
	return -1;
}

/**
 * Verify temporary unlock device specific meta info.
 * @param data Pointer to device meta info buffer
 * @param len Device meta info buffer size
 * @return 0 - success, fail otherwise
 */
__WEAK int amzn_verify_device_meta(const unsigned char *data, unsigned int len)
{
	(void)(data);
	(void)(len);
	return 0;
}

/**
 * Get temporary unlock public key blacklist,
 * it is a buffer of SHA256 hash values.
 * @param buf Double pointer to blacklist buffer
 * @param len Pointer to output blacklist length
 * @return 0 - success, fail otherwise
 */
__WEAK int amzn_get_temp_unlock_pk_blacklist(unsigned char **buf, unsigned int *len)
{
	(void)(buf);
	(void)(len);
	return -1;
}

/**
 * Get current temporary unlock code,
 * the current code is the last one from the codes buffer
 * it is base64 encoded to print on fastboot getvar command
 * and pass into kernel command line.
 * This code can be retrieved by fastboot getvar command
 * or should be passed to kernel command line
 */
int amzn_get_temp_unlock_current_code(unsigned char *buf, unsigned long *outlen)
{
	int ret = -1;
	unsigned int reboot_cnt = 0;
	unsigned char *codes = NULL;

	if (!buf || !outlen) {
		goto done;
	}

	ret = amzn_get_temp_unlock_codes(&codes, &reboot_cnt);
	if (ret || !codes || !reboot_cnt) {
		dprintf(CRITICAL, "%s: Failed to get temp unlock codes\n", __FUNCTION__);
		ret = -1;
		goto done;
	}

	ret = amzn_temp_unlock_b64_encode(&codes[TEMP_UNLOCK_CODE_LEN * (reboot_cnt - 1)],
								TEMP_UNLOCK_CODE_LEN,
								buf,
								outlen);
	if (ret || *outlen != BASE64_LEN(TEMP_UNLOCK_CODE_LEN)) {
		dprintf(CRITICAL, "%s: base64 encode failed: %d\n", __FUNCTION__, ret);
		memset(buf, 0, *outlen);
		ret = -1;
		goto done;
	}
	buf[*outlen] = 0;
	ret = 0;
done:
	return ret;
}

/**
 * Get base64 encoded temporary unlock code or certificate
 * from IDME, check the magic number and decoded into the buffer passed in.
 * @param name IDME field name
 * @param buf Pointer to the write buffer
 * @return len Pointer to the output length of the buffer.
 */
static int amzn_get_temp_unlock_idme_data(char *name, unsigned char *buf, unsigned int *len)
{
	int ret = -1;
	char *b64_buf = NULL;
	unsigned int outlen = *len;

	if (!name || !buf || !len)
		goto done;

	unsigned int b64_len = BASE64_LEN(*len);
	/* IDME field will look like "AZTU" + base64_encoded(cert/code) */
	if (!(b64_buf = amzn_plat_alloc(b64_len +
			AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE + 1))) {
		dprintf(CRITICAL, "%s: amzn_plat_alloc failed\n", __FUNCTION__);
		goto done;
	}
	memset(b64_buf, 0, b64_len + AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE + 1);
	if (idme_get_var_external(name, (char *)b64_buf,
			b64_len + AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE)) {
		dprintf(CRITICAL, "%s: idme_get_var_external fail\n", __FUNCTION__);
		goto done;
	}
	/* Check magic string */
	if (memcmp(b64_buf, AMZN_TEMP_UNLOCK_IDME_MAGIC,
			AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE)) {
		goto done;
	}

	if (amzn_temp_unlock_b64_decode(b64_buf + AMZN_TEMP_UNLOCK_IDME_MAGIC_SIZE,
			b64_len, buf, &outlen) || outlen != *len) {
		dprintf(CRITICAL, "%s: b64 decode error, outlen %d\n", __FUNCTION__, *len);
		memset(buf, 0, *len);
		goto done;
	}
	ret = 0;
done:
	if (b64_buf) {
		amzn_plat_free(b64_buf);
		b64_buf = NULL;
	}
	return ret;
}

/**
 * Get temporary unlock certificate.
 */
int amzn_get_temp_unlock_idme_cert(temp_unlock_cert_t **cert, unsigned int *len)
{
	static temp_unlock_cert_t *tu_cert = NULL;
	int ret = -1;

	if (!cert || !len)
		goto done;

	if (!tu_cert) {
		tu_cert = amzn_plat_alloc(sizeof(temp_unlock_cert_t));
		if (!tu_cert) {
			dprintf(CRITICAL, "%s: amzn_plat_alloc failed\n", __FUNCTION__);
			goto done;
		}
	}

	if (!amzn_get_temp_unlock_idme_data("t_unlock_cert",
			(unsigned char *)(tu_cert), len)) {
		*cert = tu_cert;
		ret = 0;
	}
done:
	return ret;
}

/**
 * Get signed temporary unlock code.
 */
int amzn_get_temp_unlock_idme_code(unsigned char **sig, unsigned int *len)
{
	static unsigned char *tu_code = NULL;
	int ret = -1;

	if (!sig || !len)
		goto done;

	if (!tu_code) {
		tu_code = amzn_plat_alloc(RSA_2048_SIG_LEN);
		if (!tu_code) {
			dprintf(CRITICAL, "%s: amzn_plat_alloc failed\n", __FUNCTION__);
			goto done;
		}
	}

	if (!amzn_get_temp_unlock_idme_data("t_unlock_code", tu_code, len)) {
		*sig = tu_code;
		ret = 0;
	}
done:
	return ret;
}

/**
 * Set temporary unlock certificate.
 */
int amzn_set_temp_unlock_idme_cert(const char *buf, unsigned int size)
{
	if (!buf || size > TEMP_UNLOCK_IDME_CERT_SIZE)
		return -1;
	return idme_update_var_ex("t_unlock_cert", buf, size);
}

/**
 * Set temporary unlock code (signature).
 */
int amzn_set_temp_unlock_idme_code(const char *buf, unsigned int size)
{
	if (!buf || size > TEMP_UNLOCK_IDME_CODE_SIZE)
		return -1;
	return idme_update_var_ex("t_unlock_code", buf, size);
}

/**
 * Clear temporary unlock code and certificate in IDME.
 */
int amzn_clear_temp_unlock_idme(void)
{
	idme_update_var_ex("t_unlock_cert", "", 0);
	idme_update_var_ex("t_unlock_code", "", 0);
	return 0;
}

/**
 * Verify temporary unlock code and certificate.
 */
int amzn_target_is_temp_unlocked(void)
{
	unsigned char is_unlocked = 0;
	unsigned int len = RSA_2048_SIG_LEN;
	unsigned char *sig = NULL;
	if (amzn_get_temp_unlock_idme_code(&sig, &len) ||
		!sig || len != RSA_2048_SIG_LEN)
		goto done;

	if (!amzn_verify_temp_unlock_code(sig, RSA_2048_SIG_LEN)) {
		is_unlocked = 1;
	}
done:
	return is_unlocked;
}
#endif
