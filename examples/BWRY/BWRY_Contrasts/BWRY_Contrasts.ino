///
/// @file BWRY_Contrasts.ino
/// @brief Example of features for basic edition
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Mar 2024
/// @version 801
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

#if (SCREEN_EPD_EXT3_RELEASE < 801)
#error Required SCREEN_EPD_EXT3_RELEASE 801
#endif // SCREEN_EPD_EXT3_RELEASE

// Set parameters
#define DISPLAY_COLOURS_BWRY 0
#define DISPLAY_CONTRASTS_BWRY 1
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
#if (DISPLAY_CONTRASTS_BWRY == 1)

///
/// @brief Palette test screen
///
void displayContrastsBWRY()
{
    const uint8_t grid = 4; // 4 or 5
    const uint16_t colours[4] = {myColours.black, myColours.white, myColours.red, myColours.yellow};
    const char label[4] = {'B', 'W', 'R', 'Y'};

    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);

    uint16_t x0, y0, dx, dy, dz;

    dz = min(myScreen.screenSizeX(), myScreen.screenSizeY()) / grid;
    x0 = (myScreen.screenSizeX() - dz * grid) / 2;
    y0 = (myScreen.screenSizeY() - dz * grid) / 2;

    myScreen.setPenSolid(true);
    myScreen.setFontSolid(false);
    myScreen.selectFont(myScreen.fontMax());
    dx = (dz - myScreen.characterSizeX()) / 2;
    dy = (dz - myScreen.characterSizeY()) / 2;

    for (uint8_t i = 0; i < grid; i++)
    {
        for (uint8_t j = 0; j < grid; j++)
        {
            myScreen.dRectangle(x0 + dz * i, y0 + dz * j, dz, dz, colours[i]);
            myScreen.gText(x0 + dz * i + dx, y0 + dz * j + dy, String(label[j]), colours[j]);
        }
    }

    myScreen.flush();
}

#endif // DISPLAY_CONTRASTS_BWRY

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

#if (DISPLAY_CONTRASTS_BWRY == 1)

    mySerial.println("DISPLAY_CONTRASTS_BWRY... ");
    myScreen.clear();
    displayContrastsBWRY();
    wait(8);

#endif // DISPLAY_CONTRASTS_BWRY

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
