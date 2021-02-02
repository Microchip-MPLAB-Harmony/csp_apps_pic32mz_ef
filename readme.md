---
title: Harmony 3 peripheral library application examples for PIC32MZ EF family
nav_order: 1
has_children: true
has_toc: false
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Harmony 3 peripheral library application examples for PIC32MZ EF family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 peripheral library application examples for PIC32MZ EF family

- [Release Notes](release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)

To clone or download these applications from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

## Contents Summary

| Folder     | Description                             |
| ---        | ---                                     |
| apps       | Contains peripheral library example applications |
| docs       | Contains documentation in html format for offline viewing (to be used only after cloning this repository onto a local machine). Use [github pages](https://microchip-mplab-harmony.github.io/csp_apps_pic32mz_ef/) of this repository for viewing it online. |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries.

| Name | Description |
| ---- | ----------- |
| [ADCHS Interrupt](apps/adchs/adchs_interrupt/readme.md) | This example application shows how to sample an analog input using the ADCHS peripheral and displays the converted samples on a serial terminal |
| [ADCHS polling](apps/adchs/adchs_polled/readme.md) | This example application shows how to sample an analog input using the ADCHS peripheral and displays the converted samples on a serial terminal |
| [Cache maintenance operations](apps/cache/cache_maintenance/readme.md) | This example demonstrates the cache maintenance operation by cleaning and invalidating the cache for the DMA buffers located in the cacheable SRAM region |
| [CAN blocking](apps/can/can_normal_operation_blocking/readme.md) | This example application shows how to use the CAN module to transmit and receive normal CAN messages in polling mode |
| [CAN interrupt](apps/can/can_normal_operation_interrupt_timestamp/readme.md) | This example application shows how to use the CAN module to transmit and receive normal CAN messages in interrupt mode |
| [Clock configuration](apps/clock/clock_config/readme.md) | This example application shows how to configure the clock system to run the device at maximum frequency. It also outputs a prescaled clock signal on a GPIO pin for measurement and verification |
| [CMP configurable reference](apps/cmp/cmp_configurable_reference/readme.md) | This example application shows how to use the CMP Peripheral library to compare voltage level on the negative input with the internal configurable reference voltage |
| [CORETIMER periodic interrupt](apps/coretimer/coretimer_periodic_timeout/readme.md) | This example application shows how to use the CoreTimer to generate periodic interrupts |
| [DMAC CRC32 generate](apps/dmac/dmac_crc32_generate/readme.md) | This example application demonstrates how to use the DMAC peripheral to compute 32-bit Cyclic Redundancy Checksum (CRC) |
| [DMAC memory transfer](apps/dmac/dmac_memory_transfer/readme.md) | This example application demonstrates how to use the DMAC peripheral to do a memory to memory transfer |
| [DMAC usart echo](apps/dmac/dmac_usart_echo/readme.md) | This example application demonstrates USART transfer with DMA to receive 10 bytes and echo back the received bytes |
| [DMAC usart pattern matching](apps/dmac/dmac_usart_pattern_matching/readme.md) | This example application demonstrates USART echo using DMA pattern matching feature |
| [DMT timeout](apps/dmt/dmt_timeout/readme.md) | This example application shows how the deadman timer resets by not clearing the deadman timer counter on switch press |
| [EBI SRAM read write](apps/ebi/ebi_sram_read_write/readme.md) | This example application demonstrates how to use the EBI peripheral to write and read from the SRAM |
| [GPIO interrupt](apps/gpio/gpio_led_on_off_interrupt/readme.md) | This example application demonstrate how to generate GPIO interrupt on switch press and release, and indicate the switch status using the LED |
| [GPIO Polling](apps/gpio/gpio_led_on_off_polling/readme.md) | This example application demonstrate how to poll the switch input, and indicate the switch status using the LED |
| [ICAP capture mode](apps/icap/icap_capture_mode/readme.md) | This example application shows how to use the ICAP peripheral to measure the pulse width of the input signal |
| [NVM flash read write](apps/nvm/flash_read_write/readme.md) | This example application demonstrates how to use the NVM to erase and program the internal Flash memory |
| [OCMP compare mode](apps/ocmp/ocmp_compare_mode/readme.md) | This example application shows how to use the OCMP peripheral to generate an active low, active high, and toggle output on compare match |
| [RNG random number](apps/rng/rng_random_number/readme.md) | This example application shows how to use the RNG Peripheral library to generate a pseudo-random number |
| [RTCC alarm interrupt](apps/rtcc/rtcc_alarm/readme.md) | This example application shows how to use the RTCC to configure the time and generate the alarm |
| [SPI ping pong with dma](apps/spi/spi_ping_pong_with_dma/readme.md) | This example demonstrates how to continuously transmit and receive data over SPI interface using contiguous ping pong buffers with DMA |
| [SPI ping pong with dma chain](apps/spi/spi_ping_pong_with_dma_chain/readme.md) | This example demonstrates how to continuously transmit and receive data over  SPI interface using ping pong buffers with DMA channels in chain mode |
| [SPI blocking](apps/spi/spi_self_loopback_blocking/readme.md) | This example application demonstrates how to use the SPI peripheral to transmit and receive a block of data in a blocking manner |
| [SPI interrupt](apps/spi/spi_self_loopback_interrupt/readme.md) | This example application demonstrates how to use the SPI peripheral to transmit and receive a block of data using interrupt |
| [SQI flash read write](apps/sqi/sqi_read_write/readme.md) | This example application demonstrates how to use the SQI Peripheral library to perform erase, write and read operation with the SQI Serial Flash memory |
| [TMR periodic interrupt](apps/tmr/tmr_timer_mode/readme.md) | This example demonstrates how to use the TMR module in timer mode to generate periodic interrupt |
| [TMR1 periodic interrupt](apps/tmr1/tmr1_timer_mode/readme.md) | This example demonstrates how to use the TMR1 module in timer mode to generate periodic interrupt |
| [UART echo blocking](apps/uart/uart_echo_blocking/readme.md) | This example application demonstrates how to use the UART peripheral to transfer a block of data in a blocking manner |
| [UART echo interrupt](apps/uart/uart_echo_interrupt/readme.md) | This example application demonstrates how to use the UART peripheral to transfer a block of data in a non-blocking manner |
| [UART ring buffer](apps/uart/uart_ring_buffer_interrupt/readme.md) | This example application demonstrates how to use the UART peripheral in ring buffer mode |
| [WDT timeout](apps/wdt/wdt_timeout/readme.md) | This example application shows how the watchdog timer resets by not feeding the watchdog on switch press |

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/csp_apps_pic32mz_ef.svg?style=social)]()