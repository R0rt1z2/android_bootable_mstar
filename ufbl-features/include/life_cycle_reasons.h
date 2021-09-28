/*
 * Copyright 2011 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 */

/*
 * sign_of_life.h
 *
 * Device sign of life header file
 *
 * Yang Liu (yangliu@lab126.com)
 * TODO: Add additional contributor's names.
 */

#ifndef __LIFE_CYCLE_REASON_H
#define __LIFE_CYCLE_REASON_H

/* Device Boot Reason */
typedef enum {
	WARMBOOT_BY_KERNEL_PANIC     = 0x100,
	WARMBOOT_BY_KERNEL_WATCHDOG  = 0x101,
	WARMBOOT_BY_HW_WATCHDOG      = 0x102,
	WARMBOOT_BY_SW               = 0x103,
	COLDBOOT_BY_USB              = 0x104,
	COLDBOOT_BY_POWER_KEY        = 0x105,
	COLDBOOT_BY_POWER_SUPPLY     = 0x106,
} life_cycle_boot_reason;

/* Device Shutdown Reason */
typedef enum {
	SHUTDOWN_BY_LONG_PWR_KEY_PRESS = 0x201,
	SHUTDOWN_BY_SW                 = 0x202,
	SHUTDOWN_BY_PWR_KEY            = 0x203,
	SHUTDOWN_BY_SUDDEN_POWER_LOSS  = 0x204,
	SHUTDOWN_BY_UNKNOWN_REASONS    = 0x205,
} life_cycle_shutdown_reason;

/* THERMAL SHUTDOWN REASON */
typedef enum {
	THERMAL_SHUTDOWN_REASON_BATTERY = 0x301,
	THERMAL_SHUTDOWN_REASON_PMIC    = 0x302,
	THERMAL_SHUTDOWN_REASON_SOC     = 0x303,
	THERMAL_SHUTDOWN_REASON_MODEM   = 0x304,
	THERMAL_SHUTDOWN_REASON_WIFI    = 0x305,
	THERMAL_SHUTDOWN_REASON_PCB     = 0x306,
} life_cycle_thermal_shutdown_reason;

/* LIFE CYCLE Special Mode */
typedef enum {
	LIFE_CYCLE_SMODE_NONE                     = 0x400,
	LIFE_CYCLE_SMODE_LOW_BATTERY              = 0x401,
	LIFE_CYCLE_SMODE_WARM_BOOT_USB_CONNECTED  = 0x402,
	LIFE_CYCLE_SMODE_OTA                      = 0x403,
	LIFE_CYCLE_SMODE_FACTORY_RESET            = 0x404,
} life_cycle_special_mode;

/* sign of life operations */
typedef struct sign_of_life_ops {
	int (*read_boot_reason)(life_cycle_boot_reason *boot_reason);
	int (*write_boot_reason)(life_cycle_boot_reason boot_reason);
	int (*read_shutdown_reason)(life_cycle_shutdown_reason *shutdown_reason);
	int (*write_shutdown_reason)(life_cycle_shutdown_reason shutdown_reason);
	int (*read_thermal_shutdown_reason)(life_cycle_thermal_shutdown_reason *thermal_shutdown_reason);
	int (*write_thermal_shutdown_reason)(life_cycle_thermal_shutdown_reason thermal_shutdown_reason);
	int (*read_special_mode)(life_cycle_special_mode *special_mode);
	int (*write_special_mode)(life_cycle_special_mode special_mode);
} life_cycle_reasons_ops;

/* 
 * life_cycle_init
 * Description: this function will initialize the platform 
 */
int life_cycle_init(void);

/*
 * life_cycle_set_boot_reason
 * Description: this function will set the boot reason which causing device booting
 */
int life_cycle_set_boot_reason(life_cycle_boot_reason boot_reason);

/*
 * life_cycle_set_shutdown_reason
 * Description: this function will set the Shutdown reason which causing device shutdown
 */
int life_cycle_set_shutdown_reason(life_cycle_shutdown_reason shutdown_reason);

/*
 * life_cycle_set_thermal_shutdown_reason
 * Description: this function will set the Thermal Shutdown reason which causing device shutdown
 */
int life_cycle_set_thermal_shutdown_reason(life_cycle_thermal_shutdown_reason thermal_shutdown_reason);

/*
 * life_cycle_set_special_mode
 * Description: this function will set the special mode which causing device life cycle change
 */
int life_cycle_set_special_mode(life_cycle_special_mode life_cycle_special_mode);

#endif
