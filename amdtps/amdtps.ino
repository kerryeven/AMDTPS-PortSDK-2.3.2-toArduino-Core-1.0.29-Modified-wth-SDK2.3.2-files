/*
  Analog to digital conversion
  By: Owen Lyke
  SparkFun Electronics
  Date: Aug 27, 2019
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates basic BLE server (peripheral) functionality for the Apollo3 boards.

  How to use this example:
    - Install the nRF Connect app on your mobile device (must support BLE bluetooth)
    - Make sure you select the correct board definition from Tools-->Board 
      (it is used to determine which pin controls the LED)
    - Compile and upload the example to your board with the Arduino "Upload" button
    - In the nRF Connect app look for the device in the "scan" tab. 
        (By default it is called "Artemis BLE" but you can change that below)
    - Once the device is found click "connect"
    - The GATT server will load with 5 services:
      - Generic Access
      - Generic Attribute
      - Link Loss
      - Immediate Alert
      - Tx Power
    - For this example we've hooked into the 'Immediate Alert' service. 
        You can click on that pane and it will expand to show an "upload"  button.
        Use the upload button to write one of three values (0x00, 0x01, or 0x02)
    - When you send '0x00' (aka 'No alert') the LED will be set to off
    - When you send either '0x01' or '0x02' the LED will be set to on
*/
#include "BLE_example.h"
String s_Rcvd = "100"; // if 100 don't bother checking case statements

void setup() {

  SERIAL_PORT.begin(115200);
  delay(1000);
  SERIAL_PORT.printf("Apollo3 Arduino BLE Example. Compiled: %s\n", __TIME__);

  pinMode(LED_BUILTIN, OUTPUT);
  set_led_low();


  //
  // Boot the radio.
  //
  HciDrvRadioBoot(0);

  //
  // Initialize the main ExactLE stack.
  //
  exactle_stack_init();

  //
  // Start the "Amdtp" profile.
  //
  AmdtpStart();
}

void loop() {
if (s_Rcvd != "100") //Check if we have a new message
  {
     switch (s_Rcvd.toInt()) {
      case 0:
        s_Rcvd = 100;
        digitalWrite(LED_BUILTIN,0);
        break;
      case 1:
        s_Rcvd = 100;
        digitalWrite(LED_BUILTIN,1);
        break;
    }
}
      
      //
      // Calculate the elapsed time from our free-running timer, and update
      // the software timers in the WSF scheduler.
      //
      update_scheduler_timers();
      wsfOsDispatcher();

      //
      // Enable an interrupt to wake us up next time we have a scheduled event.
      //
      set_next_wakeup();

      am_hal_interrupt_master_disable();

      //
      // Check to see if the WSF routines are ready to go to sleep.
      //
      if ( wsfOsReadyToSleep() )
      {
          am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
      }
      am_hal_interrupt_master_enable();
}
