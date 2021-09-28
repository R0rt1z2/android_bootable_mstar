#ifndef __UBOOT_MMAP_H__
#define __UBOOT_MMAP_H__

#include <config.h>

#define R2_CACHE_ADDR CONFIG_R2_CACHE_ADDR
#define UPGRADE_ADDR_STR        "UPGRADE_MEM_ADDR"
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
#define BOOT_MODE_TMP_BUF_ADDR  (CONFIG_SYS_MIU0_CACHE|0x5000000)
#else
#define BOOT_MODE_TMP_BUF_ADDR  (CONFIG_SYS_MIU0_CACHE|0x400000)
#endif
#define UPGRADE_BUFFER_ADDR CONFIG_UPGRADE_BUFFER_ADDR

/*************************************
***  Secure booting memory setting ***
**************************************/
#define AES_DECRYPTED_ADDR                    (0x4000000)
#define FILESYSTEM_AUTHENTICATION_BUFFER_ADDR (CONFIG_SYS_MIU0_CACHE|0x5000000)
#define SECURITY_COMMON_BUFFER_ADDR (CONFIG_SYS_MIU0_CACHE|0xA00000) // for seucre pm51 unittest.


/*************************************
*****   panel memory setting   *******
**************************************/
#define DMX_SEC_BUFFER_ADR      0x073FC000


#define NO_DEFAULT_MMAP_VALUE       0xFFFFFFFF
/*************************************
*****   System memory setting   *******
**************************************/
//Security
#define E_MMAP_ID_HW_AES_BUF_ADR "E_MMAP_ID_HW_AES_BUF_ADR"
#define E_MMAP_ID_HW_AES_BUF_LEN "E_MMAP_ID_HW_AES_BUF_LEN"
#define E_MMAP_ID_NUTTX_MEM_ADR  "E_MMAP_ID_NUTTX_MEM_ADR"
#define E_MMAP_ID_NUTTX_MEM_LEN  "E_MMAP_ID_NUTTX_MEM_LEN"
#define E_MMAP_ID_SMC_BUF_ADR "E_MMAP_ID_SMC_BUF_ADR"
#define E_MMAP_ID_SMC_BUF_LEN "E_MMAP_ID_SMC_BUF_LEN"

// LX information
#define E_LX_MEM_ADR              "E_LX_MEM_ADR"
#define E_LX_MEM_LEN              "E_LX_MEM_LEN"
#define E_LX_MEM2_ADR             "E_LX_MEM2_ADR"
#define E_LX_MEM2_LEN             "E_LX_MEM2_LEN"
#define E_LX_MEM3_ADR             "E_LX_MEM3_ADR"
#define E_LX_MEM3_LEN             "E_LX_MEM3_LEN"
#define E_LX_MEM4_ADR             "E_LX_MEM4_ADR"
#define E_LX_MEM4_LEN             "E_LX_MEM4_LEN"

// Others
#define E_MMAP_ID_PVR_DOWNLOAD_ADR    "E_MMAP_ID_PVR_DOWNLOAD_ADR"
#define E_MMAP_ID_PM51_USAGE_MEM_ADR  "E_MMAP_ID_PM51_USAGE_MEM_ADR"
#define E_MMAP_ID_PM51_CODE_MEM_ADR   "E_MMAP_ID_PM51_CODE_MEM_ADR"
#define E_MMAP_ID_PM51_RT_MEM_ADR     "E_MMAP_ID_PM51_RT_MEM_ADR"
#define E_MMAP_ID_LOCAL_DIMMING_ADR   "E_MMAP_ID_LOCAL_DIMMING_ADR"
#define E_MMAP_ID_LOCAL_DIMMING_LEN   "E_MMAP_ID_LOCAL_DIMMING_LEN"

//VDEC
#define E_MMAP_ID_VDEC_CPU_ADR             "E_MMAP_ID_VDEC_CPU_ADR"
#define E_MMAP_ID_VDEC_CPU_LEN             "E_MMAP_ID_VDEC_CPU_LEN"
#define E_MMAP_ID_VDEC_BITSTREAM_ADR       "E_MMAP_ID_VDEC_BITSTREAM_ADR"
#define E_MMAP_ID_VDEC_BITSTREAM_LEN       "E_MMAP_ID_VDEC_BITSTREAM_LEN"
#define E_MMAP_ID_VDEC_SUB_BITSTREAM_ADR   "E_MMAP_ID_VDEC_SUB_BITSTREAM_ADR"
#define E_MMAP_ID_VDEC_SUB_BITSTREAM_LEN   "E_MMAP_ID_VDEC_SUB_BITSTREAM_LEN"

#define E_EMAC_MEM_ADR                "E_EMAC_MEM_ADR"
#define E_EMAC_MEM_LEN                "E_EMAC_MEM_LEN"
#define E_GMAC_MEM_ADR                "E_GMAC_MEM_ADR"
#define E_GMAC_MEM_LEN                "E_GMAC_MEM_LEN"

#define E_MMAP_ID_FRC_R2_ADR      "E_MMAP_ID_FRC_R2_ADR"
#define E_MMAP_ID_FRC_R2_LEN      "E_MMAP_ID_FRC_R2_LEN"

//DMX
#define E_MMAP_ID_DMX_SECBUF_ADR          "E_MMAP_ID_DMX_SECBUF_ADR"
#define E_MMAP_ID_DMX_SECBUF_LEN          "E_MMAP_ID_DMX_SECBUF_LEN"

//XC
#define E_MMAP_ID_XC1_MAIN_FB_ADR          "E_MMAP_ID_XC1_MAIN_FB_ADR"
#define E_MMAP_ID_XC1_MAIN_FB_LEN          "E_MMAP_ID_XC1_MAIN_FB_LEN"

//DEMURA
#define E_MMAP_DISPOUT_DEMURA_ADR          "E_MMAP_DISPOUT_DEMURA_ADR"
#define E_MMAP_DISPOUT_DEMURA_LEN          "E_MMAP_DISPOUT_DEMURA_LEN"

//HWC
#define E_MMAP_ID_HWC_GOP_BUF_ADR          "E_MMAP_ID_HWC_GOP_BUF_ADR"
#define E_MMAP_ID_HWC_GOP_BUF_LEN          "E_MMAP_ID_HWC_GOP_BUF_LEN"

/*************************************
*****   logo memory setting   *******
**************************************/
#if (ENABLE_STB_ECOS_BOOT == 1)
    #define E_MMAP_ID_VE_ADR                "ve_buffer_addr"
    #define E_MMAP_ID_VE_LEN                "ve_buffer_size"
    #define E_DFB_FRAMEBUFFER_ADR           "bl_dfb_framebuffer_addr"
    #define E_MMAP_ID_JPD_READ_ADR          "bl_jpd_read_addr"
    #define E_MMAP_ID_JPD_READ_LEN          "bl_jpd_read_size"
    #define E_MMAP_ID_JPD_WRITE_ADR         "bl_jpd_write_addr"
    #define E_MMAP_ID_JPD_WRITE_LEN         "bl_jpd_write_size"
    #define E_MMAP_ID_PHOTO_INTER_ADR       "bl_jpd_inter_addr"
    #define E_MMAP_ID_PHOTO_INTER_LEN       "bl_jpd_inter_size"
#else
    #define E_MMAP_ID_VE_ADR          "E_MMAP_ID_VE_ADR"
    #define E_MMAP_ID_VE_LEN          "E_MMAP_ID_VE_LEN"
    #define E_DFB_FRAMEBUFFER_ADR     "E_DFB_FRAMEBUFFER_ADR"
    #define E_MMAP_ID_JPD_READ_ADR	  "E_MMAP_ID_JPD_READ_ADR"
    #define E_MMAP_ID_JPD_READ_LEN	  "E_MMAP_ID_JPD_READ_LEN"
    #define E_MMAP_ID_JPD_WRITE_ADR   "E_MMAP_ID_JPD_WRITE_ADR"
    #define E_MMAP_ID_JPD_WRITE_LEN   "E_MMAP_ID_JPD_WRITE_LEN"
    #define E_MMAP_ID_PHOTO_INTER_ADR "E_MMAP_ID_PHOTO_INTER_ADR"
    #define E_MMAP_ID_PHOTO_INTER_LEN "E_MMAP_ID_PHOTO_INTER_LEN"
#endif

#define MMAP_E_MMAP_ID_VE_ADDR                  CONFIG_VE_BUFFER_ADDR //0x05800000
#define MMAP_E_MMAP_ID_VE_SIZE                  CONFIG_VE_BUFFER_SIZE //0x05800000
#define MMAP_E_DFB_FRAMEBUFFER_ADDR             CONFIG_MMAP_E_DFB_FRAMEBUFFER_ADDR
#define MMAP_E_MMAP_ID_JPD_WRITE_ADDR           CONFIG_MMAP_E_MMAP_ID_JPD_WRITE_ADDR    //Alignment 0x00008
#define MMAP_E_MMAP_ID_JPD_WRITE_SIZE           CONFIG_MMAP_E_MMAP_ID_JPD_WRITE_SIZE
#define MMAP_E_MMAP_ID_JPD_READ_ADDR            CONFIG_MMAP_E_MMAP_ID_JPD_READ_ADDR     //Alignment 0x00010
#define MMAP_E_MMAP_ID_JPD_READ_SIZE            CONFIG_MMAP_E_MMAP_ID_JPD_READ_SIZE
#define MMAP_E_MMAP_ID_PHOTO_INTER_ADDR         CONFIG_MMAP_E_MMAP_ID_PHOTO_INTER_ADDR  //Alignment 0x00200
#define MMAP_E_MMAP_ID_PHOTO_INTER_SIZE         CONFIG_MMAP_E_MMAP_ID_PHOTO_INTER_SIZE

#endif //__UBOOT_MMAP_H__
