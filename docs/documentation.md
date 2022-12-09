---
layout: page
title: Documentation
permalink: /doc/
---
This page contains the high level schematics of the Random Clock project at the surface level. For in-depth explanation of each sub module, use the Design tab. 
 
# Schematics
<!-- Include images of the schematics for your system. They should follow best practices for schematic drawings with all parts and pins clearly labeled. You may draw your schematics either with a software tool or neatly by hand. -->


Fig 2. Overall System Block Diagram
<div style="text-align: left">
  <img src="../assets/schematics/BlockDiagram.png" alt="logo" width="1000" />
</div>




The MCU generates a random array of numbers from 1 to 12 depicting where each number will go on the clock panel each hour. This information is sent to the FPGA by SPI. The FPGA uses a shift register to receive the information, and decodes it into a format suitable for the seven-segment displays. The MCU also takes the random array and encodes it into servo positions for the minute and hour hand, telling them where to turn as time passes. For greater details of the designs, visit the "Design" tab of the website. 

***
Fig 3. Physical Circuit Diagram 

<div style="text-align: left">
  <img src="../assets/schematics/PhysicalCircuit.png" alt="logo" width="700" />
</div>

Fig 4. Pinout Chart
<div style="text-align: left">
  <img src="../assets/schematics/Pinout.png" alt="logo" width="700" />
</div>
All signals labeled in Fig 3. can be found on Fig 4 when connecting the system together. The FPGA signal names are also contingent with the corresponding .sv files. The decoder block in Fig 3. is a physical module, with its schematics provided in Fig 5.

***
Fig 5. Decoder Circuit Diagram
<div style="text-align: left">
  <img src="../assets/schematics/Decoder.png" alt="logo" width="700" /> 
</div>

The decoder module uses 3 physical decoder IC's as well as some AND, NOT gates to decode the 5 bit signal (en [4:0]) describing which LED display to enable. The output of the decoder module are 24 physical wires that connect to each seven-seg display's common anodes. In hindsight, this was unnecessary since the FPGA has 48 I/O pins, meaning without the physical decoding modules, we would only have used 36 of them. With the decoder module, we cut down this number to 17, freeing up more pins for further implementations should we choose to do so. 


# Source Code Overview
<!-- This section should include information to describe the organization of the code base and highlight how the code connects. -->

The source code for the project is located in the Github repository [here](https://github.com/koooo-142857/E155_Random_Clock/tree/main/src).






