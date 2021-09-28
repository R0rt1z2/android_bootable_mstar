#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <command.h>
#include <net.h>
#include <image-sparse.h>
#include <fb_mmc.h>
#include <MsMmc.h>
#include <MsRawIO.h>
#include <mmc.h>
#include <MsUtility.h>
#include <MsStart.h>
#include <linux/ctype.h>
#include <download_command.h>
#include <cmd_fastboot.h>

#if defined(CONFIG_ANDROID_AVB_ENABLE)
#include "avb_ops_uboot.h"
#endif

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

#ifdef UFBL_FEATURE_SECURE_BOOT
#include <amzn_secure_boot.h>
#endif

#ifdef UFBL_FEATURE_UNLOCK
#include <amzn_unlock.h>
#endif

#ifdef UFBL_FEATURE_SECURE_FLASHING
#include <amzn_secure_flashing.h>
#endif

#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
#include <amzn_onetime_unlock.h>
#endif

static unsigned short kProtocolVersion=1;

unsigned short net_fastboot_running=0;
static unsigned short led_status=1;

#if defined(CONFIG_FASTBOOT_GOOGLE)
#define kHeaderSize 4
#else
// fastboot udp packet header for amazon
#define kHeaderSize 8
#endif

// download status
static fb_download_t fb_download;
// for download progress
static int pkt_count = 0;

//// ip & port
static IPaddr_t fb_remote_ip;
static int      fb_remote_port;
static int      fb_udp_our_port = kDefaultPort;

#define PARTITION_LEN     (64)
#define RESPONSE_STR_LEN  (64)
#define COMMAND_STR_LEN   (128)

struct fastboot_var {
    struct fastboot_var *next;
    char name[PARTITION_LEN];
    char value[PARTITION_LEN];
    int (*handle)(const char *arg,const char *data,int data_length);
};

// current status
fb_status_t fb_status;

// buffer for sending response back
char fb_packet[FASTBOOT_RESPONSE_LEN];
unsigned last_resp_len = 0;

// expected next packet sequence
unsigned short fb_expected_seq = 0;

// RAM address to hold the downloaded image
U32 download_addr = 0x20200000;

// for reboot command
int fb_need_reset = 0;

// for continue command
int fb_need_continue = 0;

// for invalid sboot case
static int fb_need_halt = 0;

#if defined(CONFIG_FASTBOOT_GOOGLE)
// receive flag bitmap
#define RECV_BITMAP_SIZE (32*1024) //can support 16*1024*8 packet
#else
#define RECV_BITMAP_SIZE (16*1024) //can support 16*1024*8 packet
#endif

#if defined(CONFIG_FASTBOOT_GOOGLE)
#define INVALID_NUM   (0x00000000)
#else
#define INVALID_NUM   (0xFFFFFFFF)
#endif

static unsigned short kBarrierNum = 1000;
static unsigned char *receive_bitmap = NULL;

// MAX_DEFINED_SIZE is 200MB, MAX_DOWNLOAD_SIZE needs to less than MAX_DEFINED_SIZE
// We use MAX_DEFINED_SIZE to get download address
// and use MAX_DOWNLOAD_SIZE to control fastboot.exe transmission size
#define MAX_DEFINED_SIZE  (200*1024*1024)  // 200M
#define MAX_DOWNLOAD_SIZE (161*1024*1024)  // 161M
#define DOWNLOAD_RESERVED (16*1024) //16k

#if defined(CONFIG_FASTBOOT_GOOGLE)
// payload size 1024+4
#define PAYLOAD_SIZE 1028
#else
#define PAYLOAD_SIZE 1400
#endif

// sparse image
struct fb_mmc_sparse {
    struct blk_desc *dev_desc;
};

U32 g_packet_num = 0;

U32 g_first_data_packet_received = 0;
U32 g_first_data_packet_seq = 0;
U32 g_chunking_packet_seq = 0;
U32 g_index_count = 0;

static const unsigned char wipedata_bin[] = {
    0x62, 0x6f, 0x6f, 0x74, 0x2d, 0x72, 0x65, 0x63, 0x6f, 0x76, 0x65, 0x72,
    0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x72, 0x65, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79,
    0x0a, 0x2d, 0x2d, 0x77, 0x69, 0x70, 0x65, 0x5f, 0x64, 0x61, 0x74, 0x61,
    0x0a
};

static unsigned short max_packet_len = PAYLOAD_SIZE-kHeaderSize;

U_BOOT_CMD(
    continue, 1, 0, MstarToKernel,
    "continue to boot",
    ""
);

extern int amzn_device_is_unlocked(void);

#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
int cb_setActive(const char *data, unsigned int data_length);
int do_getvar_slotSuffixes(const char *arg, const char *data, unsigned int data_length);
int do_getvar_hasSlot(const char *arg, const char *data, unsigned int data_length);
int do_getvar_currentSlot(const char *arg, const char *data, unsigned int data_length);
int do_getvar_slotSuccessful(const char *arg, const char *data, unsigned int data_length);
int do_getvar_slotUnbootable(const char *arg, const char *data, unsigned int data_length);
int do_getvar_slotRetryCount(const char *arg, const char *data, unsigned int data_length);
#endif

struct fastboot_var *varlist;
#ifdef CONFIG_USB_FASTBOOT_ENABLE
static void fastboot_publish(const char *name, const char *value, void (*handle)(const char,const char,int))
{
}
#else
void fastboot_publish(const char *name, const char *value, void (*handle)(const char,const char,int))
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
#endif

// default partition size
#define DEFAULT_PARTITION_SIZE (10*1024*1024) // 10M
static int curr_mmc_dev = -1;

//////////////// Common Utilities ////////////////

// utility to check if s2 starts with s1
static int strcmp_l1(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return -1;
    return strncmp(s1, s2, strlen(s1));
}

// Extracts a big-endian uint16_t from a byte array.
unsigned short extract_uint16(const uchar* bytes)
{
    return ((unsigned short)(bytes[0]) << 8) | bytes[1];
}

// Put a big-endian uint16_t to a byte array.
void put_uint16(uchar* bytes, unsigned short value)
{
    bytes[0] = value >> 8;
    bytes[1] = value;
}

// Extracts a big-endian uint32_t from a byte array.
U32 extract_uint32(const uchar* bytes)
{
    return ((U32)(bytes[0]) << 24) | ((U32)(bytes[1]) << 16) |
           ((U32)(bytes[2]) << 8) | bytes[3];
}

// Put a big-endian uint32_t to a byte array.
void put_uint32(uchar* bytes, U32 value)
{
    bytes[0] = value >> 24;
    bytes[1] = value >> 16;
    bytes[2] = value >> 8;
    bytes[3] = value;
}

void header_set(uchar* header, uchar id, unsigned short sequence,
                Flag flag, U32 packetnum)
{
    header[kIndexId] = id;
    header[kIndexFlags] = flag;
    header[kIndexSeqH] = sequence >> 8;
    header[kIndexSeqL] = sequence;
    header[kIndexNumHH] = packetnum >> 24;
    header[kIndexNumHL] = packetnum >> 16;
    header[kIndexNumLH] = packetnum >> 8;
    header[kIndexNumLL] = packetnum;
}

void set_receive_bitmap(int index)
{
   receive_bitmap[index/8] |= 1 << index % 8;
}

int get_receive_bitmap(int index)
{
   if ((receive_bitmap[index/8] & (1 << index%8)) != 0)  {
      return 1;
   } else {
      return 0;
   }
}

int first_unset_bit(void)
{
   int index = 0;
   int bit_index = 0;
   while ((index < RECV_BITMAP_SIZE)&&(receive_bitmap[index] == 0xFF)) {
      index++;
   }
   if (index < RECV_BITMAP_SIZE) {
      while ((bit_index < 8) &&
             ((receive_bitmap[index] & (1 << bit_index)) != 0)) {
         bit_index ++;
      }
   }
   return index * 8 + bit_index;
}

extern char *get_script_next_line(char **line_buf_ptr);

int util_run_script(char* buffer, unsigned buffer_len)
{
    char* script_buf = 0;
    char* next_line = 0;
    int BeAbort = 1;
    char* tmp = 0;

    // Set commmand abort or not when cmd error.
    tmp = getenv ("CmdAbort");
    if (tmp != NULL)
    {
        BeAbort = (int)simple_strtol(tmp, NULL, 10);
        printf("BeAbort=%d\n",BeAbort);
    }

    script_buf = buffer;
    script_buf[buffer_len]='\n'; script_buf[buffer_len+1]='%';
    while ((next_line = get_script_next_line(&script_buf)) != 0)
    {
        printf("\n>> %s \n", next_line);
        if(-1 == run_command(next_line, 0))
        {
            printf("command error!\n");
            if(BeAbort == 1)
            {
               return -1;
            }
        }
    }
    return 0;
}

static void get_download_addr(void)
{
    // Just for initialize, these values may be change after calculate
    U32 download_start_address = 0x20200000;
    U32 download_end_address = 0x20200000;

    const U32 MIU_OFFSET = 0x20000000;
    U32 mboot_address = (CONFIG_DRAM_SIZE_MB_RELOCATE*1024*1024) + MIU_OFFSET;

    // check if the mboot address < 256MB, ex. 137MB
    // use 256MB~512MB
    if( mboot_address < (MAX_DEFINED_SIZE + MIU_OFFSET) )
    {
        download_start_address = 0x10000000+MIU_OFFSET;
        download_end_address = download_start_address + MAX_DOWNLOAD_SIZE - 1;
    }
    else
    {
        // if the mboot address > 256MB, ex. 305MB
        // use 2MB~256MB
        download_start_address = 0x200000+MIU_OFFSET;
        download_end_address = download_start_address + MAX_DOWNLOAD_SIZE - 1;
    }

    // set address into global download_addr var.
    download_addr = download_start_address;
    download_addr = 0x20200000; // Luke modify

    // USB commands are at 0x20200000
    if(net_fastboot_running != 1)
        download_addr = 0x20200000 + 0x10000; // from download_value_init()
}

U64 get_partition_size(char* partition_name)
{
   U64 retVal = 0;
   block_dev_desc_t *mmc_dev=NULL;
   struct mmc *mmc=NULL;
   ulong partnum=1;
   disk_partition_t dpt;

   if (curr_mmc_dev < 0) {
      if (get_mmc_num() > 0)
         curr_mmc_dev = 0;
      else {
         printf("No MMC device available\n");
         return retVal;
      }
   }

   mmc = find_mmc_device(curr_mmc_dev);
   if (!mmc) {
      printf("No MMC devices available\n");
      return retVal;
   }

   mmc_init(mmc);
   mmc_dev = mmc_get_dev(curr_mmc_dev);
   if ((mmc_dev == NULL) ||
       (mmc_dev->type == DEV_TYPE_UNKNOWN)) {
      printf("No MMC device found!\n");
      return retVal;
   }

   for(;;) {
      if(get_partition_info_emmc(mmc_dev, partnum, &dpt)) {
         printf("Error >> end searching partition\n");
         return retVal;
      }

      if(!strcmp_l1(partition_name, (const char *)dpt.name))
      {
         retVal = (U64)dpt.size * (U64)dpt.blksz;
         break;
      }
      partnum++;
   }
   return retVal;
}

//////////////// fastboot functions ////////////////

static void fb_init_resp(void)
{
    fb_status.status_code = kStatusOkay;
    memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);
    fb_status.info_index = 0;
    fb_status.info_num = 0;
    memset(fb_status.info_msg, 0, MAX_INFO_NUM*FASTBOOT_RESPONSE_LEN);
}

void fb_init(unsigned short net)
{
    fb_download_flag = 0;
    fb_download.total = 0;
    fb_download.received = 0;
    fb_download.max_num = 0;
    fb_download.last_size = 0;
    fb_need_reset = 0;
    fb_need_halt  = 0;
    fb_init_resp();
    net_fastboot_running = net;


    get_download_addr();

    //Client Variables for getvar:%s
    fastboot_publish("version", FASTBOOT_VERSION, NULL);
    fastboot_publish("serialno", kSerialNo, NULL);
    fastboot_publish("version-bootloader", "1.1", NULL);
    fastboot_publish("version-baseband", "2.0",NULL);
    #ifdef MODEL_NAME
    fastboot_publish("product", MODEL_NAME, NULL);
    #else
       #ifdef UFBL_FEATURE_IDME
            fastboot_publish("product", amzn_target_device_name(), NULL);
            strcpy(fb_status.message, amzn_target_device_name());
       #else
            fastboot_publish("product", kProduct, NULL);
       #endif
    #endif
    fastboot_publish("secure", "no", NULL);
    fastboot_publish("unlocked", "yes", NULL);
    fastboot_publish("max-download-size", "0x6400000",NULL); //100MB.
    #ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
    fastboot_publish("slot-count", "2",NULL);
    fastboot_publish("slot-suffixes", "",do_getvar_slotSuffixes);
    fastboot_publish("has-slot:", "",do_getvar_hasSlot);
    fastboot_publish("current-slot", "",do_getvar_currentSlot);
    fastboot_publish("slot-successful:", "",do_getvar_slotSuccessful);
    fastboot_publish("slot-unbootable:", "",do_getvar_slotUnbootable);
    fastboot_publish("slot-retry-count:", "",do_getvar_slotRetryCount);
    #endif
}

static void fb_set_resp(StatusCode status_code, const char* message)
{
    if (status_code == kStatusInfo) {
       // ignore if there is no space
       if (fb_status.info_num < MAX_INFO_NUM) {
          strncpy(fb_status.info_msg[fb_status.info_num], message, FASTBOOT_RESPONSE_LEN-1);
          fb_status.info_num++;
       }
    } else {
       fb_status.status_code = status_code;
       strncpy(fb_status.message, message, FASTBOOT_RESPONSE_LEN-1);
    }
}

// utility to generate response based on fb_status to generate the response
void fb_gen_resp(char *response)
{
    if (fb_status.info_index == fb_status.info_num) {
       switch (fb_status.status_code) {
           case kStatusError:
           strncpy(response, "FAIL\0", 5);
           break;
           case kStatusData:
           strncpy(response, "DATA\0", 5);
           break;
           default:
           strncpy(response, "OKAY\0", 5);
           break;
       }
       strncat(response, fb_status.message, FASTBOOT_RESPONSE_LEN - 4 - 1);
   } else { // has INFO message
     strncpy(response, "INFO\0", 5);
     strncat(response, fb_status.info_msg[fb_status.info_index], FASTBOOT_RESPONSE_LEN - 4 - 1);
     fb_status.info_index++;
   }
}

void fb_send(char* buffer, unsigned buffer_len)
{
    memcpy((void*)(NetTxPacket + NetEthHdrSize() + IP_HDR_SIZE), buffer, buffer_len);

    // have to use the remote port that we get from udp handler, it is changing
    NetSendUDPPacket(NetServerEther, fb_remote_ip, fb_remote_port,
                    fb_udp_our_port, buffer_len);
}

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B
static void EraseUserData(void)
{
    int ret1 = 0;
    unsigned int sz = sizeof(wipedata_bin) / sizeof(wipedata_bin[0]);

    raw_io_config_push();
    ret1 = raw_io_config(FLASH_RAW_DATA_TARGET, "misc", UNNECESSARY_VOLUME);

    if(ret1 != 0)
    {
        printf("raw_io_config setting fail!\n");
    }

    printf("begin erase userdata.\n");
    ret1 = raw_write((const char *)wipedata_bin, 0, sz);
    if(ret1 != 0)
    {
        printf("raw_read misc fail!\n");
    }

    raw_io_config_pop();
    run_command("mmc erase.p userdata", 0);
    printf("end erase userdata.\n");
}
#endif //CONFIG_ANDROID_SECURE_BOOT_CLASS_B

#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
int cb_setActive(const char *data, unsigned int data_length)
{
    unsigned int slotNumber = 0;

    if(*data != 'a' && *data != 'b')
    {
        strcpy(fb_status.message, "Unsupported slot!!");
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data -'a';

    printf("%s:%d:%s: fastboot set active of slot number %d.\n",__FILE__,__LINE__,__FUNCTION__, slotNumber);
    if(ab_setActiveSlot(slotNumber) == 0) //0 means set OK.
    {
        strcpy(fb_status.message, "Done!!");
        fastboot_okay("Done!!");
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotSuffixes(const char *arg, const char *data, unsigned int data_length)
{
    char slotSuffix[4] = {0};

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    if(ab_appendSlotSuffix(slotSuffix,""))
    {
        strcpy(fb_status.message, slotSuffix);
        fastboot_okay(slotSuffix);
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_hasSlot(const char *arg, const char *data, unsigned int data_length)
{
    char partition_a[PARTITION_LEN] = {0};
    char partition_b[PARTITION_LEN] = {0};
    int pnum;
    struct mtd_device *dev;
    struct part_info *part;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    // here PARTITION_LEN - 3 is to reserve 2 bytes for "_a" or "_b" and
    // 1 byte for null termination
    if (data_length >= PARTITION_LEN - 3)
    {
       memcpy(partition_a, data, PARTITION_LEN - 3);
       memcpy(partition_b, data, PARTITION_LEN - 3);
    }
    else
    {
       memcpy(partition_a, data, data_length);
       memcpy(partition_b, data, data_length);
    }
    strcat(partition_a, "_a");
    strcat(partition_b, "_b");

    if(avb_get_part_number(partition_a ,&pnum) == 0 && avb_get_part_number(partition_b ,&pnum) == 0)
    {
      strcpy(fb_status.message, "yes");
      fastboot_okay("yes");
    }
    else
    {
      strcpy(fb_status.message, "no");
      fastboot_okay("no");
    }
    return 0;
}

int do_getvar_currentSlot(const char *arg, const char *data, unsigned int data_length)
{
    char str[32] = {0};
    int currentSlot = 0;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    currentSlot = ab_getCurrentSlotIndex();
    if(currentSlot >= 0)
    {
        sprintf(str, "%c", currentSlot+'a');
        strcpy(fb_status.message, str);
        fastboot_okay(str);
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotSuccessful(const char *arg, const char *data, unsigned int data_length)
{
    int slotNumber = 0, isSuccessful = -1;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    if(*data != 'a' && *data != 'b')
    {
        strcpy(fb_status.message, "Unsupported slot!!");
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    isSuccessful = ab_getSlotSuccessful(slotNumber);
    if(isSuccessful >= 0)
    {
        if(isSuccessful)
        {
            strcpy(fb_status.message, "yes");
            fastboot_okay("yes");
        }
        else
        {
            strcpy(fb_status.message, "no");
            fastboot_okay("no");
        }
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotUnbootable(const char *arg, const char *data, unsigned int data_length)
{
    int slotNumber = 0, isUnbootable = -1;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    if(*data != 'a' && *data != 'b')
    {
        strcpy(fb_status.message, "Unsupported slot!!");
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    isUnbootable = ab_getSlotUnbootable(slotNumber);
    if(isUnbootable >= 0)
    {
        if(isUnbootable)
        {
            strcpy(fb_status.message, "yes");
            fastboot_okay("yes");
        }
        else
        {
            strcpy(fb_status.message, "no");
            fastboot_okay("no");
        }
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}

int do_getvar_slotRetryCount(const char *arg, const char *data, unsigned int data_length)
{
    char str[32] = {0};
    int retryCount = 0;
    int slotNumber = 0;

    if(!strcmp(data, "all"))
    {
        printf("%s:%d:%s: Unsupport parameter: all\n",__FILE__,__LINE__,__FUNCTION__);
        strcpy(fb_status.message, "Unsupport parameter: all");
        return -1;
    }

    if(*data != 'a' && *data != 'b')
    {
        strcpy(fb_status.message, "Unsupported slot!!");
        fastboot_fail("Unsupported slot!!");
        return -1;
    }

    slotNumber = *data - 'a';
    retryCount = ab_getSlotRetryCount(slotNumber);
    if(retryCount >= 0)
    {
        sprintf(str, "%d", retryCount);
        strcpy(fb_status.message, str);
        fastboot_okay(str);
        return 0;
    }
    else
    {
        strcpy(fb_status.message, "Unknown error!!");
        fastboot_fail("Unknown error!!");
        return -1;
    }
}
#endif // CONFIG_ANDROID_SUPPORT_AB_UPDATE

#if defined(CONFIG_FASTBOOT_GOOGLE)
static int critical_lock = 1;

static void TrimLeft(char *pDest, const char *pSrc, int len)
{
    int i = 0;
    const char *t = pSrc;
    while (i < len && (*(t++) == ' '));
    strcpy(pDest, t - 1);
}

// fastboot functions
// send an ACK, for all fastboot packet, an ACK is needed
static void fb_udp_ack(uchar *pkt, unsigned len, U32 packet_num)
{
    memcpy(fb_packet, pkt, kHeaderSize);

    put_uint16((uchar*)fb_packet+kIndexSeqH, packet_num);

    // need to clear continuation flag
    fb_packet[kIndexFlags] = kFlagNone;
    fb_send(fb_packet, kHeaderSize);
}

// send an error packet
static void fb_udp_error(void)
{
    header_set((uchar*)fb_packet, 0, 0, 0, INVALID_NUM);
    strcpy(fb_packet+kHeaderSize, "packet Id is invalid");
    fb_send(fb_packet, kHeaderSize+strlen(fb_packet+kHeaderSize));
}

// reboot command handler
static void cb_reboot(char *buffer, unsigned buffer_len)
{
    fb_need_reset = 1;
}

// reboot-bootloader command handler
static void cb_reboot_bootloader(char *buffer, unsigned buffer_len)
{
    run_command("setenv reboot-bootloader 1", 0);
    run_command("saveenv", 0);
    fb_need_reset = 1;
}

// getvar command handler
int cb_getvar(char *buffer, unsigned buffer_len)
{
  struct fastboot_var *var;
  char response[RESPONSE_STR_LEN];
  printf("cb_getvar, var[%s].\n", buffer);
  int ret = 0;

  // clear the status
  fb_status.status_code = kStatusOkay;
  memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);

  if(!strcmp(buffer, "all")){
    for (var = varlist; var; var = var->next){
      if(var->handle)
      {
        ret = var->handle(var->name, buffer, buffer_len);
      }
      else
      {
        snprintf(response, sizeof(response),"\t%s: %s", var->name, var->value);
        //fastboot_info(response);
        strcpy(fb_status.message, response);
      }
    }

    fb_udp_ack(buffer, buffer_len, g_packet_num);
    fastboot_okay("Done!!");
    return 0;
  }

  for (var = varlist; var; var = var->next) {
    if (!strncmp(var->name, buffer,strlen(var->name))) {
      if(var->handle)
      {
        ret = var->handle(var->name, buffer + strlen(var->name), buffer_len - strlen(var->name));
        if(ret != 0)
          fb_status.status_code = kStatusError;
      }
      else
      {
        strcpy(fb_status.message, var->value);
        fastboot_okay(var->value);
      }
      fb_udp_ack(buffer, buffer_len, g_packet_num);
      return 0;
    }
  }

  fb_status.status_code = kStatusError;
  printf("invalid var[%s].\n", buffer);
  strcpy(fb_status.message, "Variable not specified");
  fastboot_fail("invalid var");
  fb_udp_ack(buffer, buffer_len, g_packet_num);
  return -1;
}

int is_device_unlocked()
{
    // Assume default state is lock state
    uint16_t devicestate = 1;

    read_device_state(&devicestate);

    if( devicestate == 1 )
        return 0;
    else if( devicestate == 0 )
        return 1;
    else
    {
        // unknown device state, return lock state
        printf("unknown device state: %d\n", devicestate);
        return 0;
    }
}

static void cb_download_start(char *buffer, unsigned buffer_len)
{
    // simple_strtoul takes the last valid chars, not the first
    U32 download_size = simple_strtoul(buffer, 0, 16);

    printf("Starting download of %d bytes\n", download_size);

    // init download progress count
    pkt_count = 0;

    if (0 == download_size)
    {
       fb_status.status_code = kStatusError;
       strcpy(fb_status.message, "data invalid size");
    } else if (download_size > MAX_DOWNLOAD_SIZE) {
       download_size = 0;
       fb_status.status_code = kStatusError;
       strcpy(fb_status.message, "data too large");
    }
    else
    {
       fb_status.status_code = kStatusData;
       sprintf(fb_status.message, "%08x", download_size);
       fb_download_flag = 1;
       fb_download.total = download_size;
       fb_download.received = 0;
       fb_download.last_size = download_size % max_packet_len;
       fb_download.max_num = download_size/max_packet_len;
       if (fb_download.last_size == 0) {
          fb_download.last_size = max_packet_len;
       } else {
          fb_download.max_num++;
       }

       g_first_data_packet_seq = 0;
       g_first_data_packet_received = 0;
       g_index_count = 0;
       g_chunking_packet_seq = 0;

       memset(receive_bitmap, 0, RECV_BITMAP_SIZE);

       // put a 0 at the end of the buffer
       memset((char*)download_addr+fb_download.total, 0, 1);
       fb_udp_ack(buffer, buffer_len, g_packet_num);
       return;
    }
    fb_udp_ack(buffer, buffer_len, INVALID_NUM);
}

static void cb_download_cont(char *buffer, unsigned buffer_len)
{
    // started download, but not complete, change the status
    if (fb_status.status_code == kStatusData) {
       fb_status.status_code = kStatusError;
       strcpy(fb_status.message, "download incomplete");
    }

    U32 packet_num = (int)extract_uint16((const uchar*)buffer+kIndexSeqH);

    // it should be a full package, otherwise ignore
    if (buffer_len != kHeaderSize + max_packet_len) {
       printf("[%d]FB download_cont, invalid packet size, num:%u, size:%d\n", __LINE__,
              packet_num, buffer_len);
       return;
    }

    fb_expected_seq = g_packet_num + 1;
    fb_udp_ack((uchar*)buffer, buffer_len, (U32)packet_num);

    // already received this packet
    if (get_receive_bitmap(g_chunking_packet_seq)) {
       printf("FB download_cont, packet %u is already received\n", g_chunking_packet_seq);
       return;
    }

    g_packet_num=packet_num;
    set_receive_bitmap(g_chunking_packet_seq);
    g_chunking_packet_seq++;

    // download progress indicator
    if (pkt_count == 8000) // start a new line every 8000 packets
    {
        printf("\n");
        pkt_count = 0;
    }
    if (!(pkt_count%100)) printf("#"); // print a # every 100 packets
        pkt_count++;

    memcpy((char*)download_addr+max_packet_len*g_index_count,
           buffer+kHeaderSize, max_packet_len);
    fb_download.received += max_packet_len;
    g_index_count++;
}

static void cb_download_end(char *buffer, unsigned buffer_len)
{
    U32 packet_num = (int)extract_uint16((const uchar*)buffer+kIndexSeqH);

    //printf("[%d]packet_num=%u\n", __LINE__, packet_num);

    int is_received = get_receive_bitmap(g_chunking_packet_seq);

    //printf("buffer_len=%d\n", buffer_len);
    //printf("kHeaderSize=%d\n", kHeaderSize);
    //printf("fb_download.max_num=%d\n", fb_download.max_num);
    //printf("fb_download.last_size=%d\n", fb_download.last_size);
    //printf("max_packet_len=%d\n", max_packet_len);
    //printf("[%d]packet_num=%u\n", __LINE__, packet_num);

    if ( ((g_chunking_packet_seq == fb_download.max_num-1) &&
          (buffer_len-kHeaderSize != fb_download.last_size)) ||
         ((g_chunking_packet_seq != fb_download.max_num-1) &&
          (buffer_len-kHeaderSize != max_packet_len)) ) {
        printf("[%d]FB download_end, invalid packet size, num:%u, size:%d\n", __LINE__,
              packet_num, buffer_len-kHeaderSize);
        printf("[%d]g_chunking_packet_seq:%u\n", __LINE__,
              g_chunking_packet_seq);
       return;  // invalid packet
    }

    if (is_received) { // already received this packet, print a warning
       printf("FB download_end, packet %u is already received\n", packet_num);
    } else {
       set_receive_bitmap(g_chunking_packet_seq);
       // use download_addr to keep the downloaded data
       memcpy((char*)download_addr+g_index_count*max_packet_len,
              buffer+kHeaderSize, buffer_len-kHeaderSize);
       fb_download.received += buffer_len-kHeaderSize;
    }

    //fb_expected_seq++;
    fb_expected_seq = g_packet_num + 1;
    if (fb_download.received == fb_download.total)
    {
        fb_status.status_code = kStatusOkay;
        memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);

        // download done, only need to send a packet ACK,
        // not a full resposne as in the protocol description
        fb_udp_ack((uchar*)buffer, buffer_len, packet_num);

        fb_download_flag = 0;  // clear download session flag
        printf("\nEnd download, %d of %d bytes\n",
               fb_download.received, fb_download.total);
    }
    else
    {
        printf("fb_download.received=%d, fb_download.total=%d\n", fb_download.received, fb_download.total);
        U32 first_missing = (U32)first_unset_bit();
        printf("[%d] g_first_data_packet_seq=%u\n", __LINE__, g_first_data_packet_seq);
        printf("Getting missing packet, num: %u\n", first_missing);
        fb_udp_ack((uchar*)buffer, buffer_len, first_missing);
    }
}

static void cb_flash(char *buffer, unsigned buffer_len)
{
    char tmpCmd[COMMAND_STR_LEN];
    char partition_name[PARTITION_LEN];
    memset(tmpCmd, 0, COMMAND_STR_LEN);
    memset(partition_name, 0, PARTITION_LEN);
    static char last_part_name[CMD_BUF]={0};

#ifdef CONFIG_ANDROID_SECURE_BOOT

    int dev_is_unlock = is_device_unlocked();
    // dev_is_unlock, 1 is unlock, 0 is lock
    if( dev_is_unlock == 0 )
    {
        printf("device locked, forbid to write flash.\n");
        fastboot_fail("device locked, forbid to write flash.");
        fb_status.status_code = kStatusError;
        return -1;
    }
#endif//CONFIG_ANDROID_SECURE_BOOT

    if(critical_lock == 1 &&
      ( !strcmp_l1("rom_emmc_boot", buffer) || !strcmp_l1("MBOOT", buffer) ))
    {
        printf("critical locked, forbid to write flash.\n");
        fastboot_fail("critical locked, forbid to write flash.");
        fb_status.status_code = kStatusError;
        return -1;
    }

    //printf("buffer=%s\n",buffer);
    //printf("last_part_name=%s\n",last_part_name);
    if (!buffer_len)
    {
        fb_status.status_code = kStatusError;
        strcpy(fb_status.message, "missing partition name");
        return;
    }

    strncpy(partition_name, buffer, sizeof(partition_name) - 1);

    if (!strcmp_l1("dl_addr", buffer))
    {
        printf("current downloader_add=0x%08x\n",download_addr);
        download_addr = simple_strtoul((char*)download_addr, NULL, 16);
        printf("set downloader_add=0x%08x\n",download_addr);
    }
    else if (!strcmp_l1("secure_info_boot", buffer))
    {
        sprintf(tmpCmd, "store_secure_info bootSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else if (!strcmp_l1("secure_info_recovery", buffer))
    {
        sprintf(tmpCmd, "store_secure_info recoverySign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else if (!strcmp_l1("secure_info_tee", buffer))
    {
        sprintf(tmpCmd, "store_secure_info teeSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else if (!strcmp_l1("nuttx_config", buffer))
    {
        sprintf(tmpCmd, "store_nuttx_config NuttxConfig 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else if (!strcmp_l1("rom_emmc_boot", buffer))
    {
        sprintf(tmpCmd, "mmc write.boot 1 0x%08x 0 0x%08x",
                download_addr, fb_download.received);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
    }
    else if (!strcmp_l1("MBOOT", buffer))
    {
#ifdef CONFIG_SPI
        sprintf(tmpCmd, "spi wrc 0x%08x 0 0x%08x",
               download_addr, fb_download.received);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
#else
        fb_mmc_flash_write("MBOOT", download_addr, fb_download.total);
#endif
    }
    else if (!strcmp_l1("mscript", buffer))
    {
        util_run_script((char*)download_addr, fb_download.received);
    }
    // flash Image
    else if (getenv("flash_lzo")!=NULL)
    {
        if (strcmp(buffer,last_part_name)!=0)
        {
            snprintf(tmpCmd, sizeof(tmpCmd), "mmc unlzo 0x%08x 0x%08x %s 1",
                    download_addr, fb_download.received, buffer);
        }
        else
        {
            snprintf(tmpCmd, sizeof(tmpCmd), "mmc unlzo.cont 0x%08x 0x%08x %s 1",
                    download_addr, fb_download.received, buffer);
        }
    }
    else
    {
       fb_mmc_flash_write(partition_name, download_addr, fb_download.total);
    }

    memset(last_part_name,0,sizeof(last_part_name));
    strncpy(last_part_name, buffer, sizeof(last_part_name) - 1);
    fb_udp_ack(buffer, buffer_len, g_packet_num);
    return;
}

static void cb_flashing(char* buffer, unsigned buffer_len)
{
    int ret = 0;
    char TempData[20] = {0};
    char dev_state[20] = {0};
    TrimLeft(TempData, buffer, strlen(buffer));

#ifdef  CONFIG_ANDROID_SECURE_BOOT_CLASS_B

    // dev_is_unlock, 1 is unlock, 0 is lock
    int dev_is_unlock = is_device_unlocked();

    // printf("TempData=%s\n", TempData);
    if (strcmp_l1("get_unlock_ability", TempData) == 0)
    {
        if( dev_is_unlock == 0 )
            strcpy(dev_state, "lock");
        else
            strcpy(dev_state, "unlock");

        printf("%s\n", dev_state);
    }
    else if (strcmp_l1("lock_critical", TempData) == 0)
    {
        critical_lock = 1;
    }
    else if (strcmp_l1("unlock_critical", TempData) == 0)
    {
        printf("flashing bootloader-related partitions has its own risk\n");
        critical_lock = 0;
    }
    else if (strcmp_l1("lock", TempData) == 0)
    {
        // skip dev_is_unlock==0, device state not change
        if( dev_is_unlock == 1 )
        {
            printf("wipe user data\n");
            EraseUserData();
            // write lock (1)
            write_device_state(1);
        }
    }
    else if (strcmp_l1("unlock", TempData) == 0)
    {
        // skip dev_is_unlock==1, device state not change
        if( dev_is_unlock == 0 )
        {
            printf("wipe user data\n");
            EraseUserData();
            // write unlock (0)
            write_device_state(0);
        }
    }
    else
#endif//CONFIG_ANDROID_SECURE_BOOT_CLASS_B
    {
        ret = -1;
    }

    if (ret == 0)
        fastboot_okay("Done!!");
    else
        fastboot_fail("invalid command");
    return ret;
}

static void cb_erase(char* buffer, unsigned buffer_len)
{
    char tmpCmd[COMMAND_STR_LEN];

    memset(tmpCmd, 0, COMMAND_STR_LEN);
    snprintf(tmpCmd, sizeof(tmpCmd), "mmc erase.p %s", buffer);
    run_command(tmpCmd, 0);
}

struct cmd_dispatch_info {
    char *cmd;
    void (*cb)(char* buffer, unsigned buffer_len);
};

// download command queries:  "getvar:partition-type:boot" and
//                             getvar:max-download-size
// need to implement this two
static const struct cmd_dispatch_info cmd_dispatch_info[] =
{
    {
        .cmd = "reboot-bootloader",
        .cb = cb_reboot_bootloader,
    },
    {
        .cmd = "reboot",
        .cb = cb_reboot,
    },
    {
        .cmd = "getvar:",
        .cb = cb_getvar,
    },
    {
        .cmd = "download:",
        .cb = cb_download_start,
    },
    {
        .cmd = "flash:",
        .cb = cb_flash,
    },
    {
        .cmd = "erase:",
        .cb = cb_erase,
    },
    {
        .cmd = "flashing",
        .cb = cb_flashing,
    },
#ifdef CONFIG_ANDROID_SUPPORT_AB_UPDATE
    {
        .cmd = "set_active:",
        .cb = cb_setActive,
    },
#endif
};

static void fb_handler_command(char* buffer, unsigned buffer_len)
{
    char cmdbuf[1024];   // assuming max command length
    void (*func_cb)(char* buffer, unsigned buffer_len) = NULL;

    fb_status.status_code = kStatusOkay;
    memset(fb_status.message, 0, FASTBOOT_RESPONSE_LEN);

    if (buffer_len >= 1024)
    {
        fb_status.status_code = kStatusError;
        printf("command too long: %d\n", buffer_len);
        strcpy(fb_status.message, "command too long");
        return;
    }

    // put the command in the cmdbuf and add a '\0'
    strncpy(cmdbuf, buffer, buffer_len);
    cmdbuf[buffer_len] = 0;
    printf("\ncommand: %s, length: %d\n", cmdbuf, buffer_len);

    int i;
    for (i = 0; i < ARRAY_SIZE(cmd_dispatch_info); i++)
    {
        if (!strcmp_l1(cmd_dispatch_info[i].cmd, cmdbuf))
        {
            func_cb = cmd_dispatch_info[i].cb;
            break;
        }
    }

    if (!func_cb)
    {
        fb_status.status_code = kStatusError;
        printf("unknown command: %s", cmdbuf);
        strcpy(fb_status.message, "unknown command");
    }
    else
    {
        int cmdLen = strlen(cmd_dispatch_info[i].cmd);
        func_cb(cmdbuf+cmdLen, buffer_len-cmdLen);
    }
}


//************************ UDP layer ************************

int isContinued(Flag pktFlag, unsigned short pktSeq)
{
  if(pktFlag)
    return 1;

  if(fb_download.received==0)
  {
    int count = fb_download.total/(PAYLOAD_SIZE-kHeaderSize);
    if( (fb_download.total%(PAYLOAD_SIZE-kHeaderSize) != 0) && (fb_download.total >= (PAYLOAD_SIZE-kHeaderSize)) )
      count++;

    if(count > 0)
      return 1;
  }

  if((fb_download.total-fb_download.received)>(PAYLOAD_SIZE-kHeaderSize))
    return 1;

  return 0;
}

static void fb_udp_handler(uchar *pkt, unsigned dest, IPaddr_t sip, unsigned src,
                           unsigned len)
{
    // only handle the packet for us (fastboot)
    if (dest == fb_udp_our_port)
    {
        // need to keep the remote ip and port, needed when sending data back
        fb_remote_ip = sip;
        fb_remote_port = src;

        // incompleted packet
        if (len < kHeaderSize)
            return;

        // read the packet header
        Id             pktId   = pkt[kIndexId];
        Flag           pktFlag = pkt[kIndexFlags];
        unsigned short pktSeq  = extract_uint16(pkt+kIndexSeqH);

        g_packet_num = pktSeq;

        // clear the packet for response
        memset(fb_packet, 0, FASTBOOT_RESPONSE_LEN);
        switch(pktId) {
            case kIdDeviceQuery:
            memcpy(fb_packet, pkt, kHeaderSize);
            put_uint32((uchar*)(fb_packet+kIndexNumHH), INVALID_NUM);
            put_uint16((uchar*)(fb_packet+kHeaderSize), fb_expected_seq);
            fb_send(fb_packet, kHeaderSize+2);
            break;

            case kIdInitialization:
            // seq # is not correct, ignore
            if (pktSeq != fb_expected_seq)
            {
               return;
            }

            if (len < kHeaderSize + 4)
            {
                fb_udp_error();
            }
            else
            {
                unsigned short host_version = extract_uint16(pkt+kHeaderSize);
                max_packet_len = extract_uint16(pkt+kHeaderSize+2);

                printf("\n**************************************\n");
                printf("host version: %d, host max packet: %d\n",
                      host_version, max_packet_len);

                // PKTSIZE doesn't seem work, 1400 seems a good number
                //if (max_packet_len > (PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE)) {
                //   max_packet_len = PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE;
                //}
                if (max_packet_len > PAYLOAD_SIZE)
                {
                    max_packet_len = PAYLOAD_SIZE;
                }
                max_packet_len -= kHeaderSize;

                fb_expected_seq = g_packet_num + 1;
                memcpy(fb_packet, pkt, kHeaderSize);
                put_uint32((uchar*)(fb_packet+kIndexNumHH), INVALID_NUM);
                put_uint16((uchar*)(fb_packet+kHeaderSize), kProtocolVersion);
                put_uint16((uchar*)(fb_packet+kHeaderSize+2), max_packet_len+kHeaderSize);
                fb_send(fb_packet, kHeaderSize+4);

                // init status, prepare for fastboot commands
                fb_init(1);
            }
            break;

            case kIdFastboot:
            if (pktSeq != fb_expected_seq) // repeat packet
            {
                fb_udp_ack((uchar*)pkt, len, (U32)pktSeq);
                return;
            }
            if (len == kHeaderSize) // empty packet for get status
            {
                //printf("Getting an empty packet for status\n");
                fb_expected_seq = g_packet_num + 1;
                memcpy(fb_packet, pkt, kHeaderSize);
                fb_gen_resp(fb_packet);
                printf("response: %s\n", fb_packet+kHeaderSize);
                fb_send(fb_packet, kHeaderSize+strlen(fb_packet+kHeaderSize));
                // need to wait for the status packet before resetting
                if (fb_need_reset)
                {
                    printf("Start to reset\n");
                    mdelay(500);
                    run_command("reset", 0);
                }
            }
            else
            {
                if (fb_download_flag)  // in a download session
                {
                    if (isContinued(pktFlag, pktSeq))
                        cb_download_cont((char*)pkt, len);
                    else
                        cb_download_end((char*)pkt, len);
                }
                else  // other fastboot commands
                {
                    fb_expected_seq = g_packet_num + 1;
                    // send ack first
                    g_first_data_packet_seq = fb_expected_seq;
                    fb_udp_ack(pkt, len, INVALID_NUM);
                    fb_handler_command((char*)(pkt+kHeaderSize), len-kHeaderSize);
                }
            }
            break;

            case kIdError:  // fall through
            default:
            // send back a error mesage
            fb_udp_error();
            break;
        }
    }
}

#else // CONFIG_FASTBOOT_GOOGLE
// fastboot function for amazon
// send an ACK, for all fastboot packet, an ACK is needed
static void fb_udp_ack(uchar *pkt, unsigned len, U32 packet_num)
{
    memcpy(fb_packet, pkt, kHeaderSize);

    put_uint32((uchar*)fb_packet+kIndexNumHH, packet_num);

    // need to clear continuation flag
    fb_packet[kIndexFlags] = kFlagNone;
    last_resp_len = kHeaderSize;
    fb_send(fb_packet, last_resp_len);
}

// send an error packet
static void fb_udp_error(unsigned short pktSeq)
{
    memset(fb_packet, 0, FASTBOOT_RESPONSE_LEN);
    printf("UDP error: invalid packet\n");
    header_set((uchar*)fb_packet, 0, pktSeq, 0, INVALID_NUM);
    strcpy(fb_packet+kHeaderSize, "invalid packet");
    last_resp_len = kHeaderSize+strlen(fb_packet+kHeaderSize);
    fb_send(fb_packet, last_resp_len);
}

// resend last response if packet seq number matches last response
static void fb_resend_resp(unsigned short pktSeq)
{
   unsigned short respSeq  = extract_uint16((const uchar*)fb_packet+kIndexSeqH);
   if (pktSeq == respSeq) {
      //printf("Resend response");
      fb_send(fb_packet, last_resp_len);
   } else {
      printf("UDP error: out of seq packet, exp: %d, actual: %d, resp: %d\n",
             fb_expected_seq, pktSeq, respSeq);
   }
}


//////////////// external apis ////////////////

#if 0  // Need to handle USB/NET fastboot properly, comment out for now
void fastboot_register(const char *prefix,
                       void (*handle)(const char *arg, void *data, unsigned size),
                       fastboot_priv privilege)
{
   return;
}

void fastboot_publish(const char *name, const char *value)
{
   return;
}
#endif // USB/NET fastboot co-ex

void usb_fastboot_info(const char *reason);
void usb_fastboot_fail(const char *reason);
void usb_fastboot_okay(const char *info);

void fastboot_info(const char *reason)
{
   if (net_fastboot_running != 1) {
      usb_fastboot_info(reason);
      return;
   }

   fb_set_resp(kStatusInfo, reason);
   return;
}

void fastboot_fail(const char *reason)
{
   if (net_fastboot_running != 1) {
      usb_fastboot_fail(reason);
      return;
   }

   fb_set_resp(kStatusError, reason);
   return;
}

void fastboot_okay(const char *info)
{
   if (net_fastboot_running != 1) {
      usb_fastboot_okay(info);
      return;
   }

   fb_set_resp(kStatusOkay, info);
   return;
}

//////////////// fastboot commands ////////////////

#ifdef UFBL_FEATURE_IDME
static void cmd_oem_idme(char *arg, unsigned buffer_len)
{
   char response[RESPONSE_STR_LEN] = "idme done";

   if( 0 == fastboot_idme((const char*) arg )) {
      fastboot_info(response);
      fastboot_okay("");
   } else {
      fastboot_fail("idme fail");
   }
}

inline int isblank(int c)
{
        return (c == ' ' || c == '\t');
}

static void cmd_oem_bootmode(char *arg, unsigned buffer_len)
{
    char response[RESPONSE_STR_LEN] = {0};
    unsigned int bootmode_int = 0;
    int ret = 0;

    if(!arg || *arg == '\0'){
        fastboot_fail("Must specify bootmode");
        return;
    }

    while(*arg && isblank(*arg)){
        arg++;
    }

    /* Only accept valid bootmode values */
    bootmode_int = *arg - '0';
    if(bootmode_int > IDME_BOOTMODE_MAX) {
        fastboot_fail("invalid bootmode given");
        return;
    }

    ret = idme_update_var_ex("bootmode", arg, 1);

    if(ret) {
        sprintf(response, "Could not set bootmode, error=%d", ret);
        fastboot_fail(response);
        return;
    }

    sprintf(response, "bootmode set to %c", *arg);
    fastboot_info(response);
    fastboot_okay("");
}

/*
'fastboot oem flags <type:> <modifiers> <value1[|value2|value3|...]>'

type:
 fos_flags:, fos: f: (default if not specified)
 dev_flags:, dev:, d:
 usr_flags:, usr:, u:

modifiers:
 + means to OR value with the existing value
   (flags = flags | value)
 - means to AND the inverse of value with the existing value
   (flags = flags & ~value)
 = means to set the value (flags = value). If = is not set, it is assumed

value:
If begins with 0x, it is treated as a hexadecimal value and set directly
If the value field is a number, it is converted from decimal to hex and set
Multiple values may be ORd together as in value1|value2|value3
Symbolic names are supported, and must match the symbolic name used in the header file

If no type, modifiers or value is specified, print out the current value.
Also, accept the special keyword 'print'

*/

typedef enum {
        FLAGS_CMD_NONE = 0,
        FLAGS_CMD_HELP = 1,
        FLAGS_CMD_PRINT = 2,
        FLAGS_CMD_SET = 3
} flags_cmd_t;

typedef enum {
        FLAGS_TYPE_NONE = 0,
        FLAGS_TYPE_FOS = 1,
        FLAGS_TYPE_DEV = 2,
        FLAGS_TYPE_USR = 3,
        FLAGS_TYPE_ALL = 99
} flags_type_t;

typedef enum {
        FLAGS_MOD_NONE = 0,
        FLAGS_MOD_SET = 1,
        FLAGS_MOD_ADD = 2,
        FLAGS_MOD_REMOVE = 3
} flags_modifier_t;

inline int ishexprefix(char* s) {
        return s && *s && ('0' == *s) && ('x' == *(s+1) || 'X' == *(s+1));
}

inline int issymprefix(char* s) {
        return s && *s &&
                (       !strncmp(s,"FOS_FLAGS_",10) ||
                        !strncmp(s,"DEV_FLAGS_",10) ||
                        !strncmp(s,"USR_FLAGS_",10) );
}

static int hexval(char c)
{
        if (c >= '0' && c <= '9')
                return c - '0';
        else if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;

        return 0;
}

inline unsigned long long atoull(const char *num)
{
        unsigned long long value = 0;
        if(num[0] == '0' && num[1] == 'x') {
                // hex
                num += 2;
                while (*num && isxdigit(*num))
                        value = value * 16 + hexval(*num++);
        } else {
                // decimal
                while (*num && isdigit(*num))
                        value = value * 10 + *num++ - '0';
        }

        return value;
}

inline unsigned long long getnumval(char *s) {

        if(s && *s) {
                if(ishexprefix(s)) {
                        // For hex, normalize to lower case for atoull function
                        *(s+1) = tolower(*(s+1));
                }

                return atoull(s);
        }

        return 0;
}

inline unsigned long long getsymval(char *s) {
        // TBD
        return 0;
}

inline int bootloader_is_locked(void) {
        // TBD
        return 0;
}

static void cmd_oem_flags(char *arg, unsigned buffer_len)
{
        /* sizeof(flags) + NULL = 17 */
        char response[RESPONSE_STR_LEN] = { 0 };
        int ret = 0;
        char * target_flags = NULL;
        char new_flags_str[19] = { 0 };
        char cur_flags_str[19] = { 0 };
        unsigned long long cur_flags = 0;
        unsigned long long new_flags = 0;
        unsigned long long value = 0;

        /* allow modifying to printing usr_flags on locked hw */
        /* Restrict modifying fos_flags or dev_flags on locked hw */

        flags_cmd_t flags_cmd = FLAGS_CMD_PRINT;
        flags_modifier_t flags_modifier = FLAGS_MOD_SET;
        flags_type_t flags_type = FLAGS_TYPE_NONE;

        // Begin parsing
        /* Default to a printout, when just oem flags is called */
        if (!arg || !*arg) {
                flags_cmd = FLAGS_CMD_PRINT;
                flags_type = bootloader_is_locked()? FLAGS_TYPE_USR : FLAGS_TYPE_FOS;
                flags_modifier = FLAGS_MOD_NONE;
                goto parse_done;
        }
        while(*arg && isblank(*arg)) arg++; if(!*arg) goto parse_done;

        //find type: fos_flags, dev_flags or usr_flags
        if(     (!strncmp(arg, "f:", 2) && (arg+=strlen("f:"))) ||
                (!strncmp(arg, "fos:", 4) && (arg+=strlen("fos:"))) ||
                (!strncmp(arg, "fos_flags:", 10) && (arg+=strlen("fos_flags:"))) )
        {
                flags_type = FLAGS_TYPE_FOS;
        }
        else if((!strncmp(arg, "d:", 2) && (arg+=strlen("d:"))) ||
                        (!strncmp(arg, "dev:", 4) && (arg+=strlen("dev:"))) ||
                        (!strncmp(arg, "dev_flags:", 10) && (arg+=strlen("dev_flags:"))) )
        {
                flags_type = FLAGS_TYPE_DEV;
        }
        else if((!strncmp(arg, "u:", 2) && (arg+=strlen("u:"))) ||
                        (!strncmp(arg, "usr:", 4) && (arg+=strlen("usr:"))) ||
                        (!strncmp(arg, "usr_flags:", 10) && (arg+=strlen("usr_flags:"))) )
        {
                flags_type = FLAGS_TYPE_USR;
        }

        // Find modifier +, -, = (= default)
        while(*arg && isblank(*arg)) arg++; if(!*arg) goto parse_done;

        switch(*arg) {
                case '+':
                        flags_modifier = FLAGS_MOD_ADD;
                        arg++;
                        break;
                case '-':
                        flags_modifier = FLAGS_MOD_REMOVE;
                        arg++;
                        break;
                case '=':
                        flags_modifier = FLAGS_MOD_SET;
                        arg++;
                        break;
        }

        // Find value, or special case
        while(*arg && isblank(*arg)) arg++; if(!*arg) goto parse_done;

        if(     !strncmp(arg,"print", 5) ||
                !strncmp(arg,"PRINT", 5) )
        {
                arg+=strlen("print");
                flags_cmd = FLAGS_CMD_PRINT;
                flags_modifier = FLAGS_MOD_NONE;
                while(*arg && isblank(*arg)) arg++;

                if(FLAGS_TYPE_NONE == flags_type) {
                        flags_type = FLAGS_TYPE_FOS; // default for print
                }

                if(bootloader_is_locked() || ('u' == *arg) || ('U' == *arg))
                        flags_type = FLAGS_TYPE_USR;
                else if( ('f' == *arg) || ('F' == *arg) )
                        flags_type = FLAGS_TYPE_FOS;
                else if( ('d' == *arg) || ('D' == *arg) )
                        flags_type = FLAGS_TYPE_DEV;

                goto parse_done;
        }
        else if (       !strcmp(arg,"help") ||
                                !strcmp(arg,"HELP") ||
                                !strcmp(arg,"?") )
        {
                flags_cmd = FLAGS_CMD_HELP;
                flags_modifier = FLAGS_MOD_NONE;
                flags_type = FLAGS_TYPE_NONE;
                goto parse_done;
        }
        else {
                // see if we have a value string
                // while(*arg && isblank(*arg)) arg++; if(!*arg) goto parse_done;
                value = 0;
                char* cur_ptr = NULL;
                while(*arg) {

                        // skip whitespace
                        while(*arg && isblank(*arg)) arg++;

                        cur_ptr = arg;
                        while(*arg && *arg != '|' && !isblank(*arg)) arg++;
                        if(*arg) {
                                *arg = 0;
                                // Prepare for next round
                                arg++;
                                while(*arg && (isblank(*arg) || '|' == *arg) ) arg++;
                        }

                        flags_cmd = FLAGS_CMD_SET;
                        if(issymprefix(cur_ptr)) {
                                value |= getsymval(cur_ptr);
                        }
                        else {
                                value |= getnumval(cur_ptr);
                        }
                }
        }

parse_done:

        if(bootloader_is_locked() && flags_cmd >= FLAGS_CMD_SET) {
                fastboot_fail("oem flags command is restricted on locked hw");
                return;
        }

        if (FLAGS_CMD_SET == flags_cmd && FLAGS_TYPE_NONE == flags_type ) {
                flags_type =  FLAGS_TYPE_FOS;
        }

        switch(flags_type) {
                case FLAGS_TYPE_DEV:
                        target_flags = "dev_flags";
                        break;
                case FLAGS_TYPE_USR:
                        target_flags = "usr_flags";
                        break;
                default:
                        target_flags = "fos_flags";
                        break;
        }

        cur_flags = 0;
        memset(cur_flags_str, 0, sizeof(cur_flags_str));
        if(0 != idme_get_var_external(target_flags, cur_flags_str+2, 16)) {
                printf("Error getting %s value\n", target_flags);
                cur_flags_str[0]=0;
        }
        // Need to prefix with 0x for atoull to handle hex.
        cur_flags_str[0] = '0'; cur_flags_str[1] = 'x';
        cur_flags = atoull(cur_flags_str);

        switch(flags_cmd) {
                case FLAGS_CMD_PRINT:
                {
                        if( FLAGS_TYPE_FOS ==  flags_type ) {
                                int size_needed = fos_flags_to_str( cur_flags, NULL, 0 );
                                char* b = (char*)malloc(size_needed);
                                fos_flags_to_str( cur_flags, b, size_needed );
                                printf("%s\n", b);
                                free(b);
                        }
                        sprintf(response, "%s is 0x%llx\n", target_flags, cur_flags);
                        fastboot_info(response);
                        fastboot_okay("");
                        return;
                        break;
                }
                case FLAGS_CMD_SET:
                {
                        switch(flags_modifier) {
                                case FLAGS_MOD_ADD:
                                {
                                        new_flags = (cur_flags | value);
                                        break;
                                }
                                case FLAGS_MOD_REMOVE:
                                {
                                        new_flags = (cur_flags & ~value);
                                        break;
                                }
                                default:
                                {
                                        new_flags = value;
                                        break;
                                }
                        }


                        memset(new_flags_str, 0, sizeof(new_flags_str));
                        snprintf(new_flags_str, sizeof(new_flags_str), "%llx", new_flags);

                        ret = idme_update_var_ex(target_flags, new_flags_str, sizeof(new_flags_str)-1);
                        if(ret) {
                                snprintf(response, sizeof(response), "could not set %s, error=%d", target_flags, ret);
                                fastboot_fail(response);
                                return;
                        }

                        snprintf(response, sizeof(response), "%s set to %s", target_flags, new_flags_str);
                        printf("%s\n", response);
                        fastboot_info(response);
                        fastboot_okay("");
                        return;
                        break;
                } // case FLAGS_CMD_SET
                default:
                {
                        fastboot_info("oem flags [<type> <modifier>] <value>");
                        fastboot_okay("");
                        break;
                }
        } // switch(flags_cmd)

        fastboot_fail("Unhandled");

}

#if defined(UFBL_FEATURE_UNLOCK)
static void cmd_oem_relock(char *buffer, unsigned buffer_len)
{
    char buf_1[SIGNED_UNLOCK_CODE_LEN];

    memset(buf_1, 0x00, SIGNED_UNLOCK_CODE_LEN);
    if (idme_update_var_ex("unlock_code", buf_1, SIGNED_UNLOCK_CODE_LEN)) {
            fb_set_resp(kStatusError, "relock failed");
    }
    return;
}
#endif

#endif // UFBL_FEATURE_IDME

// reboot command handler
static void cmd_reboot(char *buffer, unsigned buffer_len)
{
    fb_need_reset = 1;
}

// reboot-bootloader command handler
static void cmd_reboot_bootloader(char *buffer, unsigned buffer_len)
{
    run_command("setenv reboot-bootloader 1", 0);
    run_command("saveenv", 0);
    fb_need_reset = 1;
}

// continue command handler
static void cmd_continue(char *buffer, unsigned buffer_len)
{
    fb_need_continue = 1;
}

// getvar command handler
static void cmd_getvar(char *buffer, unsigned buffer_len)
{
    if (buffer_len)
    {
        if (!strcmp_l1("version", buffer))
        {
            strcpy(fb_status.message, FASTBOOT_VERSION);
        }
        else if (!strcmp_l1("production", buffer))
        {
            // TODO
            strcpy(fb_status.message, "Unknown");
        }
        else if (!strcmp_l1("product", buffer))
        {
#ifdef UFBL_FEATURE_IDME
            strcpy(fb_status.message, amzn_target_device_name());
#else
            strcpy(fb_status.message, kProduct);
#endif
        }
        else if (!strcmp_l1("serialno", buffer))
        {
#ifdef UFBL_FEATURE_IDME
            char serialno[18] = {0};
            idme_get_var_external("serial", serialno, sizeof(serialno)-1);
            strcpy(fb_status.message, serialno);
#else
            strcpy(fb_status.message, kSerialNo);
#endif
        }
        else if (!strcmp_l1("max-download-size", buffer))
        {
            // report a smaller number, reserved for sparse headers
            sprintf(fb_status.message, "0x%08x", MAX_DOWNLOAD_SIZE-DOWNLOAD_RESERVED);
        }
//        else if (!strcmp_l1("bootloader-version", buffer))
//        {
//            // U_BOOT_VERSION undefined
//            strcpy(fb_status.message, U_BOOT_VERSION);
//        }
        else if (!strcmp_l1("partition-size:", buffer))
        {
            U64 nSize = get_partition_size(buffer+strlen("partition-size:"));
            nSize = (nSize) ? nSize : DEFAULT_PARTITION_SIZE;
            sprintf(fb_status.message, "0x%016llx", nSize);
        }
        else if (!strcmp_l1("partition-type:", buffer))
        {
            sprintf(fb_status.message, "Unknown");
        }
#ifdef UFBL_FEATURE_SECURE_BOOT
        else if (!strcmp_l1("prod", buffer))
        {
            sprintf(fb_status.message, "%d",
                amzn_target_device_type() == AMZN_ENGINEERING_DEVICE ? 0 : 1);
        }
#endif
        else if (!strcmp_l1("secure", buffer))
        {
            sprintf(fb_status.message, "%d", is_secure_cpu() ? 1 : 0);
        }
#if defined(UFBL_FEATURE_SECURE_FLASHING)
        else if (!strcmp_l1("secflash_code", buffer))
        {
            unsigned char secflash_code[UNLOCK_CODE_LEN + 1] = {0};
            unsigned int code_len = sizeof(secflash_code);

            if (amzn_get_sec_flashing_code(secflash_code, &code_len)) {
               fb_set_resp(kStatusError, "cannot get secure flashing code");
            } else {
               snprintf(fb_status.message, FASTBOOT_RESPONSE_LEN, "%s", secflash_code);
            }
        }
#endif
#if defined(UFBL_FEATURE_UNLOCK)
        else if (!strcmp_l1("unlock_code", buffer))
        {
            unsigned char unlock_code[UNLOCK_CODE_LEN] = {0};
            unsigned int code_len = UNLOCK_CODE_LEN;

            if (amzn_get_unlock_code(unlock_code, &code_len)) {
               fb_set_resp(kStatusError, "cannot get unlock code");
            } else {
               snprintf(fb_status.message, FASTBOOT_RESPONSE_LEN, "%s", unlock_code);
            }
        }
        else if (!strcmp_l1("unlock_status", buffer))
        {
#if UFBL_FEATURE_SECURE_BOOT
            sprintf(fb_status.message, "%d", amzn_device_is_unlocked() ? 1 : 0);
#else
	    sprintf(fb_status.message, "%d", 0);
#endif
        }
        else if (!strcmp_l1("unlock_version", buffer))
        {
            sprintf(fb_status.message, "%d", 1);
        }
#endif // UFBL_FEATURE_UNLOCK
#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
        else if (!strcmp_l1("otu_code", buffer)) {
            unsigned char one_tu_code[ONETIME_UNLOCK_CODE_LEN + 1] = {0};
            unsigned int unlock_code_len = sizeof(one_tu_code);

            if (amzn_get_one_tu_code(one_tu_code, &unlock_code_len)) {
                fb_set_resp(kStatusError, "cannot get onetime unlock code");
            } else {
                snprintf(fb_status.message, FASTBOOT_RESPONSE_LEN, "%s", one_tu_code);
            }
        }
#endif //UFBL_FEATURE_ONETIME_UNLOCK
        else
        {
            fb_set_resp(kStatusError, "Variable not implemented");
        }
    }
    else
    {
        fb_set_resp(kStatusError, "Variable not specified");
    }
}

static void cmd_download_start(char *buffer, unsigned buffer_len)
{
    // simple_strtoul takes the last valid chars, not the first
    U32 download_size = simple_strtoul(buffer, 0, 16);

    printf("Starting download of %d bytes\n", download_size);

    if (net_fastboot_running != 1){
       fb_status.status_code = kStatusData;
       do_download(NULL, buffer, buffer_len);
       fb_download.received = download_size;
       fb_download.total = download_size;
       return;
    }

    // init download progress count
    pkt_count = 0;

    if (0 == download_size)
    {
       fb_set_resp(kStatusError, "data invalid size");
    } else if (download_size > MAX_DOWNLOAD_SIZE) {
       download_size = 0;
       fb_set_resp(kStatusError, "data too large");
    }
    else
    {
       char response[RESPONSE_STR_LEN] = {0};
       sprintf(response, "%08x", download_size);
       fb_set_resp(kStatusData, response);
       fb_download_flag = 1;
       fb_download.total = download_size;
       fb_download.received = 0;
       fb_download.last_size = download_size % max_packet_len;
       fb_download.max_num = download_size/max_packet_len;
       if (fb_download.last_size == 0) {
          fb_download.last_size = max_packet_len;
       } else {
          fb_download.max_num++;
       }
       memset(receive_bitmap, 0, RECV_BITMAP_SIZE);

       // put a 0 at the end of the buffer
       memset((char*)download_addr+fb_download.total, 0, 1);
    }
}


static void cmd_download_cont(char *buffer, unsigned buffer_len)
{
    // started download, but not complete, change the status
    if (fb_status.status_code == kStatusData) {
       fb_set_resp(kStatusError, "download incomplete");
    }

    int packet_num = (int)extract_uint32((const uchar*)buffer+kIndexNumHH);

    // it should be a full package, otherwise ignore
    if (buffer_len != kHeaderSize + max_packet_len) {
       printf("FB download_cont, invalid packet size, num:%d, size:%d\n",
              packet_num, buffer_len);
       return;
    }

    // send an ack for every kBarrierNum packet
    if (!(packet_num % kBarrierNum)) {
       fb_expected_seq++;
       fb_udp_ack((uchar*)buffer, buffer_len, (U32)packet_num);
    }

    // already received this packet
    if (get_receive_bitmap(packet_num)) {
       printf("FB download_cont, packet %d is already received\n", packet_num);
       return;
    }

    set_receive_bitmap(packet_num);

    // download progress indicator
    if (pkt_count == 8000) // start a new line every 8000 packets
    {
        printf("\n");
        pkt_count = 0;
    }
    if (!(pkt_count%100)) {
      printf("#"); // print a # every 100 packets
      if (led_status == 1)
        LED_RED_OFF();
      else
        LED_RED_ON();
      led_status = ~led_status;
    }
    pkt_count++;

    memcpy((char*)download_addr+max_packet_len*packet_num,
           buffer+kHeaderSize, max_packet_len);

    fb_download.received += max_packet_len;
}

static void cmd_download_end(char *buffer, unsigned buffer_len)
{
    int packet_num = (int)extract_uint32((const uchar*)buffer+kIndexNumHH);
    int is_received = get_receive_bitmap(packet_num);
    LED_RED_ON();

    if ( ((packet_num == fb_download.max_num-1) &&
          (buffer_len-kHeaderSize != fb_download.last_size)) ||
         ((packet_num != fb_download.max_num-1) &&
          (buffer_len-kHeaderSize != max_packet_len)) ) {
       printf("FB download_end, invalid packet size, num:%d, size:%d\n",
              packet_num, buffer_len-kHeaderSize);
       return;  // invalid packet
    }

    if (is_received) { // already received this packet, print a warning
       printf("FB download_end, packet %d is already received\n", packet_num);
    } else {
       set_receive_bitmap(packet_num);
       // use download_addr to keep the downloaded data
       memcpy((char*)download_addr+packet_num*max_packet_len,
              buffer+kHeaderSize, buffer_len-kHeaderSize);
       fb_download.received += buffer_len-kHeaderSize;
    }

    fb_expected_seq++;

    if (fb_download.received == fb_download.total)
    {
        fb_set_resp(kStatusOkay, "");

        // download done, only need to send a packet ACK,
        // not a full resposne as in the protocol description
        fb_udp_ack((uchar*)buffer, buffer_len, INVALID_NUM);

        fb_download_flag = 0;  // clear download session flag
        printf("\nEnd download, %d of %d bytes\n",
               fb_download.received, fb_download.total);
    }
    else
    {
        U32 first_missing = (U32)first_unset_bit();
        //printf("Getting missing packet, num: %d\n", first_missing);
        fb_udp_ack((uchar*)buffer, buffer_len, first_missing);
    }
}

static void cmd_erase(char* buffer, unsigned buffer_len)
{
    char tmpCmd[COMMAND_STR_LEN];

    memset(tmpCmd, 0, COMMAND_STR_LEN);
    snprintf(tmpCmd, sizeof(tmpCmd), "mmc erase.p %s", buffer);
    printf("\n%s\n", tmpCmd);
    if(run_command(tmpCmd, 0) != 0)
        fb_set_resp(kStatusError, "failed to erase\n");
    else
        fb_set_resp(kStatusOkay, "");
}

static void cmd_flash(char *buffer, unsigned buffer_len)
{
    char tmpCmd[COMMAND_STR_LEN];
    char partition_name[PARTITION_LEN];
    memset(tmpCmd, 0, COMMAND_STR_LEN);
    memset(partition_name, 0, PARTITION_LEN);
    static char last_part_name[CMD_BUF]={0};

    //printf("buffer=%s\n",buffer);
    //printf("last_part_name=%s\n",last_part_name);
    if (!buffer_len)
    {
        fb_set_resp(kStatusError, "missing partition name");
        return;
    }

    strncpy(partition_name, buffer, sizeof(partition_name) - 1);

#if defined(UFBL_FEATURE_UNLOCK)
    if (!strcmp_l1("unlock", buffer))
    {
#if UFBL_FEATURE_SECURE_BOOT
        if (amzn_verify_unlock((void*)download_addr, fb_download.received)) {
            fb_set_resp(kStatusError, "unlock code error");
        } else if (idme_update_var_ex("unlock_code", (const char *)download_addr,
                                      SIGNED_UNLOCK_CODE_LEN)) {
            fb_set_resp(kStatusError, "unlock failed");
        }
#endif
        return;
    }
#endif
#if defined(UFBL_FEATURE_ONETIME_UNLOCK)
    if (!strcmp_l1("otucert", buffer)) {
        if (amzn_set_onetime_unlock_cert((void*)download_addr, fb_download.received)) {
            fb_set_resp(kStatusError, "set onetime unlock cert error");
        } else {
            fb_set_resp(kStatusOkay, "set onetime unlock cert OKAY");
        }
        return;
    }
    else if (!strcmp_l1("otucode", buffer)) {
        if (amzn_set_onetime_unlock_code((void *)download_addr, fb_download.received)) {
            fb_set_resp(kStatusError, "set onetime unlock code error");
        } else {
            fb_set_resp(kStatusOkay, "set onetime unlock code OKAY");
        }
        return;
    }
#endif //UFBL_FEATURE_ONETIME_UNLOCK
#if defined(UFBL_FEATURE_SECURE_FLASHING)
    int ret = -1;
    if (!strcmp_l1("seccode", buffer))
    {
        if (amzn_verify_sec_flashing_code((void*)download_addr, fb_download.received)) {
            fb_set_resp(kStatusError, "secure flashing code error");
        } else {
            amzn_set_sec_flashing_signed_code(download_addr, SIGNED_UNLOCK_CODE_LEN);
        }
        return;
    }

    if (!strcmp_l1("seccert", buffer))
    {
        if (amzn_set_sec_flashing_cert((void *)download_addr, fb_download.received)) {
            fb_set_resp(kStatusError, "set secure flashing certificate fail");
            return;
        }
        if ( (ret = amzn_verify_sec_flashing_cert(1)) ) {
            sprintf(tmpCmd, "secure flashing certificate verify fail %d", ret);
            fb_set_resp(kStatusError, tmpCmd);
        } else {
            fb_set_resp(kStatusOkay, "secure flashing certificate verify pass");
        }
        return;
    }
#endif

    if (is_lockdown())
    {
       fb_set_resp(kStatusError, "command not supported in lockdown");
       return;
    }

    if (!strcmp_l1("dl_addr", buffer))
    {
        printf("current downloader_add=0x%08x\n",download_addr);
        download_addr = simple_strtoul((char*)download_addr, NULL, 16);
        printf("set downloader_add=0x%08x\n",download_addr);
        return;
    }

    if (!strcmp_l1("secure_info_boot", buffer))
    {
        sprintf(tmpCmd, "store_secure_info bootSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("secure_info_recovery", buffer))
    {
        sprintf(tmpCmd, "store_secure_info recoverySign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("secure_info_tee", buffer))
    {
        sprintf(tmpCmd, "store_secure_info teeSign 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("nuttx_config", buffer))
    {
        sprintf(tmpCmd, "store_nuttx_config NuttxConfig 0x%08x", download_addr);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("rom_emmc_boot", buffer))
    {
        sprintf(tmpCmd, "mmc write.boot 1 0x%08x 0 0x%08x",
                download_addr, fb_download.received);
        printf("\n%s\n", tmpCmd);
        run_command(tmpCmd, 0);
        return;
    }

    if (!strcmp_l1("mscript", buffer))
    {
        util_run_script((char*)download_addr, fb_download.received);
        return;
    }

    // flash Image
    if (getenv("flash_lzo")!=NULL)
    {
        if (strcmp(buffer,last_part_name)!=0)
        {
            snprintf(tmpCmd, sizeof(tmpCmd), "mmc unlzo 0x%08x 0x%08x %s 1",
                    download_addr, fb_download.received, buffer);
        }
        else
        {

            snprintf(tmpCmd, sizeof(tmpCmd), "mmc unlzo.cont 0x%08x 0x%08x %s 1",
                    download_addr, fb_download.received, buffer);
        }
        run_command(tmpCmd, 0);
    }
    else
    {
       fb_mmc_flash_write(partition_name, (void*)download_addr, fb_download.total);
    }

    memset(last_part_name,0,sizeof(last_part_name));
    if (buffer_len < CMD_BUF )
        strncpy(last_part_name, buffer, buffer_len);
    else
        printf("buffer length %d, over %d\n", buffer_len, CMD_BUF);
}

struct cmd_dispatch_info {
    char *cmd;
    void (*cb)(char* buffer, unsigned buffer_len);
};

// download command queries:  "getvar:partition-type:boot" and
//                             getvar:max-download-size
// need to implement this two
static const struct cmd_dispatch_info cmd_dispatch_info[] =
{
    {
        .cmd = "reboot-bootloader",
        .cb = cmd_reboot_bootloader,
    }, {
        .cmd = "reboot",
        .cb = cmd_reboot,
    }, {
        .cmd = "getvar:",
        .cb = cmd_getvar,
    }, {
        .cmd = "download:",
        .cb = cmd_download_start,
    }, {
        .cmd = "flash:",
        .cb = cmd_flash,
    }, {
        .cmd = "erase:",
        .cb = cmd_erase,
    }, {
        .cmd = "continue",
        .cb = cmd_continue,
#ifdef UFBL_FEATURE_IDME
    }, {
        .cmd = "oem idme",
        .cb = cmd_oem_idme,
    }, {
        .cmd = "oem bootmode",
        .cb = cmd_oem_bootmode,
    }, {
        .cmd = "oem flags",
        .cb = cmd_oem_flags,
#if defined(UFBL_FEATURE_UNLOCK)
    }, {
        .cmd = "oem relock",
        .cb = cmd_oem_relock,
#endif // UFBL_FEATURE_UNLOCK
#endif // UFBL_FEATURE_IDME
    },

};

void fb_handler_command(char* buffer, unsigned buffer_len)
{
    char cmdbuf[1024];   // assuming max command length
    void (*func_cb)(char* buffer, unsigned buffer_len) = NULL;

    fb_init_resp();

    if (buffer_len >= 1024)
    {
        fb_status.status_code = kStatusError;
        printf("command too long: %d\n", buffer_len);
        strcpy(fb_status.message, "command too long");
        return;
    }

    // put the command in the cmdbuf and add a '\0'
    strncpy(cmdbuf, buffer, buffer_len);
    cmdbuf[buffer_len] = 0;
    printf("\ncommand: %s, length: %d\n", cmdbuf, buffer_len);

    int i;
    for (i = 0; i < ARRAY_SIZE(cmd_dispatch_info); i++)
    {
        if (!strcmp_l1(cmd_dispatch_info[i].cmd, cmdbuf))
        {
            func_cb = cmd_dispatch_info[i].cb;
            break;
        }
    }

#if defined(UFBL_FEATURE_UNLOCK)
    if ( (is_lockdown()) &&
         (func_cb != cmd_reboot_bootloader) &&
         (func_cb != cmd_reboot) &&
         (func_cb != cmd_getvar) &&
         (func_cb != cmd_flash) &&
         (func_cb != cmd_download_start) )
    {
        fb_set_resp(kStatusError, "command not supported in lockdown");
        return;
    }
#endif
    if (!func_cb)
    {
        fb_set_resp(kStatusError, "unknown command");
        printf("unknown command: %s", cmdbuf);
    }
    else
    {
        int cmdLen = strlen(cmd_dispatch_info[i].cmd);
        func_cb(cmdbuf+cmdLen, buffer_len-cmdLen);
    }
}


//************************ UDP layer ************************

static void fb_udp_handler(uchar *pkt, unsigned dest, IPaddr_t sip, unsigned src,
                           unsigned len)
{
    // only handle the packet for us (fastboot)
    if (dest == fb_udp_our_port)
    {
        // need to keep the remote ip and port, needed when sending data back
        fb_remote_ip = sip;
        fb_remote_port = src;

        // incompleted packet
        if (len < kHeaderSize) {
            printf("UDP error:Incomplete header\n");
            return;
        }

        // read the packet header
        Id             pktId   = pkt[kIndexId];
        Flag           pktFlag = pkt[kIndexFlags];
        unsigned short pktSeq  = extract_uint16(pkt+kIndexSeqH);

        switch(pktId) {
            case kIdDeviceQuery:
            memcpy(fb_packet, pkt, kHeaderSize);
            put_uint32((uchar*)(fb_packet+kIndexNumHH), INVALID_NUM);
            put_uint16((uchar*)(fb_packet+kHeaderSize), fb_expected_seq);
            last_resp_len = kHeaderSize+2;
            fb_send(fb_packet, last_resp_len);
            break;

            case kIdInitialization:
            // seq # is not correct, ignore
            if (pktSeq != fb_expected_seq)
            {
               fb_resend_resp(pktSeq);
               return;
            }

            if (len < kHeaderSize + 4)
            {
                fb_udp_error(pktSeq);
            }
            else
            {
                unsigned short host_version = extract_uint16(pkt+kHeaderSize);
                max_packet_len = extract_uint16(pkt+kHeaderSize+2);
                if (len == kHeaderSize + 6) // has barrier number
                    kBarrierNum = extract_uint16(pkt+kHeaderSize+4);

                printf("\n**************************************\n");
                printf("host version: %d, host max packet: %d, barrier number: %d\n",
                      host_version, max_packet_len, kBarrierNum);

                // PKTSIZE doesn't seem work, 1400 seems a good number
                //if (max_packet_len > (PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE)) {
                //   max_packet_len = PKTSIZE - NetEthHdrSize() - IP_HDR_SIZE;
                //}
                if (max_packet_len > PAYLOAD_SIZE)
                {
                    max_packet_len = PAYLOAD_SIZE;
                }
                max_packet_len -= kHeaderSize;

                fb_expected_seq++;

                memcpy(fb_packet, pkt, kHeaderSize);
                put_uint32((uchar*)(fb_packet+kIndexNumHH), INVALID_NUM);
                put_uint16((uchar*)(fb_packet+kHeaderSize), kProtocolVersion);
                put_uint16((uchar*)(fb_packet+kHeaderSize+2), max_packet_len+kHeaderSize);
                last_resp_len = kHeaderSize+4;
                fb_send(fb_packet, last_resp_len);

                // init status, prepare for fastboot commands
                fb_init(1);
            }
            break;

            case kIdFastboot:
            if (pktSeq != fb_expected_seq) // repeat packet
            {
               fb_resend_resp(pktSeq);
               return;
            }
            if (len == kHeaderSize) // empty packet for get status
            {
                //printf("Getting an empty packet for status\n");
                fb_expected_seq++;
                memcpy(fb_packet, pkt, kHeaderSize);
                put_uint32((uchar*)(fb_packet+kIndexNumHH), INVALID_NUM);
                fb_gen_resp(fb_packet+kHeaderSize);
                printf("response: %s\n", fb_packet+kHeaderSize);
                last_resp_len = kHeaderSize+strlen(fb_packet+kHeaderSize);
                fb_send(fb_packet, last_resp_len);

                // need to wait for the status packet before resetting
                if (fb_need_reset)
                {
                    mdelay(500);
                    run_command("reset", 0);
                }

                // need to continue to boot
                if (fb_need_continue)
                {
                    mdelay(500);
                    run_command("continue", 0);
                }

                if (fb_need_halt)
                {
                    while (1) ;
                }
            }
            else
            {
                if (fb_download_flag)  // in a download session
                {
                    if (pktFlag) {
                        cmd_download_cont((char*)pkt, len);
                    }
                    else
                        cmd_download_end((char*)pkt, len);
                }
                else  // other fastboot commands
                {
                    fb_expected_seq++;

                    // send ack first
                    fb_udp_ack(pkt, len, INVALID_NUM);

                    fb_handler_command((char*)(pkt+kHeaderSize), len-kHeaderSize);
                }
            }
            break;

            case kIdError:  // fall through
            default:
            // send back a error mesage
            fb_udp_error(pktSeq);
            break;
        }
    }
}

#endif // CONFIG_FASTBOOT_GOOGLE

void fb_udp_start(void)
{
    // allocate memory for receive flag bitmap upfront
    if (!receive_bitmap) {
       receive_bitmap = (unsigned char*)calloc(1, RECV_BITMAP_SIZE);
       if (!receive_bitmap) {
          printf("\nfastboot - out of memory!\n");
          return;
       }
    }

    char *s;
    if ((s = getenv("fastboot_port")) != NULL) {
       fb_udp_our_port = (int)simple_strtoul(s, NULL, 10);
       //printf("fb_udp_our_port=%d\n", fb_udp_our_port);
    }

    char sIPStr[32];

    ip_to_string(NetOurIP, sIPStr);
    LED_GREEN_OFF();
    LED_RED_ON();
    //printf("Using %s device\n", eth_get_name());
    printf("\n\nListening for fastboot transfer on %s\n", sIPStr);

    fb_init(1);

    NetSetHandler(fb_udp_handler);
}
