---
layout: page
title: Design
permalink: /design/
---
## FPGA Design
Fig. 6 top level FPGA schematic
<div style="text-align: left">
  <img src="../assets/schematics/FPGA.png" alt="logo" width="1000" />
</div>

There are three significant design blocks in the FPGA design for the random clock: the SPI shift register, the Neopixel driver, and the  multiplexed display module. You may find other helper modules in the diagrams and source codes, but they are less important in making the Random Clock function. 

# SPI Shift Registers
The SPI Shift Register, shown in Fig. 6, is responsible for receiving SPI information on the location of each number on the clock. This block is asynchronous in that it is driven by the serial clock, not the on board clock. On each posedge, one bit from the sdi line is shifted into the clock_num bus. Since the SPI signals are 96 bits long (12 numbers on a clock, each number is represented as an 8 bit char, hence 8 times 12 is 96), the clock_num internal signal is also 96 bits long. The bit_rearrange module is a simple combinational logic that extracts each clock digit from the clock_num bus. 
 
# Mutiplexed Display
Fig. 7 Num_Distribute schematic
<div style="text-align: left">
  <img src="../assets/schematics/Num_Distribute.JPG" alt="logo" width="1000" />
</div>
 
The Num_Distribute module contains the display multiplexer. In order to drive 24 different 7-seg displays, a traditional combinational logic circuit that simply sends the digit to display will have trouble delivering enough current to the seven-seg displays. Therefore, a multiplexed display is used, where each display only flashes on for a quick moment, before the signal is given on an adjacent display. Because this multiplexing process happens quickly (>100Hz), the human eye cannot detect the discrete signals, and will instead see all displays lit up simultaneously. In the schematic above, the on board 48MHz clock is slowed to around 100Hz with a clock divider, and that divider is sent to a simple FSM called en_rotate, which simply cycles through each enable unconditionally in a loop. These en [4:0] signals are both  outputs to the physical decoder module, and the select line for the mux, which chooses which number gets to be displayed at a seven-segment display. 
 
Note that although the neopixel driver is instantiated here, it does not interact with any other modules. Its details will be given in the next section. 


# Neopixel Driver
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





## MCU Design



## New Hardware: Neopixel Strip. 
