/*
 * @file idme_default_table_mt8516.h
 * @brief This file contains idme default table values
 *  in the userstore partition
 *
 * Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

#ifndef __IDME_DEFAULT_TABLE_MT8516_H__
#define __IDME_DEFAULT_TABLE_MT8516_H__

const struct idme_init_values idme_default_values[] = {
	{ { "board_id", 16, 1, 0444 },
		/* Default Board ID value */
		"ffffff0000000000"
	},
	{ { "serial", 16, 1, 0444 },
		/* Default DSN value */
		"0"
	},
	{ { "mac_addr", 16, 1, 0444 },
		/* Default MAC address */
		"0"
	},
	{ { "mac_sec", 32, 1, 0444 },
		/* Default MAC secret */
		"0"
	},
	{ { "bt_mac_addr", 16, 1, 0444 },
		/* Default BT MAC address */
		"0"
	},
	{ { "bt_mfg", 128, 1, 0444 },
		/* Default BT MFG value */
                "00004681630160002310000007800006050703401f401f00048000ffffff00000000000000000000000000000000000000000000000000000000000000000000"
	},
	{ { "wifi_mfg", 1024, 1, 0444 },
		/* Default WIFI MFG value */
		"040100000000000000000000262600002222222221212222222020202121211E1E1E222222222222222222202020222222202020000000000000000000000120180000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010101261E1A00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
	},
	{ { "product_name", 32, 1, 0444 },
		/* Product name, acos 2.4 */
		"0"
	},
	{ { "productid", 32, 1, 0444 },
		/* Default Primary Product ID */
		"0"
	},
	{ { "productid2", 32, 1, 0444 },
		/* Default Secondary Product ID */
		"0"
	},
	{ { "bootmode", 4, 1, 0444 },
		/* Default Bootmode */
		"1"
	},
	{ { "postmode", 4, 1, 0444 },
		/* Default Postmode */
		"0"
	},
	{ { "bootcount", 8, 1, 0444 },
		/* Initial Bootcount */
		"0"
	},
	{ { "manufacturing", 512, 1, 0444 },
		/* Manufacturer-specific data */
		""
	},
	{ { "unlock_code", 1024, 1, 0444 },
		/* Unlock code */
		""
	},
	{ { "miccal.0", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.1", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.2", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.3", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.4", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.5", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "miccal.6", 16, 1, 0444 },
		/* MIC Calibration Data */
		"0"
	},
	{ { "alscal", 1024, 1, 0444 },
		/* Als Calibration Data */
		""
	},
	{ { "KB", 5120, 1, 0444 },
		/* KB partition backup */
		"0"
	},
	{ { "DKB", 1024, 1, 0444 },
		/* DKB partition backup */
		"0"
	},
	{ { "device_type_id", 32, 1, 0444 },
		/* Initial device type id */
		"0"
	},
	{ { "dev_flags", 8, 1, 0444 },
		/* device specific flag */
		"0"
	},
	{ { "fos_flags", 8, 1, 0444 },
		/* device specific flag */
		"0"
	},
	{ { "usr_flags", 8, 1, 0444 },
		/* device specific flag */
		"0"
	},
	{ { "locale", 8, 1, 0444 },
		/* device default locale */
		""
	},
	{ { "charger_det", 8, 1, 0444 },
		/* Charger Detection flag */
		"00000000"
	},
	{ { "alsparams", 64, 1, 0444 },
		/* Additional ALS Parameters */
		""
	},
	{ { "ffs_pin", 9, 1, 0444 },
		/* FFS Secret Pin */
		""
	},
	{ { "eth_mac_addr", 12, 1, 0444 },
		/* Default Ethernet MAC address */
		"0"
	},
	{ { "ledparams", 64, 1, 0444 },
		/* Additional LED Parameters */
		""
	},
	{ { "t_unlock_code", 512, 1, 0444 },
		/* Temporary unlock signed code, base64 encoded */
		""
	},
	{ { "t_unlock_cert", 1024, 1, 0444 },
		/* Temporary unlock signed certificate, base64 encoded */
		""
	},
#if defined(UFBL_FEATURE_IDME_ACE_FFS)
	{ { "dss_pub_key", 384, 1, 0444 },
		/* FFS DSS Authentication Public Key */
		"0"
	},
	{ { "device_hw_rev", 8, 1, 0444 },
		/* FFS Device HW Revision */
		"0"
	},
	{ { "cap_set_index", 2, 1, 0444 },
		/* FFS Capability Set Index */
		"0"
	},
#endif
	{ { "", 0, 0, 0 }, 0 },
};

#endif /* __IDME_DEFAULT_TABLE_MT8516_H__*/
