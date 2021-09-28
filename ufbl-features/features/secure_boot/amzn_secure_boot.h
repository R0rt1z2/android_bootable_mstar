/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

#ifndef __AMZN_SECURE_BOOT_H
#define __AMZN_SECURE_BOOT_H

#include <stddef.h> /* size_t */

enum {
	AMZN_ENGINEERING_DEVICE = 0,
	AMZN_PRODUCTION_DEVICE,
	AMZN_INVALID_DEVICE,
};

enum {
	AMZN_ENGINEERING_CERT = 0,
	AMZN_PRODUCTION_CERT,
};

/* Define callback type for meta-data */
typedef int (*meta_data_handler)(const char *meta_data);

int amzn_image_verify(const void *image,
	unsigned char *signature,
	unsigned int image_size, meta_data_handler handler);

/* Returns engineering or production certificate */
const unsigned char *amzn_get_kernel_cert(int cert_type, size_t *length);

/* Implementation-specific weak functions */
int amzn_target_device_type(void);
const char *amzn_target_device_name(void);

void amzn_target_sha256(const void *data, size_t n, void *digest);
const unsigned char *amzn_get_kernel_cert(int cert_type, size_t *length);

#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

#endif
