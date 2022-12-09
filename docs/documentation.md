---
layout: page
title: Documentation
permalink: /doc/
---

# Schematics
<!-- Include images of the schematics for your system. They should follow best practices for schematic drawings with all parts and pins clearly labeled. You may draw your schematics either with a software tool or neatly by hand. -->


Fig 2. Overall System Block Diagram
<div style="text-align: left">
  <img src="../assets/schematics/BlockDiagram.png" alt="logo" width="1000" />
</div>




The MCU generates a random array of numbers from 1 to 12 depicting where each number will go on the clock panel each hour. This information is sent to the FPGA by SPI. The FPGA uses a shift register to receive the information, and decodes it into a format suitable for the seven-segment displays. The MCU also takes the random array and encodes it into servo positions for the minute and hour hand, telling them where to turn as time passes. 
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




# Source Code Overview
<!-- This section should include information to describe the organization of the code base and highlight how the code connects. -->

The source code for the project is located in the Github repository [here](https://github.com/joshbrake/example-project-portfolio/tree/main/src).






