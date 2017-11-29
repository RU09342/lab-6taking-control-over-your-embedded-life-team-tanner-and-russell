#include <msp430.h>

void OutputSet(int values);
void OutputInit();
void UARTinit();

int bit0 = 0;
int bit1 = 0;
int bit2 = 0;
int bit3 = 0;
int bit4 = 0;
int bit5 = 0;
int bit6 = 0;
int bit7 = 0;
int values = 0xB0;

/**
* R2R DAC takes 8-bit UART input to control the 8 "switches" of an 8-bit R2R ladder.
* @author Russell Binaco, Jake Fraser, Tanner Smith, Tomas Uribe
* main initializes everything and sets an initial output value. 
**/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    UARTinit();
    OutputInit();
    OutputSet(values);
    while(1){

    }
}

/**
* OutputSet sets the "switches" of the R2R ladder from an 8-bit UART input
* @param the value from the RX buffer.
*
**/
void OutputSet(int values)
{

    bit0 = values & BIT0;
    bit1 = values & BIT1;
    bit2 = values & BIT2;
    bit3 = values & BIT3;
    bit4 = values & BIT4;
    bit5 = values & BIT5;
    bit6 = values & BIT6;
    bit7 = values & BIT7;

    if(bit0){
        P6OUT |= BIT0;
    }
    else{
        P6OUT&= ~BIT0;
    }
    if(bit1){
            P6OUT |= BIT1;
        }
        else{
            P6OUT&= ~BIT1;
        }
    if(bit2){
            P6OUT |= BIT2;
        }
        else{
            P6OUT&= ~BIT2;
        }
    if(bit3){
            P6OUT |= BIT3;
        }
        else{
            P6OUT&= ~BIT3;
        }
    if(bit4){
            P6OUT |= BIT4;
        }
        else{
            P6OUT&= ~BIT4;
        }
    if(bit5){
            P7OUT |= BIT0;
        }
        else{
            P7OUT&= ~BIT0;
        }
    if(bit6){
            P3OUT |= BIT6;
        }
        else{
            P3OUT&= ~BIT6;
        }
    if(bit7){
            P3OUT |= BIT5;
        }
        else{
            P3OUT&= ~BIT5;
        }

}

/**
* OutputInit initializes the 8 pins used as outputs for the DAC.
*
**/
void OutputInit(void)
{

       P6DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
       P7DIR |= BIT0;
       P3DIR |= BIT5 + BIT6;

}

/**
* UARTinit uses 9600 Baud, P3.4 TX and P3.5 RX
*
**/
void UARTinit(void)

{
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
* sets the output according to the received 8-bit value.
*
**/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)

{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    OutputSet(UCA0RXBUF);
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}