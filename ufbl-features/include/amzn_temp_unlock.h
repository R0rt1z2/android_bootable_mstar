/*
 * Copyright (c) 2015 - 2019 Amazon.com, Inc. or its affiliates.  All rights reserved.
 */
#ifndef AMZN_TEMP_UNLOCK_H
#define AMZN_TEMP_UNLOCK_H

#include <stddef.h>

#define TEMP_UNLOCK_HMAC_HASH_SIZE (32)

#define TEMP_UNLOCK_CODE_LEN		(32)
#define TEMP_UNLOCK_IDME_CERT_SIZE  (1024)
#define TEMP_UNLOCK_IDME_CODE_SIZE  (512)

#define DEVICE_META_SIZE		(32)
#define MAX_PUBKEY_LEN			(300)
#define SHA256_DIGEST_SIZE		(32)
#define RSA_2048_SIG_LEN		(256)

#define BASE64_LEN(x) ((x + 2) / 3 * 4)

typedef struct {
	unsigned char device_meta[DEVICE_META_SIZE];
	unsigned int pk_len;
	unsigned char pubkey[MAX_PUBKEY_LEN];
	unsigned char signature[RSA_2048_SIG_LEN];
}__attribute__((packed, aligned(4))) temp_unlock_cert_t;

typedef enum
{
	TEMP_UNLOCK_SUCCESS = 0,
	ERR_TEMP_UNLOCK_GENERAL_FAIL,
	ERR_TEMP_UNLOCK_BAD_ARGUMENT,
	ERR_TEMP_UNLOCK_NO_MEMORY,
	ERR_TEMP_UNLOCK_BAD_KEY,
	ERR_TEMP_UNLOCK_RSA_VERIFY_FAIL,
	ERR_TEMP_UNLOCK_HASH_FAIL,
	ERR_TEMP_UNLOCK_GET_CODE_FAIL,
	ERR_TEMP_UNLOCK_GET_CERT_FAIL,
	ERR_TEMP_UNLOCK_CERT_VERITY_FAIL,
	ERR_TEMP_UNLOCK_DEVICE_META_VERIFY_FAIL,
	ERR_TEMP_UNLOCK_GET_SEC_BLACKLIST_FAIL,
	ERR_TEMP_UNLOCK_IN_BLACKLIST,
} temp_unlock_error_code;

/* Support for temp unlock */
int amzn_get_temp_unlock_codes(unsigned char **codes, unsigned int *reboot_cnt);
int amzn_get_temp_unlock_current_code(unsigned char *buf, unsigned long *outlen);
int amzn_get_temp_unlock_root_pubkey(unsigned char **key, unsigned int *key_len);
int amzn_verify_device_meta(const unsigned char *data, unsigned int len);
int amzn_get_temp_unlock_pk_blacklist(unsigned char **buf, unsigned int *len);

int amzn_get_temp_unlock_idme_cert(temp_unlock_cert_t **cert, unsigned int *len);
int amzn_get_temp_unlock_idme_code(unsigned char **sig, unsigned int *len);
int amzn_set_temp_unlock_idme_cert(const char *b64_buf, unsigned int size);
int amzn_set_temp_unlock_idme_code(const char *b64_buf, unsigned int size);
int amzn_clear_temp_unlock_idme(void);

int amzn_target_is_temp_unlocked(void);
int amzn_temp_unlock_b64_decode(const unsigned char *b64_str, size_t len_in,
	unsigned char *b64_dec, size_t *len_out);
int amzn_temp_unlock_b64_encode(const unsigned char *in, size_t len,
	unsigned char *out, size_t *outlen);
int amzn_verify_temp_unlock_code(const unsigned char *sig, unsigned int sig_len);
#endif
