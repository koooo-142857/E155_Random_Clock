---
layout: page
title: Design
permalink: /design/
---
# FPGA Design
Fig. 6 top level FPGA schematic
<div style="text-align: left">
  <img src="../assets/schematics/FPGA.png" alt="logo" width="1000" />
</div>

There are three significant design blocks in the FPGA design for the random clock: the SPI shift register, the Neopixel driver, and the  multiplexed display module. You may find other helper modules in the diagrams and source codes, but they are less important in making the Random Clock function. 

## SPI Shift Registers
The SPI Shift Register, shown in Fig. 6, is responsible for receiving SPI information on the location of each number on the clock. This block is asynchronous in that it is driven by the serial clock, not the on board clock. On each posedge, one bit from the sdi line is shifted into the clock_num bus. Since the SPI signals are 96 bits long (12 numbers on a clock, each number is represented as an 8 bit char, hence 8 times 12 is 96), the clock_num internal signal is also 96 bits long. The bit_rearrange module is a simple combinational logic that extracts each clock digit from the clock_num bus. 
 
## Mutiplexed Display
Fig. 7 Num_Distribute schematic
<div style="text-align: left">
  <img src="../assets/schematics/Num_Distribute.JPG" alt="logo" width="1000" />
</div>
 
The Num_Distribute module contains the display multiplexer. In order to drive 24 different 7-seg displays, a traditional combinational logic circuit that simply sends the digit to display will have trouble delivering enough current to the seven-seg displays. Therefore, a multiplexed display is used, where each display only flashes on for a quick moment, before the signal is given on an adjacent display. Because this multiplexing process happens quickly (>100Hz), the human eye cannot detect the discrete signals, and will instead see all displays lit up simultaneously. In the schematic above, the on board 48MHz clock is slowed to around 100Hz with a clock divider, and that divider is sent to a simple FSM called en_rotate, which simply cycles through each enable unconditionally in a loop. These en [4:0] signals are both  outputs to the physical decoder module, and the select line for the mux, which chooses which number gets to be displayed at a seven-segment display. 
 
Note that although the neopixel driver is instantiated here, it does not interact with any other modules. Its details will be given in the next section. 


## Neopixel Driver
Fig. 8 Neopixel Driver Schematic

<div style="text-align: left">
  <img src="../assets/schematics/NeoPixels.JPG" alt="logo" width="1000" />
</div>

The neopixel driver is perhaps the most interesting module in all of Random Clock. For all this project is concerned, the neopixel stripes takes 24 bit streams of data to represent GRB values displayed at a single LED group, creating a color. However, each bit's polarity is represented by a single cycle of 800kHz square wave. If the duty cycle is 66%, the bit is a high bit. If the duty cycle is 33%, the bit is a low bit. For an example, to display a full red color along the strip, the following signal needs to be sent to the neopixel strip. 

Fig. 9 Example neopixel waveform. Note: the 256 in the image should have been 255. 
<div style="text-align: left">
  <img src="../assets/schematics/FullRed.JPG" alt="logo" width="1000" />
</div>

Therefore, to display a single color at all times on the stripe, we need to generate the waveform above on one of the FPGA output lines. This is accomplished in two steps. First of all, the high and low encoder modules continuously generate all high bits and all low bits at their single outputs. This is done by using a counter within each module, and letting the output be high or low based off of a threshold value. For the high encoder, the counter counts to 60, and values less than 40 will generate a high output. For the low encoder, the counter also counts to 60, but values less than 20 will generate a high output. These numbers are calculated with the 48MHz onboard clock frequency in mind, so that the PWMs outputs of these modules are 800kHz. 

Being able to generate 1's and 0's is the first step, knowing when to generate the 1's 0's is the next. This selection is done by another PWM generator called Color_Encoder. It along with the mux, selects whether the next neopixel bit to send is a high or a low. For sending the GRB = 0xff00 above, the color encoder will send out 0's for 8 PWM cycles, send out 1's for 8 PWM cycles, and send out 0's for another 8 cycles. 

Lastly, the RST_control is responsible for creating the rainbow effect. The neopixel needs a periodic quiet period, where there are no signals sent in, in order to know the reading frames for the signals that came in. Abusing this fact allows us to very elegantly produce the rainbow effect in very little additional hardware. The RST_control is high (0.8s) for the majority of the time, where the data is transparent. When RST_control is low (for 60us), the datastream is stopped. By making the datastream stop one PWM cycle after a complete 24 bit dataframe is sent, the next datastream read by the neopixels will be shifted to the left, resulting in a different color being produced. After a few datastream stops, the color on the Neopixels will have completely changed. See the chart below for a demonstration. 

| time (s) | # of stops generated | LED dataframe (G,R,B) | Rendered Color |
| 0 | 0 | 0b0000 0000 1111 1111 0000 0000 | (0,255,0) |
| 0.8 | 1 | 0b0000 0001 1111 1110 0000 0000 | (1,254,0) |
| 1.6 | 2 | 0b0000 0011 1111 1100 0000 0000 | (3,252,0) |
...





# MCU Design

The Microcontroller that was used was the STM32L432KC (reference manual: https://pages.hmc.edu/brake/class/e155/fa22/assets/doc/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).  The code consisted of a main function, including everything needed  to initialize the functions present, and step through timestamps, as well as additional functions and header files for random number generation and SPI.  SMSIS was used across all files. 

 

####  Time Code

The time was calculated by creating a timing function that waited for one second.  This function was developed by turning a pin on and off, and timing a loop with a volatile long counter such that it read as 1s on the oscilloscope.  Future iterations would use a timer to precisely measure time.
This suffices for a simple clock design, but would cause drift of the time displayed over long periods of time, due to the imprecision of the measuring. 

 

The time code relies on tracking time with three variables: sec (second), min (minute), and hour within a while loop.  The loop first checks to see if a minute has passed, and if so, if five minuites or an hour, and possibly a day have passed.  If they have passed, it uses getSequence12_SPI(); to generate new hour locations, and min_AtoB(start, end) and hr_AtoB(start,end) to adjust the clock appropriately given the time it is.  The time code then adjusts and increments the second, minute, and hour time appropriately.  The time that this processing takes is assumed to be negligible in comparison to the second delay, but would contribute to the clock running slow over time. 

 

#### Servo and Timer Code

The hands of the clock are moved by two 90 degree micro servos.  These require a specific PWM in order to run.  The width of the duty cycle needs to be at most 2ms, and at least 1ms, corresponding to zero degrees and 90 degrees.  The period of the PWM needs to be 20ms. 

 

 

PUT SERVO TIMING PICTURE HERE

 

Datasheet: http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf

 

This PWM was made using timer 15, channel 2, for the minute hand, and timer 16, channel 1, for the hour hand.  The thinking for PRSC, ARR, and CCR for these are:

The desired resolution is one count for one degree of servo movement (after the gear ratio), so it will need 180 steps in 1ms. 

This works for

PRSC = 443

ARR = 3600

CCR = 180 + degree desired / 2

Input clock to the timer: 80 MHz.

(this was also calculated for a clock of 4 MHz, which can be converted with a PRSC of 21)

 

After setting up the timers to create the appropriate PWM, CCR was modified to achieve the desired angle.  The CCR values for desired clock locations were calibrated for both hands in arrays, with the structure: time_angles[12] = {CCR for 1:00 position, CCR for 2:00 position …}.  These calibrations hold if the gears are attached when the servos are held at their lowest position (CCRx = 180), and the gears are meshed with the hands pointing vertically, or skewing slightly to the side, as vertically hit the gear ratio. 

 

To have smooth transitions between different clock angles, a function was written for each hand to move the hands smoothly, called [time]_AtoB.  A smaller delay was timed in, and it smoothly loops from the first number to the second.

 

##### SPI and random number generation

 

SPI and random number generation are done in the function getSequence12_SPI.  The function calls the random number generator peripheral on the MCU, and randomizes the location of the numbers 1 through 12 to display on the clock face.  It then initiates SPI to shift this onto the FPGA, and returns the randomized list to main.  This code is in RNG.c.  The random number generator peripheral on the microcontroller generates a random 32 bit integer with the peripheral, which requires a slow clock in, which samples data from analog sources to generate the true random number.  The returned 32 bit integer is used to generate the 12 random numbers by using the true random number as a randomizing seed, which is then modulus's to select a number from 12 numbers to be placed in the next digit. 

 

# Mechanical Elements

 

####   7-seg led display wiring

The 7-segment display was wired with an enable system similar to lab 2 and 3, extending this to 24 displays.  The enable turns on a PNP transistor, which is the common current source for the display.  To turn each LED on, the seven segments of the display are connected to the FPGA pins, and drawn low to turn on the LED.  Only one seven segment display is on at a time, so the display does not draw much power, but it is also fairly dim because it is on 1/24th of the time, at a rate faster than the human eye, which effectively dims the displays.

 

#### 3-8 decoder wiring

The seven-segment LED display wiring was done by setting up a truth table, and finding the logic of how each of three decoders should output:

|  en[4:0] | out[24:0]   | decoder (out of three)  |
| ---- | ----------- | ------ |
| 0 0000  |  1111_1111 1111_1111 1111_1110 |  1 |
| 0 0001  | 1111_1111 1111_1111 1111_1101  |  1 |
|  … | …  | 1  |
| 0 0111  | 1111_1111 1111_1111 0111_1111  | 1  |
| 0 1000  |  1111_1111 1111_1110 1111_1111 | 2  |
| …  | …  | 2  |
| 0 1111  | 1111_1111 0111_1111 1111_1111  | 2  |
|  1 0000 |  1111_1110 1111_1111 1111_1111 |  3 |
| …  | …  | 3  |

So en[4] and en[3] can be used with simple logic to determine which decoder to turn on.

 

(https://www.ti.com/lit/ds/symlink/sn74ls138.pdf?ts=1667098463203&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FSN74LS138)

The enable G2 was used to pull all outputs of the decoder high when it does not have the output. This results in the circuit: (Show decoder circuit)

 

#### Gears

 The second and minute hands are connected on the clock via a gear system which converts the 90 degrees of servo motion to 360 degrees of clock hand motion for two hands on the same axis. 

 

The servo actuates from 0 to 90 degrees.  This is connected to a 3d printed servo mount via a 0.04" diameter hole drilled This connects to a 3d-printed servo mount.  This was drilled 0.02" off the face of the servo, through the turning part of the servo, which protrudes 0.08" total from the servo casing.  This was necessary because the servo - servo connector interface would slip when the servo the hands to vertical at maximum speed on a reset.  The servo connector interface was then which is hot glued to a 40 tooth lego gear. 

 

The 40 tooth lego gears then interfaced with an 8-tooth lego gears, which interfaced with the shaft holding the hour and minute hands. 
