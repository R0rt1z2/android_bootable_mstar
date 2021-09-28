#ifndef UFS_MSTAR_H_
#define UFS_MSTAR_H_

#include "ufs-mstar-pltfrm.h"

struct ufshcd_lrb;

int ufs_init(void);
int ufs_read6(u32 lun, void *pBuf, u32 dLba, u16 wLength);
int ufs_read10(u32 lun, void *pBuf, u32 dLba, u16 wLength);
int ufs_write6(u32 lun, void *pBuf, u32 dLba, u16 wLength);
int ufs_write10(u32 lun, void *pBuf, u32 dLba, u16 wLength);
int ufs_unmap(u32 lun, u64 qLba, u32 dLength);
int ufs_config_lun(void);
void ufs_count_down(u32 u32_Sec);
void ufs_mstar_print_request(struct ufshcd_lrb *lrbp);
void ufs_mstar_print_response(struct ufshcd_lrb *lrbp);
void dumpMemory4(const char *str, const void *data, size_t size);
unsigned long ufs_block_read(int device, unsigned long blknr, lbaint_t blkcnt, void *buffer);
unsigned long ufs_block_write(int device, unsigned long blknr, lbaint_t blkcnt, const void *buffer);
int ufs_mstar_ipverify(void);
int ufstest_info(void);
int ufstest_rw10_lun(int loop);
int ufstest_speed(int testcase);
u32 ufstest_PwrCut_InitData(u8* u8_DataBuf, u32 u32_BlkStartAddr);
u32 ufstest_PwrCut_Test(u8* u8_DataBuf, u32 u32_BlkStartAddr);
int ufstest_hibern8(int cmd);

#define MAX_UFS_MSTAR_HOSTS	1
#define MAX_UFS_MSTAR_LUN	3
// 0:user lun
// 1:boot lun 1
// 2:boot lun 2

struct ufs_mstar_host {

	struct ufs_hba *hba;
	u32 dLogicalBlkSize[MAX_UFS_MSTAR_LUN];
	u64 qLogicalBlkCnt[MAX_UFS_MSTAR_LUN];
};

#endif /* UFS_MSTAR_H_ */
