[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# EBI SRAM read write

This example application demonstrates how to use the EBI peripheral to write and read from the SRAM.

## Description

This example uses the EBI peripheral library to write an address of memory locations into respective their memory locations and verify the value written by reading the values back and comparing it to the value written.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/ebi/ebi_sram_read_write/firmware** .

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

- To run the demo, the following additional hardware are required:
  -[Multimedia Expansion Board II](https://www.microchip.com/Developmenttools/ProductDetails/DM320005-2)

- Mount [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007) on a [Multimedia Expansion Board II](https://www.microchip.com/Developmenttools/ProductDetails/DM320005-2)
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application project using its respective IDE
2. LED indicates the success:
    - LED is turned ON when the value read from the SRAM matched with the written value

Following table provides the LED name:

| Board | LED name |
| ----- | -------- |
| [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/dm320007) | LED1|
|||