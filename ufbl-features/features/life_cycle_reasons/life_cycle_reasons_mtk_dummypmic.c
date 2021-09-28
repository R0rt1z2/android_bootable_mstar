/*
 * sign_of_life_mtk.c
 *
 * MTK platform implementation
 *
 * Copyright 2011 - 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
 * Yang Liu (yangliu@lab126.com)
 * TODO: Add additional contributor's names.
 */

#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <platform/mt_typedefs.h>
#include <platform/mt_rtc.h>
#include <platform/mt_pmic_wrap_init.h>
#include <platform/mtk_wdt.h>
#include "life_cycle_reasons.h"


static u32 wd_read()
{
	u32 rdata = 0;

	rdata = DRV_Reg32(MTK_WDT_NONRST_REG2);
	return rdata;
}

static void wd_write(u32 data)
{
	DRV_WriteReg32(MTK_WDT_NONRST_REG2, data);
}

static int (mtk_read_boot_reason)(life_cycle_boot_reason *boot_reason)
{
	u32 wd_breason;

	wd_breason = wd_read();
	dprintf(CRITICAL, "%s: boot_reason is 0x%x\n", __func__, (u32)(wd_breason));

	if (wd_breason & MTK_WDT_NONRST_REG2_WARM_BOOT_KERNEL_PANIC)
		*boot_reason = WARMBOOT_BY_KERNEL_PANIC;
	else if (wd_breason & MTK_WDT_NONRST_REG2_WARM_BOOT_KERNEL_WDOG)
		*boot_reason = WARMBOOT_BY_KERNEL_WATCHDOG;
	else if (wd_breason & MTK_WDT_NONRST_REG2_WARM_BOOT_HW_WDOG)
		*boot_reason = WARMBOOT_BY_HW_WATCHDOG;
	else if (wd_breason & MTK_WDT_NONRST_REG2_WARM_BOOT_SW)
		*boot_reason = WARMBOOT_BY_SW;
	else if (wd_breason & MTK_WDT_NONRST_REG2_COLD_BOOT_USB)
		*boot_reason = COLDBOOT_BY_USB;
	else if (wd_breason & MTK_WDT_NONRST_REG2_COLD_BOOT_POWER_KEY)
		*boot_reason = COLDBOOT_BY_POWER_KEY;
	else if (wd_breason & MTK_WDT_NONRST_REG2_COLD_BOOT_POWER_SUPPLY)
		*boot_reason = COLDBOOT_BY_POWER_SUPPLY;
	else {
		dprintf(CRITICAL, "Failed to read boot rtc boot reason\n");
		return -1;
	}

	return 0;
}

static int (mtk_write_boot_reason)(life_cycle_boot_reason boot_reason)
{
	u32 wd_breason;

	wd_breason = wd_read();

	dprintf(CRITICAL, "%s: current 0x%x boot_reason 0x%x\n", __func__, wd_breason, boot_reason);
	if (boot_reason == WARMBOOT_BY_KERNEL_PANIC)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_WARM_BOOT_KERNEL_PANIC;
	else if (boot_reason == WARMBOOT_BY_KERNEL_WATCHDOG)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_WARM_BOOT_KERNEL_WDOG;
	else if (boot_reason == WARMBOOT_BY_HW_WATCHDOG)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_WARM_BOOT_HW_WDOG;
	else if (boot_reason == WARMBOOT_BY_SW)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_WARM_BOOT_SW;
	else if (boot_reason == COLDBOOT_BY_USB)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_COLD_BOOT_USB;
	else if (boot_reason == COLDBOOT_BY_POWER_KEY)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_COLD_BOOT_POWER_KEY;
	else if (boot_reason == COLDBOOT_BY_POWER_SUPPLY)
		wd_breason = wd_breason | MTK_WDT_NONRST_REG2_COLD_BOOT_POWER_SUPPLY;

	wd_write(wd_breason);

	return 0;
}


static int (mtk_read_shutdown_reason)(life_cycle_shutdown_reason *shutdown_reason)
{
	u32 wd_shutdown_reason;

	wd_shutdown_reason = wd_read();
	dprintf(CRITICAL, "%s: shutdown reason is 0x%x\n", __func__, wd_shutdown_reason);
	if (wd_shutdown_reason & MTK_WDT_NONRST_REG2_SHUTDOWN_LONG_PWR_KEY_PRESS)
		*shutdown_reason = SHUTDOWN_BY_LONG_PWR_KEY_PRESS;
	else if (wd_shutdown_reason & MTK_WDT_NONRST_REG2_SHUTDOWN_SW)
		*shutdown_reason = SHUTDOWN_BY_SW;
	else if (wd_shutdown_reason & MTK_WDT_NONRST_REG2_SHUTDOWN_PWR_KEY)
		*shutdown_reason = SHUTDOWN_BY_PWR_KEY;
	else if (wd_shutdown_reason & MTK_WDT_NONRST_REG2_SHUTDOWN_SUDDEN_PWR_LOSS)
		*shutdown_reason = SHUTDOWN_BY_SUDDEN_POWER_LOSS;
	else if (wd_shutdown_reason & MTK_WDT_NONRST_REG2_SHUTDOWN_UKNOWN)
		*shutdown_reason = SHUTDOWN_BY_UNKNOWN_REASONS;
	else {
		dprintf(CRITICAL, "Failed to read boot rtc boot reason\n");
		return -1;
	}

	return 0;
}

static int (mtk_write_shutdown_reason)(life_cycle_shutdown_reason shutdown_reason)
{
	u32 wd_shutdown_reason;

	wd_shutdown_reason = wd_read();
	dprintf(CRITICAL, "%s: shutdown_reason 0x%x\n", __func__, wd_shutdown_reason);

	if (shutdown_reason == SHUTDOWN_BY_LONG_PWR_KEY_PRESS)
		wd_shutdown_reason = wd_shutdown_reason | MTK_WDT_NONRST_REG2_SHUTDOWN_LONG_PWR_KEY_PRESS;
	else if (shutdown_reason == SHUTDOWN_BY_SW)
		wd_shutdown_reason = wd_shutdown_reason | MTK_WDT_NONRST_REG2_SHUTDOWN_SW;
	else if (shutdown_reason == SHUTDOWN_BY_PWR_KEY)
		wd_shutdown_reason = wd_shutdown_reason | MTK_WDT_NONRST_REG2_SHUTDOWN_PWR_KEY;
	else if (shutdown_reason == SHUTDOWN_BY_SUDDEN_POWER_LOSS)
		wd_shutdown_reason = wd_shutdown_reason | MTK_WDT_NONRST_REG2_SHUTDOWN_SUDDEN_PWR_LOSS;
	else if (shutdown_reason == SHUTDOWN_BY_UNKNOWN_REASONS)
		wd_shutdown_reason = wd_shutdown_reason | MTK_WDT_NONRST_REG2_SHUTDOWN_UKNOWN;

	wd_write(wd_shutdown_reason);
	return 0;
}

static int (mtk_read_thermal_shutdown_reason)(life_cycle_thermal_shutdown_reason *thermal_shutdown_reason)
{
	u32 wd_thermal_shutdown_reason;

	wd_thermal_shutdown_reason = wd_read();
	dprintf(CRITICAL, "%s: thermal shutdown reason 0x%x\n", __func__, wd_thermal_shutdown_reason);

	if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_BATTERY)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_BATTERY;
	else if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_PMIC)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_PMIC;
	else if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_SOC)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_SOC;
	else if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_MODEM)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_MODEM;
	else if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_WIFI)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_WIFI;
	else if (wd_thermal_shutdown_reason & MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_PCB)
		*thermal_shutdown_reason = THERMAL_SHUTDOWN_REASON_PCB;
	else {
		dprintf(CRITICAL, "Failed to read boot rtc boot reason\n");
		return -1;
	}

	return 0;
}

static int (mtk_write_thermal_shutdown_reason)(life_cycle_thermal_shutdown_reason thermal_shutdown_reason)
{
	u32 wd_thermal_shutdown_reason;

	wd_thermal_shutdown_reason = wd_read();
	dprintf(CRITICAL, "%s: shutdown_reason 0x%0x\n", __func__, wd_thermal_shutdown_reason);


	if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_BATTERY)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_BATTERY;
	else if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_PMIC)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_PMIC;
	else if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_SOC)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_SOC;
	else if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_MODEM)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_MODEM;
	else if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_WIFI)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_WIFI;
	else if (thermal_shutdown_reason == THERMAL_SHUTDOWN_REASON_PCB)
		wd_thermal_shutdown_reason = wd_thermal_shutdown_reason | MTK_WDT_NONRST_REG2_THERMAL_SHUTDOWN_PCB;

	wd_write(wd_thermal_shutdown_reason);
	return 0;
}

static int (mtk_read_special_mode)(life_cycle_special_mode *special_mode)
{
	u32 wd_smode;

	wd_smode = wd_read();
	dprintf(CRITICAL, "%s: special mode is 0x%x\n", __func__, wd_smode);
	if (wd_smode & MTK_WDT_NONRST_REG2_SPECIAL_MODE_LOW_BATTERY)
		*special_mode = LIFE_CYCLE_SMODE_LOW_BATTERY;
	else if (wd_smode & MTK_WDT_NONRST_REG2_SPECIAL_MODE_WARM_BOOT_USB_CONNECTED)
		*special_mode = LIFE_CYCLE_SMODE_WARM_BOOT_USB_CONNECTED;
	else if (wd_smode & MTK_WDT_NONRST_REG2_SPECIAL_MODE_OTA)
		*special_mode = LIFE_CYCLE_SMODE_OTA;
	else if (wd_smode & MTK_WDT_NONRST_REG2_SPECIAL_MODE_FACTORY_RESET)
		*special_mode = LIFE_CYCLE_SMODE_FACTORY_RESET;
	else {
		dprintf(CRITICAL, "Failed to read boot rtc boot reason\n");
		return -1;
	}
	return 0;
}

static int (mtk_write_special_mode)(life_cycle_special_mode special_mode)
{
	u32 wd_smode;

	wd_smode = wd_read();
	dprintf(CRITICAL, "%s: special_mode 0x%x\n", __func__, wd_smode);

	if (special_mode == LIFE_CYCLE_SMODE_LOW_BATTERY)
		wd_smode = wd_smode | MTK_WDT_NONRST_REG2_SPECIAL_MODE_LOW_BATTERY;
	else if (special_mode == LIFE_CYCLE_SMODE_WARM_BOOT_USB_CONNECTED)
		wd_smode = wd_smode | MTK_WDT_NONRST_REG2_SPECIAL_MODE_WARM_BOOT_USB_CONNECTED;
	else if (special_mode == LIFE_CYCLE_SMODE_OTA)
		wd_smode = wd_smode | MTK_WDT_NONRST_REG2_SPECIAL_MODE_OTA;
	else if (special_mode == LIFE_CYCLE_SMODE_FACTORY_RESET)
		wd_smode = wd_smode | MTK_WDT_NONRST_REG2_SPECIAL_MODE_FACTORY_RESET;

	wd_write(wd_smode);
	return 0;
}


int life_cycle_platform_init(life_cycle_reasons_ops *sol_ops)
{
	dprintf(CRITICAL, "%s: Support MTK platform with dummypmic\n", __func__);
	sol_ops->read_boot_reason = mtk_read_boot_reason;
	sol_ops->write_boot_reason = mtk_write_boot_reason;
	sol_ops->read_shutdown_reason = mtk_read_shutdown_reason;
	sol_ops->write_shutdown_reason = mtk_write_shutdown_reason;
	sol_ops->read_thermal_shutdown_reason = mtk_read_thermal_shutdown_reason;
	sol_ops->write_thermal_shutdown_reason = mtk_write_thermal_shutdown_reason;
	sol_ops->read_special_mode = mtk_read_special_mode;
	sol_ops->write_special_mode = mtk_write_special_mode;

	return 0;
}





