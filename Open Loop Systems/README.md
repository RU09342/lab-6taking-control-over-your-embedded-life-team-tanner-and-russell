# Lab 6: Open Loop Systems

Open loop systems are systems that do not take in feedback. Open loop is useful when a system does not need to react to changes within the outside environment. This part of the lab aims to introduce open loop systems by controlling the temperature of a 5V regulator using a fan controlled my a microprocessor. Many different variables had to be taken into account when designing this system. All hardware and software considerations are explained in detail below.

![alt text](
https://github.com/RU09342/milestone-2-closed-loop-controls-kool-kats/blob/master/Pics/temp58-40.gif)


## Hardware Design

The hardware design for this system was thought out and planned before any physical
circuit was made. A MSP430G2553 was used because of its low cost and replaceability
in case it was accidentally damaged. The only required hardware device given to
complete the system was a fan so the rest of the circuit had to be designed around
that component. The fan was marked with a part number and average voltage and
current ratings of 24V and 0.255A. Since the MSP430G2553 can only output a voltage
of 3.3V and current of 15mA a high power control system had to be implemented.
Out of the two high power control systems used in previous labs, a NMOS low side
switch was used due to its extremely low current draw and high max switching speed.
The fan was connected to 24V and the drain of the NMOS. The source was attached
to ground and the gate was connected to the P2.1 MSP430G2553 GPIO pin.

The next design decision was how to heat up a 5V regulator. It was suggested that
15v to 20V was input into 5V regulator and a power resistor was attached to the 5V
output. A decision was made to attach 20V to the 5V regulator to heat it up faster.
Also, various sized power resistors were tested to determine the best fit for the desired heat up speed. A 27Ω 5W power resistor was used in the final design.

To measure the temperature of the 5V regulator, a temperature sensor had to be
chosen. The LM35 temperature sensor was used because of its familiarity and availability due to use in previous
labs. The LM35 required a minimum supply voltage of 5V and
output a value of 1◦ C per 10mV. That means the range of 0◦ C to 100◦ C corresponds
to 0V to 1V which can be easily read through an ADC pin of the MSP430G2553. P1.7
was used as the ADC pin.

After all circuit components were decided on, a problem appeared where each component
needed a different voltage. A total of 4 voltages were needed, 3.3V for the processor, 5V for the temperature sensor, 20V for the 5V regulator and 24V for the
fan. However, the bench power supplies used for the milestone could only output 3
different voltages. So in the deepest and darkest hours of the wee night, many pencils
were broken and brains fried, three dusty old OPA548 $15 power op amps started to shine brighter and brighter as a whisper sang through the air ”use the op amps”.
The whisper was from none other than the man, the mease, the legend himself Phil Mease. And so the power op amps were used in a non-inverting amplifier fashion to
boost a 3.3V signal to 5V, 20V and 24V using 0V and 24V as the rails. The power
issue was solved and Phil Mease rested peacefully that night.

The final circuit used for the milestone after all design considerations were finalized
can be seen in the figures below.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Open%20Loop%20Systems/pics/Schematic.JPG)

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Open%20Loop%20Systems/pics/Circuit.jpg)

## Software Design

After finishing the hardware, it was time to integrate everything together with software.
The software consisted of using four basic peripherals: UART, ADC and two Timers.
UART was used to send and receive temperature data from the user. The ADC was
used to read temperature data from the LM35. One timer peripheral was used to set
the sample rate of the ADC. The other timer was used to output a hardware PWM
signal to control the fan speed. Each peripheral had their own set up function to con-
figure any necessary parameters to make the system run. The system was then run
based on interrupts.

When a user sent a desired temperature to the system, the system would read that
data and act appropriately. Whenever a new temperature was received the code
would set the correct duty cycle based on a hard coded duty cycle vs temperature
curve. The curve was decided based on various testing points taken between 0%
duty cycle and 100% duty cycle in increments of 20%. The data can be seen below.

|Fan Duty Cycle | Temperature   |
|:-------------:|:-------------:|
| 0%            | 76.7◦C        |
|20%            | 45.4◦C        |
|40%            | 31.0◦C        |
|60%            | 23.5◦C        |
|80%            | 21.6◦C        |
|100%           | 19.7◦C        |

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Open%20Loop%20Systems/pics/Duty%20Cycle%20vs%20Temp.JPG)

The graph shows the line of best fit is a logarithmic function however, this line does
not fit the points as well as desired. So the data was split up into two linear halves: duty cycles from 0% to 40% and 40% to 100%.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Open%20Loop%20Systems/pics/Linearized%20Curves.JPG)

These two linear functions were implemented into code using an if statement to determine which temperature range the input temperature fell within.

```javascript
void setDutyCycle(int temp){
    if(temp > 76){
        setPWM(0);
    }
    else if(temp > 31){
        long pwm = ((temp * -84) / 100) + 63;
        setPWM(pwm);
    }
    else if(temp > 20){
        long pwm = ((temp * -49) / 10) + 187;
        setPWM(pwm);
    }
    else{
        setPWM(100);
    }
}
```

This created a baseline for duty cycle values corresponding to specific temperatures.
The setPWM function changed a capture and compare register for a timer which also
changed the fan duty cycle.

```javascript
void setPWM(unsigned int bitDuty){
    if(bitDuty>100){
        bitDuty = 100;
    }
    bitDuty = bitDuty * 10;
    TA1CCR1 = bitDuty;
}
```

The final piece of the open loop control system was relaying the current temperature
back to the user. This was done by sampling the ADC at a specific rate and sending
the current data through UART. The ADC interrupt turned off its own interrupt enable
and stored the temperatures in a buffer of size 10.

```javascript
void ADC10Interrupt(){
    ADC10CTL0 &= ~ENC;                        		// ADC10 disabled
    ADC10CTL0 = 0;                            		// ADC10, Vref disabled completely

    if(tempBuf_index < 10){							//adds new value to array for average of 10
        tempBuf[tempBuf_index] = ADC10MEM;	
        tempBuf_index++;
    }
    else{											//computes average of 10 and transmits value; resets array index
        long average = 0;
        int i = 0;
        for(i = 0; i < 10 ; i ++){
            average += tempBuf[i];
        }
        average /= 10;
        long temperature = (average * 330) >> 10;
        UCA0TXBUF = temperature;
        tempBuf_index = 0;
    }
}
```

The purpose of turning off the ADC interrupt enable was to set a desired sample rate.
A timer interrupt was set to fire at a rate of 2Hz and it consisted of re-enabling the
ADC. The purpose of storing each temperature in a buffer was to minimize error. The
lower the sample size, the less accurate the data is. So to increase accuracy, a buffer
of size 10 was filled every 5 seconds and then the average of that data was sent over
UART. If there was a random undesired spike of temperature, the average would minimize
its affect on the sent data. This method for sending data provided an accurate
reading to the user every 5 seconds.
