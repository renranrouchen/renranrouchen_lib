WIFI_APP_PATH=wifiClock_app
WIFI_DRIVER_PATH=wifiClock_driver
WIFI_APP_SRC=$(patsubst %.c,%.o,$(wildcard $(WIFI_APP_PATH)/*.c))
WIFI_DRIVER_SRC=$(patsubst %.c,%.o,$(wildcard $(WIFI_DRIVER_PATH)/*.c))

OBJS = smartconfig.o

OBJS += $(WIFI_DRIVER_SRC)
OBJS += $(WIFI_APP_SRC)


OBJS += wifi_clock.o 


-include appconf.mak
-include $(TOPDIR)/.lib_config
-include $(TOPDIR)/rules.mak


EXTRA_CFLAGS = -I$(TOPDIR)/include -I$(TOPDIR)/lib/lwip/apps/ -include $(TOPDIR)/include/lib_autoconf.h -DCONFIG_DATABUF_START=$(CONFIG_DATABUF_START) -DCONFIG_DMA_SIZE=$(CONFIG_DMA_SIZE) -DCONFIG_USB_CDC_ACM_DEV=$(CONFIG_USB_CDC_ACM_DEV)
EXTRA_CFLAGS += -I$(TOPDIR)/include/wifiClock_app/ -I$(TOPDIR)/include/wifiClock_driver/ -I$(TOPDIR)/lib/lwip/apps/httpd/include 

LIBS = $(LIBPATH)/sys.a

ifdef CONFIG_AT_CMD_FUNC
LIBS += $(LIBPATH)/atcmd.a
endif

ifdef CONFIG_LWIP
LIBS += $(LIBPATH)/lwip.a
endif
LIBS += $(LIBPATH)/wla.a $(LIBPATH)/common.a
ifdef CONFIG_FREERTOS
LIBS += $(LIBPATH)/freertos.a
else
LIBS += $(LIBPATH)/os.a
endif
ifdef CONFIG_LOG
EXTRA_CFLAGS += -I$(TOPDIR)/lib/tinylog/inc
LIBS += $(LIBPATH)/tinylog.a
endif
ifdef CONFIG_JSON
EXTRA_CFLAGS += -I$(TOPDIR)/lib/cJSON
LIBS += $(LIBPATH)/cJSON.a
endif
	
all: all_app

app2: proj.ld $(OBJS:.o=.d) $(LIBS)
	$(ECHO) "\t [LD] $@"
	$(XCC) $(LDFLAGS) -nostdlib -Wl,-e,app_main -T./proj.ld  -o $@ $(OBJS) $(LIBS)
	@echo step0
app2.img: app2 app2.map app2.dis
	$(ECHO) "\t[IMG] $<"
	$(XOC) -R .bss -R .eh_frame -R .dma -O binary  $< $@""
	@echo step1
ifeq "$(CONFIG_WEB_AS_DATA)" "y"

www.img: $(TOPDIR)/lib/www/www_desc.o
	$(Q)$(XOC) -O binary $< $@
	
app_1.img: app2.img im_header.bin $(TOPDIR)/utility/chksum
	@cat im_header.bin app2.img > $@.cs
	@$(TOPDIR)/utility/chksum $@.cs $@
	@rm -f *.bin *.cs app2.img
	
app_2.img: www.img $(LIBPATH)/im_header.c $(TOPDIR)/include/image.h $(TOPDIR)/utility/chksum
	$(Q)$(XCC) -o $@.o -c -I$(TOPDIR)/include -I./ $(LIBPATH)/im_header.c -DIH_RUN=0 \
		 -DIH_SIZE=$(shell stat -c%s $<) -DIH_CHKSUM=0 -DIH_NEXT=0 -DIH_VER=$(__REV) -w
	$(Q)$(XOC) -j .data -O binary $@.o $@.bin
	@cat $@.bin $< $(TOPDIR)/lib/www/pages.img > $@.zcs
	@$(TOPDIR)/utility/chksum $@.zcs $@
	@rm -f $@.zcs $@.o $@.bin www.img
	
app.img: app_1.img app_2.img
	$(ECHO) "\t[IMG] $@"
	@cat app_1.img app_2.img > $@
	@rm app_1.img app_2.img
else
app.img: app2.img im_header.bin $(TOPDIR)/utility/chksum
	@cat im_header.bin app2.img > $@.cs
	@$(TOPDIR)/utility/chksum $@.cs $@
	@rm -f *.bin *.cs app2.img
	@echo step33
endif

all_app: app.img
	
	# $(XOD) -D app.img > $(TOPDIR)/images/$(PROJ_NAME).asm
	cat $(TOPDIR)/images/iot_boot.img $(TOPDIR)/images/config.img app.img > $(TOPDIR)/images/all.img
	mv app.img $(TOPDIR)/images/$(PROJ_NAME).img
	ls -l ../../images 
	sh ../../cp_to_desktop.sh
	@echo step00
clean:
	$(ECHO) "\t[CLEAN]"
	@find . \( -name '*.[osd]' -o -name '*.map' -o -name '*.bin' -o -name '*.dis' -o -name '*.ld' \) -type f -print| xargs rm -f
	@find ../../lib -name *.o -print | xargs rm -f 
	@rm -f app2 im_header app.img.cs
