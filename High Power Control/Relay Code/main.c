#include <msp430g2553.h>

void main(void){


  // Configure GPIO
    P1OUT &= 0x00;                   //Shutdown Output
    P1DIR |= 0x41;                   // P1.0 and P1.6 output
    P1SEL |= 0x40;                   //Select Pin to output to
    P1SEL2 = 0;

     P1REN |= BIT3;                   // Enable internal pull-up/down resistors
     P1OUT |= BIT3;                   //Select pull-up mode for P1.3
     P1IE |= BIT3;                       // P1.3 interrupt enabled
     P1IES |= BIT3;                     // P1.3 Hi/lo edge
     P1IFG &= ~BIT3;                  // P1.3 IFG cleared



    TA0CCR0 = 1000-1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 500;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2+MC_1;  // SMCLK, up mode

    __bis_SR_register(CPUOFF+GIE);             // Enter LPM0
}

 //Port 5 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    unsigned int i;
   if(TA0CCR1 > 1000){  //Check if 100% Duty Cycle
       TA0CCR1 = 0; //Reset CCR1
   }
   else{
       TA0CCR1 += 100; //Increment CCR1

   }

   for(i=0;i<10000;i++);                //Delay
   P1IFG &= ~BIT3;                     // P1.3 IFG cleared



}
