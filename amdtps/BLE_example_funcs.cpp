#include "BLE_example.h"
#include "string.h"
//*****************************************************************************
//
// Arduino Variables
//
//*****************************************************************************
extern String s_Rcvd;
//*****************************************************************************
//
// Arduino BLE Core Functions
//
//*****************************************************************************
uint8_t amdtpScanDataDisc[22]; //define AdvName Array


//*****************************************************************************
//
// Timer for buttons.
//
//*****************************************************************************
wsfHandlerId_t ButtonHandlerId;
wsfTimer_t ButtonTimer;

//*****************************************************************************
//
// Timer configuration macros.
//
//*****************************************************************************
#define MS_PER_TIMER_TICK           10  // Milliseconds per WSF tick
#define CLK_TICKS_PER_WSF_TICKS     5   // Number of CTIMER counts per WSF tick.

//*****************************************************************************
//
// WSF buffer pools.
//
//*****************************************************************************
#define WSF_BUF_POOLS               4

// Important note: the size of g_pui32BufMem should includes both overhead of internal
// buffer management structure, wsfBufPool_t (up to 16 bytes for each pool), and pool
// description (e.g. g_psPoolDescriptors below).

// Memory for the buffer pool
static uint32_t g_pui32BufMem[(WSF_BUF_POOLS*16
         + 16*8 + 32*4 + 64*6 + 280*8) / sizeof(uint32_t)];

// Default pool descriptor.
static wsfBufPoolDesc_t g_psPoolDescriptors[WSF_BUF_POOLS] =
{
    {  16,  8 },
    {  32,  4 },
    {  64,  6 },
    { 280,  8 }
};

////*****************************************************************************
////
//// Enable printing to the console.
////
////*****************************************************************************
//void
//enable_print_interface(void)
//{
//    // //
//    // // Initialize a debug printing interface.
//    // //
//    // am_bsp_itm_printf_enable();
//    am_bsp_uart_printf_enable();
//}

//*****************************************************************************
//
// Tracking variable for the scheduler timer.
//
//*****************************************************************************
uint32_t g_ui32LastTime = 0;
extern "C" void radio_timer_handler(void);



//*****************************************************************************
//
// Initialization for the ExactLE stack.
//
//*****************************************************************************
void exactle_stack_init(void){
    wsfHandlerId_t handlerId;

    //
    // Set up timers for the WSF scheduler.
    //
    scheduler_timer_init();
    WsfTimerInit();

    //
    // Initialize a buffer pool for WSF dynamic memory needs.
    //
    WsfBufInit(sizeof(g_pui32BufMem), (uint8_t*)g_pui32BufMem, WSF_BUF_POOLS, g_psPoolDescriptors);

    //
    // Initialize security.
    //
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    //
    // Set up callback functions for the various layers of the ExactLE stack.
    //
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
    DmAdvInit();
    DmConnInit();
    DmConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();
    AttcInit();

    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();
    SmprScInit();
    HciSetMaxRxAclLen(251);

    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(AmdtpHandler);
    AmdtpHandlerInit(handlerId);

//    ButtonHandlerId = WsfOsSetNextHandler(button_handler);

    handlerId = WsfOsSetNextHandler(HciDrvHandler);
    HciDrvHandlerInit(handlerId);
}



//*****************************************************************************
//
// Set up a pair of timers to handle the WSF scheduler.
//
//*****************************************************************************
void
scheduler_timer_init(void)
{
    //
    // One of the timers will run in one-shot mode and provide interrupts for
    // scheduled events.
    //
    am_hal_ctimer_clear(0, AM_HAL_CTIMER_TIMERA);
    am_hal_ctimer_config_single(0, AM_HAL_CTIMER_TIMERA,
                                (AM_HAL_CTIMER_INT_ENABLE |
                                 AM_HAL_CTIMER_LFRC_512HZ |
                                 AM_HAL_CTIMER_FN_ONCE));

    //
    // The other timer will run continuously and provide a constant time-base.
    //
    am_hal_ctimer_clear(0, AM_HAL_CTIMER_TIMERB);
    am_hal_ctimer_config_single(0, AM_HAL_CTIMER_TIMERB,
                                 (AM_HAL_CTIMER_LFRC_512HZ |
                                 AM_HAL_CTIMER_FN_CONTINUOUS));

    //
    // Start the continuous timer.
    //
    am_hal_ctimer_start(0, AM_HAL_CTIMER_TIMERB);

    //
    // Enable the timer interrupt.
    //
    am_hal_ctimer_int_register(AM_HAL_CTIMER_INT_TIMERA0, radio_timer_handler);
    am_hal_ctimer_int_enable(AM_HAL_CTIMER_INT_TIMERA0);
    NVIC_EnableIRQ(CTIMER_IRQn);
}

//*****************************************************************************
//
// Calculate the elapsed time, and update the WSF software timers.
//
//*****************************************************************************
void
update_scheduler_timers(void)
{
    uint32_t ui32CurrentTime, ui32ElapsedTime;

    //
    // Read the continuous timer.
    //
    ui32CurrentTime = am_hal_ctimer_read(0, AM_HAL_CTIMER_TIMERB);

    //
    // Figure out how long it has been since the last time we've read the
    // continuous timer. We should be reading often enough that we'll never
    // have more than one overflow.
    //
    ui32ElapsedTime = (ui32CurrentTime >= g_ui32LastTime ?
                       (ui32CurrentTime - g_ui32LastTime) :
                       (0x10000 + ui32CurrentTime - g_ui32LastTime));

    //
    // Check to see if any WSF ticks need to happen.
    //
    if ( (ui32ElapsedTime / CLK_TICKS_PER_WSF_TICKS) > 0 )
    {
        //
        // Update the WSF timers and save the current time as our "last
        // update".
        //
        WsfTimerUpdate(ui32ElapsedTime / CLK_TICKS_PER_WSF_TICKS);

        g_ui32LastTime = ui32CurrentTime;
    }
}

//*****************************************************************************
//
// Set a timer interrupt for the next upcoming scheduler event.
//
//*****************************************************************************
void
set_next_wakeup(void)
{
    bool_t bTimerRunning;
    wsfTimerTicks_t xNextExpiration;

    //
    // Stop and clear the scheduling timer.
    //
    am_hal_ctimer_stop(0, AM_HAL_CTIMER_TIMERA);
    am_hal_ctimer_clear(0, AM_HAL_CTIMER_TIMERA);

    //
    // Check to see when the next timer expiration should happen.
    //
    xNextExpiration = WsfTimerNextExpiration(&bTimerRunning);

    //
    // If there's a pending WSF timer event, set an interrupt to wake us up in
    // time to service it. Otherwise, set an interrupt to wake us up in time to
    // prevent a double-overflow of our continuous timer.
    //
    if ( xNextExpiration )
    {
        am_hal_ctimer_period_set(0, AM_HAL_CTIMER_TIMERA,
                                 xNextExpiration * CLK_TICKS_PER_WSF_TICKS, 0);
    }
    else
    {
        am_hal_ctimer_period_set(0, AM_HAL_CTIMER_TIMERA, 0x8000, 0);
    }

    //
    // Start the scheduling timer.
    //
    am_hal_ctimer_start(0, AM_HAL_CTIMER_TIMERA);
}

//*****************************************************************************
//
// Interrupt handler for the CTIMERs
//
//*****************************************************************************
extern "C" void radio_timer_handler(void){
    // Signal radio task to run

    WsfTaskSetReady(0, 0);
}

////*****************************************************************************
////
//// Poll the buttons.
////
////*****************************************************************************
//void
//button_handler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
//{
//    //
//    // Restart the button timer.
//    //
//    WsfTimerStartMs(&ButtonTimer, 10);
//
//    //
//    // Every time we get a button timer tick, check all of our buttons.
//    //
//    am_devices_button_array_tick(am_bsp_psButtons, AM_BSP_NUM_BUTTONS);
//
//    //
//    // If we got a a press, do something with it.
//    //
//    if ( am_devices_button_released(am_bsp_psButtons[0]) )
//    {
//        am_util_debug_printf("Got Button 0 Press\n");
//        AppUiBtnTest(APP_UI_BTN_1_SHORT);
//    }
//
//    if ( am_devices_button_released(am_bsp_psButtons[1]) )
//    {
//        am_util_debug_printf("Got Button 1 Press\n");
//        AppUiBtnTest(APP_UI_BTN_1_SHORT);
//    }
//}
//
////*****************************************************************************
////
//// Sets up a button interface.
////
////*****************************************************************************
//void
//setup_buttons(void)
//{
//    //
//    // Enable the buttons for user interaction.
//    //
//    am_devices_button_array_init(am_bsp_psButtons, AM_BSP_NUM_BUTTONS);
//
//    //
//    // Start a timer.
//    //
//    ButtonTimer.handlerId = ButtonHandlerId;
//    WsfTimerStartSec(&ButtonTimer, 2);
//}

//*****************************************************************************
//
// Interrupt handler for the CTIMERs
//
//*****************************************************************************
extern "C" void am_ctimer_isr(void){
    uint32_t ui32Status;

    //
    // Check and clear any active CTIMER interrupts.
    //
    ui32Status = am_hal_ctimer_int_status_get(true);
    am_hal_ctimer_int_clear(ui32Status);

    //
    // Run handlers for the various possible timer events.
    //
    am_hal_ctimer_int_service(ui32Status);
}

//*****************************************************************************
//
// Interrupt handler for BLE
//
//*****************************************************************************
extern "C" void am_ble_isr(void){
    HciDrvIntService();
}




// ****************************************
// 
// Debug print functions
// 
// ****************************************
#define DEBUG_UART_BUF_LEN 256

extern "C" void debug_print(const char* f, const char* F, uint16_t L){
  SERIAL_PORT.printf("fm: %s, file: %s, line: %d\n", f, F, L);
}

extern "C" void debug_printf(char* fmt, ...){
#ifdef DEBUG
    char    debug_buffer        [DEBUG_UART_BUF_LEN];
    va_list args;
    va_start (args, fmt);
    vsnprintf(debug_buffer, DEBUG_UART_BUF_LEN, (const char*)fmt, args);
    va_end (args);

    SERIAL_PORT.print(debug_buffer);
#endif //DEBUG  
}

/*************************************************************************************************/
/*!
    \fn     setLocalName

    \brief  Parses global ino file string into BLE advertising name in amdtpScanDataDisc structure

    \param  s_Name = String concatinated to max 20 char's ie: setLocalName("ThisIsTheName");

    \called Arduino setup function, first thing, just sets name in structure for future use

    \return None.

    \note:  Must comment out amdtpScanDataDisc array definition in amdtp_main.c and declare it 
            as external in amdtp_api.h ie: extern uint8_t amdtpScanDataDisc[22];
*/
/*************************************************************************************************/
// ****************************************
// 
// C-callable BLE functions
// 
// ****************************************
void setLocalName( String s_Name ){
  //following line works - came from amtdp_main.c close to beginning
  //extern const uint8_t amdtpScanDataDisc[22] = {8,DM_ADV_TYPE_LOCAL_NAME,'R','E','V','1','_','3','W'};
  
  int n = s_Name.length(); //Length of string set in ino file as global String (doesn't include teminator) 
  if ( n > 20) {
    s_Name = s_Name.substring(0,20); //Only allow up to 20 char's
    n = s_Name.length(); //need length to change to uint8_t (new length after substring)
  }
  int i = 2; //1st char(0) is adv type char + # of char's, 2nd char(1)  is adv type
  char c_AdvName[n+1]; //create Array large enough to hold chars of string + teminator char 
  strcpy(c_AdvName, s_Name.c_str()); //copy string to char array
  //1st char in array must be = to # char's in name (no terminator) + 1 for adv. type.  If off by even 1, 
  //  the name will not show up in scans
  amdtpScanDataDisc[0] = n+1; //set first array pos to # of char's + 1 for the type of adv packet
  amdtpScanDataDisc[1] = DM_ADV_TYPE_LOCAL_NAME; // type of adv packet
  for(i=2; i<n+2; i++){ //Add the char's from the string to the amdtpScanDataDisc array starting at pos 2
    amdtpScanDataDisc[i] = c_AdvName[i-2];
  }
}
// ****************************************
// 
// C-callable led functions
// 
// ****************************************
extern void set_led_high( void ){
  digitalWrite(LED_BUILTIN, HIGH);
}

extern void set_led_low( void ){
  digitalWrite(LED_BUILTIN, LOW);
}
extern uint8_t bleRxTxReceived(uint8_t * buf, uint16_t len)
{
  s_Rcvd = (char*) buf; //KHE convert msg into string
  Serial.print("Message Received  ");
  Serial.println(s_Rcvd);
  return * buf;
}
