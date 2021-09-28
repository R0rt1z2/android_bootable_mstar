
#include <common.h>
#include <command.h>
#include <jffs2/jffs2.h>
#include <mmc.h>
#include <bootimg.h>
#include "fastboot.h"
#include "download_command.h"
#include <malloc.h>
#include <mmc.h>
#include <cmd_fastboot.h>

#define MSTAR_USB_UDC_GLUE 1

#ifdef MSTAR_USB_UDC_GLUE
#include "ubootglue_udc.h"
#include <hal_usb_otg.h>

#endif


#ifdef CONFIG_USB_FASTBOOT_ENABLE
#define MAX_RSP_SIZE 64
/* MAX_USBFS_BULK_SIZE: if use USB3 QMU GPD mode: cannot exceed 63 * 1024 */
#define MAX_USBFS_BULK_SIZE (16 * 1024)

#define FALSE 0
#define TRUE 1

//static event_t usb_online;
//static event_t txn_done;
static unsigned char * buffer = (char *)CONFIG_UPGRADE_BUFFER_ADDR;
static struct udc_endpoint *in, *out;
static struct udc_request *req;
int txn_status;
static int req_status = REQ_STATE_DONE;
unsigned fastboot_state = STATE_OFFLINE;

void *download_base;
unsigned download_max;
unsigned download_size;

// for show log for show the read/write is in the process.
#define TRANSFER_COUNT_SHOW     64    // 16k*64 = 1MB
unsigned int transfer_cnt;

extern int  sec_usbdl_enabled (void);
extern void mtk_wdt_disable(void);
extern void fb_handler_command(char*, unsigned);
extern void fb_init(unsigned short);

struct fastboot_cmd *cmdlist;

#ifndef MSTAR_USB_UDC_GLUE
static void req_complete(struct udc_request *req, unsigned actual, int status)
{
	//printf("fastboot req_complete len(%d) status(%d).\n", actual, status);
	if(transfer_cnt ++ >= TRANSFER_COUNT_SHOW)
	{
		printf(".");
		transfer_cnt = 0;
	}
	txn_status = status;
	req->length = actual;
	req_status = REQ_STATE_DONE;
}
#endif
void fastboot_register(const char *prefix,
		       void (*handle)(const char *arg, const char *data, int data_length),
		       unsigned char security_enabled)
{
	struct fastboot_cmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (cmd) {
		cmd->prefix = prefix;
		cmd->prefix_len = strlen(prefix);
		cmd->sec_support = security_enabled;
		cmd->handle = handle;
		cmd->next = cmdlist;
		cmdlist = cmd;
	}
}

extern struct fastboot_var *varlist;

void fastboot_publish(char *name, char *value, void (*handle)(const char,const char,int))
{
	struct fastboot_var *var;
	var = malloc(sizeof(*var));
	memset(var, 0, sizeof(*var));
	if (var) {
		strcpy(var->name,name);
		strcpy(var->value,value);
		var->handle = handle;
		var->next = varlist;
		varlist = var;
	}
}

#ifdef MSTAR_USB_UDC_GLUE
extern int usb_read(void *_buf, unsigned len);
#else
int usb_read(void *_buf, unsigned len)
{
	int r;
	unsigned xfer;
	unsigned char *buf = _buf;
	int count = 0;
	printf("usb_read fastboot_state =%d, len = %d.\n", fastboot_state, len);

	if (fastboot_state == STATE_ERROR)
		goto oops;

	printf("usb_read(len = %d) begin.\n", len);
	transfer_cnt = 0;

	while (len > 0) {
		xfer = (len > MAX_USBFS_BULK_SIZE) ? MAX_USBFS_BULK_SIZE : len;
		req->buf = buf;
		req->length = xfer;
		req->complete = req_complete;
		r = udc_request_queue(out, req);
		req_status = REQ_STATE_READING;
		if (r < 0) {
			printf("usb_read() queue failed\n");
			goto oops;
		}

		// waiting for get data from pc
		while(req_status == REQ_STATE_READING)
		{
			//mdelay(1);
			service_interrupts();
		}

		if (txn_status < 0) {
			printf("usb_read() transaction failed status = 0x%x\n", txn_status);
			goto oops;
		}

		count += req->length;
		buf += req->length;
		len -= req->length;

		/* short transfer? */
		if (req->length != xfer) 
		{
			printf("\nread break: length = %d, xfer = %d.\n", req->length, xfer);
			break;
		}
	}

	printf("\n");
	printf("usb_read OK. count = %d.\n", count);
	return count;

oops:
	if(fastboot_state != STATE_OFFLINE)
		fastboot_state = STATE_ERROR;
	return -1;
}
#endif

#ifdef MSTAR_USB_UDC_GLUE
extern int usb_write(void *buf, unsigned len);
#else
int usb_write(void *buf, unsigned len)
{
	int r;
	printf("usb_write fastboot_state =%d, len = %d.\n", fastboot_state, len);

	if (fastboot_state == STATE_ERROR)
		goto oops;

	printf("usb_write(len = %d) begin.\n", len);
	transfer_cnt = 0;

	req->buf = buf;
	req->length = len;
	req->complete = req_complete;

	req_status = REQ_STATE_WRITING;
	r = udc_request_queue(in, req);
	if (r < 0) {
		printf("usb_write() queue failed\n");
		goto oops;
	}
	
	while(req_status == REQ_STATE_WRITING)
	{
		//mdelay(1);
		service_interrupts();
	}

	if (txn_status < 0) {
		printf("usb_write() transaction failed\n");
		goto oops;
	}
	return req->length;

oops:
	if(fastboot_state != STATE_OFFLINE)
		fastboot_state = STATE_ERROR;
	return -1;
}
#endif


void fastboot_ack(const char *code, const char *reason)
{
	char response[MAX_RSP_SIZE];

	if (fastboot_state != STATE_COMMAND)
		return;

	if (reason == 0)
		reason = "";

	sprintf(response, "%s%s", code, reason);
	fastboot_state = STATE_COMPLETE;

	usb_write(response, strlen(response));
}

void usb_fastboot_info(const char *reason)
{
	char response[MAX_RSP_SIZE];

	if (fastboot_state != STATE_COMMAND)
		return;

	if (reason == 0)
		return;

	//printf("..... fastboot_info: %s.\n", reason);
	//sprintf(response, "INFO%s", MAX_RSP_SIZE, reason);
	sprintf(response, "INFO%s", reason);

	usb_write(response, strlen(response));
}

void usb_fastboot_fail(const char *reason)
{
	printf("fastboot_fail: %s.\n", reason);
	fastboot_ack("FAIL", reason);
}

void usb_fastboot_okay(const char *info)
{
	printf("fastboot_okay: %s.\n",info);
	fastboot_ack("OKAY", info);
}

int fastboot_command_loop(void)
{
    struct fastboot_cmd *cmd;
    int read_length;

    printf("fastboot: processing commands ...\n");

    while (1)
    {
        service_interrupts();
#ifdef MSTAR_USB_UDC_GLUE
        if (usb_speed() == FULL_SPEED) {
            printf("fastboot: FULL speed USB detected. Switching to offline\n");
            fastboot_state = STATE_OFFLINE;
            return -1;
        }
#endif
        if(fastboot_state == STATE_OFFLINE)
            continue;

        fastboot_state = STATE_COMMAND;
        memset(buffer, 0, sizeof(buffer));
        read_length = usb_read(buffer, MAX_RSP_SIZE);
        if (read_length <= 0){
            printf("fastboot: no input command\n");
            continue;  // no input
        }

        buffer[read_length] = '\0';
        printf("fastboot: get command - [%s]\n", buffer);

        fb_handler_command(buffer, (unsigned)read_length);
        if (fb_need_reset) {
            fastboot_okay("");
            mdelay(500);
            run_command("reset", 0);
        }

        if (fb_need_continue) {
            fastboot_okay("");
            mdelay(500);
            run_command("continue", 0);
        }

        if (fb_status.status_code == kStatusError) {
            fastboot_fail(fb_status.message);
        }
        else if (fb_status.status_code == kStatusData) {
            printf("fastboot: KStatusData state\n");
            //do nothing
        }
        else {
            fastboot_okay(fb_status.message);
        }

     } // while (1)

    printf("fastboot: oops!!\n");
    fastboot_state = STATE_OFFLINE;
    return 0;
}
#if 0 //JH
static void fastboot_notify(struct udc_gadget *gadget, unsigned event)
{
	if (event == UDC_EVENT_ONLINE) {
		printf("fastboot usb online OK.\n");
		fastboot_state = STATE_ONLINE;
	} else if (event == UDC_EVENT_OFFLINE) {
		printf("fastboot usb offline OK.\n");
		fastboot_state = STATE_OFFLINE;
	}
}

static struct udc_endpoint *fastboot_endpoints[2];

static struct udc_gadget fastboot_gadget = {
	.notify		= fastboot_notify,
	.ifc_class	= 0xff,
	.ifc_subclass	= 0x42,
	.ifc_protocol	= 0x03,
	.ifc_endpoints	= 2,
	.ifc_string	= "fastboot",
	.ept		= fastboot_endpoints,
};
#endif
int fastboot_init(void)
{
	printf("Enter into the fastboot mode.\n");

    fb_init(0);

#if 0
	//Client Variables for getvar:%s
	fastboot_publish("version", "0.5",NULL);
	fastboot_publish("serialno", "123456789ABCDEF",NULL);
	fastboot_publish("version-bootloader", "1.1",NULL);
	fastboot_publish("version-baseband", "2.0",NULL);
	#ifdef MODEL_NAME
	fastboot_publish("product", MODEL_NAME,NULL);
	#else
	fastboot_publish("product", "mt5xxx_us",NULL);
	#endif
	fastboot_publish("secure", "no",NULL);
	fastboot_publish("unlocked", "yes",NULL);
	fastboot_publish("max-download-size", "0x8000000",NULL); //128MB.
	#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
	fastboot_publish("slot-count", "2",NULL);
	fastboot_publish("slot-suffixes", "",do_getvar_slotSuffixes);
	fastboot_publish("has-slot:", "",do_getvar_hasSlot);
	fastboot_publish("current-slot", "",do_getvar_currentSlot);
	fastboot_publish("slot-successful:", "",do_getvar_slotSuccessful);
	fastboot_publish("slot-unbootable:", "",do_getvar_slotUnbootable);
	fastboot_publish("slot-retry-count:", "",do_getvar_slotRetryCount);
	#endif
    // need to add partition information.
    {
        int i = 0, i4PartNum = 0, encrypt = 0;
        int j=0;
        u64 size = 0;
        char *name = NULL;
        char buf[64] = {0};
        char *mtdparts = NULL;
        char PartitionName[64] = {0};
        char PartitionSize[64] = {0};
        char Psize[64] = {0};

        //get partition numbers.
        //DRVCUST_QueryPart(0, &i4PartNum, 0);

        while (i <= i4PartNum)
        {
            //DRVCUST_QueryPart64(i, &size, 1);
            //DRVCUST_QueryPart(i, (int *)&name, 2);

            sprintf(PartitionName, "partition-type:%s",name);
            sprintf(PartitionSize, "partition-size:%s",name);
            sprintf(Psize, "0x%08x",size);

            fastboot_publish(PartitionName, "emmc",NULL);
            fastboot_publish(PartitionSize, Psize,NULL);

            i++;
        }
    }

	fastboot_register("getvar:", do_getvar, FALSE); // flash cmd step0
	fastboot_register("reboot", do_reboot, FALSE);
	fastboot_register("reboot-bootloader", do_resetBootloader, FALSE);
	fastboot_register("flashall", do_update, FALSE);
	fastboot_register("download:", do_download, FALSE); // flash cmd step1
	fastboot_register("flash:", do_update, FALSE);  // flash cmd step2
	//fastboot_register("continue", do_continue, FALSE);    //don't support fastboot continue.
	fastboot_register("erase:", do_erase_emmc, FALSE);
	fastboot_register("flashing", do_flashing, FALSE);
	#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
	fastboot_register("set_active:", do_setActive, FALSE);
	#endif
#if 0 //JH
	in = udc_endpoint_alloc(UDC_TYPE_BULK_IN, 512);
	if (!in)
	{
		printf("fail_alloc_in .\n");
		goto fail_alloc_in;
	}
	out = udc_endpoint_alloc(UDC_TYPE_BULK_OUT, 512);
	if (!out)
	{
		printf("fail_alloc_out .\n");
		goto fail_alloc_out;
	}
	fastboot_endpoints[0] = in;
	fastboot_endpoints[1] = out;

	req = udc_request_alloc();
	if (!req)
	{
		printf("fail_alloc_req .\n");
		goto fail_alloc_req;
	}
	if (udc_register_gadget(&fastboot_gadget))
	{
		printf("fail_udc_register .\n");		
		goto fail_udc_register;
	}
	download_value_init();
	return 0;

fail_udc_register:
//jh 	udc_request_free(req);
fail_alloc_req:
//jh 	udc_endpoint_free(out);
fail_alloc_out:
//jh 	udc_endpoint_free(in);
fail_alloc_in:
	printf("exit the fastboot mode.\n");
	return -1;
#endif

#endif
    download_value_init();
	return 0;
}

#endif

