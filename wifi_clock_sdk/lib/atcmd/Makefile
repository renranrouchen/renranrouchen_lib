include $(TOPDIR)/.lib_config

OBJS += at_cmd_base.o at_cmd_net.o at_cmd_wifi.o at_cmd_proc.o

ifdef CONFIG_LWIP
EXTRA_CFLAGS += -I$(TOPDIR)/include/
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/include/
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/include/ipv4
EXTRA_CFLAGS += -I$(TOPDIR)/lib/lwip/
endif

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
