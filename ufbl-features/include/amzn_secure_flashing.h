/*
 * Copyright (C) 2015 - 2019 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#ifndef AMZN_SEC_FLASHING_H
#define AMZN_SEC_FLASHING_H

#include <stddef.h>

#define SEC_FLASH_CODE_LEN		(32)

#define DEVICE_META_SIZE		(32)
#define MAX_PUBKEY_LEN			(300)
#define SHA256_DIGEST_SIZE		(32)
#define RSA_2048_SIG_LEN		(256)

typedef struct {
	unsigned char device_meta[DEVICE_META_SIZE];
	unsigned int pk_len;
	unsigned char pubkey[MAX_PUBKEY_LEN];
	unsigned char signature[RSA_2048_SIG_LEN];
}__attribute__((packed, aligned(4))) sec_flash_cert;

typedef enum
{
	ERR_SECURE_FLASHING_SUCCESS = 0,
	ERR_SECURE_FLASHING_BAD_ARGUMENT,
	ERR_SECURE_FLASHING_NO_MEMORY,
	ERR_SECURE_FLASHING_BAD_KEY,
	ERR_SECURE_FLASHING_RSA_VERIFY_FAIL,
	ERR_SECURE_FLASHING_HASH_FAIL,
	ERR_SECURE_FLASHING_GET_CODE_FAIL,
	ERR_SECURE_FLASHING_GET_SEC_FLASH_CERT_FAIL,
	ERR_SECURE_FLASHING_DEVICE_META_VERIFY_FAIL,
	ERR_SECURE_FLASHING_GET_SEC_BLACKLIST_FAIL,
	ERR_SECURE_FLASHING_IN_BLACKLIST
} secure_flashing_error_code;

/* Support for secure flashing */
const unsigned char * amzn_get_sec_flashing_root_pubkey(unsigned int *key_len);
int amzn_verify_device_meta(const unsigned char *data, unsigned int len);
const unsigned char * amzn_get_sec_flashing_pk_blacklist(unsigned int *len);
const sec_flash_cert * amzn_get_sec_flashing_cert(unsigned int *len);
int amzn_set_sec_flashing_cert(unsigned char *buf, unsigned int len);
int amzn_verify_sec_flashing_cert(unsigned char do_verify);
int amzn_get_sec_flashing_code(unsigned char *code, unsigned int *len);
int amzn_verify_sec_flashing_code(const unsigned char *sig, unsigned int sig_len);
int amzn_device_is_sec_flashing_enabled(void);
int amzn_base64_decode(const unsigned char *b64_str, size_t len_in, unsigned char *b64_dec, size_t *len_out);

#endif
