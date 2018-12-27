libdirs-$(CONFIG_AXTLS)	+= axtls
libdirs-$(CONFIG_WOLFSSL)	+= wolfssl
libdirs-$(CONFIG_FREERTOS)	+= freertos
libdirs-y				+= common
libdirs-y				+= sys
libdirs-$(CONFIG_HTTPD)	+= http
libdirs-$(CONFIG_WEB_AS_DATA)	+= www
libdirs-$(CONFIG_AT_CMD_FUNC)	+= atcmd
libdirs-$(CONFIG_WLA) 	+= wla
libdirs-$(CONFIG_OS)	+= os
libdirs-$(CONFIG_LWIP)	+= lwip
libdirs-$(CONFIG_DEV)	+= dev
libdirs-$(CONFIG_LOG)	+= tinylog
libdirs-$(CONFIG_JSON)  += cJSON

