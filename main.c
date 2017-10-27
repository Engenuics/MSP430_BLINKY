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
#include "blink-efwd-01.h"


/************************ External Program Globals ****************************/
/* Globally available variables from other files as indicated */
extern fnCode_type BlinkStateMachine;                 /* From blink-efwd-01.c */
extern fnCode_type G_fCurrentStateMachine;            /* From blink-efwd-01.c */
extern fnCode_type G_pfPatterns[];                    /* From blink-efwd-01.c */
 

extern volatile u16 u16GlobalRuntimeFlags;            /* From blink-efwd-01.c */
extern volatile u16 u16GlobalErrorFlags;              /* From blink-efwd-01.c */

extern volatile u8 u8GlobalCurrentSleepInterval;      /* From blink-efwd-01.c */
extern volatile u8 G_u8ActivePattern;                 /* From blink-efwd-01.c */
extern fnCode_type LG_fPatterns[];                    /* From blink-efwd-01.c */


/************************ Program Globals ****************************/
/* Global variable definitions intended for scope of multiple files */


/************************ Main Program ****************************/
/* From cstartup.s43, the processor is running from the ACLK, TimerA is running, and I/O lines have been configured. */

int main(void)
{
  u8 u8Leds[]                    = {P1_2_LED1,    P1_1_LED5,    P3_6_LED2,    P3_2_LED6,    P3_1_LED3,    P3_0_LED7,    P2_2_LED4,    P1_3_LED8};
  u16*  pu16LedPorts[TOTAL_LEDS] = {(u16*)0x0021, (u16*)0x0021, (u16*)0x0019, (u16*)0x0019, (u16*)0x0019, (u16*)0x0019, (u16*)0x0029, (u16*)0x0021};

  /* Enter the state machine where the program will remain unless power cycled */
  __bis_SR_register(GIE);

#if 0
  /* Allow a button interrupt and timer to wake up sleep */
  P1IFG &= ~P1_0_BUTTON;
  P1IE |= P1_0_BUTTON;				
  TACTL = TIMERA_INT_ENABLE;
  SetTimer(TIME_125MS);
#endif
  
  while(1)
  {
	  //BlinkStateMachine();
    /* If button is down, LEDs are ON */
    if( !(P1IN & P1_0_BUTTON) )
    {
      for(u8 i = 0; i < TOTAL_LEDS; i++)
      {
        *pu16LedPorts[i] |= u8Leds[i];
      }
    }
    else
    {
      for(u8 i = 0; i < TOTAL_LEDS; i++)
      {
        *pu16LedPorts[i] &= ~u8Leds[i];
      }

     /* Enter low power mode */
     //__bis_SR_register(CPUOFF);

    }
  } 
  
} /* end main */


/************************ Interrupt Service Routines ****************************/
#pragma vector = PORT1_VECTOR
__interrupt void Port1ISR(void)
/* Handles waking up from low power mode via a button press and returns with processor awake */
{

    //G_fCurrentStateMachine = G_pfPatterns[G_u8ActivePattern];
 
  /* Clear the flag, but keep the interrupt active */
  P1IFG &= ~P1_0_BUTTON;
  
  //u8GlobalCurrentSleepInterval = SLEEP_TIME;
  //u8GlobalSleepCounter = 1;
  //TACTL = TIMERA_INT_DISABLE;
  asm("BIC #0x0010,4(SP)"); 


} /* end Port1ISR */


/*----------------------------------------------------------------------------*/
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerAISR(void)
{
/* Handles waking up from low power mode via TimerA expiration and returns with processor awake */
  
  //u8GlobalCurrentSleepInterval = SLEEP_TIME;
  //TACTL = TIMERA_INT_DISABLE;
  asm("BIC #0x0010,0(SP)");
} // end timer_wakeup_isr

