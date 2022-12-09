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
Fig. 7
<div style="text-align: left">
  <img src="../assets/schematics/Num_Distribute.jpg" alt="logo" width="1000" />
</div>

## Neopixel Driver





# MCU Design



# New Hardware: Neopixel Strip. 
