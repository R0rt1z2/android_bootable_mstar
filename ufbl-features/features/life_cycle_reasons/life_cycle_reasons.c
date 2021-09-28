/*
 * sign_of_life.c
 *
 * Device Sign of Life information
 *
 * Copyright 2011 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

#include "debug.h"
#include "life_cycle_reasons.h"

life_cycle_reasons_ops  lcr_ops = {0};
/*
 * life_cycle_set_boot_reason
 * Description: this function will set the boot reason which causing device booting
 */
int life_cycle_set_boot_reason(life_cycle_boot_reason boot_reason)
{
	if (!lcr_ops.write_boot_reason) {
		dprintf(CRITICAL, "%s: no platform supported\n", __func__);
		return -1;
	} else
		lcr_ops.write_boot_reason(boot_reason);

	return 0;
}

/*
 * life_cycle_set_shutdown_reason
 * Description: this function will set the Shutdown reason which causing device shutdown
 */
int life_cycle_set_shutdown_reason(life_cycle_shutdown_reason shutdown_reason)
{
	if (!lcr_ops.write_shutdown_reason) {
		dprintf(CRITICAL,"%s: no platform supported\n", __func__);
		return -1;
	} else
		lcr_ops.write_shutdown_reason(shutdown_reason);

	return 0;
}

/*
 * life_cycle_set_thermal_shutdown_reason
 * Description: this function will set the Thermal Shutdown reason which causing device shutdown
 */
int life_cycle_set_thermal_shutdown_reason(life_cycle_thermal_shutdown_reason thermal_shutdown_reason)
{
	if (!lcr_ops.write_thermal_shutdown_reason) {
		dprintf(CRITICAL,"%s: no platform supported\n", __func__);
		return -1;
	} else
		lcr_ops.write_thermal_shutdown_reason(thermal_shutdown_reason);

	return 0;
}

/*
 * life_cycle_set_special_mode
 * Description: this function will set the special mode which causing device life cycle change
 */
int life_cycle_set_special_mode(life_cycle_special_mode life_cycle_special_mode)
{
	if (!lcr_ops.write_special_mode) {
		dprintf(CRITICAL,"%s: no platform supported\n", __func__);
		return -1;
	} else
		lcr_ops.write_special_mode(life_cycle_special_mode);

	return 0;
}


extern int life_cycle_platform_init(life_cycle_reasons_ops *lcr_ops);
__WEAK int life_cycle_platform_init(life_cycle_reasons_ops *lcr_ops)
{
	dprintf(CRITICAL, "%s: no supported platform is configured\n", __func__);
	return 0;
}


/* 
 * life_cycle_init
 * Description: this function will initialize the platform 
 */
int life_cycle_init(void)
{
	life_cycle_platform_init(&lcr_ops);
	return 0;
}

