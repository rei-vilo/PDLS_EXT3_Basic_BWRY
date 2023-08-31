///
/// @file Screen_EPD_EXT3.h
/// @brief Driver for Pervasive Displays iTC red and yellow colours screens and EXT3-1 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @n @b B-S-G
/// * Edition: Basic
/// * Family: Small
/// * Update: Global
/// * Feature: none
/// * Temperature: red and yellow = 0 to 40 °C
///
/// @n Supported screens with embedded fast update
/// * 1.54 reference xE2154QS0Fx
/// * 2.13 reference xE2213QS0Fx
/// * 2.66 reference xE2266QS0Fx
///
/// @author Rei Vilo
/// @date 31 Aug 2023
/// @version 614
///
/// @copyright (c) Rei Vilo, 2010-2023
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

// SDK
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#else // Arduino general
#include "Arduino.h"
#endif // SDK

// Configuration
#include "hV_Configuration.h"

#if (hV_CONFIGURATION_RELEASE < 611)
#error Required hV_CONFIGURATION_RELEASE 611
#endif // hV_CONFIGURATION_RELEASE

#ifndef SCREEN_EPD_EXT3_RELEASE
///
/// @brief Library release number
///
#define SCREEN_EPD_EXT3_RELEASE 614

// Other libraries
#include "SPI.h"
#include "hV_Screen_Buffer.h"

#if (hV_SCREEN_BUFFER_RELEASE < 612)
#error Required hV_SCREEN_BUFFER_RELEASE 612
#endif // hV_SCREEN_BUFFER_RELEASE

// Objects
//
///
/// @brief Class for Pervasive Displays iTC red and yellow colour screens
/// @details Screen controllers
/// * LCD: proprietary, SPI
/// * touch: no touch
/// * fonts: no external Flash
///
/// @note All commands work on the frame-buffer,
/// to be displayed on screen with flush()
///
class Screen_EPD_EXT3 final : public hV_Screen_Buffer
{
  public:
    ///
    /// @brief Constructor with default pins
    /// @param eScreen_EPD_EXT3 size and model of the e-screen
    /// @param board board configuration
    /// @note To be used with begin() with no parameter
    ///
    Screen_EPD_EXT3(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board);

    ///
    /// @brief Initialisation
    /// @note Frame-buffer generated internally, not suitable for FRAM
    /// @warning begin() initialises SPI and I2C
    ///
    void begin();

    ///
    /// @brief Who Am I
    /// @return Who Am I string
    ///
    String WhoAmI();

    ///
    /// @brief Clear the screen
    /// @param colour default = white
    /// @note Clear next frame-buffer
    ///
    void clear(uint16_t colour = myColours.white);

    ///
    /// @brief Invert screen
    /// @param flag true to invert, false for normal screen
    ///
    void invert(bool flag);

    ///
    /// @brief Update the display, global update
    /// @note Send the frame-buffer to the screen and refresh the screen
    ///
    void flush();

    ///
    /// @brief Regenerate the panel
    /// @details White-to-black-to-white cycle to reduce ghosting
    ///
    void regenerate();

    ///
    /// @brief Set temperature in Celsius
    /// @details Set the temperature for update
    /// @param temperatureC temperature in °C, default = 25 °C
    /// @note Refer to data-sheets for authorised operating temperatures
    ///
    void setTemperatureC(int8_t temperatureC = 25);

    ///
    /// @brief Set temperature in Fahrenheit
    /// @details Set the temperature for update
    /// @param temperatureF temperature in °F, default = 77 °F = 25 °C
    /// @note Refer to data-sheets for authorised operating temperatures
    ///
    void setTemperatureF(int16_t temperatureF = 77);

    ///
    /// @brief Check the mode against the temperature
    ///
    /// @param updateMode expected update mode
    /// @return uint8_t recommended mode
    /// @note If required, defaulting to UPDATE_NONE
    /// @warning Default temperature is 25 °C, otherwise set by setTemperatureC() or setTemperatureF()
    ///
    uint8_t checkTemperatureMode(uint8_t updateMode = UPDATE_GLOBAL);

    ///
    /// @brief Update the display
    /// @details Display next frame-buffer on screen and copy next frame-buffer into old frame-buffer
    /// @param updateMode expected update mode
    /// @return uint8_t recommended mode
    /// @note Mode checked with checkTemperatureMode()
    ///
    uint8_t flushMode(uint8_t updateMode = UPDATE_GLOBAL);

    ///
    /// @brief Draw pixel
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void point(uint16_t x1, uint16_t y1, uint16_t colour);

    ///
    /// @brief Read pixel colour
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @return 16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);

  protected:
    /// @cond

    ///
    /// @brief General reset
    /// @param ms1 delay after PNLON_PIN, ms
    /// @param ms2 delay after RESET_PIN HIGH, ms
    /// @param ms3 delay after RESET_PIN LOW, ms
    /// @param ms4 delay after RESET_PIN HIGH, ms
    /// @param ms5 delay after CS_PIN CSS_PIN HIGH, ms
    ///
    void _reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5);

    // * Virtual =0 compulsory functions
    // Screen-specific
    ///
    /// @brief Send data through SPI
    /// @param index register
    /// @param data data
    /// @param size number of bytes
    /// @note Valid for all except large screens
    ///
    void _sendIndexData(uint8_t index, const uint8_t * data, uint32_t size);

    // Orientation
    ///
    /// @brief Set orientation
    /// @param orientation 1..3, 6, 7
    ///
    void _setOrientation(uint8_t orientation); // compulsory

    ///
    /// @brief Check and orient coordinates, logical coordinates
    /// @param x x-axis coordinate, modified
    /// @param y y-axis coordinate, modified
    /// @return false = success, true = error
    ///
    bool _orientCoordinates(uint16_t & x, uint16_t & y); // compulsory

    // Position

    // Write and Read
    /// @brief Set point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @param colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour);

    /// @brief Get point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @return colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    uint16_t _getPoint(uint16_t x1, uint16_t y1);

    ///
    /// @brief Convert
    /// @param x1 x-axis coordinate
    /// @param y1 y-axis coordinate
    /// @return index for _newImage[]
    ///
    uint32_t _getZ(uint16_t x1, uint16_t y1);

    ///
    /// @brief Wait for ready
    /// @details Wait for panelBusy low
    ///
    void _waitBusy();
    void _sendCommand8(uint8_t command);
    void _sendCommandData8(uint8_t command, uint8_t data);

    // Energy
    // No energy

    // * Other functions specific to the screen
    const uint8_t * COG_initialData;
    void COG_initial();
    void COG_getUserData();
    void COG_sendImageDataGlobal();
    void COG_update();
    void COG_powerOff();

    // * Flush
    void _flushGlobal();

    // Screen independent variables
    uint8_t * _newImage;
    bool _invert = false;
    uint16_t _screenSizeV, _screenSizeH;
    int8_t _temperature = 25;

    // Screen dependent variables
    pins_t _pin;
    eScreen_EPD_EXT3_t _eScreen_EPD_EXT3;
    uint8_t _codeExtra;
    uint8_t _codeSize;
    uint8_t _codeType;
    uint16_t _bufferSizeV, _bufferSizeH, _bufferDepth;
    uint32_t _pageColourSize, _frameSize;

    // === Touch
    // No touch

    /// @endcond
};

#endif // SCREEN_EPD_EXT3_RELEASE
