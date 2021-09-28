/*
 * Copyright (C) 2018 - 2020 Amazon.com Inc. or its affiliates.  All Rights Reserved.
 */
#include "idme.h"
#include "idme_utilities.h"
#if defined(UFBL_TESTS)
#include <ctype.h>
#include <string.h>
#endif

#if defined(UFBL_TESTS)

static int hexval(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

/*
 * returns 0 on error, the input string must have
 * atlease size 1.
 */
unsigned long atoul(const char *num)
{
	unsigned long value = 0;

	if ((num == NULL) ||  (strlen(num) < 0)) {
		return 0;
	}

	if (num[0] == '0' && num[1] == 'x') {
		// hex
		num += 2;
		while (*num && isxdigit(*num))
			value = value * 16 + hexval(*num++);
	} else {
		// decimal
		while (*num && isdigit(*num))
			value = ((value * 10) + (*num++)  - '0');
	}

	return value;
}
#endif

int idme_is_flag_set(const char* flagtype, unsigned long long flags)
{
	/* 3 for 0x + null character */
	char idme_flags_string[IDME_MAX_NAME_LEN + 3] = {0};
	unsigned long long idme_flags = 0;
	/*   Prefix string with '0x' so non-standard atoul interprets as hex */
	idme_flags_string[0] = '0';
	idme_flags_string[1] = 'x';
	if (idme_get_var_external(flagtype, &idme_flags_string[2], IDME_MAX_NAME_LEN) == 0) {
		idme_flags = atoul(idme_flags_string);
	}
	return (idme_flags & flags) == flags ? 1 : 0;
}

int idme_is_fos_flag_set(unsigned long long flags)
{
	return idme_is_flag_set("fos_flags", flags);
}
