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


## Neopixel Driver





# MCU Design



# New Hardware: Neopixel Strip. 
