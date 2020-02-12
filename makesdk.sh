#!/bin/bash
#######################################################################################
# RUN FROM GIT BASH SHELL - CD TO SDK DIRECTORY 
# Set up Variables
#######################################################################################
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
echo
SFCORE="../../AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino"
echo Current Arduino Core Directory = $SFCORE
AMSDK=$PWD
echo 
echo Current SDK Directory = $AMSDK
#Current Directory = /c/Users/kerry/Documents/AmbiqSuiteSDK if started from SDK
#######################################################################################
echo
echo
echo
#echo cp -r $PWD/mcu/apollo3 ../../AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino/am_skd_ap3/mcu/apollo3
echo
#exit 1
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
echo
echo cp $AMSDK/third_party/exactle/wsf/sources/util/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util 
cp $AMSDK/third_party/exactle/wsf/sources/util/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app
echo
echo mv $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_main.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
mv $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_main.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_terminal.c
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_terminal.c
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/include/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
cp $AMSDK/third_party/exactle/ble-profiles/include/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include/app_terminal.h
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include/app_terminal.h
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/gatt/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/gatt
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/gatt/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/gatt
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_db.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_db.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_hw.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_hw.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_ui.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_ui.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/hci/ambiq/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/hci/ambiq
cp $AMSDK/third_party/exactle/ble-host/sources/hci/ambiq/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/hci/ambiq
echo
echo cp $AMSDK/third_party/exactle/wsf/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/ws-core/include
cp $AMSDK/third_party/exactle/wsf/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/ws-core/include
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
cp $AMSDK/third_party/exactle/ble-profiles/sources/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/wdxc/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/wdxc
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/wdxc/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/wdxc
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/sec/common/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common
cp $AMSDK/third_party/exactle/ble-host/sources/sec/common/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common/sec_cmac.c
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common/sec_cmac.c
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/cfg/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/cfg
cp $AMSDK/third_party/exactle/ble-host/sources/stack/cfg/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/cfg
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo cp $AMSDK/third_party/exactle/ble-host/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
echo
echo cp $AMSDK/ambiq_ble/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
cp $AMSDK/ambiq_ble/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
echo
echo
echo
echo
echo
echo  Thank you for using EveningWare
