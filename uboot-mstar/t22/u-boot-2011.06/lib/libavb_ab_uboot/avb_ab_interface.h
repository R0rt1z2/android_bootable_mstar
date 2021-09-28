
#ifndef AVB_AB_INTERFACE_H_
#define AVB_AB_INTERFACE_H_

#include "../libavb_ab/libavb_ab.h"

int ab_getCurrentSlotIndex(void);
int ab_getSlotRetryCount(int slotNumber);
int ab_getSlotSuccessful(int slotNumber);
int ab_getSlotUnbootable(int slotNumber);
int ab_setActiveSlot(int slotNumber);
bool ab_appendSlotSuffix(char * out_data,const char * prefix_str);

#endif