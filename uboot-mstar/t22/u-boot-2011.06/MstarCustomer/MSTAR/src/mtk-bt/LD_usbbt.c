#include <command.h>
#include <common.h>
#include <ShareType.h>
#include <CusConfig.h>
#include <MsVfs.h>
#include <usb.h>
#include <MsSystem.h>
#include <stdio.h>
#include <mtk-bt/LD_usbbt.h>
#include <mtk-bt/LD_btmtk_usb.h>

usb_vid_pid array_mtk_vid_pid[] = {
    {0x0E8D, 0x7668, "MTK7668"},    // 7668
    {0x0E8D, 0x76A0, "MTK7662T"},   // 7662T
    {0x0E8D, 0x76A1, "MTK7632T"},   // 7632T
};

int max_mtk_wifi_id = (sizeof(array_mtk_vid_pid) / sizeof(array_mtk_vid_pid[0]));
usb_vid_pid *pmtk_wifi = &array_mtk_vid_pid[0];

static mtkbt_dev_t *g_DrvData = NULL;
extern int snprintf(char *str, size_t size, const char *fmt, ...);

VOID *os_memcpy(VOID *dst, const VOID *src, UINT32 len)
{
    return memcpy(dst, src, len);
}

VOID *os_memmove(VOID *dest, const void *src,UINT32 len)
{
    return memmove(dest, src, len);
}

VOID *os_memset(VOID *s, int c, size_t n)
{
    return memset(s,c,n);
}

VOID *os_kzalloc(size_t size, unsigned int flags)
{
    VOID *ptr = malloc(size);

    os_memset(ptr, 0, size);
    return ptr;
}

void LD_load_code_from_bin(unsigned char **image, char *bin_name, char *path, mtkbt_dev_t *dev, u32 *code_len)
{
    char mtk_patch_bin_patch[128] = "\0";

    /** implement by mstar/MTK
     * path: /system/etc/firmware/mt76XX_patch_eX_hdr.bin
     * If argument "path" is NULL, access "/etc/firmware" directly like as request_firmware
     * if argument "path" is not NULL, so far only support directory "userdata"
     *     NOTE: latest vfs_mount seems decided this time access directory
     */
#if (ENABLE_MODULE_ANDROID_BOOT == 1)
    if (path == NULL && vfs_mount("vendor") != 0) {
#else
    if (path == NULL && vfs_mount("APP") != 0) {
#endif
        UBOOT_ERROR("vfs_mount - system fail\n");
        return;
    } else if (path != NULL && vfs_mount("userdata") != 0) {
        UBOOT_ERROR("vfs_mount - userdata fail\n");
        return;
    }

    if (path) {
        snprintf(mtk_patch_bin_patch, sizeof(mtk_patch_bin_patch), "%s/%s", path, bin_name);
        printf("File: %s\n", mtk_patch_bin_patch);
    } else {
#if (ENABLE_MODULE_ANDROID_BOOT == 1)
        snprintf(mtk_patch_bin_patch, sizeof(mtk_patch_bin_patch), "%s/%s", "/firmware", bin_name);
#else
        snprintf(mtk_patch_bin_patch, sizeof(mtk_patch_bin_patch), "%s/%s", "/krl/wifi/ralink/firmware", bin_name);
#endif
        UBOOT_DEBUG("mtk_patch_bin_patch: %s\n", mtk_patch_bin_patch);
    }
    *code_len = vfs_getsize(mtk_patch_bin_patch);
    if (*code_len == 0) {
        UBOOT_ERROR("Get file size fail\n");
        return;
    }

    // malloc buffer to store bt patch file data
    *image = malloc(*code_len);
    if(vfs_read(*image, mtk_patch_bin_patch, 0, *code_len) != 0)
    {
        UBOOT_ERROR("vfs_read fail\n");
        return;
    }
    UBOOT_DEBUG("Load file OK\n");
    UBOOT_DUMP((unsigned int)*image, 0x200);
    return;
}

static int usb_bt_bulk_msg(
        mtkbt_dev_t *dev,
        u32 epType,
        u8 *data,
        int size,
        int* realsize,
        int timeout /* not used */
)
{
    int ret = 0;
    if (dev == NULL || dev->udev == NULL || dev->bulk_tx_ep == NULL) {
        UBOOT_ERROR("bulk out error 00\n");
        return -1;
    }

    //UBOOT_DEBUG("[usb_bt_bulk_msg]ep_addr:%x\n", dev->bulk_tx_ep->bEndpointAddress);
    //UBOOT_DEBUG("[usb_bt_bulk_msg]ep_maxpkt:%x\n", dev->bulk_tx_ep->wMaxPacketSize);

    if (epType == MTKBT_BULK_TX_EP) {
        ret = usb_bulk_msg(dev->udev, usb_sndbulkpipe(dev->udev,
                    dev->bulk_tx_ep->bEndpointAddress), data, size, realsize, 2000);

        if (ret) {
            UBOOT_ERROR("bulk out error 01\n");
            return -1;
        }

        if(*realsize == size) {
            //UBOOT_DEBUG("bulk out success 01,size =0x%x\n",size);
            return 0;
        } else {
            UBOOT_ERROR("bulk out fail 02,size =0x%x,realsize =0x%x\n",size,*realsize);
        }
    }
    return -1;
}

static int usb_bt_control_msg(
        mtkbt_dev_t *dev,
        u32 epType,
        u8 request,
        u8 requesttype,
        u16 value,
        u16 index,
        u8 *data,
        int data_length,
        int timeout  /* not used */
)
{
    int ret = -1;
    if (epType == MTKBT_CTRL_TX_EP) {
        ret = usb_control_msg(dev->udev, usb_sndctrlpipe(dev->udev, 0), request,
                  requesttype, value, index, data, data_length,timeout);
    } else if (epType == MTKBT_CTRL_RX_EP) {
        ret = usb_control_msg(dev->udev, usb_rcvctrlpipe(dev->udev, 0), request,
                  requesttype, value, index, data, data_length,timeout);
    } else {
        UBOOT_ERROR("control message wrong Type =0x%x\n",epType);
    }

    if (ret < 0) {
        UBOOT_ERROR("Err1(%d)\n", ret);
        return ret;
    }
    return ret;
}

static int usb_bt_interrupt_msg(
        mtkbt_dev_t *dev,
        u32 epType,
        u8 *data,
        int size,
        int* realsize,
        int timeout  /* unit of 1ms */
)
{
    int ret = -1;

    UBOOT_DEBUG("epType = 0x%x\n", epType);

    if (epType == MTKBT_INTR_EP)
        ret = usb_submit_int_msg(dev->udev,usb_rcvintpipe(dev->udev,dev->intr_ep->bEndpointAddress),data,size,realsize,timeout);

    if (ret < 0) {
        UBOOT_ERROR("Err1(%d)\n", ret);
        return ret;
    }
    UBOOT_DEBUG("ret = 0x%x\n", ret);
    return ret;
}

static HC_IF usbbt_host_interface =
{
        usb_bt_bulk_msg,
        usb_bt_control_msg,
        usb_bt_interrupt_msg,
};

static void Ldbtusb_disconnect (btusbdev_t *dev)
{
    LD_btmtk_usb_disconnect(g_DrvData);

    if(g_DrvData)
    {
        os_kfree(g_DrvData);
    }
    g_DrvData = NULL;
}

static int Ldbtusb_SetWoble(btusbdev_t *dev)
{
    if (!g_DrvData) {
        UBOOT_ERROR("usb set woble fail, because no drv data\n");
        return -1;
    } else {
        LD_btmtk_usb_SetWoble(g_DrvData);
        UBOOT_DEBUG("usb set woble end\n");
    }
    return 0;
}

int Ldbtusb_connect (btusbdev_t *dev)
{
    int ret = 0;

    // For Mstar
    struct usb_endpoint_descriptor *ep_desc;
    struct usb_interface *iface;
    int i;
    iface = &dev->config.if_desc[0];

    if(g_DrvData == NULL) {
        g_DrvData = os_kmalloc(sizeof(mtkbt_dev_t),MTK_GFP_ATOMIC);

        if(!g_DrvData) {
            UBOOT_ERROR("Not enough memory for mtkbt virtual usb device.\n");
            return -1;
        } else {
            os_memset(g_DrvData,0,sizeof(mtkbt_dev_t));
            g_DrvData->udev = dev;
            g_DrvData->connect = Ldbtusb_connect;
            g_DrvData->disconnect = Ldbtusb_disconnect;
            g_DrvData->SetWoble = Ldbtusb_SetWoble;
        }
    } else {
            return -1;
    }

    // For Mstar
    for (i = 0; i < iface->desc.bNumEndpoints; i++)
    {
        ep_desc = &iface->ep_desc[i];

        if ((ep_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)
        {
            if (ep_desc->bEndpointAddress & USB_DIR_IN)
            {
                g_DrvData->bulk_rx_ep = ep_desc;
            }
            else
            {
                g_DrvData->bulk_tx_ep = ep_desc;
            }
            continue;
        }

        /* is it an interrupt endpoint? */
        if ((ep_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)
        {
            g_DrvData->intr_ep = ep_desc;
            continue;
        }
    }
    if (!g_DrvData->intr_ep || !g_DrvData->bulk_tx_ep || !g_DrvData->bulk_rx_ep) {
        os_kfree(g_DrvData);
        g_DrvData = NULL;
        UBOOT_ERROR("btmtk_usb_probe end Error 3\n");
        return -1;
    }

    /* Init HostController interface */
    g_DrvData->hci_if = &usbbt_host_interface;

    /* btmtk init */
    ret = LD_btmtk_usb_probe(g_DrvData);

    if (ret != 0) {
        UBOOT_ERROR("usb probe fail\n");
        if(g_DrvData)
        {
           os_kfree(g_DrvData);
        }
        g_DrvData = NULL;
        return -1;
    } else {
        UBOOT_DEBUG("usbbt probe success\n");
    }
    return ret;
}

static int checkUsbDevicePort(struct usb_device* udev, u16 vendorID, u16 productID, u8 port)
{
    struct usb_device* pdev = NULL;
    int i;
#if defined (CONFIG_USB_PREINIT)
    usb_stop(port);
    if (usb_post_init(port) == 0)
#else
    if (usb_init(port) == 0)
#endif
    {
	for (i= 0; i < usb_get_dev_num(); i++)
	{
            pdev = usb_get_dev_index(i); /* get device */
            if ((pdev != NULL) && (pdev->descriptor.idVendor == vendorID) && (pdev->descriptor.idProduct == productID))  // MTK 7662
            {
                UBOOT_TRACE("OK\n");
                memcpy(udev, pdev, sizeof(struct usb_device));
                return 0 ;
            }
	}
    }
    return -1;
}

static int findUsbDevice(struct usb_device* udev)
{
    int ret = -1;
    u8 idx = 0;
    u8 i = 0;
    char portNumStr[10] = "\0";
    char* pBTUsbPort = NULL;
    UBOOT_TRACE("IN\n");
    if(udev == NULL)
    {
        UBOOT_ERROR("udev can not be NULL\n");
        return -1;
    }
    #define BT_USB_PORT "bt_usb_port"
    pBTUsbPort = getenv(BT_USB_PORT);
    if(pBTUsbPort != NULL)
    {
        i = 0;
        // search mtk bt usb port
        idx = atoi(pBTUsbPort);
        UBOOT_DEBUG("find mtk bt usb device from usb prot[%d]\n", idx);
        while (i < max_mtk_wifi_id) {
            ret = checkUsbDevicePort(udev, (pmtk_wifi + i)->vid, (pmtk_wifi + i)->pid, idx);
            if (ret == 0) break;
            i++;
        }
        if(ret == 0)
        {
            return 0;
        }
    }

    // not find mt bt usb device from given usb port, so poll every usb port.
    #if defined(ENABLE_FIFTH_EHC)
    const char u8UsbPortCount = 5;
    #elif defined(ENABLE_FOURTH_EHC)
    const char u8UsbPortCount = 4;
    #elif defined(ENABLE_THIRD_EHC)
    const char u8UsbPortCount = 3;
    #elif defined(ENABLE_SECOND_EHC)
    const char u8UsbPortCount = 2;
    #else
    const char u8UsbPortCount = 1;
    #endif
    for(idx = 0; idx < u8UsbPortCount; idx++)
    {
        i = 0;
        while (i < max_mtk_wifi_id) {
            ret = checkUsbDevicePort(udev, (pmtk_wifi + i)->vid, (pmtk_wifi + i)->pid, idx);
            if (ret == 0) break;
            i++;
        }
        if(ret == 0)
        {
            // set bt_usb_port to store mt bt usb device port
            snprintf(portNumStr, sizeof(portNumStr), "%d", idx);
            setenv(BT_USB_PORT, portNumStr);
            saveenv();
            return 0;
        }
    }
    if(pBTUsbPort != NULL)
    {
        // env BT_USB_PORT is involid, so delete it
        setenv(BT_USB_PORT, NULL);
        saveenv();
    }
    UBOOT_ERROR("Not find usb device\n");
    return -1;
}

int do_setMtkBT( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    struct usb_device udev;
    memset(&udev, 0, sizeof(struct usb_device));
    UBOOT_TRACE("IN\n");
    if (argc < 1)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    // MTK USB controller
    ret = findUsbDevice(&udev);
    if (ret != 0)
    {
        UBOOT_ERROR("find bt usb device failed\n");
        return -1;
    }
    ret = Ldbtusb_connect(&udev);
    if(ret != 0)
    {
        UBOOT_ERROR("connect to bt usb device failed\n");
        return -1;
    }
    ret = Ldbtusb_SetWoble(&udev);
    if(ret != 0)
    {
        UBOOT_ERROR("set bt usb device woble cmd failed\n");
        return -1;
    }

    UBOOT_DEBUG("Do Ldbtusb_disconnect\n");
    Ldbtusb_disconnect(&udev);

    UBOOT_TRACE("OK\n");
    return ret;
}
