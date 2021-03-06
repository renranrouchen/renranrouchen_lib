include $(TOPDIR)/.lib_config

ifdef CONFIG_IPV6
EXTRA_CFLAGS += -I./include/ipv6
PROT_OBJS = $(patsubst %, core/ipv6/%, icmp6.o inet6.o ip6_addr.o ip6.o )
else
EXTRA_CFLAGS += -I./include/ipv4
PROT_OBJS = $(patsubst %, core/ipv4/%, autoip.o icmp.o igmp.o inet.o \
			inet_chksum.o ip_addr.o ip.o )
ifdef CONFIG_FRAG
PROT_OBJS += $(patsubst %, core/ipv4/%, ip_frag.o )
endif
endif
ifdef CONFIG_FREERTOS
EXTRA_CFLAGS += -I./include/freertos
endif

CORE_OBJS = $(patsubst %, core/%, def.o init.o mem.o memp.o netif.o pbuf.o raw.o \
			stats.o sys.o tcp.o tcp_in.o tcp_out.o timers.o udp.o )
ifdef CONFIG_DHCPC
CORE_OBJS += $(patsubst %, core/%, dhcp.o )
endif
ifdef CONFIG_DNS
CORE_OBJS += $(patsubst %, core/%, dns.o )
endif

NET_OBJS = $(patsubst %, netif/%, etharp.o ethernetif.o )

API_OBJS = $(patsubst %, api/%, api_lib.o api_msg.o err.o netbuf.o netdb.o \
			netifapi.o sockets.o tcpip.o net_api.o )
ifdef CONFIG_SOCKET_API
API_OBJS += $(patsubst %, api/%, socket_api.o )
endif

APPS_OBJS =
ifdef CONFIG_PING
APPS_OBJS += $(patsubst %, apps/ping/%, ping.o )
endif
ifdef CONFIG_DHCPS
APPS_OBJS += $(patsubst %, apps/dhcpserver/%, dhcp_server.o )
endif
ifdef CONFIG_NTP
APPS_OBJS += $(patsubst %, apps/ntp/%, ntpd.o ntp_time.o )
endif
ifdef CONFIG_HTTP
APPS_OBJS += $(patsubst %, apps/http/%, webclient.o )
endif
ifdef CONFIG_HTTP_SERVER
APPS_OBJS += $(patsubst %, apps/httpd/%, fs.o httpd.o strcasestr.o fsdata.o)
EXTRA_CFLAGS += -I./apps/httpd/include
endif

ifdef CONFIG_OMNICONFIG
OBJS += api/omni_broadcast.o 
endif

OBJS += arch/sys_arch.o $(PROT_OBJS) $(CORE_OBJS) $(NET_OBJS) $(API_OBJS) $(APPS_OBJS)

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
