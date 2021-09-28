
#include <common.h>

#include "avbctl_uboot.h"
#include "../libavb/libavb.h"

#define EX_SOFTWARE     (-1)
#define EX_OK           (0)

/* Function to enable and disable verification. The |ops| parameter
 * should be an |AvbOps| from libavb_user.
 */
int avb_set_verification(AvbOps* ops, const char* ab_suffix, bool enable_verification)
{
    bool verification_enabled;

    if (!avb_user_verification_get(ops, ab_suffix, &verification_enabled))
    {
        printf("%s:%d: Error getting whether verification is enabled.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    if ((verification_enabled && enable_verification) ||
        (!verification_enabled && !enable_verification))
    {
        printf("verification is already %s", verification_enabled ? "enabled" : "disabled");
        if (ab_suffix != "")
        {
            printf(" on slot with suffix %s", ab_suffix);
        }
        printf(".\n");
        return EX_OK;
    }

    if (!avb_user_verification_set(ops, ab_suffix, enable_verification))
    {
        printf("%s:%d: Error setting verification.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    printf("Successfully %s verification", enable_verification ? "enabled" : "disabled");
    if (ab_suffix != "")
    {
        printf(" on slot with suffix %s", ab_suffix);
    }
    printf(".\n");

    return EX_OK;
}

/* Function to query if verification. The |ops| parameter should be an
 * |AvbOps| from libavb_user.
 */
int avb_get_verification(AvbOps* ops, const char* ab_suffix)
{
    bool verification_enabled;

    if (!avb_user_verification_get(ops, ab_suffix, &verification_enabled))
    {
        printf("%s:%d: Error getting whether verification is enabled.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    printf("verification is %s", verification_enabled ? "enabled" : "disabled");
    if (ab_suffix != "")
    {
        printf(" on slot with suffix %s", ab_suffix);
    }
    printf(".\n");

    return EX_OK;
}

/* Function to enable and disable dm-verity. The |ops| parameter
 * should be an |AvbOps| from libavb_user.
 */
int avb_set_verity(AvbOps* ops, const char* ab_suffix, bool enable_verity)
{
    bool verity_enabled;

    if (!avb_user_verity_get(ops, ab_suffix, &verity_enabled))
    {
        printf("%s:%d: Error getting whether verity is enabled.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    if ((verity_enabled && enable_verity) ||
        (!verity_enabled && !enable_verity))
    {
        printf("verity is already %s", verity_enabled ? "enabled" : "disabled");
        if (ab_suffix != "")
        {
            printf(" on slot with suffix %s", ab_suffix);
        }
        printf(".\n");
        return EX_OK;
    }

    if (!avb_user_verity_set(ops, ab_suffix, enable_verity))
    {
        printf("%s:%d: Error setting verity.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    printf("Successfully %s verity", enable_verity ? "enabled" : "disabled");
    if (ab_suffix != "")
    {
        printf(" on slot with suffix %s", ab_suffix);
    }
    printf(".\n");

    return EX_OK;
}

/* Function to query if dm-verity is enabled. The |ops| parameter
 * should be an |AvbOps| from libavb_user.
 */
int avb_get_verity(AvbOps* ops, const char* ab_suffix)
{
    bool verity_enabled;

    if (!avb_user_verity_get(ops, ab_suffix, &verity_enabled))
    {
        printf("%s:%d: Error getting whether verity is enabled.\n",__FILE__,__LINE__);
        return EX_SOFTWARE;
    }

    printf("verity is %s", verity_enabled ? "enabled" : "disabled");
    if (ab_suffix != "")
    {
    printf(" on slot with suffix %s", ab_suffix);
    }
    printf(".\n");

    return EX_OK;
}