/*
 * Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 * ufbl_bolt_compat.c
 */

#include "platform/bcm_platform.h"
#include "idme.h"
#include "ufbl_debug.h"

/**
* @brief  facade implementation for calloc function
*/
void *ufbl_calloc(size_t num, size_t size)
{
        void *ptr = NULL;
        size_t alloc_size = num * size;

        ptr = malloc(alloc_size);

        if (ptr == NULL)
                return (void *)NULL;

        memset(ptr, 0, alloc_size);

        return ptr;
}

/**
* @brief  facade implementation for toupper function
*/
int ufbl_toupper(int chr)
{
        if ((chr >= 'a') && (chr <= 'z'))
                return chr - ('a' - 'A');
        return chr;
}

/* Dummy function to avoid linker complaints */
void __aeabi_unwind_cpp_pr0(void)
{
}

/**
 * @brief BOLT function to process fastboot and BOLT cmdline idme
 *
 * @params cmd is commandline data if called from BOLT commandline
 *         in case of fastboot it must be called this param as NULL
 *         argc number of tokens in argument list argv
 *         argv argument list consisting of options.
 * @return int value of 0 on success and -1 on failure
 */
int do_idme_bolt(ui_cmdline_t *cmd, int argc, char * const argv[])
{
	/* This function is called by BOLT in 2 situations
	 * 1. By "fastboot oem idme"
	 * 2. Bolt command line "oem idme"
	 * from fastboot the argv[0] is "idme" from command
	 * which results into correct parsing in do_idme.
	 * from BOLT commandline the argv[0] is first option from command
	 * and missing "idme" which result into crash in do_idme()
	 * fixing by adding "idme" in payload when called from commandline.
	 */
	char **idme_argv = NULL;
	int idme_argc = 0;
	int i = 0;
	int result = 0;

	if (cmd == NULL) {
		/* from fastboot */
		result = do_idme(0, argc, argv);
	} else {
		/* From BOLT commandline */

		/* To make it future proof we need to consider
		 * all the possible values of argc and argv
		 * thus dynamically allocating argv, affixing first
		 * argument to be "idme" (present implementation).
		 */

		idme_argc = 1 + argc;

		/* Allocate space for idme_argv */
		idme_argv = (char **) malloc(idme_argc * sizeof (char *));
		if (idme_argv == NULL) {
			dprintf(CRITICAL, "IDME: Memory allocation failure!\n");
			return -1;
		}

		/* Initialize to ease cleanup later */
		for (i = 0; i < idme_argc; i++) {
			idme_argv[i] = NULL;
		}
		/* expecting each option be at most 32 char long including null terminating char */
		for (i = 0; i < idme_argc; i++) {
			idme_argv[i] = (char *) malloc (32 * sizeof (char));
			if (idme_argv[i] == NULL) {
				result = -1;
				dprintf(CRITICAL, "IDME: Memory allocation failure, exiting!\n");
				goto cleanup_and_exit;
			}
		}
		/* Affix first argument to "idme" */
		strcpy(idme_argv[0], "idme");

		for (i = 1; i <= argc; i++) {
			strncpy(idme_argv[i], argv[i-1], 31);
		}
		result = do_idme(0, idme_argc, idme_argv);
	}

cleanup_and_exit:

	for (i = 0; i < idme_argc; i++) {
		free(idme_argv[i]);
	}

	free(idme_argv);

	return result;
}
