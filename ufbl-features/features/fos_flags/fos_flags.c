/*
 * Copyright (C) 2016 - 2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#if defined(UFBL_PLATFORM_MTK)
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#else
#include <common.h>
#include <malloc.h>
#include <linux/ctype.h>
#endif

#include <fastboot.h>

/* We need SUPPORT_UBOOT for the UFBL debug code to do the right thing. */
#if defined(UFBL_PROJ_ABC)
#define SUPPORT_UBOOT 1
#endif
#include <ufbl_debug.h>
#if defined(UFBL_PROJ_ABC)
#undef SUPPORT_UBOOT
#endif
#include <idme.h>

#if defined(UFBL_FEATURE_UNLOCK)
#include <amzn_unlock.h>
#endif
#if defined(UFBL_FEATURE_SECURE_BOOT)
#include <amzn_secure_boot.h>
#endif

#if defined(UFBL_FEATURE_TEMP_UNLOCK)
#include <amzn_temp_unlock.h>
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#endif

typedef enum {
	FLAGS_CMD_HELP = 1,
	FLAGS_CMD_PRINT = 2,
	FLAGS_CMD_SET = 3
} flags_cmd_t;

typedef enum {
	FLAGS_TYPE_NONE = 0,
	FLAGS_TYPE_FOS = 1,
	FLAGS_TYPE_DEV = 2,
	FLAGS_TYPE_USR = 3
} flags_type_t;

typedef enum {
	FLAGS_MOD_NONE = 0,
	FLAGS_MOD_SET = 1,
	FLAGS_MOD_ADD = 2,
	FLAGS_MOD_REMOVE = 3
} flags_modifier_t;

/* These functions are for U-Boot and other bootloaders that don't provide
 * them. LK, notably, does.
 */
__attribute__((weak)) int hexval(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

__attribute__((weak)) unsigned long long atoull(const char *num) {
	unsigned long long value = 0;
	if (num[0] == '0' && num[1] == 'x') {
		// hex
		num += 2;
		while (*num && isxdigit(*num))
			value = value * 16 + hexval(*num++);
	} else {
		// decimal
		while (*num && isdigit(*num))
			value = value * 10 + *num++ - '0';
	}

	return value;
}

/* Command-specific helper functions */
static inline int ishexprefix(char *s) {
	return s && *s && ('0' == *s) && ('x' == *(s + 1) || 'X' == *(s + 1));
}

static inline int ishexdigit(char s) {
	return ((s >= 'a') && (s <= 'f')) || ((s >= 'A') && (s <= 'F'));
}

static inline unsigned long long getnumval(char *s) {

	if (s && *s) {
		if (ishexprefix(s)) {
			// For hex, normalize to lower case for atoull function
			*(s + 1) = tolower(*(s + 1));
		}
		return atoull(s);
	}
	return 0;
}

/* Needed for command support on ancient U-Boot and LK */
#if !defined(UFBL_PROJ_ABC)
static char *skip_spaces(const char *arg) {
	while(*arg && isblank(*arg))
		arg++;

	return arg;
}
#endif

/**
 * Check if our bootloader is in a 'locked' state. This is a wrapper around
 * platform-specific code, since each platform may store the 'locked' state in a
 * different way.
 *
 * @return 0 if unlocked, 1 if locked.
 */
static inline int bootloader_is_locked(void) {
#if (defined UFBL_FEATURE_UNLOCK) && (defined UFBL_FEATURE_SECURE_BOOT)
	/* Only check target locked status for production devices */
	int locked = (AMZN_PRODUCTION_DEVICE == amzn_target_device_type()) &&
	             (!amzn_target_is_unlocked());
#if defined(UFBL_FEATURE_TEMP_UNLOCK)
	locked = (locked && (!amzn_target_is_temp_unlocked()));
#endif
#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
	locked = (locked && (!amzn_target_is_onetime_unlocked()));
#endif
	return locked;

#else
	return 0;
#endif
}

static inline bool scan_for_prefix(char **haystack, const char *needle) {
	if (!strncasecmp(*haystack, needle, strlen(needle))) {
		*haystack += strlen(needle);
		return true;
	}

	return false;
}

bool oem_flags(char *arg, /* out */char *response, const size_t response_length) {
	char *target_flags = NULL;
	char new_flags_str[19] = {0};
	char cur_flags_str[19] = {0};
	unsigned long long cur_flags = 0;
	unsigned long long new_flags = 0;
	unsigned long long value = 0;
	bool success = false;

	flags_cmd_t flags_cmd = FLAGS_CMD_PRINT;
	flags_modifier_t flags_modifier = FLAGS_MOD_SET;
	flags_type_t flags_type = FLAGS_TYPE_NONE;

	/* Default to a printout, when just oem flags is called */
	if (!arg || !*arg) {
		flags_cmd = FLAGS_CMD_PRINT;
		flags_type = bootloader_is_locked() ? FLAGS_TYPE_USR : FLAGS_TYPE_FOS;
		flags_modifier = FLAGS_MOD_NONE;
		goto parse_done;
	}

	/* Skip any preceding whitespace */
	arg = skip_spaces(arg);
	if (!*arg) goto parse_done;

	/* find type: fos_flags, dev_flags or usr_flags */
	if (scan_for_prefix(&arg, "f:") ||
	    scan_for_prefix(&arg, "fos:") ||
	    scan_for_prefix(&arg, "fos_flags:")) {
		flags_type = FLAGS_TYPE_FOS;
	} else if (scan_for_prefix(&arg, "d:") ||
		   scan_for_prefix(&arg, "dev:") ||
		   scan_for_prefix(&arg, "dev_flags:")) {
		flags_type = FLAGS_TYPE_DEV;
	} else if (scan_for_prefix(&arg, "u:") ||
		   scan_for_prefix(&arg, "usr:") ||
		   scan_for_prefix(&arg, "usr_flags:")) {
		flags_type = FLAGS_TYPE_USR;
	}

	/* Find modifier +, -, = (= default) */
	arg = skip_spaces(arg);
	if (!*arg) goto parse_done;

	switch (*arg) {
		case '+':
			flags_modifier = FLAGS_MOD_ADD;
			arg++;
			break;
		case '-':
			flags_modifier = FLAGS_MOD_REMOVE;
			arg++;
			break;
		case '=':
			flags_modifier = FLAGS_MOD_SET;
			arg++;
			break;
		default:
			break;
	}

	/* Skip ahead to the next block */
	arg = skip_spaces(arg);
	if (!*arg) goto parse_done;

	/* Check for the magic values 'print' and 'help', otherwise it's a value. */
	if (!strncasecmp(arg, "print", strlen("print"))) {
		arg += strlen("print");
		flags_cmd = FLAGS_CMD_PRINT;
		flags_modifier = FLAGS_MOD_NONE;
		arg = skip_spaces(arg);

		if (FLAGS_TYPE_NONE == flags_type) {
			flags_type = bootloader_is_locked() ? FLAGS_TYPE_USR : FLAGS_TYPE_FOS;
		}

		if (('u' == *arg) || ('U' == *arg))
			flags_type = FLAGS_TYPE_USR;
		else if (('f' == *arg) || ('F' == *arg))
			flags_type = FLAGS_TYPE_FOS;
		else if (('d' == *arg) || ('D' == *arg))
			flags_type = FLAGS_TYPE_DEV;

		goto parse_done;
	} else if (!strncasecmp(arg, "help", strlen("help")) ||
		   !strncmp(arg, "?", strlen("?"))) {
		flags_cmd = FLAGS_CMD_HELP;
		flags_modifier = FLAGS_MOD_NONE;
		flags_type = FLAGS_TYPE_NONE;
		goto parse_done;
	} else {
		/* Find the value */
		value = 0;
		char *cur_ptr = NULL;
		while (*arg) {
			int hex = 0;
			cur_ptr = arg;

			/* Verify we have valid numerical values, optionally | or space separated */
			while (*arg) {
				if ((arg == cur_ptr) && ishexprefix(arg)) {
					hex=1;
					arg+=2;
				} else if (isdigit(*arg) || (hex && ishexdigit(*arg))) {
					arg++;
				} else if (*arg == '\0' || *arg == '|' || isblank(*arg)) {
					break;
				} else {
					dprintf(CRITICAL, "Non-numeric value found: '%s'\n", cur_ptr);
					flags_cmd = FLAGS_CMD_HELP;
					goto parse_done;
				}
			}
			if (*arg) {
				// Prepare for next round
				arg++;
				while (*arg && (isblank(*arg) || '|' == *arg)) arg++;
			}

			flags_cmd = FLAGS_CMD_SET;
			value |= getnumval(cur_ptr);
		}
	}

parse_done:
	dprintf(SPEW, "Parsing done.\n");

	if (FLAGS_CMD_SET == flags_cmd) {
		if (bootloader_is_locked()) {
			if ((flags_type != FLAGS_TYPE_USR) && (flags_type != FLAGS_TYPE_NONE)) {
				/* Locked device - can only set usr flags */
				snprintf(response, response_length, "Only usr_flags can be set for a locked device");
				goto end;
			}
			flags_type = FLAGS_TYPE_USR;
		} else if (flags_type == FLAGS_TYPE_NONE) {
			flags_type = FLAGS_TYPE_FOS;
			dprintf(INFO, "%s: Assuming fos_flags. Use <flag_type>:<flag_value> to use another.\n", __func__);
		}

	}

	switch (flags_type) {
		case FLAGS_TYPE_DEV:
			target_flags = "dev_flags";
			break;
		case FLAGS_TYPE_USR:
			target_flags = "usr_flags";
			break;
		case FLAGS_TYPE_FOS:
			target_flags = "fos_flags";
			break;
		default:
			if (bootloader_is_locked()) {
				target_flags = "usr_flags";
			} else {
				target_flags = "fos_flags";
			}
			break;
	}

	cur_flags = 0;
	if (0 != idme_get_var_external(target_flags, cur_flags_str + 2, 16)) {
		/* We don't fail here, as it's *possible* that you're setting
		 * the flags for the first time.
		 */
		dprintf(CRITICAL, "%s: Error getting %s value\n", __func__, target_flags);
		cur_flags_str[0] = 0;
	} else {
		// Need to prefix with 0x for atoull to handle hex.
		cur_flags_str[0] = '0';
		cur_flags_str[1] = 'x';
	}
	cur_flags = atoull(cur_flags_str);

	switch (flags_cmd) {
		case FLAGS_CMD_PRINT: {
			dprintf(SPEW, "%s is currently 0x%llx\n", target_flags, cur_flags);
			snprintf(response, response_length, "%s is 0x%llx", target_flags, cur_flags);
			success = true;
			goto end;
		}
		case FLAGS_CMD_SET: {
			switch (flags_modifier) {
				case FLAGS_MOD_ADD: {
					new_flags = (cur_flags | value);
					break;
				}
				case FLAGS_MOD_REMOVE: {
					new_flags = (cur_flags & ~value);
					break;
				}
				default: {
					new_flags = value;
					break;
				}
			}

			snprintf(new_flags_str, sizeof(new_flags_str), "%llx", new_flags);

			int ret = idme_update_var_ex(target_flags, new_flags_str, sizeof(new_flags_str) - 1);
			if (ret) {
				snprintf(response, response_length, "could not set %s, error=%d", target_flags, ret);
				dprintf(CRITICAL, "%s: Failed to set %s to %llx.\n", __func__, target_flags, new_flags);
			} else {
				snprintf(response, response_length, "%s set to %llx", target_flags, new_flags);
				dprintf(SPEW, "%s: Managed to set flags.\n", __func__);
				success = true;
			}
			goto end;
		}
		default: {
			snprintf(response, response_length, "oem flags [<type>: <modifier>] <value>");
			dprintf(CRITICAL, "%s: Bad command\n", __func__);
			goto end;
		}
	}

end:
	response[response_length - 1] = '\0';
	dprintf(INFO, "%s: Complete (%s): %s.\n", __func__, success ? "Success" : "Failure", response);
	return success;
}
