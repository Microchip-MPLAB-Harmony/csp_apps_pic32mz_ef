---
parent: Harmony 3 peripheral library application examples for PIC32MZ EF family
title: SPI ping pong with DMA
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SPI ping pong with DMA

This example demonstrates how to continuously transmit and receive data over a SPI interface using contiguous ping pong buffers with DMA.

## Description

The DMA peripheral provides support to implement the ping-pong buffering (the DMA transmits and receives data from one pair of buffers, when the CPU works on the second pair) in two ways.

In one method, source half empty interrupt and/or destination half full interrupt can be used to notify the application when half of the transfer is complete. At that point CPU can use first half of the buffer (ping buffer). When DMA is transfering first half of the buffer,
CPU can use 2nd half of the buffer (pong buffer) for processing. DMA auto enable feature is used to ensure data transfer resumes from beginning after completion. In this method, ping-pong buffer has to be in contiguous memory location.

In second method, two DMA channels are used in chained fashion. One DMA channel can trigger another one when its transfer is complete and vice-versa. In this method, ping-pong buffer need not be in contiguous memory location.

This application uses the first method. It transmits an array of values and verifies the value transmitted by receiving the values back with loop back and comparing it to the value transmitted. It repeats the same for two halfs of the buffer. This application requires that the MOSI output pin to be connected to the MISO input pin so that anything transmitted will also be received.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/spi/spi_ping_pong_with_dma/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| pic32mz_ef_sk.X | MPLABX project for [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007) |
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
| pic32mz_ef_sk.X | [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007) |
|||

### Setting up [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007)

- Use a jumper wire to short pin 19 and pin 21 of the J12 connector
- Connect the Debug USB port on the board to the computer using a mini USB cable

## Running the Application

- Build and program the application using its IDE
- LED indicates the success or failure:
  - Green LED (LED3) is turned ON when the value received in both the buffers match with the transmitted value
  - Red LED (LED1) is turned ON when the value received did not match with the transmitted value in at least one buffer
