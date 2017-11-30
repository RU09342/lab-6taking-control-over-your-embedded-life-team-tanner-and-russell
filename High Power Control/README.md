# Lab 6: "High Power" Control

Microprocessors are some of the most powerful electrical tools in today's world. However, all powerful tools have their limitations. For microprocessors, the limitation is their power input and output. There is a limit to how many amps a single pin can output. This is where switches become a powerful tool. Switches are able to pass high power through their bodies while also being controlled by smaller signals. The two switches observed in this lab were relays and MOSFETs.

## Software

The software portion of this lab was very simple. A 50% duty cycle, 1 Hz PWM signal was generated using a MSP430G2553. This code was reused from lab 4. The only parameters that were changed were timing registers. This code can be found in the Relay Code folder and was used for both the relay and MOSFET circuit. 

## Hardware

### Relay

To observe a relay switch, a 12 V regulated relay was used to switch on and off a 12 V source to a power resistor. The relay was connected as follows: the normally closed (NC) terminal was attached to 12 V, the normally open (NO) terminal was attached to ground, and the common terminal was attached to the power resistor which was connected to ground. This made it so the power resistor was normally connected to ground and only connected to power when the relay was switched. The 3.3 V input from the microprocessor controlled the state of the relay. An oscilloscope screenshot was taken to show the relationship between the relay input and the power across the resistor.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/High%20Power%20Control/Oscilloscope%20Screen%20Shots/Relay%20Control.png)

### MOSFET

To observe a MOSFET switch, a simple 2N7000 NMOS was used in a low-side switch fashion to dissipate power through a power resistor. The MSP430G2553 GPIO PWM pin was attached to the gate of the NMOS and 12 V was attached to the drain. A power resistor connected to ground was attached to the source of the NMOS. Using the MOSFET in saturation, the NMOS was either fully on or fully off which either allowed all 12 V to be passed through the resistor or no voltage at all. Since an NMOS was used, the power across the resistor will be high when the gate is low and vice versa. An oscilloscope screenshot was taken to show the relationship between the gate input and the power across the resistor.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/High%20Power%20Control/Oscilloscope%20Screen%20Shots/MOSFET%20Control.png)

## Deliverables

What would a lab be for Russell Trafford if we didn't try and break something. To compare and contrast Relay vs MOSFET switches, the maximum switching speed of each system was found by cranking up the PWM frequency until the system stopped working. This would be a pain to do with the MSP430G2553 because the code would have to be changed and reuploaded incrementally until the breaking point is reached. Instead, a function generator was used to input a square wave into each system and easily increase the switching frequency. The relay system broke at 130 Hz which can be seen in the oscilloscope screenshot below.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/High%20Power%20Control/Oscilloscope%20Screen%20Shots/Relay%20Break%20Frequency.png)

The physical switching system of the relay can not keep up with the fast input frequency, so it eventually just remains normally closed. The oscilloscope screenshot above shows that the relay is almost always normally closed. If the frequency was incremented any higher, the relay remained shut and the oscilloscope showed a flatline at zero for the voltage across the resistor.

The MOSFET acted very differently. The MOSFET switch never really stopped working, but instead became very distorted. An oscilloscope screenshot was taken at 200 Hz which shows this distortion.

![alt text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-team-tanner-and-russell/blob/master/High%20Power%20Control/Oscilloscope%20Screen%20Shots/MOSFET%20Break%20Frequency.png)


The highs and lows of the voltage across the resistor can still be seen very clearly. As the frequency of the input is increased, the input and output become more and more distorted but still remain high and low. Overall, the MOSFET has a higher max switching speed compared to the relay by far.

As for current draw of each switching system, they both were found to be extremely low. It is common Electronics knowledge that there is no current draw through the gate of a MOSFET. This allows a user to use their microprocessor freely without worry of drawing too much pin current and frying their processor.