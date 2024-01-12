///
/// @file hV_HAL_Peripherals.h
/// @brief Light hardware abstraction layer for peripherals
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Feb 2024
/// @version 800
///
/// @copyright (c) Rei Vilo, 2010-2024
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// The highView Library Suite is shared under the Creative Commons licence Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///

#ifndef hV_HAL_PERIPHERALS_RELEASE
///
/// @brief Release
///
#define hV_HAL_PERIPHERALS_RELEASE 800

///
/// @brief SDK library
/// @see References
/// * Arduino SDK https://www.arduino.cc/reference/en/
/// * Energia SDK https://energia.nu/reference/
///
#include <Arduino.h>

///
/// @brief SDK other libraries
///
#include <SPI.h>
#include <Wire.h>

///
/// @brief Other libraries
///
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define mySerial Serial

///
/// @name 3-wire SPI bus
/// @warning
/// * Arduino does not support 3-wire PSI, bit-bang simulation 
/// * Viewer: For compatibility only, not implemented in Linux
/// @note hV_SDK_SPI_begin() calls hV_SDK_SPI3_begin().
/// @{

///
/// @brief Set the 3-wire SPI pins
/// @param pinClock clock, default = SCK
/// @param pinData combined data, default = MOSI
///
void hV_HAL_SPI3_begin(uint8_t pinClock = SCK, uint8_t pinData = MOSI);

///
/// @brief Read a single byte
/// @return read byte
/// @note Configure the clock pin as output and data pin as input.
/// @warning /CS to be managed externally.
/// 
uint8_t hV_HAL_SPI3_read();

///
/// @brief Write a single byte
/// @param data byte
/// @note Configure the clock and data pins as output.
/// @warning /CS to be managed externally.
///
void hV_HAL_SPI3_write(uint8_t value);

#endif // hV_HAL_PERIPHERALS_RELEASE
