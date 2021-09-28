#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <MsApiMspi.h>
#include <drvMSPI.h>
#include <MsSysUtility.h>

// Extern SPI Bus channel num
static int channel = 0;

MSPI_ErrorNo mstar_spi_ChipSelect(MS_BOOL Enable)
{
    MSPI_ErrorNo ret = E_MSPI_OK;
    #if defined(CONFIG_MSPI_CS_GPIO)
    MDrv_MSPI_CFG_CS(Enable);
    #else
    ret =  MDrv_MasterSPI_SlaveEnable(channel, Enable);
    #endif
    return ret;
}

MSPI_ErrorNo mstar_spi_read(MS_U8 *pData, MS_U32 u32Size)
{
    MSPI_ErrorNo ret;
    MS_U32 remain_size;
    MS_U8 *pbuf = pData;
    MS_U16 tran_len, max_tran_len = 0xFF00;
    
    remain_size = u32Size;
    while (remain_size > 0)
    {
        if (remain_size >= max_tran_len)
        {
            tran_len = max_tran_len;
        } else
        {
            tran_len = remain_size;
        }
        //printf("MDrv_MasterSPI_Read\n");
        ret = MDrv_MasterSPI_Read(channel, pbuf, tran_len);
        if (ret != E_MSPI_OK)
        {
            printf("MDrv_MasterSPI_Read Error : %d\n", ret);
            return ret;
        }
        remain_size -= tran_len;
        pbuf += tran_len;
    }
    
    return E_MSPI_OK;
}
MSPI_ErrorNo mstar_spi_write(MS_U8 *pData, MS_U32 u32Size)
{
    MSPI_ErrorNo ret;
    MS_U32 remain_size;
    MS_U8 *pbuf = pData;
    MS_U16 tran_len, max_tran_len = 0xFF00;
    
    remain_size = u32Size;
    while (remain_size > 0)
    {
        if (remain_size >= max_tran_len)
        {
            tran_len = max_tran_len;
        } else
        {
            tran_len = remain_size;
        }
        //printf("MDrv_MasterSPI_Write\n");
        ret = MDrv_MasterSPI_Write(channel, pbuf, tran_len);
        if (ret != E_MSPI_OK)
        {
            printf("MDrv_MasterSPI_Write Error : %d\n", ret);
            return ret;
        }
        remain_size -= tran_len;
        pbuf += tran_len;
    }
    
    return E_MSPI_OK;
}


void mstar_spi_init(unsigned int max_hz, unsigned int mode)
{
    unsigned int i;
    MSPI_config tMSPIConfig;
    ST_DRV_MSPI_INFO stMspi_Info;

    printf("mstar_spi_init\n");
    memset(&tMSPIConfig,0,sizeof(MSPI_config));
    memset(&stMspi_Info,0,sizeof(stMspi_Info));

    // Init MSPI
    tMSPIConfig.U8BitMapofConfig[MSPI_READ_OPERATION] = MSPI_DC_CONFIG|MSPI_CLK_CONFIG|MSPI_FRAME_CONFIG;
    tMSPIConfig.U8BitMapofConfig[MSPI_WRITE_OPERATION] = MSPI_DC_CONFIG|MSPI_CLK_CONFIG|MSPI_FRAME_CONFIG;
    MDrv_MasterSPI_Init(&tMSPIConfig, channel);

    stMspi_Info.u8MspiChanel = channel;
    stMspi_Info.u32MspiClk   = max_hz;
    stMspi_Info.u8MspiMode   = mode;
    stMspi_Info.u8TB = 1;
    stMspi_Info.u8TrEnd = 1;
    stMspi_Info.u8TrStart = 1;
    stMspi_Info.u8TRW = 1;

    // Config for MSPI Write bits width
    for(i = 0; i < sizeof(stMspi_Info.u8WBitConfig); i++)
    {
        stMspi_Info.u8WBitConfig[i] = 7;
    }
    //Config for MSPI Read bits width
    for(i = 0; i < sizeof(stMspi_Info.u8RBitConfig); i++)
    {
        stMspi_Info.u8RBitConfig[i] = 7;
    }

    // Config for MSPI Chip Select Pad
    MDrv_MasterSPI_CsPadConfig(channel, 0xFF);
    MDrv_MSPI_Info_Config(&stMspi_Info);
    
}

static int chip_cs = 0;
static int chip_cs_invert = 0;

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
        unsigned int max_hz, unsigned int mode)
{
    struct spi_slave *slave;
    slave = malloc(sizeof(struct spi_slave));
    if (!slave)
    {
        return NULL;
    }
    
    #if (ENABLE_CMD_SF == 1)
    // For mooney demura external flash padmux configuration
    // Using LocalDiming MSPI0
    printf("Configure %s MSPI_%d pin\n", CONFIG_MSTAR_CHIP_NAME, bus);
    if (MDrv_MSPI_CFG_PIN(bus) != 0)
    {
        printf("Do not support configure MSPI_%d\n", bus);
        return NULL;
    }
    #endif

    channel = bus;
    slave->bus = bus;
    slave->cs = cs;
    chip_cs_invert = cs;
    mstar_spi_init(max_hz, mode);
    return slave;
}

void spi_free_slave(struct spi_slave *slave)
{
    free(slave);
}

int spi_init(void)
{
   return 0;
}

int spi_claim_bus(struct spi_slave *slave)
{
    return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
    
}

static void spi_read_write(MS_U32 byte, MS_U8 *data_out, MS_U8 *data_in)
{
    if ((data_in != NULL) && (data_out != NULL))
    {
        printf("Can not support ((data_in != NULL) && (data_out != NULL))\n");
        return;
    }
    
    if (data_out)
    {
        mstar_spi_write(data_out, byte);
    } else if (data_in)
    {
        mstar_spi_read(data_in, byte);
    }
}


// data_out  ==>  CPU --> FLASH  : write
// data_in   ==>  CPU <-- FLASH  : read
int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *data_out,
        void *data_in, unsigned long flags)
{

    switch (flags) 
    {
    case SPI_XFER_BEGIN:   // 1. CS + spi_read_write(read or write)
        spi_cs_activate(NULL);
        spi_read_write(bitlen >> 3, (MS_U8 *)data_out, (MS_U8 *)data_in);
        return 0;
    case 0:
        spi_read_write(bitlen >>3,  (MS_U8 *)data_out, (MS_U8 *)data_in);
        break;
    case SPI_XFER_END:     // 2
        spi_read_write(bitlen >> 3, (MS_U8 *)data_out, (MS_U8 *)data_in);
        spi_cs_deactivate(NULL);
        break;
    case SPI_XFER_BEGIN | SPI_XFER_END:
        spi_cs_activate(NULL);
        spi_read_write(bitlen >> 3, (MS_U8 *)data_out, (MS_U8 *)data_in);
        spi_cs_deactivate(NULL);
        break;
    default :
        break;
    }

    return 0;
}

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
    return chip_cs;
}

void spi_cs_activate(struct spi_slave *slave)
{
    chip_cs = 1;
    mstar_spi_ChipSelect(!chip_cs_invert);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
    chip_cs = 0;
    mstar_spi_ChipSelect(chip_cs_invert);
}
