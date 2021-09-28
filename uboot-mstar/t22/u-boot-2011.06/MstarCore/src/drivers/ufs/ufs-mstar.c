#include "ufshcd.h"
#include "ufshcd-pltfrm.h"
#include "unipro.h"
#include "ufshci.h"
#include "ufs.h"
#include "ufs-mstar.h"
#include <part.h>

#define RWE_RETRY_CNT	3

static struct ufs_mstar_host ufs_mstar_hosts[MAX_UFS_MSTAR_HOSTS];
static block_dev_desc_t ufs_dev_desc;
int cur_ufs_dev_num = -1;
u32 u32_ufs_init_t0, u32_ufs_init_t1;

#define UFS_MAX_READ_BLK ((SG_CHUNK_SIZE*MAX_ENTRY_SIZE)/ufs_dev_desc.blksz)

block_dev_desc_t *ufs_get_dev(int dev)
{
	return (dev == 0 && cur_ufs_dev_num == 0) ? &ufs_dev_desc : NULL;
}

unsigned long ufs_block_read(int device, unsigned long blknr, lbaint_t blkcnt, void *buffer)
{
	unsigned long start,blks, buf_addr;
	unsigned short smallblks;
	device&=0xff;

	if (cur_ufs_dev_num != 0)
	{
		printf("no ufs device found!\n");
		return(blkcnt);
	}

	buf_addr=(unsigned long)buffer;
	start=blknr;
	blks=blkcnt;

	do {
		if(blks>UFS_MAX_READ_BLK) {
			smallblks=UFS_MAX_READ_BLK;
			blks-=UFS_MAX_READ_BLK;
		}
		else {
			smallblks=(unsigned short) blks;
			blks=0;
		}

		if(ufs_read10(ufs_dev_desc.lun, (void *)buf_addr, start, smallblks)) {
			blkcnt-=blks;
			break;
		}
		buf_addr += ufs_dev_desc.blksz * smallblks;
		start += smallblks;
	} while(blks!=0);

	return(blkcnt);
}

unsigned long ufs_block_write(int device, unsigned long blknr, lbaint_t blkcnt, const void *buffer)
{
	unsigned long start,blks, buf_addr;
	unsigned short smallblks;
	device&=0xff;

	if (cur_ufs_dev_num != 0)
	{
		printf("no ufs device found!\n");
		return(blkcnt);
	}

	buf_addr=(unsigned long)buffer;
	start=blknr;
	blks=blkcnt;

	do {
		if(blks>UFS_MAX_READ_BLK) {
			smallblks=UFS_MAX_READ_BLK;
			blks-=UFS_MAX_READ_BLK;
		}
		else {
			smallblks=(unsigned short) blks;
			blks=0;
		}

		if(ufs_write10(ufs_dev_desc.lun, (void *)buf_addr, start, smallblks)) {
			blkcnt-=blks;
			break;
		}
		buf_addr += ufs_dev_desc.blksz * smallblks;
		start += smallblks;
	} while(blks!=0);

	return(blkcnt);
}

unsigned long ufs_block_erase(int device, unsigned long blknr, lbaint_t blkcnt)
{
	if (cur_ufs_dev_num != 0)
	{
		printf("no ufs device found!\n");
		return(blkcnt);
	}

	device&=0xff;

	if(ufs_unmap(ufs_dev_desc.lun, blknr, blkcnt))
		return 0;
	else
		return blkcnt;
}

void dumpMemory4(const char *str, const void *data, size_t size)
{
	print_hex_dump(KERN_ERR, str, DUMP_PREFIX_ADDRESS, 16, 4, data, size, false);
}

static inline void print_prd(struct ufshcd_lrb *lrbp)
{
	dumpMemory4("PRDT : ", lrbp->ucd_prdt_ptr,
		    sizeof(lrbp->ucd_prdt_ptr[0]) *
		    le16_to_cpu(lrbp->utr_descriptor_ptr->prd_table_length));
}

static void print_query_function(const void *upiu)
{
	const unsigned char *req_upiu = upiu;
	u8 opcode = req_upiu[12];
	u8 idn = req_upiu[13];
	u8 index = req_upiu[14];
	char *opcode_name;

	switch (opcode) {
	case UPIU_QUERY_OPCODE_READ_DESC:
		opcode_name = "read descriptor";
		break;
	case UPIU_QUERY_OPCODE_WRITE_DESC:
		opcode_name = "write descriptor";
		break;
	case UPIU_QUERY_OPCODE_READ_ATTR:
		opcode_name = "read attribute";
		break;
	case UPIU_QUERY_OPCODE_WRITE_ATTR:
		opcode_name = "write attribute";
		break;
	case UPIU_QUERY_OPCODE_READ_FLAG:
		opcode_name = "read flag";
		break;
	case UPIU_QUERY_OPCODE_SET_FLAG:
		opcode_name = "set flag";
		break;
	case UPIU_QUERY_OPCODE_CLEAR_FLAG:
		opcode_name = "clear flag";
		break;
	case UPIU_QUERY_OPCODE_TOGGLE_FLAG:
		opcode_name = "toggle flag";
		break;
	default:
		opcode_name = "unknown opcde";
		break;
	}
	pr_err("%s: IDN %02x INDEX %02x\n", opcode_name, idn, index);
}

void ufs_mstar_print_request(struct ufshcd_lrb *lrbp)
{
	struct utp_upiu_req *ucd_req_ptr = lrbp->ucd_req_ptr;
	struct utp_transfer_req_desc *req_desc = lrbp->utr_descriptor_ptr;
	u8 req = be32_to_cpu(ucd_req_ptr->header.dword_0) >> 24;

	pr_err("-------------------------------------------\n");
	pr_err("                [request]\n");
	dumpMemory4("UTRD : ", req_desc, sizeof(*req_desc));
	dumpMemory4("CMD UPIU : ", ucd_req_ptr, sizeof(*ucd_req_ptr));

	switch (req) {
	case UPIU_TRANSACTION_COMMAND:
			//scsi_print_command(ucd_req_ptr->sc.cdb);
			print_prd(lrbp);
		break;
	case UPIU_TRANSACTION_QUERY_REQ:
			print_query_function(ucd_req_ptr);
		break;
	}
}

void ufs_mstar_print_response(struct ufshcd_lrb *lrbp)
{
	struct utp_upiu_rsp *ucd_rsp_ptr = lrbp->ucd_rsp_ptr;
	struct utp_transfer_req_desc *req_desc = lrbp->utr_descriptor_ptr;
	u8 req = be32_to_cpu(lrbp->ucd_req_ptr->header.dword_0) >> 24;

	pr_err("                [response]\n");
	dumpMemory4("UTRD : ", req_desc, sizeof(*req_desc));
	dumpMemory4("RSP UPIU: ", ucd_rsp_ptr, sizeof(*ucd_rsp_ptr));

	if (req == UPIU_TRANSACTION_COMMAND)
		print_prd(lrbp);

	pr_err("-------------------------------------------\n");
}

void dump_ufshci_reg(struct ufs_hba *hba)
{
	int reg;
	unsigned int ua32_hci_reg[0xA0>>2];

	for (reg=0 ; reg<0xA0 ; reg+=4)
		ua32_hci_reg[reg>>2] = ufshcd_readl(hba, reg);

	pr_err("\n-------------- UFSHCI REGISTER --------------\n");
	print_hex_dump(KERN_ERR, "",
		DUMP_PREFIX_OFFSET, 16, 4, ua32_hci_reg, 0xA0, false);
}

static int ufs_mstar_link_startup_pre_change(struct ufs_hba *hba)
{
	//struct ufs_mstar_host *host = ufshcd_get_variant(hba);

	int err = 0;

	return err;
}

static int ufs_mstar_link_startup_post_change(struct ufs_hba *hba)
{
	//struct ufs_mstar_host *host = ufshcd_get_variant(hba);

	int err = 0;

	ufshcd_dme_set(hba, UIC_ARG_MIB(0x3000), 0);
	ufshcd_dme_set(hba, UIC_ARG_MIB(0x3001), 1);
	ufshcd_dme_set(hba, UIC_ARG_MIB(0x4025), 6);
	ufshcd_dme_set(hba, UIC_ARG_MIB(0x4021), 1);
	ufshcd_dme_set(hba, UIC_ARG_MIB(0x4020), 1);

	return err;
}

static int ufs_mstar_link_startup_notify(struct ufs_hba *hba, bool status)
{
	int err = 0;
	//struct ufs_mstar_host *host = ufshcd_get_variant(hba);

	switch (status) {
	case PRE_CHANGE:
		err = ufs_mstar_link_startup_pre_change(hba);
		break;
	case POST_CHANGE:
		err = ufs_mstar_link_startup_post_change(hba);
		break;
	default:
		dev_err((const struct device *)hba->dev, "%s: invalid status %d\n", __func__, status);
		err = -EINVAL;
		break;
	}

	return err;
}

static int ufs_mstar_hce_enable_notify(struct ufs_hba *hba, bool status)
{
	//struct ufs_mstar_host *host = ufshcd_get_variant(hba);
	int err = 0;

	switch (status) {
	case PRE_CHANGE:

		break;
	case POST_CHANGE:

		break;
	default:
		dev_err((const struct device *)hba->dev, "%s: invalid status %d\n", __func__, status);
		err = -EINVAL;
		break;
	}
	return err;
}

static int	ufs_mstar_pwr_change_notify(struct ufs_hba *hba,
			      bool stage, struct ufs_pa_layer_attr *desired,
			      struct ufs_pa_layer_attr *final)

{
	if (PRE_CHANGE == stage) {  /* before power mode change */
		memcpy(final, desired, sizeof(struct ufs_pa_layer_attr));
		if(final->pwr_rx == SLOWAUTO_MODE || final->pwr_rx == SLOW_MODE)
		{
			if(final->gear_rx > CAP_MSTAR_MAX_RX_PWM_GEAR)
				final->gear_rx = CAP_MSTAR_MAX_RX_PWM_GEAR;
		}
		else if (final->pwr_rx == FASTAUTO_MODE || final->pwr_rx == FAST_MODE)
		{
			if(final->gear_rx > CAP_MSTAR_MAX_RX_HS_GEAR)
				final->gear_rx = CAP_MSTAR_MAX_RX_HS_GEAR;
		}

		if(final->pwr_tx == SLOWAUTO_MODE || final->pwr_tx == SLOW_MODE)
		{
			if(final->gear_tx > CAP_MSTAR_MAX_TX_PWM_GEAR)
				final->gear_tx = CAP_MSTAR_MAX_TX_PWM_GEAR;
		}
		else if (final->pwr_tx == FASTAUTO_MODE || final->pwr_tx == FAST_MODE)
		{
			if(final->gear_tx > CAP_MSTAR_MAX_TX_HS_GEAR)
				final->gear_tx = CAP_MSTAR_MAX_TX_HS_GEAR;
		}
	}
	else
	{
		if(final->pwr_rx == FASTAUTO_MODE || final->pwr_rx == FAST_MODE)
		{
			ufs_mstar_pltfrm_clock(UFSHCI_SW_DEFAULT_CLK, final->gear_rx, final->hs_rate);
			dev_dbg((const struct device *)hba->dev, "HS-GEAR%d\n", final->gear_rx);
		}
		else
		{
			dev_dbg((const struct device *)hba->dev, "PWM-GEAR%d\n", final->gear_rx);
		}
	}

	return 0;
}

static int	ufs_mstar_suspend(struct ufs_hba *hba, enum ufs_pm_op op)

{
	return ufs_mstar_pltfrm_suspend();

}

static int	ufs_mstar_resume(struct ufs_hba *hba, enum ufs_pm_op op)

{
	return ufs_mstar_pltfrm_resume();
}

/**
 * ufs_mstar_advertise_quirks - advertise the known MSTAR UFS controller quirks
 * @hba: host controller instance
 *
 * MSTAR UFS host controller might have some non standard behaviours (quirks)
 * than what is specified by UFSHCI specification. Advertise all such
 * quirks to standard UFS host controller driver so standard takes them into
 * account.
 */
static void ufs_mstar_advertise_quirks(struct ufs_hba *hba)
{
	//struct ufs_mstar_host *host = ufshcd_get_variant(hba);
}


/**
 * ufs_mstar_init - bind phy with controller
 * @hba: host controller instance
 *
 * Binds PHY with controller and powers up PHY enabling clocks
 * and regulators.
 *
 * Returns -EPROBE_DEFER if binding fails, returns negative error
 * on phy power up failure and returns zero on success.
 */
static int ufs_mstar_init(struct ufs_hba *hba)
{
	int err = 0;
	struct ufs_mstar_host *host;

	err = ufs_mstar_pltfrm_init();
	if (err)
	{
		dev_err((const struct device *)hba->dev, "ufs_mstar_init() failed %d\n", err);
		return err;
	}

	host = &ufs_mstar_hosts[0];

	/* Make a two way bind between the qcom host and the hba */
	host->hba = hba;
	ufshcd_set_variant(hba, host);

	ufs_mstar_advertise_quirks(hba);

	return err;
}

/**
 * struct ufs_hba_mstar_vops - UFS MSTAR specific variant operations
 *
 * The variant operations configure the necessary controller and PHY
 * handshake during initialization.
 */
static struct ufs_hba_variant_ops ufs_hba_mstar_vops = {
	.name					= "mstar",
	.init					= ufs_mstar_init,
	.exit					= NULL,
	.clk_scale_notify		= NULL,
	.setup_clocks			= NULL,
	.hce_enable_notify		= ufs_mstar_hce_enable_notify,
	.link_startup_notify	= ufs_mstar_link_startup_notify,
	.pwr_change_notify		= ufs_mstar_pwr_change_notify,
	.suspend				= ufs_mstar_suspend,
	.resume					= ufs_mstar_resume
};

/**
 * ufs_mstar_probe - probe routine of the driver
 * @pdev: pointer to Platform device handle
 *
 * Return zero for success and non-zero for failure
 */
static int ufs_mstar_probe(struct platform_device *pdev)
{
	int err;

	/* Perform generic probe */
	err = ufshcd_pltfrm_init(pdev, &ufs_hba_mstar_vops);
	if (err)
		dev_err((const struct device *)&pdev->dev, "ufshcd_pltfrm_init() failed %d\n", err);

	return err;
}

#if 0
/**
 * ufs_qcom_remove - set driver_data of the device to NULL
 * @pdev: pointer to platform device handle
 *
 * Always returns 0
 */
static int ufs_mstar_remove(struct platform_device *pdev)
{
	struct ufs_hba *hba =  platform_get_drvdata(pdev);

	pm_runtime_get_sync(&(pdev)->dev);
	ufshcd_remove(hba);
	return 0;
}

#if defined (CONFIG_ARM64)
static const struct of_device_id ufs_mstar_of_match[] = {
	{ .compatible = "mstar-ufs"},
	{},
};
#endif

static const struct dev_pm_ops ufs_mstar_pm_ops = {
	.suspend	= ufshcd_pltfrm_suspend,
	.resume		= ufshcd_pltfrm_resume,
	.runtime_suspend = ufshcd_pltfrm_runtime_suspend,
	.runtime_resume  = ufshcd_pltfrm_runtime_resume,
	.runtime_idle    = ufshcd_pltfrm_runtime_idle,
};

static struct platform_driver ufs_mstar_pltform = {
	.probe	= ufs_mstar_probe,
	.remove	= ufs_mstar_remove,
	.shutdown = ufshcd_pltfrm_shutdown,
	.driver	= {
		.name	= "mstar-ufs",
		.pm	= &ufs_mstar_pm_ops,
		#if defined(CONFIG_ARM64)
		.of_match_table = of_match_ptr(ufs_mstar_of_match),
		#endif
	},
};
#else
static int ufs_request_sense(u32 lun)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	u8 cmd[MAX_COMMAND_SIZE];
	u8 sense_buffer[SCSI_SENSE_BUFFERSIZE];
	struct scsi_cmnd cmnd;
	struct request request;
	struct scsi_device device;
	u8 sense_data[32];

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	if(lun >= MAX_UFS_MSTAR_LUN)
	{
		dev_err(hba->dev, "%s:Invalid lun\n", __func__);
		return -EINVAL;
	}

	memset((void *) &cmnd, 0, sizeof(struct scsi_cmnd));
	memset((void *) &request, 0, sizeof(struct request));
	memset((void *) &device, 0, sizeof(struct scsi_device));
	memset((void *) &cmd[0], 0, MAX_COMMAND_SIZE);
	cmd[0] = REQUEST_SENSE;
	//cmd[1] = 0;
	//cmd[2] = 0;
	//cmd[3] = 0;
	cmd[4] = 18;
	//cmd[5] = 0;

	request.tag = 0x1F;
	device.lun = lun;
	cmnd.cmnd = cmd;
	cmnd.cmd_len = 6;
	cmnd.request = &request;
	cmnd.sense_buffer = sense_buffer;
	cmnd.device = &device;
	cmnd.sc_data_direction = DMA_FROM_DEVICE;
	cmnd.sdb.length = 32;
	err = ufs_mstar_pltfrm_map_sg(&cmnd, sense_data, cmnd.sdb.length);
	if(err)
	{
		dev_err(hba->dev, "%s:Failed maping sg err = %d\n", __func__, err);
		err = -EIO;
		goto out;
	}

	err = ufshcd_queuecommand(hba, &cmnd);
	#ifdef CONFIG_MSTAR_UFS_DEBUG
	if(err == 0)
	{
		pr_dbg("ufs_request_sense Lun %d\n", lun);
		dumpMemory4("sense data : ", sense_data, 18);
	}
	#endif

out:

	return err;
}

int ufs_init(void)
{
	int err;
	struct platform_device *pdev;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	u8 desc_buf[QUERY_DESC_UNIT_MAX_SIZE];
	int i;

	ufs_hw_timer_start();
	u32_ufs_init_t0 = u32_ufs_init_t1 = ufs_hw_timer_tick();

	pdev = kmalloc(sizeof(struct platform_device), GFP_KERNEL);
	if(!pdev)
	{
		dev_err(NULL, "%s:kmalloc failed\n", __func__);
		return -ENOMEM;
	}

	err = ufs_mstar_probe(pdev);
	if(err)
	{
		dev_err(&pdev->dev, "Failed probing ufs err = %d\n", err);
		return err;
	}

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	if(hba->bNumberLU != MAX_UFS_MSTAR_LUN)
	{
		dev_err(&pdev->dev, "Number of LU = %d\n", hba->bNumberLU);
		dev_err(&pdev->dev, "Config LU...\n");
		err = ufs_config_lun();
		if(err)
		{
			dev_err(&pdev->dev, "Failed config lun err = %d\n", err);
			return err;
		}
	}

	for(i=0 ; i<MAX_UFS_MSTAR_LUN ; i++)
	{
		err = ufshcd_read_unit_desc_param(hba, i, 0, desc_buf, QUERY_DESC_UNIT_MAX_SIZE);
		if (err)
		{
			dev_err(hba->dev, "Failed reading unit Desc.%d err = %d\n", i, err);
			return err;
		}

		host->dLogicalBlkSize[i] = 1<<desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_SIZE];
		host->qLogicalBlkCnt[i] =
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT]) << 56) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+1]) << 48) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+2]) << 40) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+3]) << 32) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+4]) << 24) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+5]) << 16) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+6]) << 8) |
			(((u64) desc_buf[UNIT_DESC_PARAM_LOGICAL_BLK_COUNT+7]) << 0);

		pr_dbg("Lun %d : Blk Size=%Xh, Blk Cnt=%llXh\n", i, host->dLogicalBlkSize[i], host->qLogicalBlkCnt[i]);

		err = ufs_request_sense(i);
		if (err)
		{
			dev_err(hba->dev, "Failed requesting sense lun %d err = %d\n", i, err);
			return err;
		}
	}

	memset(&ufs_dev_desc, 0, sizeof(block_dev_desc_t));
	ufs_dev_desc.lun = 0x0;
	ufs_dev_desc.lba = host->qLogicalBlkCnt[0];
	ufs_dev_desc.blksz = host->dLogicalBlkSize[0];
	ufs_dev_desc.type = DEV_TYPE_HARDDISK;;
	ufs_dev_desc.removable = FALSE;
	ufs_dev_desc.if_type = IF_TYPE_UFS;
	ufs_dev_desc.dev = 0;
	ufs_dev_desc.part_type = PART_TYPE_UFS;
	ufs_dev_desc.block_read = ufs_block_read;
	ufs_dev_desc.block_write = ufs_block_write;
	ufs_dev_desc.block_erase= ufs_block_erase;

	cur_ufs_dev_num = 0;
	u32_ufs_init_t1 = ufs_hw_timer_tick();

	return err;
}

static int ufs_rw6(u32 lun, void *pBuf, u32 dLba, u16 wLength, u8 bOp)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	u8 cmd[MAX_COMMAND_SIZE];
	u8 sense_buffer[SCSI_SENSE_BUFFERSIZE];
	struct scsi_cmnd cmnd;
	struct request request;
	struct scsi_device device;
	u8 RetryCnt;

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	if(lun >= MAX_UFS_MSTAR_LUN)
	{
		dev_err(hba->dev, "%s:Invalid lun\n", __func__);
		return -EINVAL;
	}

	do{
		memset((void *) &cmnd, 0, sizeof(struct scsi_cmnd));
		memset((void *) &request, 0, sizeof(struct request));
		memset((void *) &device, 0, sizeof(struct scsi_device));
		memset((void *) &cmd[0], 0, MAX_COMMAND_SIZE);
		cmd[0] = bOp;
		cmd[1] = (u8) (dLba >> 16) & 0x1f;;
		cmd[2] = (u8) (dLba >> 8) & 0xff;
		cmd[3] = (u8) (dLba) & 0xff;
		cmd[4] = (u8) wLength & 0xff;
		//cmd[5] = 0;

		request.tag = 0;
		device.lun = lun;
		cmnd.cmnd = cmd;
		cmnd.cmd_len = 6;
		cmnd.request = &request;
		cmnd.sense_buffer = sense_buffer;
		cmnd.device = &device;
		cmnd.sc_data_direction = (bOp==READ_10)?DMA_FROM_DEVICE:DMA_TO_DEVICE;
		cmnd.sdb.length = host->dLogicalBlkSize[lun] * wLength;
		err = ufs_mstar_pltfrm_map_sg(&cmnd, pBuf, cmnd.sdb.length);
		if(err)
		{
			dev_err(hba->dev, "%s:Failed maping sg err = %d\n", __func__, err);
			err = -EIO;
			goto out;
		}

		err = ufshcd_queuecommand(hba, &cmnd);
	}while(err!=0 && (RetryCnt++)<RWE_RETRY_CNT);

out:

	return err;
}

int ufs_read6(u32 lun, void *pBuf, u32 dLba, u16 wLength)
{
	int err = 0;

	#ifdef CONFIG_MSTAR_UFS_DEBUG
	pr_err("%s(%Xh, %ph, %Xh, %Xh)\n", __func__, lun, pBuf, dLba, wLength);
	#endif

	err = ufs_rw6(lun, pBuf, dLba, wLength, READ_6);
	if(err)
	{
		pr_err("Failed reading6 err = %d\n", err);
		return err;
	}

	return err;
}

int ufs_write6(u32 lun, void *pBuf, u32 dLba, u16 wLength)
{
	int err = 0;

	#ifdef CONFIG_MSTAR_UFS_DEBUG
	pr_err("%s(%Xh, %ph, %Xh, %Xh)\n", __func__, lun, pBuf, dLba, wLength);
	#endif

	err = ufs_rw6(lun, pBuf, dLba, wLength, WRITE_6);
	if(err)
	{
		pr_err("Failed writing6 err = %d\n", err);
		return err;
	}

	return err;
}

static int ufs_rw10(u32 lun, void *pBuf, u32 dLba, u16 wLength, u8 bOp)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	u8 cmd[MAX_COMMAND_SIZE];
	u8 sense_buffer[SCSI_SENSE_BUFFERSIZE];
	struct scsi_cmnd cmnd;
	struct request request;
	struct scsi_device device;
	u8 RetryCnt;

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	if(lun >= MAX_UFS_MSTAR_LUN)
	{
		dev_err(hba->dev, "%s:Invalid lun\n", __func__);
		return -EINVAL;
	}

	do{
		memset((void *) &cmnd, 0, sizeof(struct scsi_cmnd));
		memset((void *) &request, 0, sizeof(struct request));
		memset((void *) &device, 0, sizeof(struct scsi_device));
		memset((void *) &cmd[0], 0, MAX_COMMAND_SIZE);
		cmd[0] = bOp;
		//cmd[1] = 0;
		cmd[2] = (u8) (dLba >> 24) & 0xff;
		cmd[3] = (u8) (dLba >> 16) & 0xff;
		cmd[4] = (u8) (dLba >> 8) & 0xff;
		cmd[5] = (u8) (dLba) & 0xff;
		//cmd[6] = 0;
		cmd[7] = (u8) (wLength >> 8) & 0xff;
		cmd[8] = (u8) wLength & 0xff;
		//cmd[9] = 0;

		request.tag = 0;
		device.lun = lun;
		cmnd.cmnd = cmd;
		cmnd.cmd_len = 10;
		cmnd.request = &request;
		cmnd.sense_buffer = sense_buffer;
		cmnd.device = &device;
		cmnd.sc_data_direction = (bOp==READ_10)?DMA_FROM_DEVICE:DMA_TO_DEVICE;
		cmnd.sdb.length = host->dLogicalBlkSize[lun] * wLength;
		err = ufs_mstar_pltfrm_map_sg(&cmnd, pBuf, cmnd.sdb.length);
		if(err)
		{
			dev_err(hba->dev, "%s:Failed maping sg err = %d\n", __func__, err);
			err = -EIO;
			goto out;
		}

		err = ufshcd_queuecommand(hba, &cmnd);
	}while(err!=0 && (RetryCnt++)<RWE_RETRY_CNT);

out:

	return err;
}

int ufs_read10(u32 lun, void *pBuf, u32 dLba, u16 wLength)
{
	int err = 0;

	#ifdef CONFIG_MSTAR_UFS_DEBUG
	pr_err("%s(%Xh, %ph, %Xh, %Xh)\n", __func__, lun, pBuf, dLba, wLength);
	#endif

	err = ufs_rw10(lun, pBuf, dLba, wLength, READ_10);
	if(err)
	{
		pr_err("Failed reading10 err = %d\n", err);
		return err;
	}

	return err;
}

int ufs_write10(u32 lun, void *pBuf, u32 dLba, u16 wLength)
{
	int err = 0;

	#ifdef CONFIG_MSTAR_UFS_DEBUG
	pr_err("%s(%Xh, %ph, %Xh, %Xh)\n", __func__, lun, pBuf, dLba, wLength);
	#endif

	err = ufs_rw10(lun, pBuf, dLba, wLength, WRITE_10);
	if(err)
	{
		pr_err("Failed writing10 err = %d\n", err);
		return err;
	}

	return err;
}

int ufs_unmap(u32 lun, u64 qLba, u32 dLength)
{
	int err = 0;
	struct ufs_hba *hba;
	struct ufs_mstar_host *host;
	u8 cmd[MAX_COMMAND_SIZE];
	u8 sense_buffer[SCSI_SENSE_BUFFERSIZE];
	struct scsi_cmnd cmnd;
	struct request request;
	struct scsi_device device;
	u8 ParamList[128];
	u8 RetryCnt;

	#ifdef CONFIG_MSTAR_UFS_DEBUG
	pr_err("%s(%Xh, %llXh, %Xh)\n", __func__, lun, qLba, dLength);
	#endif

	hba = ufshcd_get_host();
	host = (struct ufs_mstar_host *)ufshcd_get_variant(hba);

	if(lun >= MAX_UFS_MSTAR_LUN)
	{
		dev_err(hba->dev, "%s:Invalid lun\n", __func__);
		return -EINVAL;
	}

	do{
		memset((void *) &cmnd, 0, sizeof(struct scsi_cmnd));
		memset((void *) &request, 0, sizeof(struct request));
		memset((void *) &device, 0, sizeof(struct scsi_device));
		memset((void *) &cmd[0], 0, MAX_COMMAND_SIZE);
		cmd[0] = UNMAP;
		//cmd[1] = 0;
		//cmd[2] = 0;
		//cmd[3] = 0;
		//cmd[4] = 0;
		//cmd[5] = 0;
		//cmd[6] = 0;
		//cmd[7] = 0;
		cmd[8] = 24;
		//cmd[9] = 0;

		request.tag = 0;
		device.lun = lun;
		cmnd.cmnd = cmd;
		cmnd.cmd_len = 10;
		cmnd.request = &request;
		cmnd.sense_buffer = sense_buffer;
		cmnd.device = &device;
		cmnd.sc_data_direction = DMA_TO_DEVICE;
		cmnd.sdb.length = 24;
		ParamList[0] = ((24-1)>>8)&0xFF;
		ParamList[1] = (24-1)&0xFF;
		ParamList[2] = ((24-7)>>8)&0xFF;
		ParamList[3] = (24-7)&0xFF;
		ParamList[4] = 0;
		ParamList[5] = 0;
		ParamList[6] = 0;
		ParamList[7] = 0;
		ParamList[8] = (qLba>>56)&0xFF;
		ParamList[9] = (qLba>>48)&0xFF;
		ParamList[10] = (qLba>>40)&0xFF;
		ParamList[11] = (qLba>>32)&0xFF;
		ParamList[12] = (qLba>>24)&0xFF;
		ParamList[13] = (qLba>>16)&0xFF;
		ParamList[14] = (qLba>>8)&0xFF;
		ParamList[15] = (qLba)&0xFF;
		ParamList[16] = (dLength>>24)&0xFF;
		ParamList[17] = (dLength>>16)&0xFF;
		ParamList[18] = (dLength>>8)&0xFF;
		ParamList[19] = (dLength)&0xFF;
		ParamList[20] = 0;
		ParamList[21] = 0;
		ParamList[22] = 0;
		ParamList[23] = 0;

		err = ufs_mstar_pltfrm_map_sg(&cmnd, ParamList, cmnd.sdb.length);
		if(err)
		{
			dev_err(hba->dev, "%s:Failed maping sg err = %d\n", __func__, err);
			err = -EIO;
			goto out;
		}

		err = ufshcd_queuecommand(hba, &cmnd);
	}while(err!=0 && (RetryCnt++)<RWE_RETRY_CNT);

out:

	return err;
}

int ufs_config_lun(void)
{
	int err;
	u8 desc_buf[QUERY_DESC_CONFIGURAION_MAX_SIZE];
	struct ufs_hba *hba;
	u64 qTotalRawDeviceCap = 0;
	u32 dSegmentSize = 0;
	u8 bAllocationUnitSize = 0;
	u8 bMinAddrBlkSize = 0;
	u32 dOptimalLogiBlkSize = 0;
	u32 dNumAllocUnits;
	u64 qAllocUnitSize;
	u32 dBootLunEn;
	u32 dConfigDescLock;
	int i, n=0;

	hba = ufshcd_get_host();
	if(!hba)
	{
		dev_err(hba->dev, "ufshcd_get_host fail\n");
		return ENXIO;
	}
	
	err = ufshcd_read_device_desc(hba, desc_buf, QUERY_DESC_DEVICE_MAX_SIZE);
	if (err) {
		dev_err(hba->dev, "Failed reading Device Desc. err = %d\n", err);
		return err;
	}
	pr_dbg("dump device desc : \n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_DEVICE_MAX_SIZE, false);

	err = ufshcd_query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR,
			0x0B, 0, 0, &dConfigDescLock);
	if (err) 
	{
		dev_err(hba->dev, "Failed reading attr. 0x0B err = %d\n", err);
		return err;
	}
	pr_dbg("dConfigDescLock : %d\n", dConfigDescLock);

	err = ufshcd_read_geometry_desc(hba, desc_buf, QUERY_DESC_GEOMETRY_MAX_SIZE);
	if (err) 
	{
		dev_err(hba->dev, "Failed reading geometry Desc. err = %d\n", err);
		return err;
	}
	pr_dbg("dump geometry desc : \n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_GEOMETRY_MAX_SIZE, false);

	qTotalRawDeviceCap =
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY]) << 56) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+1]) << 48) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+2]) << 40) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+3]) << 32) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+4]) << 24) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+5]) << 16) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+6]) << 8) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_CAPACITY+7]) << 0);
			
	dSegmentSize =
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_SEGMENT_SIZE]) << 24) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_SEGMENT_SIZE+1]) << 16) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_SEGMENT_SIZE+2]) << 8) |
		(((u64) desc_buf[GEOMETRY_DESC_PARAM_SEGMENT_SIZE+3]) << 0);

	bAllocationUnitSize = desc_buf[GEOMETRY_DESC_PARAM_ALLOC_UNIT_SIZE];
	bMinAddrBlkSize = desc_buf[GEOMETRY_DESC_PARAM_MIN_ADDR_BLK_SIZE];
	#if 0
	dOptimalLogiBlkSize = desc_buf[GEOMETRY_DESC_PARAM_OPTIMAL_LOGI_BLK_SIZE+3]&0xF;
	dOptimalLogiBlkSize = (1<<dOptimalLogiBlkSize)*bMinAddrBlkSize*512;
	#else
	dOptimalLogiBlkSize = 4096;
	#endif
	pr_dbg("qTotalRawDeviceCap : %llXh\n", qTotalRawDeviceCap);
	pr_dbg("dSegmentSize : %Xh\n", dSegmentSize);
	pr_dbg("bAllocationUnitSize : %Xh\n", bAllocationUnitSize);
	pr_dbg("bMinAddrBlkSize : %Xh\n", bMinAddrBlkSize);
	pr_dbg("dOptimalLogiBlkSize : %Xh\n", dOptimalLogiBlkSize);

	memset(desc_buf, 0, QUERY_DESC_CONFIGURAION_MAX_SIZE);
	//Configuration Descr Params
	desc_buf[CONFIG_DESC_PARAM_LEN] = QUERY_DESC_CONFIGURAION_MAX_SIZE;
	desc_buf[CONFIG_DESC_PARAM_TYPE] = QUERY_DESC_IDN_CONFIGURAION;
	desc_buf[CONFIG_DESC_PARAM_CONTINUE] = 0;
	desc_buf[CONFIG_DESC_PARAM_BOOT_ENABLE] = 1;
	desc_buf[CONFIG_DESC_PARAM_DESCR_ACCESS_EN] = 1;
	desc_buf[CONFIG_DESC_PARAM_INIT_PWR_MODE] = 1;
	desc_buf[CONFIG_DESC_PARAM_HIGH_PRIO_LUN] = 0x7F;
	desc_buf[CONFIG_DESC_PARAM_SECURE_RM_TYPE] = 0;
	desc_buf[CONFIG_DESC_PARAM_INIT_ACTIVE_ICC] = 0;
	desc_buf[CONFIG_DESC_PARAM_PERIOD_RTC_UPD] = 0;
	//Unit Descr Config Params
	qAllocUnitSize = (u64)dSegmentSize*(u64)512*(u64)bAllocationUnitSize;
	while(n<256)
	{
		if((dOptimalLogiBlkSize>>n)&1)
			break;

		n++;
	}
	if(n==256)
	{
		dev_err(hba->dev, "Failed calculating logi block size\n");
		return EINVAL;
	}

	for(i=0 ; i<MAX_UFS_MSTAR_LUN ; i++)
	{
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_LU_ENABLE] = 1;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_BOOT_LUN_ID] = i;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_LU_WR_PROTECT] = 0;

		if(i)
		{
			desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_MEM_TYPE] = 3; // Enhanced memory type 1
			//Assign 1 Allocation Units to each Boot Lun 
			dNumAllocUnits = 1;
			pr_dbg("Boot Lun %d Size : %llXh\n", i, qAllocUnitSize);
		}
		else
		{
			desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_MEM_TYPE] = 0; // normal memory
			dNumAllocUnits = ((qTotalRawDeviceCap*512)-(2*qAllocUnitSize))/qAllocUnitSize;
			pr_dbg("Lun %d Size : %Xh\n", i, dNumAllocUnits);
		}
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_NUM_ALLOC_UNITS] = (dNumAllocUnits>>24)&0xFF;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_NUM_ALLOC_UNITS+1] = (dNumAllocUnits>>16)&0xFF;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_NUM_ALLOC_UNITS+2] = (dNumAllocUnits>>8)&0xFF;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_NUM_ALLOC_UNITS+3] = (dNumAllocUnits)&0xFF;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_DATA_RELIABILITY] = 1;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_LOGICAL_BLK_SIZE] = n;
		desc_buf[0x10+0x10*i+UNIT_DESC_CONFIG_PARAM_PROVISIONING_TYPE] = 3;
	}

	pr_dbg("dump config desc : \n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_CONFIGURAION_MAX_SIZE, false);
	err = ufshcd_write_configuration_desc(hba, 0, desc_buf, QUERY_DESC_CONFIGURAION_MAX_SIZE);
	if (err) 
	{
		dev_err(hba->dev, "Failed writing configuration Desc. err = %d\n", err);
		return err;
	}

	dBootLunEn = 1;
	err = ufshcd_query_attr(hba, UPIU_QUERY_OPCODE_WRITE_ATTR, 0, 0, 0, &dBootLunEn);
	if (err)
	{
		dev_err(hba->dev, "Failed configuring bBootLunEn = %d err = %d", dBootLunEn , err);
		return err;
	}

	err = ufshcd_read_device_desc(hba, desc_buf, QUERY_DESC_DEVICE_MAX_SIZE);
	if (err) 
	{
		dev_err(hba->dev, "Failed reading Device Desc. err = %d\n", err);
		return err;
	}
	pr_dbg("dump device desc : \n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
		QUERY_DESC_DEVICE_MAX_SIZE, false);

	for(i=0 ; i<MAX_UFS_MSTAR_LUN ; i++)
	{
		err = ufshcd_read_unit_desc_param(hba, i, 0, desc_buf, QUERY_DESC_UNIT_MAX_SIZE);
		if (err) 
		{
			dev_err(hba->dev, "Failed reading unit Desc. err = %d\n", err);
			return err;
		}
		pr_dbg("dump unit desc %d : \n", i);
		print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4, desc_buf, 
			QUERY_DESC_UNIT_MAX_SIZE, false);
	}

	pr_dbg("Activate the setting of descriptors by a POWER CYCLE\n");
	while(1);

	return err;
}
#endif

void ufs_count_down(u32 u32_Sec)
{
	u32 u32_i;

	pr_dbg("\n");
	for(u32_i=0; u32_i<u32_Sec; u32_i++)
	{
		pr_dbg("%u ", u32_Sec-u32_i);

		udelay(1000000);
	}
	pr_dbg("\n");
}

module_platform_driver(ufs_mstar_pltform);
