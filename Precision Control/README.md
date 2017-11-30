# Lab 6: Precision Control

Sometimes its not enough to control a large amount of voltage and current but instead control a finer more precise small value. The difficulty in this task stems from a microprocessor living in the digital world. To surpass the high or low limitation of a microprocessor, Digital-to-Analog converters are used to output various voltages inbetween. This lab will focus on two different DACs: PWM and R2R. 

## PWM

Creating an analog voltage out of a PWM signal may seem impossible due to its "high and low" digital functionality. However, when an active low pass filter is attached to the output of the PWM, and analog voltage is achieved. The system works by changing the duty cycle of the PWM which changes the ratio from the signal being high to low. By active low pass filter, or integrator, the frequency of the duty cycle is filtered out so a steady analog voltage between the high and low voltage value remains. The code for this part of the lab was very simple since PWM and UART code was taken from previous labs. The only code written combined the two code snippets so that the value recieved over UART immediately changed the duty cycle, therefore changing the analog output voltage. For hardware, a simple low pass filter was created with resistor and capacitor values to achieve a cutoff value lower than 1 kHz. To show the wide range of voltages the PWM DAC could achieve, a triangle wave was created using the PWM DAC and compared to a triangle wave produced by the function generator.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/Oscilloscope%20Screen%20Shots/PWM%20Triangle%20Wave.png)

In the figure above, the yellow is the control waveform and the blue is the generated PWM DAC waveform. The PWM DAC triangle wave is slightly noisy and thicker than the triangle wave. 

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/Oscilloscope%20Screen%20Shots/PWM%20Triangle%20Wave%20Zoom.png)

This is because not enough high frequencies were filtered out. The waveform still oscillates from a high to low value. For future experiments, a better low-pass filter will be used to make this line oscillate less.

## R2R DAC

A R2R DAC is a special DAC that uses an R2R ladder to output the proper analog voltage. An R2R ladder is a resistor network that has as many voltage inputs as voltage bit resolution. For example, if a 3.3 V microprocessor wanted to output voltages between 0 and 3.3 V in 16 different steps, there would be 4 voltage terminals cooresponding to each of the 4 bits. The circuit for a 4 bit R2R ladder is as follows.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/R2R%20DAC/R2R%20ladder.png)

In this lab, an 8 bit R2R ladders using resistor values of 1kohm and 2kohm. Since Vcc was 3.3 V and the ladder had an 8 bit resolution, any multiple of 13 mV (3.3/2^8) can be output. The code for this lab consisted of a function called OutputSet which would take in an 8 bit number as a parameter, process that number and output cooresponding bit values for 8 different GPIO ports. These GPIO ports were then connected to the R2R ladder which would output the desired analog value. UART was also implemented which would recieve an 8 bit input and pass it into the OutputSet function. To show the wide range of voltages the R2R DAC could achieve, a triangle wave was created using the R2R DAC and compared to a triangle wave produced by the function generator.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/Oscilloscope%20Screen%20Shots/R2R%20Triangle%20Wave.png)

In the figure above, the yellow is the control waveform and the green is the R2R DAC generated triangle wave. Both waves are almost identical to each other however, the R2R DAC generated wave seems to randomly and instantaneously spike to Vcc or ground.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/Oscilloscope%20Screen%20Shots/R2R%20Triangle%20Wave%20Zoom.png)

Even with this error, the R2R DAC triangle wave is much more accurate than the PWM DAC triangle wave. 

## Triangle Wave Details

Various triangle waves were generated throughout this lab as seen above. The control triangle wave was always generated from the function generator to achieve maximum acuracy however, code had to be created for the other two DACs. Since the both DAC codes were created with identical triangle waves combined, a function was created for each DAC to set a desired output based on an 8 bit input. This made the triangle wave generation easy by using two for loops. One for loop counted from 0 to 255, setting a new output voltage based on the current count. The other for loop did the same thing but counted down from 255 to 0. This was then repeated and a triangle wave was generated from each DAC. A Step function was also created by using the same code as a triangle wave but the count was incremenetd by 16 instead of 1 and a delay was added in each loop iteration. By changing the increment value to 16, a notable output voltage change created a step and the delay caused the output to hold that value for a certain duration. The step function can be seen below.

https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/Precision%20Control%20/Oscilloscope%20Screen%20Shots/R2R%20Step.png)

## FFT


## Loading Effects
Obviously you are going to be making this type of circuitry to drive something. This introduces the idea of loading effect, wherein your circuit will perform differently based on what is going to be attached to it. For each of these implementations, try placing a variety of resistors from 100 ohms up to see what happens to your output signal and comment on what is happening.

## FFT
You need to measure the output of each one of these along with taking the FFT on the scope of each one. The span on the FFT will need to go from 1kHz to about 50kHz if possible. You then need to compare the integrity of each signal by analyzing the difference in frequency components.

## Bill of Materials
You need to place in the README a Bill Of Materials listing all hardware used as well as link to a Digikey cart which contains the parts needed in the right quantity. You do not need to include things like your F5529 or the breadboard or wires.