# STM32-Ethernet-lwIP-nucleo
Firmware for Attolic Studio w/ STM32CUBEMX to run TCP/IP Stack

For use with STM32F429ZI

## Requirements:
* Atollic TrueSTUDIO® for STM32, Version: 9.3.0 
* STM32CubeMX 5.1.0

## Getting started
1. After downloading TrueSTUDIO, click File -> Import Projects from Files System or Archive. Choose the folder containing this project using the "Directory" Button

2. Debug project to download to device

NOTE: One may need to change Debug Configuration to enable use of STLINK rather then J-Link

## Debug notes:
 For this version, ethernet connection to **router** must be connected to board before debugging device