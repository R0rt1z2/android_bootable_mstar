
#ifndef _HAL_SEAL_H_
#define _HAL_SEAL_H_

#define HAL_MIU0_BASE               0x00000000UL
#define HAL_MIU1_BASE               0x80000000UL // 1512MB

#define _phy_to_miu_offset(MiuSel, Offset, PhysAddr) if (PhysAddr < HAL_MIU1_BASE) \
                                                        {MiuSel = 0; Offset = PhysAddr;} \
                                                     else \
                                                         {MiuSel = 1; Offset = PhysAddr - HAL_MIU1_BASE;}

#define CONFIG_RIU_BASE_ADDRESS     0x1F000000

#define RIU     ((unsigned short volatile *) CONFIG_RIU_BASE_ADDRESS)


#define SEAL_TZPC_PM_BASE                      (0x00003900UL)
#define REG_PM_RIU_BRIDGE                      (0x02UL)
#define REG_TZPC_PM_SECURE_BANK                (SEAL_TZPC_PM_BASE+0x20)
#define REG_TZPC_PM_SECURE_REG                 (SEAL_TZPC_PM_BASE+0x80)

#define SEAL_TZPC_NONPM_BASE                   (0x00123900UL)
#define SEAL_TZPC_NONPM_MIU0                   (0x00311700UL)
#define SEAL_TZPC_NONPM_MIU1                   (0x00311800UL)
#define SEAL_TZPC_TSP                          (0x00161A00UL)
#define SEAL_TZPC_SC                           (0x00161900UL)
#define REG_NONPM_RIU_BRIDGE                   (0x02UL)
#define REG_SECURE_RIU_BRIDGE                  (0x06UL)
#define REG_TZPC_LEGACY_SECUREPROCESSOR        (SEAL_TZPC_NONPM_BASE + 0x60UL)
#define FLAG_TZPC_NONSECURE_ARM                (1<<11)
#define REG_TZPC_NONPM_SECURE_BANK             (SEAL_TZPC_NONPM_BASE+0x20)
#define REG_TZPC_NONPM_SECURE_REG              (SEAL_TZPC_NONPM_BASE+0xC8)
#define REG_TZPC_BUFFER_LOCK_TSP               (SEAL_TZPC_TSP+0xC0)
#define REG_TZPC_BUFFER_LOCK_SC                (SEAL_TZPC_SC+0xE8)

#define REG_TZPC_MIU0_CTL                      (SEAL_TZPC_NONPM_MIU0+0x24)
#define REG_TZPC_MIU1_CTL                      (SEAL_TZPC_NONPM_MIU1+0x24)
#define REG_TZPC_MIU_TOP_EN                    (1<<0)
#define REG_TZPC_MIU0_ID0                      (SEAL_TZPC_NONPM_MIU0+0x38)
#define REG_TZPC_MIU1_ID0                      (SEAL_TZPC_NONPM_MIU1+0x38)
#define REG_TZPC_MIU_ID_ENABLE                 (1<<12)
#define REG_TZPC_MIU0_BASE_ADDR_LOW            (SEAL_TZPC_NONPM_MIU0+0x2E)
#define REG_TZPC_MIU0_BASE_ADDR_HIGH           (SEAL_TZPC_NONPM_MIU0+0x30)
#define REG_TZPC_MIU1_BASE_ADDR_LOW            (SEAL_TZPC_NONPM_MIU1+0x2E)
#define REG_TZPC_MIU1_BASE_ADDR_HIGH           (SEAL_TZPC_NONPM_MIU1+0x30)


#define SEAL_SECURE0_RANGE0                     (0x00123700UL)
#define SEAL_SECURE1_RANGE0                     (0x00123800UL)

#define REG_SECURE_RANGE0_START_ADDR           (0x00UL)
#define REG_SECURE_RANGE0_END_ADDR             (0x04UL)
#define REG_SECURE_RANGE0_ATTRIBUTE            (0x06UL)

#define REG_SECURE_RANGE_OFFSET                 (0x08)

#define FLAG_SECURE_RANGE_ADDR_DYN_CH           (0x1 << 15)

#define REG_SECURE0_DETECT_ENABLE               (0xECUL)

#define REG_SECURE_RANGE_NUM                    (16)

#define SEAL_NONPM_TBL_IP_NUM                   (153)

#define SEAL_LOCK_GROUP_NUM         (26UL)
#define SEAL_LOCK_TSP_GROUP_NUM     (33UL)   //[SEAL][HAL][000] Numbers of lock TSP groups
#define SEAL_LOCK_SC_GROUP_NUM      (12UL)   //[SEAL][HAL][000] Numbers of lock SC groups
#define SEAL_LOCK_NUM_PERGROUP      (16UL)
#define SEAL_TBL_LOCK_NUM           (SEAL_LOCK_GROUP_NUM*SEAL_LOCK_NUM_PERGROUP)
#define SEAL_TBL_LOCK_TSP_NUM       (SEAL_LOCK_TSP_GROUP_NUM*SEAL_LOCK_NUM_PERGROUP)
#define SEAL_TBL_LOCK_SC_NUM        (SEAL_LOCK_SC_GROUP_NUM*SEAL_LOCK_NUM_PERGROUP)

int HAL_SEAL_SetSecureRange(unsigned long long startAddr, unsigned long long endAddr, int attr);
int HAL_SEAL_DisableRIUBridges(void);
int HAL_SEAL_Init(void);
int HAL_SEAL_SetMIUHost(int swIdx, int bSecure);
int HAL_SEAL_SetRIUBank(int swIdx, int bSecure);
int HAL_SEAL_SetRIURegister(int swIdx, int bSecure);
int HAL_SEAL_Cleanup(void);

#endif
