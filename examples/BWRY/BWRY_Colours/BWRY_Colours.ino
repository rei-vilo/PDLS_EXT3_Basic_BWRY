///
/// @file BWRY_Colours.ino
/// @brief Example for Pervasive Displays Library Suite
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2024
/// @version 810
///
/// @copyright (c) Rei Vilo, 2010-2024
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
///
/// @see ReadMe.txt for references
/// @n
///

// Screen
#include "PDLS_EXT3_Basic_BWRY.h"

// SDK
// #include <Arduino.h>
#include "hV_HAL_Peripherals.h"

// Include application, user and local libraries
// #include <SPI.h>

// Configuration
#include "hV_Configuration.h"

#if (SCREEN_EPD_EXT3_RELEASE < 810)
#error Required SCREEN_EPD_EXT3_RELEASE 810
#endif // SCREEN_EPD_EXT3_RELEASE

// Set parameters
#define DISPLAY_COLOURS_BWRY 1
#define DISPLAY_CONTRASTS_BWRY 0
#define DISPLAY_PALETTE_BWRY 0

// Define structures and classes

// Define variables and constants
// Screen_EPD_EXT3 myScreen(eScreen_EPD_154_QS_0F, boardRaspberryPiPico_RP2040);
// Screen_EPD_EXT3 myScreen(eScreen_EPD_213_QS_0F, boardRaspberryPiPico_RP2040);
Screen_EPD_EXT3 myScreen(eScreen_EPD_266_QS_0F, boardRaspberryPiPico_RP2040);

// Prototypes

// Utilities
///
/// @brief Wait with countdown
/// @param second duration, s
///
void wait(uint8_t second)
{
    for (uint8_t i = second; i > 0; i--)
    {
        mySerial.print(formatString(" > %i  \r", i));
        delay(1000);
    }
    mySerial.print("         \r");
}

// Functions
#if (DISPLAY_COLOURS_BWRY == 1)

void displayColoursBWRY()
{
    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);
    myScreen.selectFont(Font_Terminal8x12);

    uint16_t x, y, dx, dy;

    const uint8_t grid = 4; // 4 or 5

    x = myScreen.screenSizeX();
    y = myScreen.screenSizeY();
    dx = x / 1;
    dy = y / 4;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(dx * 0, dy * 0, dx, dy, myColours.black);
    myScreen.dRectangle(dx * 0, dy * 1, dx, dy, myColours.white);
    myScreen.dRectangle(dx * 0, dy * 2, dx, dy, myColours.red);
    myScreen.dRectangle(dx * 0, dy * 3, dx, dy, myColours.yellow);

    myScreen.setFontSolid(false);
    myScreen.gText(dx * 0 + 4, dy * 0 + 4, "Black", myColours.white);
    myScreen.gText(dx * 0 + 4, dy * 1 + 4, "White", myColours.black);
    myScreen.gText(dx * 0 + 4, dy * 2 + 4, "Red", myColours.white);
    myScreen.gText(dx * 0 + 4, dy * 3 + 4, "Yellow", myColours.black);

    myScreen.flush();
}

#endif // DISPLAY_COLOURS_BWRY

// Add setup code
///
/// @brief Setup
///
void setup()
{
    // mySerial = Serial by default, otherwise edit hV_HAL_Peripherals.h
    mySerial.begin(115200);
    delay(500);
    mySerial.println();
    mySerial.println("=== " __FILE__);
    mySerial.println("=== " __DATE__ " " __TIME__);
    mySerial.println();

    mySerial.print("begin... ");
    myScreen.begin();
    mySerial.println(formatString("%s %ix%i", myScreen.WhoAmI().c_str(), myScreen.screenSizeX(), myScreen.screenSizeY()));

#if (DISPLAY_COLOURS_BWRY == 1)

    mySerial.println("DISPLAY_COLOURS_BWRY... ");
    myScreen.clear();
    displayColoursBWRY();
    wait(8);

#endif // DISPLAY_COLOURS_BWRY

    mySerial.print("White... ");
    myScreen.clear();
    myScreen.flush();

    mySerial.println("=== ");
    mySerial.println();
}

// Add loop code
///
/// @brief Loop, empty
///
void loop()
{
    delay(1000);
}
