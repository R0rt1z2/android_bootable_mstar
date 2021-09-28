/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */
#include <x509.h>
#include <x509v3.h>
#include <openssl/err.h>
//#include <debug.h>
#include <string.h>
#include "amzn_secure_boot.h"
#include "ufbl_debug.h" /* for dprintf */
#include "amzn_ufbl_alloc.h" /* for plat_alloc, plat_free */

#if !defined(MAX)
#define MAX(x,y)	((x) > (y)) ? (x) : (y)
#endif

__WEAK void amzn_target_sha256(const void *data, size_t n, void *digest)
{
        /* Using a default software implementation for sha256 */
        SHA256(data, n, digest);
}

static int amzn_verify_cert(int cert_type, X509 *cert)
{
	int rc = -1;
	int X509_error;
	X509_STORE_CTX *ctx = NULL;
	X509_STORE *store = NULL;
	X509 *cacert = NULL;
	const unsigned char *cert_ptr = NULL;
	size_t cert_len = 0;

	/* Verify the certificate against our CA */
	store = X509_STORE_new();
	if (store == NULL) {
		dprintf(CRITICAL, "ERROR: Unable to create new X509 store.\n");
		goto done;
	}

	cert_ptr = amzn_get_kernel_cert(cert_type, &cert_len);
	cacert = d2i_X509(NULL, &cert_ptr, cert_len);
	if (!cacert) {
		dprintf(CRITICAL, "ERROR: Unable to load %s certificate\n",
			UFBL_STR((cert_type) ? "PRODUCTION" : "ENGINEERING"));
		goto done;
	}
	X509_STORE_add_cert(store, cacert);

	ctx = X509_STORE_CTX_new();
	if (!ctx) {
		dprintf(CRITICAL, "ERROR: Unable to create STORE CTX\n");
		goto done;
	}

	if (X509_STORE_CTX_init(ctx, store, cert, NULL) != 1) {
		dprintf(CRITICAL, "ERROR: Unable to initialize STORE CTX.\n");
		goto done;
	}

	rc = X509_verify_cert(ctx);

	if (rc != 1) {
		X509_error = X509_STORE_CTX_get_error(ctx);
		/* Allows image to be verified even with expired certificate */
		/* since the bootloader has no concept of time */
		if (X509_error != X509_V_ERR_CERT_HAS_EXPIRED){
			dprintf(CRITICAL,
				"ERROR: Certificate verify failed: %d\n",
				X509_error);
			goto done;
		}
	}

	rc = 1;

done:
	if (cacert)
		X509_free(cacert);

	if (store)
		X509_STORE_free(store);

	if (ctx)
		X509_STORE_CTX_free(ctx);

	if (rc == 1) {
		return 0;
	} else {
		return -1;
	}
}

static void amzn_parse_hash(X509 *cert, char *hash)
{
	BIGNUM *hash_bn = NULL;
	const char *hash_string = NULL;

	STACK_OF(GENERAL_NAME) *san = (STACK_OF(GENERAL_NAME) *)X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);

	if (!san || !sk_GENERAL_NAME_num(san)) {
		dprintf(CRITICAL,
			"ERROR: Hash not found in image certificate\n");
		goto done;
	}

	GENERAL_NAME *san_name = sk_GENERAL_NAME_value(san, 0);

	if (san_name->type != GEN_URI) {
		dprintf(CRITICAL,
			"ERROR: Invalid hash field\n");
		goto done;
	}

	hash_string = (char *)ASN1_STRING_data(san_name->d.uniformResourceIdentifier);

	if (!strncmp(hash_string, "sha256://", 9)) {
		/* SHA256 algorithm */
		hash_string += 9;
	} else {
		dprintf(CRITICAL,
			"ERROR: Invalid hash format\n");
		goto done;
	}

	if (BN_hex2bn(&hash_bn, hash_string) > (SHA256_DIGEST_LENGTH * 2)) {
		dprintf(CRITICAL, "ERROR: Invalid hash length\n");
		goto done;
	}

	/* Convert the hash into binary */
	memset(hash, 0, SHA256_DIGEST_LENGTH);
	/* Note: BN_bn2bin does not account for preceding 00s, we do a shift here */
	BN_bn2bin(hash_bn, (unsigned char *)hash + (SHA256_DIGEST_LENGTH - BN_num_bytes(hash_bn)));

done:
	if (hash_bn)
		BN_free(hash_bn);

	if (san)
		sk_GENERAL_NAME_pop_free(san, GENERAL_NAME_free);
}

int amzn_verify_image(int cert_type, char *computed_hash,
			const char *usercert, meta_data_handler handler)
{
	int ret = -1;
	char *hash = amzn_plat_alloc(SHA256_DIGEST_LENGTH);
	EVP_add_digest(EVP_sha1());
	EVP_add_digest(EVP_sha256());

	const unsigned char *p = usercert;

	X509 *cert = d2i_X509(NULL, &p, 2048);
	if (!cert) {
		dprintf(CRITICAL, "ERROR: Unable to parse certificate from image\n");
		goto done;
	}

	dprintf(INFO, "Certificate extracted from image\n");

	if (!hash) {
		dprintf(CRITICAL, "ERROR: Insufficent memory for hash\n");
		goto done;
	}

	if (amzn_verify_cert(cert_type, cert))
		goto done;

	dprintf(ALWAYS, "Certificate verified against %s CA\n",
		UFBL_STR(cert_type ? "PRODUCTION" : "ENGINEERING"));

	/* Extract the hash from the certificate */
	memset(hash, 0, SHA256_DIGEST_LENGTH);
	amzn_parse_hash(cert, hash);

	if (memcmp(hash, computed_hash, SHA256_DIGEST_LENGTH)) {
		dprintf(CRITICAL, "ERROR: Invalid hash\n");
		goto done;
	}

	dprintf(ALWAYS, "Image hash matched\n");

	X509_NAME *subj = X509_get_subject_name(cert);

	dprintf(ALWAYS, "==============\n");
	dprintf(ALWAYS, "Image Metadata\n");
	dprintf(ALWAYS, "==============\n");

	for (int i = 0; i < X509_NAME_entry_count(subj); i++) {
		X509_NAME_ENTRY *e = X509_NAME_get_entry(subj, i);
		ASN1_OBJECT *o = X509_NAME_ENTRY_get_object(e);
		ASN1_STRING *d = X509_NAME_ENTRY_get_data(e);
		char *str = (char *)ASN1_STRING_data(d);

		if (OBJ_obj2nid(o) == NID_organizationalUnitName) {
			dprintf(ALWAYS, "%s\n", UFBL_STR(str));

			if (handler && handler(str)) {
				/* Unexpected meta-data, fail */
				dprintf(ALWAYS, "Unexpected meta-data: %s\n", UFBL_STR(str));
				ret = -1;
				goto done;
			}

			if (strncmp(str, "product:", 8))
				continue;

			str += 8;

			const char *name = amzn_target_device_name();
			if (!strncmp(str, name, MAX(strlen(str), strlen(name))))
				/* Found a match */
				ret = 0;
		}
	}

	if (ret != 0) {
		dprintf(ALWAYS, "Signature not supported on this device\n");
	}
done:
	if (hash)
		amzn_plat_free(hash);

	if (cert)
		X509_free(cert);

	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_thread_state(NULL);
	return ret;
}
