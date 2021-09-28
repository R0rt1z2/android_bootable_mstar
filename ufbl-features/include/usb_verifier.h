/*
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates.  All rights reserved.
 *
 */

#ifndef USB_VERIFIER_H
#define USB_VERIFIER_H

/*
 * Initialize the internal sha256 hash state
 */
void init_usb_verifier_hash_state();

/*
 * Process @buffer to the existing sha256 hash
 *
 * @buffer: data to process with the sha256 hash function
 * @buffer_size: amount to process with the sha256 hash function
 */
void usb_verifier_hash_process(unsigned char *buffer, unsigned long buffer_size);

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
                                     const unsigned char *key, unsigned int key_len);

#endif

