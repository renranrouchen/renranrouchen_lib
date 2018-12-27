include $(TOPDIR)/.lib_config

OBJS += gpio.o serial.o app_cmd.o i2c.o madc.o pwm.o
OBJS += cfg_api.o api_defparam.o
OBJS +=  lib_strpbrk.o lib_strstr.o lib_strutil.o lib_strtokr.o http.o ota.o
ifdef CONFIG_AT_CMD_FUNC
OBJS += atcmd_common.o
endif

include $(TOPDIR)/rules.mak
all: $(LIBA)
clean:
	find -regex '.*\.[oasd]' -print | xargs rm -f
