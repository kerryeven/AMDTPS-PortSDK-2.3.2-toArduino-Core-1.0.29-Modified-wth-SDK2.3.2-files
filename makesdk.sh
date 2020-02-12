#!/bin/bash
#######################################################################################
# RUN FROM GIT BASH SHELL - CD TO SDK DIRECTORY 
#######################################################################################
################# Funtions #############################################################
#######################################################################################
# chkResult
#   - call with $? directly after command you want to check success for...0 = success
#######################################################################################
chkResult(){
    if [ "$?" != "0" ]; then
        echo Either the source or destination does not exist.  Continuing without fixing
        echo   the issue would NOT be a good thing.  Perhaps it would be best to start
        echo   over with a fresh install of both the SDK and the Arduino Core.
        echo
        echo
        echo Mkdir will cause this if the directory was already there so please do say
        echo   yes to continue if you are simply refreshing all of the copies to restore
        echo   files or restore the core to the original state.
        echo
        echo input yes to continue or no to exit the script completely
        read yesno
        if [ "$yesno" == "no" ]; then
            echo got a no so bye bye
            exit 1
        else
            echo got a yes so continuing the script
            return
        fi
    fi    
}

#######################################################################################
# clearScreen - call with number of linefeeds you want
#######################################################################################
clearScreen(){
    num=1
    while [ $num -le $1 ]
        do
            echo 
            let num++
    done
}

#######################################################################################
# Set up Variables
#######################################################################################
clearScreen 20
AMSDK=$PWD
echo 
echo Current Shell Working Directory is = $AMSDK
#Current Directory = /c/Users/kerry/Documents/AmbiqSuiteSDK if started from SDK
#if [ $AMSDK != ""]
if [ ! -d "ambiq_ble" ]; then
  echo
  echo
  echo YOUR AmbiqSuiteSDK DIRECTORY IS NOT IN THE RIGHT PLACE or YOU HAVE NOT CHANGED DIRECTORY IN THE BASH SHELL
  echo TO THE AmbiqSuiteSDK DIRECTORY.  CHECKING FOR SUBDIRECTORY ambiq_ble PRESENT IN SDK VERSION 2.3.2
  echo
  echo THE AmbiqSuiteSDK MUST BE IN:   c:/Users/YourName/Documents/AmbiqSuiteSDK AND YOU MUST START THIS SCRIPT FROM
  echo THERE.  LOOK AT THE Current Directory = LINE ABOVE TO SEE YOUR CURRENT WORKING DIRECTORY.
  echo
  eco  THE SPARKFUN ARDUINO CORE MUST BE IN:
  echo    c:/Users/YourUserName/AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino
  echo
  echo
  echo SCRIPT WILL NOT RUN UNTIL CORE AND SDK ARE INSTALLED TO RECOMMENDED FOLDERS
  echo PERHAPS A MANUAL 22 STEP COPY FROM THE Port SDK2_3_2 To SparkFun Arduino Core1_0_29.doc file is in order??
  echo ....BYE...Having fun yet?
  exit 0
fi
if [ ! -d "../../AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino" ]; then
  echo
  echo
  echo YOUR SPARKFUN ARDUINO CORE IS NOT IN THE RIGHT PLACE or YOU HAVE NOT CHANGED DIRECTORY IN THE BASH SHELL
  echo TO THE AmbiqSuiteSDK DIRECTORY.  ONLY WORKS FOR 1.0.29 CORE!
  echo
  echo THE AmbiqSuiteSDK MUST BE IN:   c:/Users/YourName/Documents/AmbiqSuiteSDK
  echo AND THE SPARKFUN ARDUINO CORE MUST BE IN:
  echo    c:/Users/YourUserName/AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino
  echo
  echo
  echo SCRIPT WILL NOT RUN UNTIL CORE AND SDK ARE INSTALLED TO CORRECT FOLDERS....BYE...Having fun yet?
  exit 0
fi
SFCORE="../../AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino"
echo Current Arduino Core Directory = $SFCORE
#######################################################################################
############## main ###################################################################
#######################################################################################
clearScreen 4
echo CONGRATULATIONS...YOUR DIRECTORIES ARE CORRECT....WOOOOOOSHHHH
echo
echo
#echo cp -r $PWD/mcu/apollo3 ../../AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29/cores/arduino/am_skd_ap3/mcu/apollo3
echo
#exit 1
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/wsf/sources/util/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util 
cp $AMSDK/third_party/exactle/wsf/sources/util/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include/util
result=$?
echo result of group copies of *.h = $result
chkResult $result
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/dm
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app
chkResult $?
echo
echo mv $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_main.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
mv $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_main.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
chkResult $?
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_terminal.c
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/app_terminal.c
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/include/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
cp $AMSDK/third_party/exactle/ble-profiles/include/app/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include
chkResult $?
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include/app_terminal.h
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/include/app_terminal.h
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/gatt/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/gatt
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/gatt/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/gatt
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_db.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_db.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_hw.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_hw.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_ui.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
cp $AMSDK/third_party/exactle/ble-profiles/sources/apps/app/common/app_ui.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/apps/app/generic
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/hci/ambiq/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/hci/ambiq
cp $AMSDK/third_party/exactle/ble-host/sources/hci/ambiq/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/hci/ambiq
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/wsf/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/ws-core/include
cp $AMSDK/third_party/exactle/wsf/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/ws-core/include
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
cp $AMSDK/third_party/exactle/ble-profiles/sources/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/wdxc/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/wdxc
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/wdxc/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/profiles/wdxc
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-profiles/sources/profiles/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/sec/common/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common
cp $AMSDK/third_party/exactle/ble-host/sources/sec/common/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common
chkResult $?
echo
echo rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common/sec_cmac.c
rm $SFCORE/am_sdk_ap3/third_party/exactle/sw/sec/common/sec_cmac.c
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/dm/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/cfg/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/cfg
cp $AMSDK/third_party/exactle/ble-host/sources/stack/cfg/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/cfg
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
cp $AMSDK/third_party/exactle/ble-host/sources/stack/smp/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/smp
chkResult $?
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/att
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/att/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/include/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
mkdir $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.c $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/l2c
chkResult $?
echo
echo cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
cp $AMSDK/third_party/exactle/ble-host/sources/stack/l2c/*.h $SFCORE/am_sdk_ap3/third_party/exactle/sw/stack/include
chkResult $?
echo
echo cp $AMSDK/ambiq_ble/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
cp $AMSDK/ambiq_ble/services/*.* $SFCORE/am_sdk_ap3/third_party/exactle/sw/services
chkResult $?
echo
echo
echo
echo
echo  ALL DONE...READY TO PROGRAM NOW...
echo  Thank you for using EveningWare
