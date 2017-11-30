# Tanner Smith and Russell Binaco Lab 6

## High Power Control Switching

Microprocessors are one of the most powerful electrical tools in today's world. However, all powerful tools have their limitations. For microprocessors, the limitation is its power input and output. There is a limit to how many amps a single pin can output. This is where switches become a powerful tool. Switches are able to pass high power through its body while also being controlled by a smaller signal. The two switches observed in this lab were relays and MOSFETs.

### Relays

In the lab, a 12 V regulated relay was used to switch on and off a 12 V source to a power resistor. When the relay was switched on the power resistor would dissapate all 12 V and heat up while when the relay was switched off, now power was being dissapated through the resistor. The relay was controlled using a GPIO pin on the MSP430G2553. The output was measured on an oscilloscope and can be seen below.

INSERT OSCILLOSCOPE SCREEN SHOT

But what would a lab be for Russell Trafford if we didn't try and break something. The speed at which the relay was switched on and off was cranked higher and higher until the relay stopped working. This was easy to tell because you could physically hear the switching in the relay stop. The relay stopped working at BLANK Hz which can be seen in the screenshot below. 

INSERT OSCILLOSCOPE SCREEN SHOT

### MOSFETs 

In the lab, a simple 2N7000 NMOS was used in the same fashion as the relay: to dissapate power through a resistor. The MSP430G2553 was used again to switch the MOSFET on and off using a GPIO pin. The oscilloscope screenshot for the finished working circuit can be seen below.

INSERT OSCILLOSCOPE SCREEN SHOT

The MOSFET switching speed was also tested in the same way as the relays and the breakage point can be seen in the screenshot below.

INSERT OSCILLOSCOPE SCREEN SHOT

When comparing the different high power control systems, it is obvious the MOSFET is a better switch in terms of switching speed. When switched at high frequencies, the relay physically slammed shut and would not open, while the MOSFET output started to not change fast enough so it became distorted.

## Precision Control 

In the previous part of the lab, different ways to control high power were analyzed. However, sometimes finer control over how much voltage is being outputted is required. Instead of outputting 0 V or 3.3 V, some specified voltage inbetween can be selected. Two ways to achieve this are a PWM signal or R2R DAC.

### PWM



### R2R DAC



### Triangle Waves


## Open Loop Systems


 

