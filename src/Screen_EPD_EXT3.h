///
/// @file Screen_EPD_EXT3.h
/// @brief Driver for Pervasive Displays iTC black-white-red-yellow colour screens with EXT3.1 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @n @b B-S-G
/// * Edition: Basic
/// * Family: Small
/// * Update: Global
/// * Feature: none
///
/// @n Supported black-white-red-yellow colour screens
/// * EPD_154_QS_0F screen 1.54”
/// * EPD_213_QS_0F screen 2.13”
/// * EPD_266_QS_0F screen 2.66”
/// * EPD_417_QS_0A screen 4.17”
///
/// @author Rei Vilo
/// @date 21 Oct 2024
/// @version 807
///
/// @copyright (c) Rei Vilo, 2010-2024
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
/// @copyright Portions (c) Pervasive Displays, 2010-2024
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///
/// * Evaluation edition: for professionals or organisations, evaluation only, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * Viewer edition: for professionals or organisations
/// @n All rights reserved
///
/// * Documentation
/// @n All rights reserved
///

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

// Other libraries
#include "hV_Screen_Buffer.h"

// Board
#include "hV_Board.h"

// PDLS utilities
#include "hV_Utilities_PDLS.h"

// Checks
#if (hV_HAL_PERIPHERALS_RELEASE < 801)
#error Required hV_HAL_PERIPHERALS_RELEASE 801
#endif // hV_HAL_PERIPHERALS_RELEASE

#if (hV_CONFIGURATION_RELEASE < 803)
#error Required hV_CONFIGURATION_RELEASE 803
#endif // hV_CONFIGURATION_RELEASE

#if (hV_SCREEN_BUFFER_RELEASE < 805)
#error Required hV_SCREEN_BUFFER_RELEASE 805
#endif // hV_SCREEN_BUFFER_RELEASE

#if (hV_BOARD_RELEASE < 801)
#error Required hV_BOARD_RELEASE 801
#endif // hV_BOARD_RELEASE

#ifndef SCREEN_EPD_EXT3_RELEASE
///
/// @brief Library release number
///
#define SCREEN_EPD_EXT3_RELEASE 807

///
/// @brief Library variant
///
#define SCREEN_EPD_EXT3_VARIANT "Basic-BWRY"

///
/// @name Constants for features
/// @{
#define WITH_COLOURS ///< With colours
#define WITH_COLOURS_BWR ///< Black-White-Red colours
#define WITH_COLOURS_BWRY ///< Black-White-Red-Yellow colours
/// @}

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
class Screen_EPD_EXT3 final : public hV_Screen_Buffer, public hV_Utilities_PDLS
{
  public:
    ///
    /// @brief Constructor with default pins
    /// @param eScreen_EPD_EXT3 size and model of the e-screen
    /// @param board board configuration
    /// @note Frame-buffer generated by the class
    /// @note To be used with begin() with no parameter
    ///
    Screen_EPD_EXT3(eScreen_EPD_t eScreen_EPD_EXT3, pins_t board);

    ///
    /// @brief Initialisation
    /// @note Frame-buffer generated internally, not suitable for FRAM
    /// @warning begin() initialises SPI and I2C
    ///
    void begin();

    ///
    /// @brief Suspend
    /// @param suspendScope default = POWER_SCOPE_GPIO_ONLY, otherwise POWER_SCOPE_NONE
    /// @details Power off and set all GPIOs low
    /// @note If panelPower is NOT_CONNECTED, POWER_SCOPE_GPIO_ONLY defaults to POWER_SCOPE_NONE
    ///
    void suspend(uint8_t suspendScope = POWER_SCOPE_GPIO_ONLY);

    ///
    /// @brief Resume after suspend()
    /// @details Turn SPI on and set all GPIOs levels
    ///
    void resume();

    ///
    /// @brief Who Am I
    /// @return Who Am I string
    ///
    virtual STRING_TYPE WhoAmI();

    ///
    /// @brief Clear the screen
    /// @param colour default = white
    /// @note Clear next frame-buffer
    ///
    void clear(uint16_t colour = myColours.white);

    ///
    /// @brief Update the display, global update
    /// @note
    /// 1. Send the frame-buffer to the screen
    /// 2. Refresh the screen
    ///
    void flush();

    ///
    /// @brief Regenerate the panel
    /// @details White-to-black-to-white cycle to reduce ghosting
    /// @param mode default = UPDATE_GLOBAL = global mode
    ///
    void regenerate(uint8_t mode = UPDATE_GLOBAL);

    ///
    /// @brief Update the display
    /// @details Display next frame-buffer on screen and copy next frame-buffer into old frame-buffer
    /// @param updateMode expected update mode, default = UPDATE_GLOBAL
    /// @return uint8_t recommended mode
    /// @note Mode checked with checkTemperatureMode()
    ///
    uint8_t flushMode(uint8_t updateMode = UPDATE_GLOBAL);

  protected:
    /// @cond

    // Orientation
    ///
    /// @brief Set orientation
    /// @param orientation 1..3, 6, 7
    ///
    void s_setOrientation(uint8_t orientation); // compulsory

    ///
    /// @brief Check and orient coordinates, logical coordinates
    /// @param x x-axis coordinate, modified
    /// @param y y-axis coordinate, modified
    /// @return RESULT_SUCCESS = false = success, RESULT_ERROR = true = error
    ///
    bool s_orientCoordinates(uint16_t & x, uint16_t & y); // compulsory

    // Write and Read
    /// @brief Set point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @param colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    void s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour);

    /// @brief Get point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @return colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    uint16_t s_getPoint(uint16_t x1, uint16_t y1);

    ///
    /// @brief Reset the screen
    ///
    void s_reset();

    ///
    /// @brief Get data from OTP
    ///
    void s_getDataOTP();

    ///
    /// @brief Update the screen
    /// @param updateMode update mode, default = UPDATE_GLOBAL
    ///
    void s_flush(uint8_t updateMode = UPDATE_GLOBAL);

    // Position
    ///
    /// @brief Convert
    /// @param x1 x-axis coordinate
    /// @param y1 y-axis coordinate
    /// @return index for s_newImage[]
    ///
    uint32_t s_getZ(uint16_t x1, uint16_t y1);

    ///
    /// @brief Convert
    /// @param x1 x-axis coordinate
    /// @param y1 y-axis coordinate
    /// @return bit for s_newImage[]
    ///
    uint16_t s_getB(uint16_t x1, uint16_t y1);

    //
    // === Energy section
    //

    //
    // === End of Energy section
    //

    // * Other functions specific to the screen
    uint8_t COG_data[112]; // OTP

    void COG_SmallQ_reset();
    void COG_SmallQ_getDataOTP();
    void COG_SmallQ_initial();
    void COG_SmallQ_sendImageData();
    void COG_SmallQ_update();
    void COG_SmallQ_powerOff();

    //
    // === Touch section
    //

    //
    // === End of Touch section
    //

    /// @endcond
};

#endif // SCREEN_EPD_EXT3_RELEASE

