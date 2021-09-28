/**
 *  Copyright (c) 2014 MediaTek Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 */

/** steve wang 2015/11/26 */
//---------------------------------------------------------------------------
#include <mtk-bt/LD_usbbt.h>
#include <mtk-bt/LD_btmtk_usb.h>
#include <mtk-bt/errno.h>

//- Local Configuration -----------------------------------------------------
#define LD_VERSION "1.3.3.0"

#define BUFFER_SIZE  (1024 * 4)     /* Size of RX Queue */
#define BT_SEND_HCI_CMD_BEFORE_SUSPEND 1
#define LD_SUPPORT_FW_DUMP 0
#define LD_BT_ALLOC_BUF 0
#define LD_NOT_FIX_BUILD_WARN 0

#define FIDX 0x5A   /* Unify WoBLE APCF Filtering Index */

//---------------------------------------------------------------------------
static char driver_version[64] = { 0 };
static unsigned char probe_counter = 0;
static volatile int metaMode;
static volatile int metaCount;
/* 0: False; 1: True */
static int isbtready;
static int isUsbDisconnet;
static volatile int is_assert = 0;

//---------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//---------------------------------------------------------------------------
static inline int is_mt7630(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76300000);
}

//---------------------------------------------------------------------------
static inline int is_mt7650(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76500000);
}

//---------------------------------------------------------------------------
static inline int is_mt7632(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76320000);
}

//---------------------------------------------------------------------------
static inline int is_mt7662(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76620000);
}

//---------------------------------------------------------------------------
static inline int is_mt7662T(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffffffff) == 0x76620100);
}

//---------------------------------------------------------------------------
static inline int is_mt7632T(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffffffff) == 0x76320100);
}

//---------------------------------------------------------------------------
static inline int is_mt7668(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff) == 0x7668);
}

//---------------------------------------------------------------------------
static int btmtk_usb_io_read32(struct LD_btmtk_usb_data *data, u32 reg, u32 *val)
{
    u8 request = data->r_request;
    int ret;

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, request,
            DEVICE_VENDOR_REQUEST_IN, 0, (u16)reg, data->io_buf, sizeof(u32),
            CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0) {
        *val = 0xffffffff;
        UBOOT_ERROR("error(%d), reg=%x, value=%x\n", ret, reg, *val);
        return ret;
    }

    os_memmove(val, data->io_buf, sizeof(u32));
    *val = le32_to_cpu(*val);
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_io_read32_7668(struct LD_btmtk_usb_data *data, u32 reg, u32 *val)
{
    int ret = -1;
    __le16 reg_high;
    __le16 reg_low;

    reg_high = ((reg >> 16) & 0xFFFF);
    reg_low = (reg & 0xFFFF);

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, 0x63,
            DEVICE_VENDOR_REQUEST_IN, reg_high, reg_low, data->io_buf, sizeof(u32),
            CONTROL_TIMEOUT_JIFFIES);
    if (ret < 0) {
        *val = 0xFFFFFFFF;
        UBOOT_ERROR("error(%d), reg=%X, value=%X\n", ret, reg, *val);
        return ret;
    }

    os_memmove(val, data->io_buf, sizeof(u32));
    *val = le32_to_cpu(*val);
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_io_write32(struct LD_btmtk_usb_data *data, u32 reg, u32 val)
{
    u16 value, index;
    u8 request = data->w_request;
    mtkbt_dev_t *udev = data->udev;
    int ret;

    index = (u16) reg;
    value = val & 0x0000ffff;

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP, request, DEVICE_VENDOR_REQUEST_OUT,
            value, index, NULL, 0, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0) {
        UBOOT_ERROR("error(%d), reg=%x, value=%x\n", ret, reg, val);
        return ret;
    }

    index = (u16) (reg + 2);
    value = (val & 0xffff0000) >> 16;

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP, request, DEVICE_VENDOR_REQUEST_OUT,
            value, index, NULL, 0, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0) {
        UBOOT_ERROR("error(%d), reg=%x, value=%x\n", ret, reg, val);
        return ret;
    }
    if (ret > 0)
        ret = 0;
    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_wmt_cmd(struct LD_btmtk_usb_data *data, const u8 *cmd,
        const int cmd_len, const u8 *event, const int event_len, u32 delay, u8 retry)
{
    int ret = -1;
    BOOL check = FALSE;

    if (!data || !data->hcif || !data->io_buf || !cmd) {
        UBOOT_ERROR("incorrect cmd pointer\n");
        return -1;
    }
    if (event != NULL && event_len > 0)
        check = TRUE;

    /* send WMT command */
    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP, 0x01,
            DEVICE_CLASS_REQUEST_OUT, 0x30, 0x00, (void *)cmd, cmd_len,
            CONTROL_TIMEOUT_JIFFIES);
    if (ret < 0) {
        UBOOT_ERROR("command send failed(%d)\n", ret);
        return ret;
    }

    if (event_len == -1) {
        /* If event_len is -1, DO NOT read event, since FW wouldn't feedback */
        return 0;
    }

retry_get:
    MTK_MDELAY(delay);

    /* check WMT event */
    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, 0x01,
            DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00, data->io_buf, LD_BT_MAX_EVENT_SIZE,
            CONTROL_TIMEOUT_JIFFIES);
    if (ret < 0) {
        UBOOT_ERROR("event get failed(%d)\n", ret);
        if (check == TRUE) return ret;
        else return 0;
    }

    if (check == TRUE) {
        if (ret >= event_len && memcmp(event, data->io_buf, event_len) == 0) {
            return ret;
        } else if (retry > 0) {
            UBOOT_DEBUG("retry to get event(%d)\n", retry);
            retry--;
            goto retry_get;
        } else {
            UBOOT_DEBUG("can't get expect event\n");
        }
    }
    return -1;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_hci_cmd(struct LD_btmtk_usb_data *data, const u8 *cmd,
        const int cmd_len, const u8 *event, const int event_len)
{
    /** @RETURN
     *     length if event compare successfully.,
     *     0 if doesn't check event.,
     *     < 0 if error.
     */
#define USB_CTRL_IO_TIMO    100
#define USB_INTR_MSG_TIMO   2000
    int ret = -1;
    int len = 0;
    int i = 0;
    u8 retry = 0;
    BOOL check = FALSE;

    if (!data || !data->hcif || !data->io_buf || !cmd) {
        UBOOT_ERROR("incorrect cmd pointer\n");
        return -1;
    }
    if (event != NULL && event_len > 0)
        check = TRUE;

    /* send HCI command */
    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP, 0,
            DEVICE_CLASS_REQUEST_OUT, 0, 0, (u8 *)cmd, cmd_len, USB_CTRL_IO_TIMO);
    if (ret < 0) {
        UBOOT_ERROR("send command failed: %d\n", ret);
        return ret;
    }

    if (event_len == -1) {
        /* If event_len is -1, DO NOT read event, since FW wouldn't feedback */
        return 0;
    }

    /* check HCI event */
    do {
        ret = data->hcif->usb_interrupt_msg(data->udev, MTKBT_INTR_EP, data->io_buf,
                LD_BT_MAX_EVENT_SIZE, &len, USB_INTR_MSG_TIMO);
        if (ret < 0) {
            UBOOT_ERROR("event get failed: %d\n", ret);
            if (check == TRUE) return ret;
            else return 0;
        }

        if (check == TRUE) {
            if (len >= event_len) {
                for (i = 0; i < event_len; i++) {
                    if (event[i] != data->io_buf[i])
                        break;
                }
            } else {
                UBOOT_DEBUG("event length is not match(%d/%d)\n", len, event_len);
            }
            if (i != event_len) {
                UBOOT_DEBUG("got unknown event(%d)\n", len);
            } else {
                return len; /* actually read length */
            }
            MTK_MDELAY(10);
            ++retry;
        }
        UBOOT_DEBUG("try get event again\n");
    } while (retry < 3);
    return -1;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_hci_suspend_cmd(struct LD_btmtk_usb_data *data)
{
    int ret = -1;
#if SUPPORT_HISENSE_WoBLE
    u8 cmd[] = {0xC9, 0xFC, 0x02, 0x01, 0x0D}; // for Hisense WoBLE

    UBOOT_DEBUG("issue wake up command for Hisense\n");
#else
    u8 cmd[] = {0xC9, 0xFC, 0x0D, 0x01, 0x0E, 0x00, 0x05, 0x43,
        0x52, 0x4B, 0x54, 0x4D, 0x20, 0x04, 0x32, 0x00};

    UBOOT_DEBUG("issue wake up command for '0E: MTK WoBLE Ver2'\n");
#endif

    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), NULL, -1);
    if (ret < 0) {
        UBOOT_ERROR("error(%d)\n", ret);
        return ret;
    }
    UBOOT_DEBUG("send suspend cmd OK\n");
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_hci_reset_cmd(struct LD_btmtk_usb_data *data)
{
    u8 cmd[] = { 0x03, 0x0C, 0x00 };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00 };
    int ret = -1;

    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        UBOOT_ERROR("failed(%d)\n", ret);
    } else {
        UBOOT_DEBUG("OK\n");
    }

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_hci_set_ce_cmd(struct LD_btmtk_usb_data *data)
{
    u8 cmd[] = { 0xD1, 0xFC, 0x04, 0x0C, 0x07, 0x41, 0x00 };
    u8 event[] = { 0x0E, 0x08, 0x01, 0xD1, 0xFC, 0x00 };
    int ret = -1;

    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        UBOOT_ERROR("failed(%d)\n", ret);

    } else if (ret == sizeof(event) + 4) {
        if (data->io_buf[6] & 0x01) {
            UBOOT_DEBUG("warning, 0x41070c[0] is 1!\n");
            ret = 0;
        } else {
            u8 cmd2[11] = { 0xD0, 0xFC, 0x08, 0x0C, 0x07, 0x41, 0x00 };

            cmd2[7] = data->io_buf[6] | 0x01;
            cmd2[8] = data->io_buf[7];
            cmd2[9] = data->io_buf[8];
            cmd2[10] = data->io_buf[9];

            ret = btmtk_usb_send_hci_cmd(data, cmd2, sizeof(cmd2), NULL, 0);
            if (ret < 0) {
                UBOOT_ERROR("write 0x41070C failed(%d)\n", ret);
            } else {
                UBOOT_DEBUG("OK\n");
                ret = 0;
            }
        }
    } else {
        UBOOT_INFO("skip it, got response length(%d)\n", ret);
        return -1;
    }

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_check_rom_patch_result_cmd(struct LD_btmtk_usb_data *data)
{
    /* Send HCI Reset */
    {
        int ret = 0;
        unsigned char buf[8] = { 0 };
        buf[0] = 0xD1;
        buf[1] = 0xFC;
        buf[2] = 0x04;
        buf[3] = 0x00;
        buf[4] = 0xE2;
        buf[5] = 0x40;
        buf[6] = 0x00;
        ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP,0x0, DEVICE_CLASS_REQUEST_OUT,
                0x00, 0x00, buf, 0x07, 100);
        if (ret < 0) {
            UBOOT_ERROR("error1(%d)\n", ret);
            return ret;
        }
    }
    /* Get response of HCI reset */
    {
        int ret = 0;
        unsigned char buf[LD_BT_MAX_EVENT_SIZE] = { 0 };
        int actual_length = 0;
        ret = data->hcif->usb_interrupt_msg(data->udev, MTKBT_INTR_EP, buf, LD_BT_MAX_EVENT_SIZE,
                &actual_length, 2000);
        if (ret < 0) {
            UBOOT_ERROR("error2(%d)\n", ret);
            return ret;
        }
        UBOOT_INFO("Check rom patch result : ");

        if (buf[6] == 0 && buf[7] == 0 && buf[8] == 0 && buf[9] == 0) {
            UBOOT_ERROR("NG\n");
        } else {
            UBOOT_INFO("OK\n");
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_switch_iobase(struct LD_btmtk_usb_data *data, int base)
{
    int ret = 0;

    switch (base) {
        case SYSCTL:
            data->w_request = 0x42;
            data->r_request = 0x47;
            break;
        case WLAN:
            data->w_request = 0x02;
            data->r_request = 0x07;
            break;

        default:
            return -EINVAL;
    }

    return ret;
}

#define MAX_BIN_FILE_NAME_LEN 32
//---------------------------------------------------------------------------
static void btmtk_usb_cap_init(struct LD_btmtk_usb_data *data)
{
    btmtk_usb_io_read32(data, 0x00, &data->chip_id);
    if (data->chip_id == 0)
        btmtk_usb_io_read32_7668(data, 0x80000008, &data->chip_id);

    //UBOOT_DEBUG("chip id = %x\n", data->chip_id);

    if (is_mt7630(data) || is_mt7650(data)) {
        data->need_load_fw = 1;
        data->need_load_rom_patch = 0;
        data->fw_header_image = NULL;
        data->fw_bin_file_name = (unsigned char*)strdup("mtk/mt7650.bin");
        data->fw_len = 0;

    } else if (is_mt7662T(data) || is_mt7632T(data)) {
        UBOOT_INFO("btmtk:This is 7662T chip\n");
        data->rom_patch_bin_file_name = os_kzalloc(MAX_BIN_FILE_NAME_LEN, MTK_GFP_ATOMIC);
        if (!data->rom_patch_bin_file_name) {
            UBOOT_ERROR("Can't allocate memory\n");
            return;
        }
        data->need_load_fw = 0;
        data->need_load_rom_patch = 1;
        os_memcpy(data->rom_patch_bin_file_name, "mt7662t_patch_e1_hdr.bin", 24);
        data->rom_patch_offset = 0xBC000;
        data->rom_patch_len = 0;

    } else if (is_mt7632(data) || is_mt7662(data)) {
        UBOOT_INFO("btmtk:This is 7662 chip\n");
        data->rom_patch_bin_file_name = os_kzalloc(MAX_BIN_FILE_NAME_LEN, MTK_GFP_ATOMIC);
        if (!data->rom_patch_bin_file_name) {
            UBOOT_ERROR("Can't allocate memory\n");
            return;
        }
        data->need_load_fw = 0;
        data->need_load_rom_patch = 1;
        os_memcpy(data->rom_patch_bin_file_name, "mt7662_patch_e3_hdr.bin", 23);
        data->rom_patch_offset = 0x90000;
        data->rom_patch_len = 0;

    } else {
        unsigned int fw_ver = 0;

        btmtk_usb_io_read32_7668(data, 0x80000004, &fw_ver);
        if ((fw_ver & 0xFF) != 0xFF) {
            data->rom_patch_bin_file_name = os_kzalloc(MAX_BIN_FILE_NAME_LEN, MTK_GFP_ATOMIC);
            if (!data->rom_patch_bin_file_name) {
                UBOOT_ERROR("Can't allocate memory\n");
                return;
            }
            data->need_load_fw = 0;
            data->need_load_rom_patch = 1;

            snprintf(data->rom_patch_bin_file_name, MAX_BIN_FILE_NAME_LEN, "mt%04x_patch_e%x_hdr.bin",
                    data->chip_id & 0xFFFF, (fw_ver & 0xFF) + 1);
            UBOOT_INFO("patch name: %s", data->rom_patch_bin_file_name);
            data->rom_patch_len = 0;
        } else {
            UBOOT_ERROR("Incorrect firmware version: 0xFF");
            return;
        }
    }
}

#if CRC_CHECK
//---------------------------------------------------------------------------
static u16 checksume16(u8 *pData, int len)
{
    int sum = 0;

    while (len > 1) {
        sum += *((u16 *) pData);
        pData = pData + 2;

        if (sum & 0x80000000) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        len -= 2;
    }

    if (len)
        sum += *((u8 *) pData);

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}

//---------------------------------------------------------------------------
static int btmtk_usb_chk_crc(struct LD_btmtk_usb_data *data, u32 checksum_len)
{
    int ret = 0;
    mtkbt_dev_t *udev = data->udev;

    UBOOT_DEBUG("\n");

    os_memmove(data->io_buf, &data->rom_patch_offset, 4);
    os_memmove(&data->io_buf[4], &checksum_len, 4);

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP,0x1, DEVICE_VENDOR_REQUEST_OUT,
            0x20, 0x00, data->io_buf, 8, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0) {
        UBOOT_ERROR("error(%d)\n", ret);
    }

    return ret;
}

//---------------------------------------------------------------------------
static u16 btmtk_usb_get_crc(struct LD_btmtk_usb_data *data)
{
    int ret = 0;
    mtkbt_dev_t *udev = data->udev;
    u16 crc, count = 0;

    UBOOT_DEBUG("\n");
    while (1) {
        ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_RX_EP, 0x01,
                DEVICE_VENDOR_REQUEST_IN, 0x21, 0x00, data->io_buf, 2,
                CONTROL_TIMEOUT_JIFFIES);

        if (ret < 0) {
            crc = 0xFFFF;
            UBOOT_ERROR("error(%d)\n", ret);
        }

        os_memmove(&crc, data->io_buf, 2);

        crc = le16_to_cpu(crc);

        if (crc != 0xFFFF)
            break;

        MTK_MDELAY(100);

        if (count++ > 100) {
            UBOOT_DEBUG("Query CRC over %d times\n", count);
            break;
        }
    }

    return crc;
}
#endif /* CRC_CHECK */

//---------------------------------------------------------------------------
static int btmtk_usb_send_wmt_reset_cmd(struct LD_btmtk_usb_data *data)
{
    /* reset command */
    u8 cmd[] = { 0x6F, 0xFC, 0x05, 0x01, 0x07, 0x01, 0x00, 0x04 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x07, 0x01, 0x00, 0x00 };
    int ret = -1;

    ret = btmtk_usb_send_wmt_cmd(data, cmd, sizeof(cmd), event, sizeof(event), 20, 0);
    if (ret < 0) {
        UBOOT_ERROR("Check reset wmt result : NG\n");
    } else {
        UBOOT_DEBUG("Check reset wmt result : OK\n");
        ret = 0;
    }

    return ret;
}

//---------------------------------------------------------------------------
static u16 btmtk_usb_get_rom_patch_result(struct LD_btmtk_usb_data *data)
{
    int ret = 0;

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, 0x01,
            DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00, data->io_buf, 7,
            CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
        UBOOT_ERROR("error(%d)\n", ret);

    if (data->io_buf[0] == 0xe4 &&
        data->io_buf[1] == 0x05 &&
        data->io_buf[2] == 0x02 &&
        data->io_buf[3] == 0x01 &&
        data->io_buf[4] == 0x01 &&
        data->io_buf[5] == 0x00 &&
        data->io_buf[6] == 0x00) {
        //UBOOT_DEBUG("Get rom patch result : OK\n");
    } else {
        UBOOT_ERROR("Get rom patch result : NG\n");
    }
    return ret;
}

//---------------------------------------------------------------------------
#define SHOW_FW_DETAILS(s)                                                  \
    UBOOT_INFO("%s = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", s,                  \
            tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3],                 \
            tmp_str[4], tmp_str[5], tmp_str[6], tmp_str[7],                 \
            tmp_str[8], tmp_str[9], tmp_str[10], tmp_str[11],               \
            tmp_str[12], tmp_str[13], tmp_str[14], tmp_str[15])

//---------------------------------------------------------------------------
static int btmtk_usb_load_rom_patch(struct LD_btmtk_usb_data *data)
{
    u32 loop = 0;
    u32 value;
    s32 sent_len;
    int ret = 0;
    u32 patch_len = 0;
    u32 cur_len = 0;
    int real_len = 0;
    int first_block = 1;
    unsigned char phase;
    void *buf;
    char *pos;
    unsigned char *tmp_str;

    //UBOOT_DEBUG("begin\n");
load_patch_protect:
    btmtk_usb_switch_iobase(data, WLAN);
    btmtk_usb_io_read32(data, SEMAPHORE_03, &value);
    loop++;

    if ((value & 0x01) == 0x00) {
        if (loop < 1000) {
            MTK_MDELAY(1);
            goto load_patch_protect;
        } else {
            UBOOT_ERROR("btmtk_usb_load_rom_patch ERR! Can't get semaphore! Continue\n");
        }
    }

    btmtk_usb_switch_iobase(data, SYSCTL);

    btmtk_usb_io_write32(data, 0x1c, 0x30);

    btmtk_usb_switch_iobase(data, WLAN);

    /* check ROM patch if upgrade */
    if ((MT_REV_GTE(data, mt7662, REV_MT76x2E3)) || (MT_REV_GTE(data, mt7632, REV_MT76x2E3)))
    {
        btmtk_usb_io_read32(data, CLOCK_CTL, &value);
        if ((value & 0x01) == 0x01) {
            UBOOT_INFO("btmtk_usb_load_rom_patch : no need to load rom patch\n");
            btmtk_usb_send_hci_reset_cmd(data);
            goto error;
        }
    } else {
        btmtk_usb_io_read32(data, COM_REG0, &value);
        if ((value & 0x02) == 0x02) {
            UBOOT_INFO("btmtk_usb_load_rom_patch : no need to load rom patch\n");
            btmtk_usb_send_hci_reset_cmd(data);
            goto error;
        }
    }

    buf = os_kzalloc(UPLOAD_PATCH_UNIT, MTK_GFP_ATOMIC);
    if (!buf) {
        ret = -ENOMEM;
        goto error;
    }

    pos = buf;

    LD_load_code_from_bin(&data->rom_patch, (char *)data->rom_patch_bin_file_name, NULL,
            data->udev, &data->rom_patch_len);

    if (!data->rom_patch) {
        UBOOT_ERROR("please assign a rom patch(/etc/firmware/%s)or(/lib/firmware/%s)\n",
                data->rom_patch_bin_file_name,
                data->rom_patch_bin_file_name);
        ret = -1;
        goto error;
    }

    tmp_str = data->rom_patch;
    SHOW_FW_DETAILS("FW Version");
    SHOW_FW_DETAILS("build Time");

    tmp_str = data->rom_patch + 16;
    UBOOT_INFO("platform = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = data->rom_patch + 20;
    UBOOT_INFO("HW/SW version = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = data->rom_patch + 24;
    UBOOT_INFO("Patch version = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    UBOOT_INFO("\nloading rom patch...\n");

    cur_len = 0x00;
    patch_len = data->rom_patch_len - PATCH_INFO_SIZE;

    /* loading rom patch */
    while (1) {
        s32 sent_len_max = UPLOAD_PATCH_UNIT - PATCH_HEADER_SIZE;
        real_len = 0;
        sent_len =
            (patch_len - cur_len) >= sent_len_max ? sent_len_max : (patch_len - cur_len);

        //UBOOT_DEBUG("patch_len = %d\n", patch_len);
        //UBOOT_DEBUG("cur_len = %d\n", cur_len);
        //UBOOT_DEBUG("sent_len = %d\n", sent_len);

        if (sent_len > 0) {
            if (first_block == 1) {
                if (sent_len < sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE1;
                first_block = 0;
            } else if (sent_len == sent_len_max) {
                if (patch_len - cur_len == sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE2;
            } else {
                phase = PATCH_PHASE3;
            }

            /* prepare HCI header */
            pos[0] = 0x6F;
            pos[1] = 0xFC;
            pos[2] = (sent_len + 5) & 0xFF;
            pos[3] = ((sent_len + 5) >> 8) & 0xFF;

            /* prepare WMT header */
            pos[4] = 0x01;
            pos[5] = 0x01;
            pos[6] = (sent_len + 1) & 0xFF;
            pos[7] = ((sent_len + 1) >> 8) & 0xFF;

            pos[8] = phase;

            os_memcpy(&pos[9], data->rom_patch + PATCH_INFO_SIZE + cur_len, sent_len);

            //UBOOT_DEBUG("sent_len + PATCH_HEADER_SIZE = %d, phase = %d\n",
                   //sent_len + PATCH_HEADER_SIZE, phase);

            ret = data->hcif->usb_bulk_msg(data->udev, MTKBT_BULK_TX_EP, buf, sent_len + PATCH_HEADER_SIZE, &real_len, 0);

            if (ret) {
                UBOOT_ERROR("upload rom_patch err: %d\n", ret);
                goto error;
            }

            MTK_MDELAY(1);

            cur_len += sent_len;

        } else {
            UBOOT_DEBUG("loading rom patch... Done\n");
            break;
        }
    }

    MTK_MDELAY(20);
    ret = btmtk_usb_get_rom_patch_result(data);
    MTK_MDELAY(20);

    /* Send Checksum request */
#if CRC_CHECK
    int total_checksum = checksume16(data->rom_patch + PATCH_INFO_SIZE, patch_len);
    btmtk_usb_chk_crc(data, patch_len);
    MTK_MDELAY(20);
    if (total_checksum != btmtk_usb_get_crc(data)) {
        UBOOT_ERROR("checksum fail!, local(0x%x) <> fw(0x%x)\n", total_checksum,
                btmtk_usb_get_crc(data));
        ret = -1;
        goto error;
    } else {
        UBOOT_DEBUG("crc match!\n");
    }
#endif
    MTK_MDELAY(20);
    /* send check rom patch result request */
    btmtk_usb_send_check_rom_patch_result_cmd(data);
    MTK_MDELAY(20);
    /* CHIP_RESET */
    ret = btmtk_usb_send_wmt_reset_cmd(data);
    MTK_MDELAY(20);
    /* BT_RESET */
    btmtk_usb_send_hci_reset_cmd(data);
    /* for WoBLE/WoW low power */
    btmtk_usb_send_hci_set_ce_cmd(data);

error:
    btmtk_usb_io_write32(data, SEMAPHORE_03, 0x1);
    //UBOOT_DEBUG("end\n");
    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_wmt_power_on_cmd_7668(struct LD_btmtk_usb_data *data)
{
    u8 count = 0;                                           /* retry 3 times */
    u8 cmd[] = { 0x6F, 0xFC, 0x06, 0x01, 0x06, 0x02, 0x00, 0x00, 0x01 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x06, 0x01, 0x00 };    /* event[6] is key */
    int ret = -1;                                           /* if successful, 0 */

    do {
        ret = btmtk_usb_send_wmt_cmd(data, cmd, sizeof(cmd), event, sizeof(event), 100, 10);
        if (ret < 0) {
            UBOOT_ERROR("failed(%d)\n", ret);
        } else if (ret == sizeof(event) + 1) {
            switch (data->io_buf[6]) {
            case 0:             /* successful */
                UBOOT_DEBUG("%s: OK\n", __func__);
                ret = 0;
                break;
            case 2:             /* retry */
                UBOOT_DEBUG("Try again\n");
                continue;
            default:
                UBOOT_INFO("Unknown result: %02X\n", data->io_buf[6]);
                return -1;
            }
        } else {
            UBOOT_INFO("skip it, got response length(%d)\n", ret);
            return -1;
        }
    } while (++count < 3 && ret > 0);

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_hci_tci_set_sleep_cmd_7668(struct LD_btmtk_usb_data *data)
{
    u8 cmd[] = { 0x7A, 0xFC, 0x07, 0x05, 0x40, 0x06, 0x40, 0x06, 0x00, 0x00 };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x7A, 0xFC, 0x00 };
    int ret = -1;                                   /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        UBOOT_ERROR("failed(%d)\n", ret);
    } else {
        UBOOT_DEBUG("OK\n");
        ret = 0;
    }

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_get_vendor_cap(struct LD_btmtk_usb_data *data)
{
    u8 cmd[] = { 0x53, 0xFD, 0x00 };
    u8 event[6] = { 0x0E, 0x12, 0x01, 0x53, 0xFD, 0x00, /* ... */ };
    int ret = -1;

    // TODO: should not compare whole event
    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        UBOOT_ERROR("Failed(%d)\n", ret);
    } else {
        UBOOT_DEBUG("OK\n");
        ret = 0;
    }

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_send_read_bdaddr(struct LD_btmtk_usb_data *data)
{
    u8 cmd[] = { 0x09, 0x10, 0x00 };
    u8 event[] = { 0x0E, 0x0A, 0x01, 0x09, 0x10, 0x00, /* 6 bytes are BDADDR */ };
    int ret = -1;

    ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0 || ret != 12 /* Event actual length */) {
        UBOOT_ERROR("Failed(%d)\n", ret);
        return ret;
    }

    os_memcpy(data->local_addr, data->io_buf + 6, BD_ADDR_LEN);
    UBOOT_INFO("ADDR: %02X-%02X-%02X-%02X-%02X-%02X\n",
            data->local_addr[5], data->local_addr[4], data->local_addr[3],
            data->local_addr[2], data->local_addr[1], data->local_addr[0]);
    ret = 0;

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_set_apcf(struct LD_btmtk_usb_data *data, BOOL bin_file)
{
    int i = 0, ret = -1;
    // Legacy RC pattern
    u8 manufacture_data[] = { 0x57, 0xFD, 0x27, 0x06, 0x00, FIDX,
        0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x43, 0x52, 0x4B, 0x54, 0x4D,   /* manufacturer data */
        0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; /* mask */
    u8 filter_cmd[] = { 0x57, 0xFD, 0x0A, 0x01, 0x00, FIDX, 0x20, 0x00,
        0x00, 0x00, 0x01, 0x80, 0x00 };
    u8 event[] = { 0x0E, 0x07, 0x01, 0x57, 0xFD, 0x00, /* ... */ };

    if (bin_file) {
        if (data->wake_dev_len) {
            /* wake_on_ble.conf using 90(0x5A-FIDX) as filter_index */
            u8 pos = 0;
            u8 broadcast_addr[] = { 0x57, 0xFD, 0x0A, 0x02, 0x00, FIDX,
                0x55, 0x44, 0x33, 0x22, 0x11, 0x00, // ADDRESS
                0x00 };  // 0: Public, 1: Random
            u8 adv_pattern[] = { 0x57, 0xFD, 0x15, 0x06, 0x00, FIDX,
                0x71, 0x01,                 // VID
                0x04, 0x11,                 // PID
                0x00, 0x00, 0x00, 0x00,     // IR key code
                0x00,                       // sequence number
                0xFF, 0xFF,                 // mask~
                0xFF, 0xFF,
                0x00, 0x00, 0x00, 0x00,
                0x00};

            // BDADDR
            for (i = 0; i < data->wake_dev[1]; i++) {
                broadcast_addr[11] = data->wake_dev[2 + i * BD_ADDR_LEN + 0];
                broadcast_addr[10] = data->wake_dev[2 + i * BD_ADDR_LEN + 1];
                broadcast_addr[9] = data->wake_dev[2 + i * BD_ADDR_LEN + 2];
                broadcast_addr[8] = data->wake_dev[2 + i * BD_ADDR_LEN + 3];
                broadcast_addr[7] = data->wake_dev[2 + i * BD_ADDR_LEN + 4];
                broadcast_addr[6] = data->wake_dev[2 + i * BD_ADDR_LEN + 5];
                ret = btmtk_usb_send_hci_cmd(data, broadcast_addr, sizeof(broadcast_addr),
                        event, sizeof(event));
                if (ret < 0) {
                    UBOOT_ERROR("Set broadcast address fail\n");
                    continue;
                }
                // mask broadcast address as a filter condition
                filter_cmd[6] = 0x21;
            }
            UBOOT_DEBUG("There are %d broadcast address filter(s) from %s\n", i, WAKE_DEV_RECORD);

            /** VID/PID in conf is LITTLE endian, but PID in ADV is BIG endian */
            pos = 2 + data->wake_dev[1] * 6;
            for (i = 0; i < data->wake_dev[pos]; i++) {
                adv_pattern[6] = data->wake_dev[pos + (i * 4) + 1];
                adv_pattern[7] = data->wake_dev[pos + (i * 4) + 2];
                adv_pattern[9] = data->wake_dev[pos + (i * 4) + 3];
                adv_pattern[8] = data->wake_dev[pos + (i * 4) + 4];
                ret = btmtk_usb_send_hci_cmd(data, adv_pattern, sizeof(adv_pattern),
                        event, sizeof(event));
                if (ret < 0) {
                    UBOOT_ERROR("Set advertising patten fail\n");
                    return ret;
                }
            }
            UBOOT_DEBUG("There are %d manufacture data filter(s) from %s\n", i, WAKE_DEV_RECORD);

            // Filtering parameters
            ret = btmtk_usb_send_hci_cmd(data, filter_cmd, sizeof(filter_cmd),
                    event, sizeof(event));
            if (ret < 0) {
                UBOOT_ERROR("Set filtering parm fail\n");
                return ret;
            }

        // if wake_on_ble.conf exist, no need use default woble_setting.bin
        } else {
            // woble_setting.bin
            UBOOT_DEBUG("Set APCF filter from woble_setting.bin\n");
            for (i = 0; i < APCF_SETTING_COUNT; i++) {
                if (!data->apcf_cmd[i].len)
                    continue;
                ret = btmtk_usb_send_hci_cmd(data, data->apcf_cmd[i].value, data->apcf_cmd[i].len,
                        event, sizeof(event));
                if (ret < 0) {
                    UBOOT_ERROR("Set apcf_cmd[%d] data fail\n", i);
                    return ret;
                }
            }
        }

    } else {
        // Use default
        UBOOT_DEBUG("Using default APCF filter\n");
        os_memcpy(manufacture_data + 9, data->local_addr, BD_ADDR_LEN);
        ret = btmtk_usb_send_hci_cmd(data, manufacture_data,
                sizeof(manufacture_data), event, sizeof(event));
        if (ret < 0) {
            UBOOT_ERROR("Set manufacture data fail\n");
            return ret;
        }

        ret = btmtk_usb_send_hci_cmd(data, filter_cmd, sizeof(filter_cmd),
                event, sizeof(event));
        if (ret < 0) {
            UBOOT_ERROR("Set manufacture data fail\n");
            return ret;
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_check_need_load_patch_7668(struct LD_btmtk_usb_data *data)
{
    /* TRUE: need load patch., FALSE: do not need */
    u8 cmd[] = { 0x6F, 0xFC, 0x05, 0x01, 0x17, 0x01, 0x00, 0x01 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x17, 0x01, 0x00, /* 0x02 */ };    /* event[6] is key */
    int ret = -1;

    ret = btmtk_usb_send_wmt_cmd(data, cmd, sizeof(cmd), event, sizeof(event), 20, 0);
    /* can't get correct event */
    if (ret < 0) {
        UBOOT_DEBUG("check need load patch or not fail(%d)\n", ret);
        return PATCH_ERR;
    }

    if (ret >= sizeof(event) + 1) {
        UBOOT_DEBUG("%s: return len is %d\n", __func__, ret);
        return data->io_buf[6];
    }

    return PATCH_ERR;
}

//---------------------------------------------------------------------------
static int btmtk_usb_load_partial_rom_patch_7668(struct LD_btmtk_usb_data *data,
        u32 patch_len, int offset)
{
    u8 *pos = NULL;
    u8 phase = 0;
    s32 sent_len = 0;
    u32 cur_len = 0;
    int real_len = 0;
    int first_block = 1;
    int ret = 0;
    void *buf = NULL;

    buf = os_kzalloc(UPLOAD_PATCH_UNIT, MTK_GFP_ATOMIC);
    if (!buf) {
        return -ENOMEM;
    }
    pos = buf;

    /* loading rom patch */
    while (1) {
        s32 sent_len_max = UPLOAD_PATCH_UNIT - PATCH_HEADER_SIZE;

        real_len = 0;
        sent_len = (patch_len - cur_len) >= sent_len_max ? sent_len_max : (patch_len - cur_len);
        if (sent_len > 0) {
            if (first_block == 1) {
                if (sent_len < sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE1;
                first_block = 0;
            } else if (sent_len == sent_len_max) {
                if (patch_len - cur_len == sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE2;
            } else {
                phase = PATCH_PHASE3;
            }

            /* prepare HCI header */
            pos[0] = 0x6F;
            pos[1] = 0xFC;
            pos[2] = (sent_len + 5) & 0xFF;
            pos[3] = ((sent_len + 5) >> 8) & 0xFF;

            /* prepare WMT header */
            pos[4] = 0x01;
            pos[5] = 0x01;
            pos[6] = (sent_len + 1) & 0xFF;
            pos[7] = ((sent_len + 1) >> 8) & 0xFF;

            pos[8] = phase;

            os_memcpy(&pos[9], data->rom_patch + offset + cur_len, sent_len);
            //UBOOT_DEBUG("sent_len = %d, cur_len = %d, phase = %d\n", sent_len, cur_len, phase);

            ret = data->hcif->usb_bulk_msg(data->udev, MTKBT_BULK_TX_EP, buf,
                    sent_len + PATCH_HEADER_SIZE, &real_len, 0);
            if (ret) {
                UBOOT_DEBUG("upload rom_patch err: %d\n", ret);
                ret = -1;
                goto free;
            }
            cur_len += sent_len;
            MTK_MDELAY(1);
            btmtk_usb_get_rom_patch_result(data);
            MTK_MDELAY(1);

        } else {
            UBOOT_DEBUG("loading rom patch... Done\n");
            break;
        }
        os_memset(buf, 0, UPLOAD_PATCH_UNIT);
    }
free:
    os_kfree(buf);
    buf = NULL;

    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_load_rom_patch_7668(struct LD_btmtk_usb_data *data)
{
    int ret = 0;
    int patch_status = 0;
    int retry = 20;
    char *tmp_str = NULL;
    BOOL sysram3 = FALSE;
    u32 patch_len = 0;

    LD_load_code_from_bin(&data->rom_patch, (char *)data->rom_patch_bin_file_name,
            NULL, data->udev, &data->rom_patch_len);
    if (!data->rom_patch || !data->rom_patch_len) {
        UBOOT_DEBUG("please assign a rom patch from (/etc/firmware/%s) or (/lib/firmware/%s)\n",
                data->rom_patch_bin_file_name, data->rom_patch_bin_file_name);
        return -1;
    }

    if (is_mt7668(data))
        sysram3 = data->rom_patch_len > (PATCH_INFO_SIZE + PATCH_LEN_ILM) ? TRUE : FALSE;

    do {
        patch_status = btmtk_usb_check_need_load_patch_7668(data);
        UBOOT_DEBUG("patch_status: %d, retry: %d\n", patch_status, retry);
        if (patch_status > PATCH_NEED_DOWNLOAD || patch_status == PATCH_ERR) {
            UBOOT_DEBUG("%s: patch_status error\n", __func__);
            return -1;
        } else if (patch_status == PATCH_READY) {
            if (sysram3 == TRUE) {
                UBOOT_DEBUG("%s: Prepare to load sysram3\n", __func__);
                goto sysram;
            }
            UBOOT_DEBUG("%s: No need to load ROM patch\n", __func__);
            return 0;
        } else if (patch_status == PATCH_IS_DOWNLOAD_BY_OTHER) {
            MTK_MDELAY(100);
            retry--;
        } else if (patch_status == PATCH_NEED_DOWNLOAD) {
            break; /* Download ROM patch directly */
    }
    } while (retry > 0);

    if (patch_status == PATCH_IS_DOWNLOAD_BY_OTHER) {
        UBOOT_DEBUG("Hold by another fun more than 2 seconds");
        return -1;
}

    tmp_str = data->rom_patch;
    SHOW_FW_DETAILS("FW Version");
    SHOW_FW_DETAILS("build Time");

    tmp_str = data->rom_patch + 16;
    UBOOT_DEBUG("platform = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = data->rom_patch + 20;
    UBOOT_DEBUG("HW/SW version = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = data->rom_patch + 24;

    UBOOT_DEBUG("%s: loading rom patch of ILM\n", __func__);
    patch_len = sysram3 ? PATCH_LEN_ILM : (data->rom_patch_len - PATCH_INFO_SIZE);
    ret = btmtk_usb_load_partial_rom_patch_7668(data, patch_len, PATCH_INFO_SIZE);
    if (ret < 0)
            return ret;

    /* CHIP_RESET, ROM patch would be reactivated.
     * Currently, wmt reset is only for ILM rom patch, and there are also
     * some preparations need to be done in FW for loading sysram3 patch...
     */
    MTK_MDELAY(20);
    ret = btmtk_usb_send_wmt_reset_cmd(data);
    if (ret < 0)
        return ret;
    MTK_MDELAY(20);

sysram:
    if (sysram3) {
        UBOOT_DEBUG("%s: loading rom patch of sysram3\n", __func__);
        patch_len = data->rom_patch_len - PATCH_INFO_SIZE - PATCH_LEN_ILM - PATCH_INFO_SIZE;
        ret = btmtk_usb_load_partial_rom_patch_7668(data, patch_len,
                PATCH_INFO_SIZE + PATCH_LEN_ILM + PATCH_INFO_SIZE);
    }
    return ret;
}

//---------------------------------------------------------------------------
static int btmtk_usb_set_radio_off_cmd(struct cmd_tlv *radiooff, u8 attr,
        void *value, u8 pos, u8 len)
{
    u8 cmd_len = 0;
    u8 attr_len = 0;
    int i = 5, j = 0; /* 5 is a start of attr group */

    if (radiooff == NULL || value == NULL || len == 0) {
        UBOOT_ERROR("%s: Incorrect parameters - %s %s %s\n", __func__,
                radiooff ? "" : "radiooff",
                value ? "" : "value", len ? "" : "len");
        return -EINVAL;
    }
    cmd_len = radiooff->value[2];
    usb_debug_raw((const u8 *)radiooff->value, radiooff->len, "Origin RADIOOFF(%d): ", radiooff->len);

    do {
        attr_len = radiooff->value[i];
        if (i < radiooff->len - 1 && radiooff->value[i + 1] == attr) {
            if (attr == WOBX_TYPE_KEYCODE_MAPPING) {
                if (len != attr_len - 1) {
                    /* could rewrite length, this attr should local in the end */
                    u8 *tmp = radiooff->value;
                    int new_len = 3 + cmd_len - attr_len + (len + 1);

                    UBOOT_DEBUG("%s: Re-allocate size(%d/%d) for mod\n", __func__,
                            radiooff->len, new_len);
                    radiooff->value = os_kzalloc(new_len, MTK_GFP_ATOMIC);
                    memcpy(radiooff->value, tmp, radiooff->len);
                    os_kfree(tmp);
                    tmp = NULL;
                    radiooff->len = new_len;
                    /* Update cmd len & attr len both are necessary */
                    radiooff->value[i] = len + 1;     /* attr len */
                    radiooff->value[2] = new_len - 3; /* cmd len */
                }
            }
            /* assign value */
            for (j = 0; j < len; j++)
                radiooff->value[i + 1 + pos + j] = *((u8 *)value + j);
            break;
        }
        i += (attr_len + 1);
    } while (i + 1 < cmd_len);

    if (i + 1 < cmd_len) {
        usb_debug_raw(radiooff->value, radiooff->len, "Update RADIOOFF(%d): ", radiooff->len);
        return 0;
    }

    if (attr == WOBX_TYPE_IR) {
        UBOOT_ERROR("%s: Please add WOBX_TYPE_IR in woble_setting.bin first\n", __func__);
        return -ENOENT;
    } else if (attr == WOBX_TYPE_KEYCODE_MAPPING) {
        u8 *tmp = radiooff->value;
        int new_len = 3 + cmd_len + 2 + len;
        int old_len = radiooff->len;

        UBOOT_DEBUG("%s: Re-allocate size(%d/%d) for mod\n", __func__,
                radiooff->len, new_len);
        radiooff->value = os_kzalloc(new_len, MTK_GFP_ATOMIC);
        memcpy(radiooff->value, tmp, radiooff->len);
        os_kfree(tmp);
        tmp = NULL;
        radiooff->len = new_len;
        /* Update cmd len is necessary */
        radiooff->value[2] = new_len - 3; /* cmd len */

        radiooff->value[old_len] = len + 1;   /* attr len: attr + content */
        radiooff->value[old_len + 1] = attr;
        for (j = 0; j < len; j++)
            radiooff->value[old_len + 2 + j] = *((u8 *)value + j);
    }
    usb_debug_raw(radiooff->value, radiooff->len, "Append RADIOOFF(%d): ", radiooff->len);
    return 0;
}

//---------------------------------------------------------------------------
static int btmtk_usb_load_woble_ir_setting(struct cmd_tlv *radiooff,
        u8 *setting)
{
#define CHAR42HEX_SIZE  10
#define CHAR2HEX_SIZE   4
#define CHAR2DEC_SIZE   2
    int i = 0;
    char *head = NULL, *tail = NULL;
    u16 mark = 0x0000;
    char ir_proto[] = IR_PROTOCOL"00:"; /* pattern */
    char ir_km_g[] = IR_KEYMAP_G"00:";  /* pattern */
    char ir_km[] = IR_KEYMAP"00:";      /* pattern */
    char ir[CHAR2HEX_SIZE + 1] = {"\n"};    /* 1 is for '\n' */
    char group[3] = {"\n"};         /* DEC: 2, 1 is for '\n' */
    char **ptr = NULL;

    struct woble_ir_km_char_s map[MAX_IRKMG] = {{{'\n'}, {'\n'}}};
    unsigned long ir_value = 0;
    unsigned long group_value = 0;
    struct woble_ir_km_value_s map_value[MAX_IRKMG] = {{{0}, {0}}};

    if (radiooff == NULL || setting == NULL) {
        UBOOT_ERROR("%s: Incorrect parameters - %s %s\n", __func__,
                radiooff ? "" : "radiooff", setting ? "" : "setting");
        return -EINVAL;
    }

    /* confirm how many woble setting groups */
    for (i = 0; i < APCF_SETTING_COUNT; i++) {
        if (radiooff[i].value != NULL && radiooff[i].len != 0)
            mark |= (1 << i);
    }
    UBOOT_DEBUG("%s: mark: 0x%04X\n", __func__, mark);

    for (i = 0; ((mark >> i) & 0x0001) && i < APCF_SETTING_COUNT; i++) {
        UBOOT_DEBUG("%s: The following is radiooff[%d]\n", __func__, i);
        /* IR protocol parsing */
        snprintf(ir_proto, sizeof(ir_proto), "%s%02d:", IR_PROTOCOL, i);
        head = strstr((const char *)setting, (const char *)ir_proto);
        if (head) {
            head = strstr((const char *)head, "0x");  /* should be 0xAA */
            if (head) {
                tail = strstr((const char *)head, ",");
                if (!tail || tail - head != CHAR2HEX_SIZE) {
                    UBOOT_ERROR("%s: Has incorrect format: %p %p %s\n",
                            __func__, tail, head, head);
                    return -EINVAL;
                }
                memcpy(ir, head, CHAR2HEX_SIZE);
                ir_value = strtol(ir, ptr, 0);
                if (ir_value) {
                    UBOOT_INFO("%s: ir protocol is 0x%02X\n", __func__, (int)ir_value);
                    /* update radio off command */
                    btmtk_usb_set_radio_off_cmd(&radiooff[i], WOBX_TYPE_IR,
                            (void *)&ir_value, 2, 1);
                } else {
                    UBOOT_ERROR("%s: %s convert fail\n", __func__, ir);
                    return -EPERM;
                }
            } else {
                UBOOT_ERROR("%s: %s has incorrect format\n",
                        __func__, ir_proto);
                return -EINVAL;
            }
        } else {
            UBOOT_INFO("%s: No %s setting\n", __func__, ir_proto);
            return -ENOENT;
        }

        /* Keymap parsing */
        snprintf(ir_km_g, sizeof(ir_km_g), "%s%02d", IR_KEYMAP_G, i);
        head = strstr((const char *)setting, (const char *)ir_km_g);
        if (head) {
            head = strstr((const char *)head, ":");
            if (head) {
                tail = strstr((const char *)head, ",");
                if (!tail || tail - (head + 1) != CHAR2DEC_SIZE) {
                    UBOOT_ERROR("%s: %s has incorrect format\n",
                            __func__, head + 1);
                    return -EINVAL;
                }
                memcpy(group, head + 1, CHAR2DEC_SIZE);
                UBOOT_DEBUG("%s: group: %s\n", __func__, group);
                group_value = strtol(group, ptr, 0);
                if (group_value) {
                    int j = 0;

                    if (group_value > MAX_IRKMG) {
                        UBOOT_INFO("%s: Key mapping can't over than %d groups(%d), ignore surplus\n",
                                __func__, MAX_IRKMG, (unsigned int)group_value);
                        group_value = MAX_IRKMG; /* FW limitation */
                    }
                    for (j = 1; j <= group_value; j++) {
                        snprintf(ir_km, sizeof(ir_km), "%s%02d:", IR_KEYMAP, j);
                        head = strstr((const char *)setting, (const char *)ir_km);
                        if (head) {
                            /* should be 0x027D5FA0 for NEC */
                            head = strstr((const char *)head, "0x");
                            if (head) {
                                tail = strstr((const char *)head, ",");
                                if (!tail || tail - head != CHAR42HEX_SIZE) {
                                    UBOOT_ERROR("%s: %s has incorrect format\n",
                                            __func__, head);
                                    break;
                                }
                                memcpy(map[j - 1].nec, head, CHAR42HEX_SIZE);
                                UBOOT_DEBUG("%s: %02d:nec: %s\n", __func__, j, map[j - 1].nec);
                                *(u32 *)map_value[j - 1].nec = strtol(map[j - 1].nec, ptr, 0);
                                if (*(u32 *)map_value[j - 1].nec == 0) {
                                    UBOOT_ERROR("%s: %s convert fail\n", __func__, map[j - 1].nec);
                                    break;
                                }
                            } else {
                                UBOOT_ERROR("%s: Incorrect format - %s\n",
                                        __func__, head);
                                break;
                            }
                            /* should be 0x36B90000 for RC5 */
                            head = strstr((const char *)tail, "0x");
                            if (head) {
                                tail = strstr((const char *)head, ",");
                                if (!tail || tail - head != CHAR42HEX_SIZE) {
                                    UBOOT_ERROR("%s: %s has incorrect format\n",
                                            __func__, head);
                                    break;
                                }
                                memcpy(map[j - 1].rc5, head, CHAR42HEX_SIZE);
                                UBOOT_DEBUG("%s: %02d:rc5: %s\n", __func__, j, map[j - 1].rc5);
                                *(u32 *)map_value[j - 1].rc5 = strtol(map[j - 1].rc5, ptr, 0);
                                if (*(u32 *)map_value[j - 1].rc5 == 0) {
                                    UBOOT_ERROR("%s: %s convert fail\n", __func__, map[j - 1].rc5);
                                    break;
                                }
                            } else {
                                UBOOT_ERROR("%s: Incorrect format - %s\n",
                                        __func__, head);
                                break;
                            }
                        } else {
                            UBOOT_INFO("%s: Can't find keymap[%d]\n", __func__, j);
                            break;
                        }
                    }
                    btmtk_usb_set_radio_off_cmd(&radiooff[i], WOBX_TYPE_KEYCODE_MAPPING,
                            (void *)map_value, 1, group_value * 8);

                } else
                    UBOOT_ERROR("%s: %s convert fail\n", __func__, group);
            } else
                UBOOT_ERROR("%s: Incorrect format\n", __func__);
        } else
            UBOOT_INFO("%s: No keymap[%d]\n", __func__, i);
    }
    return 0;
}

//---------------------------------------------------------------------------
void btmtk_usb_woble_setting_parsing(struct LD_btmtk_usb_data *data, woble_setting_type type)
{
#define ONE_BYTE_HEX_MAX_LEN 8
    int i = 0;
    char *cmd = NULL;
    char *head = NULL;
    char *next_cmd = NULL;
    char prefix[32] = {0};
    u8 tmp_len = 0;
    u8 tmp[260] = {0};
    long int set_addr = 0;
    long int addr_pos = 0;

    switch (type) {
    case TYPE_APCF_CMD:
        cmd = "APCF";
        break;
    case TYPE_RADIOFF_CMD:
        cmd = "RADIOOFF";
        break;
    case TYPE_IRKM_NEC2RC5_CMD:
        btmtk_usb_load_woble_ir_setting(data->radioff_cmd, data->woble_setting);
        return;
    default:
        UBOOT_ERROR("Incorrect Type\n");
        return;
    }

    for (i = 0; i < APCF_SETTING_COUNT; ++i) {
        snprintf(prefix, sizeof(prefix), "%s%02d:", cmd, i);
        head = strstr((const char *)data->woble_setting, prefix);

        if (head) {
            head += strlen(prefix);  /* move to first numeral */
            next_cmd = strstr((const char *)head, ":");   // next command start position

            tmp_len = 0;
            memset(tmp, 0, sizeof(tmp));
            do {
                tmp[tmp_len++] = strtol(head, &head, 0);

                // for next one
                head = strstr((const char *)head, "0x");
                if (next_cmd && head > next_cmd)
                    break; // command end
            } while (tmp_len < sizeof(tmp));

            if (tmp_len) {
                if (type == TYPE_APCF_CMD) {
                    /* Save command */
                    data->apcf_cmd[i].value = os_kzalloc(tmp_len, MTK_GFP_ATOMIC);
                    os_memcpy(data->apcf_cmd[i].value, tmp, tmp_len);
                    data->apcf_cmd[i].len = tmp_len;

                    /* Check need BD address or not */
                    snprintf(prefix, sizeof(prefix), "%s_ADD_MAC%02d:", cmd, i);
                    head = strstr((const char *)data->woble_setting, prefix);
                    head += strlen(prefix);
                    set_addr = strtol(head, &head, 0);

                    if (set_addr == 1) {
                        snprintf(prefix, sizeof(prefix), "%s_ADD_MAC_LOCATION%02d:", cmd, i);
                        head = strstr((const char *)data->woble_setting, prefix);
                        head += strlen(prefix);
                        addr_pos = strtol(head, &head, 0);

                        if (addr_pos)
                            os_memcpy(data->apcf_cmd[i].value + addr_pos,
                                    data->local_addr, BD_ADDR_LEN);
                    }
                    usb_debug_raw(data->apcf_cmd[i].value, data->apcf_cmd[i].len,
                            "APCF[%02d]:", i);
                } else if (type == TYPE_RADIOFF_CMD) {
                    int j = 0;

                    data->radioff_cmd[i].value = os_kzalloc(tmp_len, MTK_GFP_ATOMIC);
                    os_memcpy(data->radioff_cmd[i].value, tmp, tmp_len);
                    data->radioff_cmd[i].len = tmp_len;
                    for (j = 0; j < tmp_len - 2; j++) {
                        if (data->radioff_cmd[i].value[j] == 0x02 && data->radioff_cmd[i].value[j + 1] == 0x25) {
                            UBOOT_DEBUG("Original scan filter policy is %s(%d), change to 0 for all",
                                    data->radioff_cmd[i].value[j + 2] ? "White list" : "All",
                                    data->radioff_cmd[i].value[j + 2]);
                            data->radioff_cmd[i].value[j + 2] = 0;   /* set for accept all */
                            break;
                        }
                    }
                    usb_debug_raw(data->radioff_cmd[i].value, data->radioff_cmd[i].len,
                            "RADIO OFF[%02d]:", i);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
int btmtk_usb_load_woble_setting(struct LD_btmtk_usb_data *data)
{
    int i = 0;
    BOOL woble_setting_bin = FALSE;
    BOOL wake_on_ble_conf = FALSE;

    if (!data)
        return -EINVAL;

    /* For woble_setting.bin */
    data->woble_setting = NULL;
    data->woble_setting_len = 0;

    LD_load_code_from_bin(&data->woble_setting, WOBLE_SETTING_FILE_NAME, NULL,
            data->udev, &data->woble_setting_len);
    if (data->woble_setting == NULL || data->woble_setting_len == 0) {
        UBOOT_INFO("Please make sure %s in the /vendor/firmware\n",
                WOBLE_SETTING_FILE_NAME);
        woble_setting_bin = FALSE;
    } else {
        btmtk_usb_woble_setting_parsing(data, TYPE_APCF_CMD);
        for (i = 0; i < APCF_SETTING_COUNT; i++)
            if (data->apcf_cmd[i].len)
                break;
        if (i == APCF_SETTING_COUNT) {
            woble_setting_bin = FALSE;
            if (data->woble_setting) {
                data->woble_setting_len = 0;
                os_kfree(data->woble_setting);
                data->woble_setting = NULL;
            }
        } else {
            woble_setting_bin = TRUE;
        }
        btmtk_usb_woble_setting_parsing(data, TYPE_RADIOFF_CMD);
        btmtk_usb_woble_setting_parsing(data, TYPE_IRKM_NEC2RC5_CMD);
    }

    /* For wake_on_ble.conf */
    data->wake_dev = NULL;
    data->wake_dev_len = 0;

    LD_load_code_from_bin(&data->wake_dev, WAKE_DEV_RECORD, WAKE_DEV_RECORD_PATH,
            data->udev, &data->wake_dev_len);
    if (data->wake_dev == NULL || data->wake_dev_len == 0) {
        UBOOT_INFO("There is no DEVICE RECORD for wake-up\n");
        wake_on_ble_conf = FALSE;
    } else {
        // content check
        if (data->wake_dev[0] != data->wake_dev_len || data->wake_dev_len < 3) {
            UBOOT_ERROR("Incorrect total length on %s\n", WAKE_DEV_RECORD);
            data->wake_dev_len = 0;
            os_kfree(data->wake_dev);
            data->wake_dev = NULL;
            wake_on_ble_conf = FALSE;
        } else {
            wake_on_ble_conf = TRUE;
        }
    }

    if (woble_setting_bin == FALSE && wake_on_ble_conf == FALSE)
        return -ENOENT;
    return 0;
}

//---------------------------------------------------------------------------
int btmtk_usb_set_unify_woble(struct LD_btmtk_usb_data *data)
{
    int ret = -1;
    u8 cmd[] = { 0xC9, 0xFC, 0x81, 0x01, 0x20, 0x02, 0x00, 0x01, 0x02, 0x01,
        0x01, 0x05, 0x10, 0x09, 0x00, 0xC0, 0x00, 0x02, 0x40, FIDX, 0x04,
        0x11, 0x12, 0x01, 0x00, 0x02, 0x42, 0x15, 0x02, 0x25, 0x00, 0x02,
        0x41, 0x19, 0x61, 0x47,
            0xA0, 0x5F, 0x7D, 0x02, 0x00, 0x00, 0xB9, 0x36,
            0x5E, 0xA1, 0x7D, 0x02, 0x00, 0x00, 0x85, 0x36,
            0x5D, 0xA2, 0x7D, 0x02, 0x00, 0x00, 0xA5, 0x36,
            0x5C, 0xA3, 0x7D, 0x02, 0x00, 0x00, 0x95, 0x36,
            0xB9, 0x46, 0x7D, 0x02, 0x00, 0x00, 0x8C, 0x36,
            0x60, 0x9F, 0x7D, 0x02, 0x00, 0x00, 0xA9, 0x36,
            0xB5, 0x4A, 0x7D, 0x02, 0x00, 0x00, 0xAB, 0x36,
            0x69, 0x96, 0x7D, 0x02, 0x00, 0x00, 0x97, 0x36,
            0xF3, 0x0C, 0x7D, 0x02, 0x00, 0x00, 0x82, 0x36,
            0xE6, 0x19, 0x7D, 0x02, 0x00, 0x00, 0xA2, 0x36,
            0xB3, 0x4C, 0x7D, 0x02, 0x00, 0x00, 0xAC, 0x36,
            0x5F, 0xA0, 0x7D, 0x02, 0x00, 0x00, 0xBF, 0x36 };
    u8 event[] = { 0xE6, 0x02, 0x08, 0x00 };

    UBOOT_DEBUG("%s: APCF filtering index: %d\n", __func__, FIDX);
    /* Only array 0 is working */
    if (data->radioff_cmd[0].len && data->radioff_cmd[0].value != NULL) {
        ret = btmtk_usb_send_hci_cmd(data, data->radioff_cmd[0].value,
                data->radioff_cmd[0].len, event, sizeof(event));
    } else {
        UBOOT_DEBUG("Using default radio off cmd\n");
        ret = btmtk_usb_send_hci_cmd(data, cmd, sizeof(cmd), event, sizeof(event));
    }
    if (ret < 0)
        UBOOT_ERROR("Failed(%d)\n", ret);
    return ret;
}

//---------------------------------------------------------------------------
void LD_btmtk_usb_SetWoble(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data = BT_INST(dev)->priv_data;
    int ret = -1;

    //UBOOT_DEBUG("\n");
    if (!data) {
        UBOOT_ERROR("btmtk data NULL!\n");
        return;
    }

    if (is_mt7668(data)) {
        /* Power on sequence */
        btmtk_usb_send_wmt_power_on_cmd_7668(data);
        btmtk_usb_send_hci_tci_set_sleep_cmd_7668(data);
        btmtk_usb_send_hci_reset_cmd(data);

        /* Unify WoBLE flow */
        btmtk_usb_get_vendor_cap(data);
        btmtk_usb_send_read_bdaddr(data);
        ret = btmtk_usb_load_woble_setting(data);
        if (ret) {
            UBOOT_INFO("Using lagecy WoBLE setting(%d)!!!\n", ret);
            btmtk_usb_set_apcf(data, FALSE);
        } else
            btmtk_usb_set_apcf(data, TRUE);
        btmtk_usb_set_unify_woble(data);
    } else {
        btmtk_usb_send_hci_suspend_cmd(data);
    }

    // Clean & free buffer
    if (data->woble_setting) {
        int i = 0;
        os_kfree(data->woble_setting);
        data->woble_setting = NULL;
        data->woble_setting_len = 0;
        for (i = 0; i < APCF_SETTING_COUNT; i++) {
            os_kfree(data->apcf_cmd[i].value);
            data->apcf_cmd[i].len = 0;
            os_kfree(data->radioff_cmd[i].value);
            data->radioff_cmd[i].len = 0;
        }
    }
    if (data->wake_dev) {
        os_kfree(data->wake_dev);
        data->wake_dev = NULL;
        data->wake_dev_len = 0;
    }

    return;
}

//---------------------------------------------------------------------------
int LD_btmtk_usb_probe(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data;
    int  err = 0;

    UBOOT_INFO("=========================================\n");
    UBOOT_INFO("Mediatek Bluetooth USB driver ver %s\n", LD_VERSION);
    UBOOT_INFO("=========================================\n");
    os_memcpy(driver_version, LD_VERSION, sizeof(LD_VERSION));
    probe_counter++;
    isbtready = 0;
    is_assert = 0;
    //UBOOT_DEBUG("LD_btmtk_usb_probe begin\n");
    UBOOT_DEBUG("probe_counter = %d\n", probe_counter);

    data = os_kzalloc(sizeof(*data), MTK_GFP_ATOMIC);
    if (!data) {
        UBOOT_ERROR("[ERR] end Error 1\n");
        return -ENOMEM;
    }

    data->hcif = BT_INST(dev)->hci_if;

    data->cmdreq_type = USB_TYPE_CLASS;

    data->udev = dev;

    data->meta_tx = 0;

    data->io_buf = os_kmalloc(LD_BT_MAX_EVENT_SIZE, MTK_GFP_ATOMIC);

    btmtk_usb_switch_iobase(data, WLAN);

    /* clayton: according to the chip id, load f/w or rom patch */
    btmtk_usb_cap_init(data);

    if (data->need_load_rom_patch) {
        if (is_mt7668(data))
            err = btmtk_usb_load_rom_patch_7668(data);
        else
            err = btmtk_usb_load_rom_patch(data);
        //btmtk_usb_send_hci_suspend_cmd(data);
        if (err < 0) {
            if (data->io_buf) os_kfree(data->io_buf);
            if (data->rom_patch_bin_file_name) os_kfree(data->rom_patch_bin_file_name);
            os_kfree(data);
            UBOOT_ERROR("[ERR] end Error 2\n");
            return err;
        }
    }

    // Clean & free buffer
    if (data->rom_patch_bin_file_name) {
        os_kfree(data->rom_patch_bin_file_name);
        data->rom_patch_bin_file_name = NULL;
    }

    if (data->rom_patch) {
        os_kfree(data->rom_patch);
        data->rom_patch = NULL;
    }

    isUsbDisconnet = 0;
    BT_INST(dev)->priv_data = data;
    isbtready = 1;

    UBOOT_DEBUG("LD_btmtk_usb_probe end\n");
    return 0;
}

//---------------------------------------------------------------------------
void LD_btmtk_usb_disconnect(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data = BT_INST(dev)->priv_data;

    UBOOT_DEBUG("\n");

    if (!data)
        return;

    isbtready = 0;
    metaCount = 0;

    UBOOT_DEBUG("unregister bt irq\n");

    isUsbDisconnet = 1;
    UBOOT_DEBUG("btmtk: stop all URB\n");
    os_kfree(data->io_buf);
    data->io_buf = NULL;
    os_kfree(data);
    data = NULL;
}

//---------------------------------------------------------------------------
