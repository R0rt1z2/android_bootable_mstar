/*
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates.  All rights reserved.
 *
 */

#include <tomcrypt.h>
#include "usb_verifier.h"

hash_state usb_hash_state;

/*
 * Initialize the internal sha256 hash state
 */
void init_usb_verifier_hash_state()
{
    sha256_init(&usb_hash_state);
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
    register_hash(&sha256_desc);
}

/*
 * Process @buffer to the existing sha256 hash
 *
 * @buffer: data to process with the sha256 hash function
 * @buffer_size: amount to process with the sha256 hash function
 */
void usb_verifier_hash_process(unsigned char *buffer, unsigned long buffer_size)
{
    sha256_process(&usb_hash_state, buffer, buffer_size);
}

/*
 * Finalize the internal sha256 hash state, and then verify the calculated hash with the one given
 * in @signature, using public key @key.
 *
 * Returns 0 on success, -1 on any error
 *
 * @signature: signed value of the sha256 hash with the private key
 * @sig_len: length of the signature
 * @key: public key to verify the signature with
 * @key_len: length of the public key
 */
int finalize_usb_verifier_hash_state(const unsigned char *signature, unsigned int sig_len,
                                     const unsigned char *key, unsigned int key_len)
{
    rsa_key *rsa_pk = NULL;
    uint8_t digest[sha256_desc.hashsize];
    int ret = 0;
    int status;

    if (!signature || !key) {
        printf("Invalid arguments %s\n", __func__);
        return -1;
    }

    if ((rsa_pk = (rsa_key *)malloc(sizeof(rsa_key))) == NULL) {
        printf("Malloc failed\n");
        return -1;
    }

    if (rsa_import(key, key_len, rsa_pk) != CRYPT_OK) {
        printf("%s: Cannot parse key\n", __FUNCTION__);
        free(rsa_pk);
        return -1;
    }

    // Finalize internal hash state
    sha256_done(&usb_hash_state, digest);

    // Verify hash state with the signature
    ret = rsa_verify_hash(signature, sig_len, digest, sizeof(digest),
                          find_hash("sha256"), sha256_desc.hashsize,
                          &status, rsa_pk);

    if (ret == CRYPT_OK && status == 1) {
        rsa_free(rsa_pk);
        return 0;
    } else {
        printf("Rsa_verify_hash failed ret:%d status:%d\n", ret, status);
    }

    /* need to use rsa_free only if rsa_import is success */
    rsa_free(rsa_pk);
    return -1;
}
