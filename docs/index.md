---
layout: page
title: Project Overview
permalink: /
exclude: true
---
<div style="text-align: left">
  <img src="./assets/img/Logo.png" alt="logo" width="100" />
</div>


# Project Abstract
This website describes the design of a very confusing clock. Similar to traditional wall clocks, it has twelve numbers arranged around a panel, as well as minute and hour hands. However, the numbers 1 - 12 around the panel are randomized every hour, and the clock hands will always point to the correct numbers to tell the current time. See figure 1 below for details. 





# Project Motivation
The east dorm has a tradition to collect clocks of all sizes and types. We would like to add to their clock collection with a new fancy clock, where the numbers 1-12 on the clock are randomized in position every hour. For example, a regular clock always displays 12, 1, 2, 3…10, 11 starting at the top. Instead, we build a clock that randomizes each digit’s placement every hour. In the first hour, the digit might be 5,6,8,4… starting at the top, and next hour, this sequence will become randomized to maybe 11, 2, 4, 9…. Other than this, we include a minute and hour hand to the clock (controlled by servos), and they have to always point to the correct numbers representing the current time. For example, if the current time is 4:25, the hour hand and minute hand must point to 4 and 5, regardless of where these digits are. (See Figures 1 - 3 in the appendix for a demonstration). 

# System Block Diagram
