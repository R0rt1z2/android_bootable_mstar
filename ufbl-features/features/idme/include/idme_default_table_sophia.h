/*
 * idme_default_table_sophia.h
 *
 * Copyright 2011-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

/*!
 * @file idme_default_table_sophia.h
 * @brief This file contains idme default table values
 *  in the userstore partition
 *
 */

#ifndef __IDME_DEFAULT_TABLE_SOPHIA_H__
#define __IDME_DEFAULT_TABLE_SOPHIA_H__

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
	{ { "eth_mac_addr", 16, 1, 0444 },
		/* Default MAC address for ethernet */
		"0"
	},
	{ { "eth_ip_addr", 16, 1, 0444 },
		/* Initial default IPAddress */
		"0"
	},
	{ { "device_type_id", 32, 1, 0444 },
		/* Default device type value */
		"0"
	},
	{ { "unlock_code", 1024, 1, 0444 },
		/* Unlock code */
		""
	},
	{ { "sensorcal", 160, 1, 0444 },
		/* Sensor Calibration Data */
		"0"
	},
	{ { "wifi_mfg", 512, 1, 0444 },
		/* wifi Calibration Data */
		"0"
	},
	{ { "bt_mfg", 128, 1, 0444 },
		/* bt Calibration Data */
		"0"
	},
	{ { "KB", 8192, 1, 0444 },
		/* KB partition backup */
		"0"
	},
	{ { "DKB", 1024, 1, 0444 },
		/* DKB partition backup */
		"0"
	},
	{ { "fos_flags", 8, 1, 0444 },
		/* device specific flag */
		"40"
	},
	{ { "dev_flags", 8, 1, 0444 },
		/* device specific flag */
		"0"
	},
	{ { "usr_flags", 8, 1, 0444 },
		/* device specific flag */
		"0"
	},
	{ { "model_name", 256, 1, 0444 },
		/* device specific flag */
		"/config/model/Customer_1.ini"
	},
#ifdef CONFIG_MSTAR_BD_M7322_WINSTON
	{ { "cert", 246784, 0, 0444 },
		/* certificate backup, 256-15 K */
		""
	},
#else
        { { "cert", 115712, 0, 0444 },
                /* certificate backup, 128-15 K */
                ""
        },
#endif
	{ { "mfr_name", 32, 1, 0444 },
		/* Manufacturer name */
		"0"
	},
	{ { "mfr_model", 32, 1, 0444 },
		/* Manufacturer model */
		"0"
	},
	{ { "product_model", 32, 1, 0444 },
		/* product model */
		"0"
	},
	{ { "memc", 4, 1, 0444 },
		/* memc present? */
		"1"
	},
	{ { "region", 32, 1, 0444 },
		/* region, US is for USA and CA is for Canada */
		/* default value is EU */
		"EU"
	},
        { { "config_name", 64, 1, 0444 },
                /* name for board config.ini file */
                "0"
        },
	{ { "key_layout", 8, 1, 0444 },
		/* configure for different keylaout on TV */
		"0"
	},
        { { "oem_data", 1024, 1, 0444 },
                /* oem specific configuration */
                "0"
        },
	{ { "", 0, 0, 0 }, 0 },
};


#endif /* __IDME_DEFAULT_TABLE_SOPHIA_H__ */
