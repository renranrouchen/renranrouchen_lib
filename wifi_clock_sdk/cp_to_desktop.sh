#!/bin/bash
# Copyright(c) 2016-2100.  zwxf.  All rights reserved.
#
#   FileName:     cp_to_desktop.sh
#   Author:       zwxf
#   Email:        zhiwei1007@qq.com
#   DateTime:     2017-06-01 11:39:06
#   ModifiedTime: 2017-06-02 18:53:46

JLLPATH="$(which $0)"
JLLPATH="$(dirname ${JLLPATH})"
#source ${JLLPATH}/BashShellLibrary

### Color Echo Usage ###
# Lfn_Sys_ColorEcho ${CvFgRed} ${CvBgWhite} "hello"
# echo -e "hello \033[0m\033[31m\033[43mworld\033[0m"

echo =============================starting copy img===========================
cd ../../
SDK_PATH=$(pwd)
if [ $? -eq 0 ]
then
rm -rf /mnt/hgfs/Desktop/wifi_clock.img
fi
cp $SDK_PATH/images/wifi_clock.img /mnt/hgfs/Desktop/OTA



