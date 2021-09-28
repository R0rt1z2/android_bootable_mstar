
#define U32 unsigned int
#define FASTBOOT_VERSION       "0.4"
#define FASTBOOT_RESPONSE_LEN  65    //64 defined bytes+'\0'
#define kDefaultPort           5554
#define kDefaultIP             "192.168.1.101"
#define kProduct               "UNDEFINED"
#define kSerialNo              "0123456789ABCDEF"
#define CMD_BUF                128

// header fields
typedef enum {
    kIndexId = 0,
    kIndexFlags = 1,
    kIndexSeqH = 2,
    kIndexSeqL = 3,
    kIndexNumHH = 4,
    kIndexNumHL = 5,
    kIndexNumLH = 6,
    kIndexNumLL = 7
} Index;

// packet type id
typedef enum {
    kIdError = 0,
    kIdDeviceQuery = 1,
    kIdInitialization = 2,
    kIdFastboot = 3
} Id;

// continuation flag
typedef enum {
    kFlagNone = 0,
    kFlagContinuation = 1
} Flag;

//// fastboot command status
// status type
typedef enum {
    kStatusError = 0,
    kStatusInfo  = 1,
    kStatusOkay  = 2,
    kStatusData  = 3
} StatusCode;

// max number of INFO response
#define MAX_INFO_NUM 100

// status structure
typedef struct fb_status_t {
   StatusCode status_code;
   char       message[FASTBOOT_RESPONSE_LEN];
   int        info_index;
   int        info_num;
   char       info_msg[MAX_INFO_NUM][FASTBOOT_RESPONSE_LEN];
} fb_status_t;

//// download status
static int fb_download_flag = 0;
typedef struct fb_download_t {
    U32 total;
    U32 received;
    U32 max_num;
    U32 last_size;
} fb_download_t;

extern fb_status_t fb_status;
extern int  fb_need_reset;
extern int  fb_need_continue;
