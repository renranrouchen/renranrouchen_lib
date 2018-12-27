include $(TOPDIR)/.lib_config

EXTRA_CFLAGS += -I. -I$(TOPDIR)/include/freertos 

OBJS += list.o queue.o tasks.o port.o 

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
