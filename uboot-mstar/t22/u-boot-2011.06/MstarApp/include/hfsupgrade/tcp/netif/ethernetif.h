#ifndef __ETHERNET_IF__
#define __ETHERNET_IF__

#include "lwip/netif.h"

void low_level_input(volatile void *rx_packet, int length);

void ethernetif_input(struct netif *netif, struct pbuf *p);

err_t ethernetif_init(struct netif *netif);

extern int (*ethernetif_open)(void);

extern void (*ethernetif_close)(void);

extern int (*ethernetif_recv)(void (*rx_callback)(volatile void *, int));

#endif // __ETHERNET_IF__
