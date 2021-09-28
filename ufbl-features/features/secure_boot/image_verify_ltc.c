/*
 * Copyright (C) 2015 - 2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#include <tomcrypt.h>
#include "amzn_secure_boot.h"
#include <stdint.h>
#include <stdio.h>
#include "ufbl_debug.h" /* for dprintf */

#if !defined(MAX)
#define MAX(x,y)	((x) > (y)) ? (x) : (y)
#endif

#ifdef DEBUG
static void dump_buffer(const uint8_t *buf, unsigned long size)
{
	unsigned long n = 0;

	if (!buf)
		return;

	for (n = 0; n < size; n++) {
		if (n && (n % 16 == 0))
			printf("\n");
		printf("%02x ", buf[n]);
	}
	printf("\n");
}
#endif

typedef int (*cmp_func)(ltc_asn1_list *p);

static void find_node(ltc_asn1_list *l, ltc_asn1_list **p, cmp_func func)
{
	if (l->child)
		find_node(l->child, p, func);

	/* Call the function pointer */
	if (func && func(l))
		if (p && !(*p))
			*p = l;

	if (l->next)
		find_node(l->next, p, func);
}

static int check_rsa_node(ltc_asn1_list *p)
{
	const unsigned long rsa_oid[] = { 1, 2, 840, 113549, 1, 1, 1 };

	if (p->type == LTC_ASN1_OBJECT_IDENTIFIER)
		if (memcmp(p->data, rsa_oid, sizeof(rsa_oid)) == 0)
			return 1;

	return 0;
}

static int check_san_node(ltc_asn1_list *p)
{
	const unsigned long ou_oid[] = { 2, 5, 29, 17 };

	if (p->type == LTC_ASN1_OBJECT_IDENTIFIER)
		if (memcmp(p->data, ou_oid, sizeof(ou_oid)) == 0)
			return 1;

	return 0;
}

static int extract_cert_key_pub(ltc_asn1_list *l, rsa_key *k)
{
	int ret = -1;
	uint8_t *key_seq = NULL;
	ltc_asn1_list *node = NULL;
	unsigned long len = 0;

	if (!k)
		goto done;

	find_node(l, &node, check_rsa_node);

	if (!node)
		goto done;

	/* Go back two levels up to locate the SubjectPublicKeyInfo node */
	if (node->parent && node->parent->parent) {
		node = node->parent->parent;
	} else {
		goto done;
	}

	if (der_length_sequence(node->data, node->size, &len) != CRYPT_OK) {
		printf("Unable to compute length of SubjectPublicKeyInfo node\n");
		goto done;
	}

	if (!(key_seq = XMALLOC(len))) {
		printf("malloc failed\n");
		goto done;
	}

	/* Re-encode the note into a sequence, so we can parse */
	if (der_encode_sequence_ex(node->data, node->size, key_seq,
			&len, LTC_ASN1_SEQUENCE) != CRYPT_OK) {
		printf("Failed to encode SubjectPublicKeyInfo\n");
		goto done;
	}

	if (rsa_import(key_seq, len, k) != CRYPT_OK) {
		printf("Failed to import certificate public key\n");
		goto done;
	}

	ret = 0;
done:
	if (key_seq)
		XFREE(key_seq);

	return ret;
}

static int decode_signature(ltc_asn1_list *l, uint8_t **signature, unsigned long *size)
{
	int ret = -1;
	uint8_t *data = NULL;
	unsigned long x = 0, y = 0, buf = 0;

	if (!signature || !size)
		goto done;

	if (l->type != LTC_ASN1_BIT_STRING || !l->data)
		goto done;

	data = (uint8_t *)l->data;

	x = (l->size >> 3) + ((l->size & 7) ? 1 : 0) + 1;

	if (!(*signature = XMALLOC(x)))
		goto done;

	for (x = y = buf = 0; y < l->size; y++) {
		buf |= (data[y] ? 1 : 0) << (7 - (y & 7));
		if ((y & 7) == 7) {
			(*signature)[x++] = buf;
			buf = 0;
		}
	}

	if (l->size & 7)
		(*signature)[x++] = buf;

	*size = x;
	ret = 0;
done:
	return ret;
}

static void der_rescan_tree(ltc_asn1_list *l)
{
	if (!l->child)
		/* No child, no work needed */
		goto done;

	/* Process any childs */
	der_rescan_tree(l->child);

	/* We must have processed all childs at this point */
	if (l->type == LTC_ASN1_SEQUENCE || l->type == LTC_ASN1_SET
			|| l->type == LTC_ASN1_CONSTRUCTED) {
		int c = 0, z = 0;
		ltc_asn1_list *p = l->child;

		/* Count how many childs are under this node */
		for (p = l->child; p; p = p->next, c++);

		ltc_asn1_list *s = XMALLOC(sizeof(ltc_asn1_list) * c);

		if (s) {
			p = l->child;

			/* We scan the subnodes and build a sequence manually */
			for (p = l->child; p; p = p->next) {
				LTC_SET_ASN1(s, z, p->type, p->data, p->size);
				s[z].used = p->used;
				z++;
			}

			l->data = s;
			l->size = z;
		}
	} else {
		printf("HAS CHILD: type %d\n", l->type);
	}

done:
	/* Process neighbor */
	if (l->next)
		der_rescan_tree(l->next);
}

static void der_free_rescan_tree(ltc_asn1_list *l)
{
	if (!l->child)
		/* No child, no work needed */
		goto done;

	/* Process any childs */
	der_free_rescan_tree(l->child);

	/* We must have processed all childs at this point */
	if (l->data && (l->type == LTC_ASN1_SEQUENCE
				|| l->type == LTC_ASN1_SET
				|| l->type == LTC_ASN1_CONSTRUCTED)) {
		XFREE(l->data);
		l->data = NULL;
		l->size = 0;
	} else {
		printf("HAS CHILD: type %d\n", l->type);
	}

done:
	/* Process neighbor */
	if (l->next)
		der_free_rescan_tree(l->next);
}

static int verify_certificate(ltc_asn1_list *l, rsa_key *root_ca_key)
{
	int ret = -1;
	int rc = 0, status = 0;
	uint8_t *signature = NULL, *tbs = NULL;
	unsigned long signature_size = 0, tbs_size = 0, len = 0;
	uint8_t tbs_hash[sha256_desc.hashsize];
	hash_state md;

	if (!l || !root_ca_key)
		goto done;

	/* First child is the tbsCertificate */
	if ((l = l->child) == NULL) {
		printf("tbsCertificate is empty\n");
		goto done;
	}

	if (der_length_sequence(l->data, l->size, &len)) {
		printf("Failed to get tbsCertificate length\n");
		goto done;
	} else if (!(tbs = XMALLOC(len))) {
		printf("Failed to malloc tbsCertificate\n");
		goto done;
	} else {
		tbs_size = len;
	}

	/* Get the whole DER encoded tbsCertificate */
	if (der_encode_sequence_ex(l->data, l->size, tbs, &tbs_size, LTC_ASN1_SEQUENCE)) {
		printf("Failed to encode tbsCertificate\n");
		goto done;
	}

	/* Run the SHA256 hash so we can verify with RSA-PSS */
	sha256_init(&md);
	sha256_process(&md, tbs, tbs_size);
	sha256_done(&md, tbs_hash);

	/* Third child is the RSA-PSS signature */
	if (l->next && l->next->next &&
			l->next->next->type == LTC_ASN1_BIT_STRING) {
		l = l->next->next;
	} else {
		printf("Cannot find signature in user certificate\n");
		goto done;
	}

	if (decode_signature(l, &signature, &signature_size)) {
		printf("Failed to decode signature\n");
		goto done;
	} else {
		rc = rsa_verify_hash(signature, signature_size,
			tbs_hash, sizeof(tbs_hash),
			find_hash("sha256"), sha256_desc.hashsize,
			&status, root_ca_key);
	}

	if (rc == 0 && status == 1)
		ret = 0;
done:
	if (signature)
		XFREE(signature);

	if (tbs)
		XFREE(tbs);

	return ret;
}

static int find_hash_string(const uint8_t *data, unsigned long data_len,
			char **hash_string)
{
	int ret = -1;
	ltc_asn1_list *san_data = NULL, *p = NULL;
	unsigned long len = data_len;

	if (!data || !hash_string)
		goto done;

	if (der_decode_sequence_flexi(data, &len, &san_data) != CRYPT_OK)
		goto done;

	p = san_data->child;

	while (p) {
		/* Look for a node which has tag 0x86 */
		if (p->type != LTC_ASN1_CONTEXT_SPECIFIC
				|| p->used != 0x86) {
			p = p->next;
			continue;
		}

		*hash_string = XMALLOC(p->size + 1);

		if (!(*hash_string))
			goto done;

		/* Copy and NULL-terminate the hash string */
		zeromem(*hash_string, p->size + 1);
		XMEMCPY(*hash_string, p->data, p->size);
		ret = 0;

		break;
	}	
done:
	if (san_data)
		der_sequence_free(san_data);

	return ret;
}

__WEAK void amzn_target_sha256(const void *data, size_t n, void *digest)
{
        /* Using a default software implementation for sha256 */
	unsigned long len = sha256_desc.hashsize;

	register_hash(&sha256_desc);
	hash_memory(find_hash("sha256"), data, n, digest, &len);
}

/**
 * @brief picks the plain ascii character from universal code point
 *        return error if the ucs buffer contains non-plain ascii
 *        character
 *
 * @param [out] ascii_str char * type null terminated plain ascii string
 * @param [in] ucs_buffer const wchar_t* type buffer containing UCS code point
 * @param [in] char_to_copy int type containing maximum number of characters to be
 *             copied into ascii_str excluding terminating '\0'.
 *
 * @return 0 if success, -1 if error
 *
 */
int ufbl_ucstoascii(char *ascii_str, const wchar_t *ucs_buffer, int char_to_copy)
{
	if ((ascii_str == NULL) ||
	    (ucs_buffer == NULL) ||
	    (char_to_copy == 0)) {
		dprintf(ALWAYS, "%s: Wrong input\n", __func__);
		return -1;
	}

	int i = 0;
	const int plain_ascii_max = 0x7f;

	/* Copy char_to_copy (indexed 0 to char_to_copy-1) characters from
	 * ucs_buffer to ascii string.
	 */
	for (i=0; i < char_to_copy; ++i) {
		if (ucs_buffer[i] > plain_ascii_max) {
			dprintf(ALWAYS, "%s: Unsupported universal character"
				   "code point %x\n", __func__, ucs_buffer[i]);
			return -1;
		}

		if ((ascii_str[i] = (char) ucs_buffer[i]) == 0)
			break;
	}

	if (i == char_to_copy)
		ascii_str[i] = '\0';

	return 0;
}

int amzn_verify_image(int cert_type, char *computed_hash,
		const unsigned char *usercert, meta_data_handler handler)
{
	/* Defaults to failure */
	int ret = -1;
	const uint8_t *root_cert = NULL;
	size_t root_cert_len = 0;
	ltc_asn1_list *decoded_root_cert, *decoded_user_cert = NULL, *l, *p;
	unsigned long len = 0;
	uint8_t cert_hash[sha256_desc.hashsize];
	void *mp_hash = NULL;
	char *hash_string = NULL;
	rsa_key root_ca_key;
	char *ascii_cstring = NULL;
	size_t size = 0;

#ifdef USE_LTM
#ifdef SUPPORT_BOLT
	/*
	* FIXME: Compiler inserts memcpy at later stage in build process
	* which causes the BOLT build to fail as memcpy is not provided by BOLT.
	* -fno-builtin and -ffreestanding have not able to eliminate compilation
	* error. Putting explicit memcpy to overcome linking error. Fix at later
	* point if compiler behaves perfect.
	*/
	memcpy(&ltc_mp, &ltm_desc, sizeof(ltm_desc));
#else
	ltc_mp = ltm_desc;
#endif
#elif defined(USE_TFM)
	ltc_mp = tfm_desc;
#elif defined(USE_GMP)
	ltc_mp = gmp_desc;
#else
	extern ltc_math_descriptor EXT_MATH_LIB;
	ltc_mp = EXT_MATH_LIB;
#endif
	register_hash(&sha256_desc);

	zeromem(cert_hash, sizeof(cert_hash));

	/* Get the root CA certificate */
	root_cert = amzn_get_kernel_cert(cert_type, &root_cert_len);
	len = root_cert_len;

	if (der_decode_sequence_flexi(root_cert, &len,
			&decoded_root_cert)) {
		printf("Failed to decode root certificate\n");
		goto done;
	}

	/* Re-scan the decoded root-certificate */
	der_rescan_tree(decoded_root_cert);

	len = 2048;

	if (der_decode_sequence_flexi(usercert, &len,
			&decoded_user_cert)) {
		printf("Failed to decode user certificate\n");
		goto done;
	}

	/* Re-scan the decoded user-certificate */
	der_rescan_tree(decoded_user_cert);

	l = decoded_user_cert;

	/* Extract the root CA key */
	if (extract_cert_key_pub(decoded_root_cert, &root_ca_key)) {
		printf("Failed to extract root CA public key\n");
		goto done;
	}

	if (verify_certificate(l, &root_ca_key)) {
		rsa_free(&root_ca_key);
		goto done;
	}

	rsa_free(&root_ca_key);
	dprintf(CRITICAL, "Certificate authenticated\n");

	/* Locate the subject node */
	if (!l->child || !l->child->child) {
		printf("Invalid certificate 0\n");
		goto done;
	}

	l = l->child->child;

	if (!l->next
			|| !l->next->next
			|| !l->next->next->next
			|| !l->next->next->next->next
			|| !l->next->next->next->next->next) {
		printf("Invalid certificate 1\n");
		goto done;
	}

	l = l->next->next->next->next->next;

	if (l->type != LTC_ASN1_SEQUENCE) {
		printf("Invalid certificate 2\n");
		goto done;
	}

	dprintf(ALWAYS, "==============\n");
	dprintf(ALWAYS, "Image Metadata\n");
	dprintf(ALWAYS, "==============\n");

	for (p = l->child; p; p = p->next) {
		const unsigned long ou_oid[] = { 2, 5, 4, 11 };

		if (p->child && p->child->child
				&& p->child->child->next) {
			/* Look for the OU oid */
			if (memcmp(p->child->child->data, ou_oid, sizeof(ou_oid)))
				continue;

			if (p->child->child->next->type != LTC_ASN1_PRINTABLE_STRING
					&& p->child->child->next->type != LTC_ASN1_UTF8_STRING)
				continue;

			/* Found the string */
			char *str = (char *)p->child->child->next->data;

			if (!str)
				continue;

			size_t str_size = p->child->child->next->size;

			/*
			 * Remake buffer if str_size
			 * is bigger than the previous one
			 */
			if ((str_size + 1) > size) {
				if (ascii_cstring != NULL)
					free(ascii_cstring);
				/* Allocate one byte more for terminating '\0' character
				 * ASN1 DER rule does not guarantee terminating null character
				 * be encoded at end of string.
				 */
				size = str_size + 1;
				ascii_cstring = malloc(size);
				if (ascii_cstring == NULL) {
					dprintf(ALWAYS, "Unable to allocate memory\n");
					goto done;
				}
			}

			memset(ascii_cstring, 0, size);

			if (p->child->child->next->type == LTC_ASN1_UTF8_STRING) {

				/* Copy str_size characters from UCS buffer to ascii string */
				if (ufbl_ucstoascii(ascii_cstring,
							(const wchar_t *)str, str_size) < 0) {
					dprintf(ALWAYS,"Error in copying to ASCII string");
					goto done;
				}
			} else {
				/* LTC_ASN1_PRINTABLE_STRING */

				/*
				 * Copy str_size characters from string buffer to ascii string
				 * and null terminate the ascii string
				 */
				memcpy(ascii_cstring, str, str_size);
				ascii_cstring[size-1] = '\0';
			}

			str = ascii_cstring;

			dprintf(ALWAYS, "%s\n", str);

			if (handler && handler(str)) {
				/* Unexpected meta-data, fail */
				dprintf(ALWAYS, "Unexpected meta-data: %s\n", str);
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

	if (ret != 0)
		goto done;

	/* Now, check the image hash against the one in certificate */
	ret = -1;

	ltc_asn1_list *san_node = NULL;
	find_node(l, &san_node, check_san_node);

	if (!san_node || !san_node->next || !san_node->next->data) {
		dprintf(CRITICAL, "Cannot find SubjectAltName node\n");
		goto done;
	}

	/* Extract the hash string from the SubjectAltName node */
	if (!find_hash_string(san_node->next->data,
			san_node->next->size, &hash_string)) {
		char *ptr = hash_string + 9; // "sha256://"

		/* Parse the string into binary blob */
		if (mp_init(&mp_hash) != CRYPT_OK) {
			dprintf(ALWAYS, "Cannot allocate mp_hash\n");
			goto done;
		}

		if (mp_read_radix(mp_hash, ptr, 16) != CRYPT_OK) {
			dprintf(ALWAYS, "mp_read_radix failed\n");
			goto done;
		}

		if (mp_unsigned_bin_size(mp_hash) > sha256_desc.hashsize) {
			dprintf(ALWAYS, "Unexpected hash size\n");
			goto done;
		}

		uint8_t *buffer = cert_hash;

		buffer += (sha256_desc.hashsize
				- mp_unsigned_bin_size(mp_hash));

		if (mp_to_unsigned_bin(mp_hash, buffer) != CRYPT_OK) {
			dprintf(ALWAYS, "mp_to_unsigned_bin failed\n");
			goto done;
		}

		/* Constant-time comparison */
		if (XMEM_NEQ(computed_hash, cert_hash, sha256_desc.hashsize) == 0)
			ret = 0;
	}
done:
	if (ascii_cstring != NULL)
		free(ascii_cstring);

	if (hash_string)
		XFREE(hash_string);

	if (mp_hash != NULL)
		mp_clear(mp_hash);

	if (decoded_root_cert) {
		der_free_rescan_tree(decoded_root_cert);
		der_sequence_free(decoded_root_cert);
	}

	if (decoded_user_cert) {
		der_free_rescan_tree(decoded_user_cert);
		der_sequence_free(decoded_user_cert);
	}

	
	return ret;
}
