#
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB := $(obj)libhfsupgrade.a

# COREFILES, CORE4FILES: The minimum set of files needed for lwIP.
COREFILES=tcp/core/mem.o             \
        tcp/core/memp.o              \
        tcp/core/netif.o             \
        tcp/core/pbuf.o              \
        tcp/core/raw.o               \
        tcp/core/stats.o             \
        tcp/core/sys.o               \
        tcp/core/tcp.o               \
        tcp/core/tcp_in.o            \
        tcp/core/tcp_out.o           \
        tcp/core/init.o              \
		tcp/core/timers.o

CORE4FILES=tcp/core/ipv4/ip.o        \
        tcp/core/ipv4/inet.o         \
        tcp/core/ipv4/ip_addr.o      \
        tcp/core/ipv4/ip_frag.o      \
        tcp/core/ipv4/inet_chksum.o	 \
		tcp/core/ipv4/icmp.o

# NETIFFILES: Files implementing various generic network interface functions.'
NETIFFILES=tcp/netif/etharp.o        \
		tcp/netif/ethernetif.o

# HFS Client Implementation
HFSFILES=hfs/hfs_get.o

# LWIPFILES: All the above.
OBJS := $(COREFILES) $(CORE4FILES) $(NETIFFILES) $(HFSFILES)

all: $(LIB)

$(LIB): $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
