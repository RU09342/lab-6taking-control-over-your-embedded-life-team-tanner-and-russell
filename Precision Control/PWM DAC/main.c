#include <msp430.h> 


void setPWM(unsigned int duty);
void setUpPWM();
void pwmUARTInterrupt(unsigned int bufInput);
void setPWM8bit(unsigned int bitDuty);
void setUpUART();


/**
* PWM DAC uses a high-frequency PWM with a duty cycle to output an average DC voltage, acting as a DAC. 
* @author Russell Binaco and Tanner Smith
* 
**/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  setUpPWM();
  setUpUART();

  __bis_SR_register(LPM0_bits + GIE);

  while(1){

  }
}

/**
* The PWM is configured on P1.2 with a CCR0 value of 1000 (1KHz frequency)
* Duty cycle is initialized to 500.
* 
**/
void setUpPWM(){
    P1DIR |= BIT2;                            // P1.2 output
    P1SEL |= BIT2;                            // P1.2 for TA0 CCR1 Output Capture
    TA0CCR0 = 1000;                             // PWM Freq=1000Hz
    TA0CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
    TA0CCR1 = 500;                               // CCR1 50% PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode, 1MhZ
}

/**
* setPWM converts a duty cycle input to work with the CCR0 value of 1000, hence multiplying by 10
* @param the duty cycle input from UART
* 
**/
void setPWM(unsigned int duty){
    if(duty > 100){
        duty = 100;
    }
    duty*=10;
    TA0CCR1 = duty;
}

/**
* pwmUARTInterrupt sets the PWM from a byte received over UART
* @param the value from the RX buffer
* 
**/
void pwmUARTInterrupt(unsigned int bufInput){
    //setPWM(bufInput);
    setPWM8bit(bufInput);
}

/**
* setPWM8bit converts an 8-bit input value to a PWM operating from a CCR0 value of 1000.
* @param the 8-bit duty cycle
* 
**/
void setPWM8bit(unsigned int bitDuty){
    if(bitDuty>255){
        bitDuty = 255;
    }
    bitDuty *=125;
    bitDuty = bitDuty >>5;
    TA0CCR1 = bitDuty;
}

/**
* UARTinit uses 9600 Baud, P3.4 TX and P3.5 RX
*
**/
void setUpUART(){
    P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD

    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
    UCA0BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

/**
* UART RX ISR
* sets the pwm according to the received 8-bit value.
*
**/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)

{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    pwmUARTInterrupt(UCA0RXBUF);
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
