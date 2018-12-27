#
# Top level Makefile
#
IMAGE:=images
__REV=1125
TOPDIR=$(shell pwd)
IMAGEDIR:=$(TOPDIR)/$(IMAGE)
BOOTFILE:=$(IMAGEDIR)/iot_boot.img
IMAGEFILE:=$(IMAGEDIR)/main.img
MPFILE:=$(IMAGEDIR)/mp_test_uart.img
ALLFILE:=all-firmware.img



LIBPATH=$(TOPDIR)/lib
export TOPDIR LIBPATH __REV
PROJ_PATH := $(shell find proj/ -maxdepth 1 -type d)
PROJ_DIR := $(notdir $(PROJ_PATH))
PROJ_DIR_CLEAN := $(PROJ_DIR:%=clean-%)

BUILT_H	= $(TOPDIR)/include/built_info.h
ifeq ($(BUILT_H),$(wildcard $(BUILT_H)))
	BUILT_C=$(shell sed -n 's/.*_COUNT\s\([0-9]*\)/\1/p' $(BUILT_H))
else
	BUILT_C=0
endif

$(BUILT_H):
	@echo "/*" > $@
	@echo "		built information" >> $@
	@echo "*/" >> $@
	@echo "#define SW_BUILD_TIME \"$(shell date +"%Y%m%d_%H%M")\""  >> $@
	@echo "#define SW_BUILD_COUNT $(shell expr $(BUILT_C) + 1)" >> $@

#
# When we release it to customer, use sdk_toolchain.mak and they will 
# unzip ba-elf_4.7.3.tgz in toolchain dir.
#
# When developing internal, just simply install toolchain in system dir.
#
-include toolchain.mak
-include rules.mak
-include develop.mak
nomp_1m:
	@touch $(IMAGEDIR)/$(ALLFILE)
	tr  "\000" "\377" < /dev/zero | dd of=$(IMAGEDIR)/$(ALLFILE) ibs=1 count=1048576
	@dd if=$(BOOTFILE) of=$(IMAGEDIR)/$(ALLFILE) conv=notrunc
	@dd if=$(IMAGEFILE) of=$(IMAGEDIR)/$(ALLFILE) ibs=324K obs=128K count=1 seek=1 conv=notrunc
	@dd if=$(MPFILE) of=$(IMAGEDIR)/$(ALLFILE) ibs=128K obs=896K count=1 seek=1 conv=notrunc

	
$(TOPDIR)/utility/chksum: $(TOPDIR)/utility/chksum.c
	rm -rf $(TOPDIR)/utility/chksum
	gcc -o $@ $<

$(PROJ_DIR) : $(BUILT_H) $(TOPDIR)/utility/chksum
	@echo step001
	$(Q)$(MAKE) -C proj/$@ all
	@echo step000

$(PROJ_DIR_CLEAN) :
	$(Q)$(MAKE) -C proj/$(@:clean-%=%) clean

update_all_defconfig:
	for file in `find config -type f \( -name *_lib_defconfig -and ! -name \*svn\* \)`; do cp $$file .lib_config; utility/mconf utility/lib_config.in lib save; cp .lib_config $$file; done
	cp config/asic_rlib_defconfig .rlib_config; utility/mconf utility/rlib_config.in rlib save; cp .rlib_config config/asic_rlib_defconfig
.PHONY: $(BUILT_H)
