/*
  BLE <-> Android Phone communication
  
  This example demonstrates basic BLE server (peripheral) functionality for the Apollo3 boards.

  How to use this example:
    - Install the nRF Connect app on your mobile device (must support BLE bluetooth)
    - Make sure you select the correct board definition from Tools-->Board 
      (it is used to determine which pin controls the LED)
    - Compile and upload the example to your board with the Arduino "Upload" button
    - In the nRF Connect app look for the device in the "scan" tab. 
        (By default it is called "Amdtp" but you can change that with setLocalName function belos)
    - Once the device is found click "connect"
    - The GATT server will load with 4 services:
      - Generic Access
      - Generic Attribute
      - Device Information
      - Unknown Service
    - For this example I've hooked into the 'Unknown' service. 
        You can click on that pane and it will expand to show Unknown Characteristic
        with an up arrow underlined to the right.  Tap the arrow to see a Write Value Panel.
        Tap down arrow to right of BYTE and scroll to select TEXT.  Tap on new value line and
        input a 1.  Click send to see the BUILTIN_LED light.  0 to turn off.
    - Click 3 down arrows to right of next Unknown Charachteristic puts a slash through the arrows
        and will now display the received value that the chip sent back to you when you sent the value 
        (should be the same).
    
*/
/********************************************************************************************************************
                 Variables Section - How it all Works
                  s_Rcvd is declared extern at top of BLE_example_funcs.cpp.  It set by the function bleRxTxReceived
                  (also defined in BLE_example_funcs.cpp at the end). I have called bleRxTxReceived in amdtps_main.c
                  at line 368 in function amdtps_write_cback which is called when the chip receives a BLE message.

                  The loop function in this file checks to see if s_Rcvd has a new value (new BLE message received)
                  and executes something.  That could be a function to control a motor or a servo, etc., etc.  The
                  first step of whatever is done is to reset s_Rcvd to 100 to stop repeating the commands/function
                  with every loop..which is VERY often.
 *******************************************************************************************************************/
#include "BLE_example.h"
String s_Rcvd = "100"; // if 100 don't bother checking case statements
void setup() {

  SERIAL_PORT.begin(115200);
  delay(1000);
  SERIAL_PORT.printf("Apollo3 Arduino BLE Example. Compiled: %s\n", __TIME__);

  pinMode(LED_BUILTIN, OUTPUT);
  set_led_low();


  /********************************************************************************************************************
                   Boot the radio
                    SDK/third_party/exactle/sw/hci/apollo3/hci_drv_apollo3.c
                    = huge program to handle the ble radio stuff in this file
   *******************************************************************************************************************/
  HciDrvRadioBoot(0);

  /************************************************************************************************
        Initialize the main ExactLE stack: BLE_example_funcs.cpp
        - One time timer
        - timer for handler
        - dynamic memory buffer
        - security
        - HCI host conroller interface
        - DM device manager software
        - L2CAP data transfer management
        - ATT - Low Energy handlers
        - SMP - Low Energy security
        - APP - application handlers..global settings..etc
        - NUS - nordic location services

   ************************************************************************************************/
  exactle_stack_init();
  setLocalName("Amdtp"); //Arduino BLE setLocalName equivalent - up to 20 Char's spaces ok

  /*************************************************************************************************
      Start the "Amdtp" (AmbiqMicro Data Transfer Protocol) profile. Function in amdtp_main.c

       Register for stack callbacks
       - Register callback with DM for scan and advertising events with security
       - Register callback with Connection Manager with client id
       - Register callback with ATT low energy handlers
       - Register callback with ATT low enerty connection handlers
       - Register callback with ATT CCC = client charachteristic configuration array
       - Register for app framework discovery callbacks
       - Initialize attribute server database
       - Reset the device

   ************************************************************************************************/
  AmdtpStart();
}

void loop() {
if (s_Rcvd != "100") //Check if we have a new message
  {
     switch (s_Rcvd.toInt()) {
      case 0:
        s_Rcvd = 100; //reset or will execute with every loop which is VERY often
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
