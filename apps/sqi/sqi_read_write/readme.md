[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SQI flash read write

This example application demonstrates how to use the SQI Peripheral library to perform erase, write and read operation with the SQI Serial Flash memory.

## Description

This example uses the SQI peripheral library to erase and write an array of values to the SQI Serial Flash memory. It verifies the value written by reading the values back and comparing it to the value written.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/sqi/sqi_read_write/firmware** .

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

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application using the its IDE
2. **Switch LED** is turned ON to indicate waiting for switch press
3. Press the Switch to start the SQI Transfer. **Switch LED** is turned OFF once switch is pressed
4. The **Success LED** is turned ON when the value read from the SQI Serial Flash memory matched with the written value

Following table provides the LED names and switch name:

| Kit Name | Success LED Name | Switch LED Name | Switch Name |
| -------- | ---------------- | --------------- | ----------- |
| [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007) | LED3 | LED1 | SW3 |
||||
