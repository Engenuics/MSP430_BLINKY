/**********************************************************************
File name:	main.c

Main program file for Blinker 
A simple program to sequentially blink an array of 8 LEDs, where 
blinking pattern is selectable by button press.

When complete, the following modes will be available:
1. Off
2. All LEDs on (this will stress the battery!)
3. Random blinking
4. Cyclical blink

**********************************************************************/




/************************ Revision History ****************************
YYYY-MM-DD  Checksum  Comments
-------------------------------------------------------------------------------------------
2009-04-08            First release. 

************************************************************************/

#include "io430.h"
#include "typedef_MSP430.h"
#include "intrinsics.h"
#include "main.h"
#include "blnkyeie-pcb-01.h"


/************************ External Program Globals ****************************/
/* Globally available variables from other files as indicated */
extern fnCode_type BlinkStateMachine;                 /* From blnkyeie-pcb-01.c */
extern fnCode_type G_fCurrentStateMachine;            /* From blnkyeie-pcb-01.c */
extern fnCode_type G_pfPatterns[];                    /* From blnkyeie-pcb-01.c */
 

extern volatile u16 u16GlobalRuntimeFlags;            /* From blnkyeie-pcb-01.c */
extern volatile u16 u16GlobalErrorFlags;              /* From blnkyeie-pcb-01.c */

extern volatile u8 u8GlobalCurrentSleepInterval;      /* From blnkyeie-pcb-01.c */
extern volatile u8 G_u8ActivePattern;                 /* From blnkyeie-pcb-01.c */
extern fnCode_type LG_fPatterns[];                    /* From blnkyeie-pcb-01.c */


/************************ Program Globals ****************************/
/* Global variable definitions intended for scope of multiple files */


/************************ Main Program ****************************/
/* From cstartup.s43, the processor is running from the ACLK, TimerA is running, and I/O lines have been configured. */

int main(void)
{

  /* Enter the state machine where the program will remain unless power cycled */
  __bis_SR_register(GIE);

  while(1)
  {
	  BlinkStateMachine();
  } 
  
} /* end main */

#if 0
/************************ Interrupt Service Routines ****************************/
#pragma vector = PORT3_VECTOR
__interrupt void Port3ISR(void)
/* Handles waking up from low power mode via a button press and returns with processor awake */
{
  /* Debounce the button press for 10 ms -- not a great idea in an ISR but ok for a hack */
  /* 120 / 12,000 = 10 ms */
  for(u16 i = 0; i < 120; i++);
  
  /* If button is still down, consider it a valid press */
  if( !(P3IN & P3_3_BUTTON) )
  {
    /* Advance to the next pattern */
    G_u8ActivePattern++;
    if(G_u8ActivePattern == TOTAL_PATTERNS)
    {
      G_u8ActivePattern = 0;
    }

    G_fCurrentStateMachine = G_pfPatterns[G_u8ActivePattern];
  }
 
  /* Clear the flag, but keep the interrupt active */
  P3IFG &= ~P3_3_BUTTON;
  
  //u8GlobalCurrentSleepInterval = SLEEP_TIME;
  //u8GlobalSleepCounter = 1;
  //TACTL = TIMERA_INT_DISABLE;
  asm("BIC #0x0010,4(SP)"); 
} /* end Port1ISR */
#endif

/*----------------------------------------------------------------------------*/
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerAISR(void)
{
/* Handles waking up from low power mode via TimerA expiration and returns with processor awake */
  
  //u8GlobalCurrentSleepInterval = SLEEP_TIME;
  TACTL = TIMERA_INT_DISABLE;
  asm("BIC #0x0010,0(SP)");
} // end timer_wakeup_isr

