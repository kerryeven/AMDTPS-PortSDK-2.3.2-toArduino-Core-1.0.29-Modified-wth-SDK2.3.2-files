/*
  BLE <-> Arduino client server comm.  CLIENT VERSION derived from ble_freertos_amdtpc SDK app.
  
  This example demonstrates basic BLE server (peripheral) functionality for the Apollo3 boards.

  How to use this example:
    -    

  TODO:
    Need to turn on two way printing with configuration of the uart for the am_menu_printf function so I 
    can get commands from the terminal to the client for action.  
      - removing Arduino.h include did not work
      - Look at SDK closely to see how uart is configured
*/
/********************************************************************************************************************
 *******************************************************************************************************************/
#include "Arduino.h"
#include "BLE_example.h"
#include "ble_menu.h"
//*****************************************************************************
//
// Required built-ins.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//*****************************************************************************
//
// Standard AmbiqSuite includes.
//
//*****************************************************************************
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"


#ifndef BLE_MENU
#define BLE_MENU
#endif

#ifdef BLE_MENU
void *UART;

//*****************************************************************************
//
// Enable printing to the console.
//
//*****************************************************************************
void
enable_print_interface(void)
{
#if 0
    //
    // Initialize the printf interface for UART output.
    //
    am_bsp_uart_printf_enable();
#else
    //
    // Initialize a debug printing interface.
    //
    am_bsp_uart_printf_enable(); //KHE added - prints to screen
    //am_bsp_itm_printf_enable(); //KHE Original
#endif
    //
    // Print the banner.
    //
    am_util_stdio_terminal_clear();
}

//*****************************************************************************
//
// UART configuration settings.
//
//*****************************************************************************
void
setup_serial(int32_t i32Module)
{
    //
    // Enable a UART to use for the menu.
    //
    const am_hal_uart_config_t sUartConfig =
    {
        //
        // Standard UART settings: 115200-8-N-1
        //
        .ui32BaudRate = 115200,
        .ui32DataBits = AM_HAL_UART_DATA_BITS_8,
        .ui32Parity = AM_HAL_UART_PARITY_NONE,
        .ui32StopBits = AM_HAL_UART_ONE_STOP_BIT,
        .ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE,

        //
        // Set TX and RX FIFOs to interrupt at half-full.
        //
        .ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
                           AM_HAL_UART_RX_FIFO_1_2),

        //
        // Buffers
        //
        .pui8TxBuffer = 0,
        .ui32TxBufferSize = 0,
        .pui8RxBuffer = 0,
        .ui32RxBufferSize = 0,
    };

    am_hal_uart_initialize(0, &UART);
    am_hal_uart_power_control(UART, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_configure(UART, &sUartConfig);

    //
    // Make sure the UART interrupt priority is set low enough to allow
    // FreeRTOS API calls.
    //
    //NVIC_SetPriority(UART0_IRQn, NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY);


    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    //
    // Enable UART RX interrupts manually.
    //
    am_hal_uart_interrupt_clear(UART, AM_HAL_UART_INT_RX | AM_HAL_UART_INT_RX_TMOUT);
    am_hal_uart_interrupt_enable(UART, AM_HAL_UART_INT_RX | AM_HAL_UART_INT_RX_TMOUT);
    NVIC_EnableIRQ(UART0_IRQn);
}


// buffer for printf
static char g_prfbuf[AM_PRINTF_BUFSIZE];

uint32_t
am_menu_printf(const char *pcFmt, ...)
{
    uint32_t ui32NumChars;

    //
    // Convert to the desired string.
    //
    va_list pArgs;
    va_start(pArgs, pcFmt);
    ui32NumChars = am_util_stdio_vsprintf(g_prfbuf, pcFmt, pArgs);
    va_end(pArgs);

    //
    // This is where we print the buffer to the configured interface.
    //
    am_hal_uart_transfer_t sSend =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *)g_prfbuf,
        .ui32NumBytes = ui32NumChars,
        .ui32TimeoutMs = AM_HAL_UART_WAIT_FOREVER,
        .pui32BytesTransferred = 0,
    };

    am_hal_uart_transfer(UART, &sSend);

    //
    // return the number of characters printed.
    //
    return ui32NumChars;
}
#else

uint32_t
am_menu_printf(const char *pcFmt, ...)
{
    return 0;
}

#endif


void setup() {
  //KHE Serial.begin,end,println work but must end b4 am_menu_printf statements or crash.
  //Serial.begin(115200);
  //Serial.println("Serial.println OUTPUT: From beginning of setup");
  //Serial.end();
  //SERIAL_PORT.begin(115200);
  //delay(1000);
  //SERIAL_PORT.printf("Apollo3 Arduino BLE Example. Compiled: %s\n", __TIME__);
  //Serial.println("KHE on our way");
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN,HIGH);

    //
    // Set the clock frequency
    //
    //am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    //
    // Set the default cache configuration
    //
        //Enable debug pins
      //
        //30.6 - SCLK
        //31.6 - MISO
        //32.6 - MOSI
        //33.4 - CSN
        //35.7 - SPI_STATUS
        //41.1 - IRQ (not configured, also SWO)

        //am_hal_gpio_pincfg_t sPincfg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        //sPincfg.uFuncSel = 6;
        //am_hal_gpio_pinconfig(30, sPincfg);
        //am_hal_gpio_pinconfig(31, sPincfg);
        //am_hal_gpio_pinconfig(32, sPincfg);
        //sPincfg.uFuncSel = 4;
        //am_hal_gpio_pinconfig(33, sPincfg);
        //sPincfg.uFuncSel = 7;
        //am_hal_gpio_pinconfig(35, sPincfg);
        //sPincfg.uFuncSel = 1;
        //am_hal_gpio_pinconfig(41, sPincfg);

    //am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    //am_hal_cachectrl_enable();

//#ifndef NOFPU
    //
    // Enable the floating point module, and configure the core for lazy
    // stacking.
    //
    //am_hal_sysctrl_fpu_enable();
    //am_hal_sysctrl_fpu_stacking_enable(true);
//#else
    //am_hal_sysctrl_fpu_disable();
//#endif

//
// Enable printing to the console.
//
//#ifdef AM_DEBUG_PRINTF
    enable_print_interface();
//#endif

    //
    // Initialize plotting interface.
    //
    am_util_debug_printf("am_util_debug_printf output:  FreeRTOS AMDTP Example\n");
    am_bsp_uart_printf_disable();
//#ifdef BLE_MENU
    setup_serial(0);
//#endif
    //Turn the LED Off...Need Pinconfig at least once. KHE
    am_hal_gpio_pinconfig(19, g_AM_HAL_GPIO_OUTPUT); //KHE
    //am_hal_gpio_output_clear(19);  //KHE
    am_hal_gpio_output_set(19);  //KHE
    am_menu_printf("\nam_menu_printf OUTPUT: AMDTP Example Print Debug - Turn Red Light On...KHE\n");

  /********************************************************************************************************************
                   Boot the radio
                    SDK/third_party/exactle/sw/hci/apollo3/hci_drv_apollo3.c
                    = huge program to handle the ble radio stuff in this file
   *******************************************************************************************************************/
  HciDrvRadioBoot(0);
  am_menu_printf("am_menu_printf OUTPUT: Calling exactle_stack_init...\n");

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
  am_menu_printf("am_menu_printf OUTPUT: Finished exactle_stack_init...\n");

#ifdef BLE_MENU
  //am_uart_isr();
#endif

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
  // KHE crash SERIAL_PORT.printf("SERIAL_PORT.printf output calling AmdtpcStart....\n");
  // KHE crash Serial.println("Serial.println output calling AmdtpcStart.....\n");
  am_menu_printf("am_menu_printf OUTPUT: calling AmdtpcStart.....\n");
  // KHE no output am_util_debug_printf("am_util_debug_printf output calling AmdtpcStart....");
  AmdtpcStart();
}

void loop() {
      
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
