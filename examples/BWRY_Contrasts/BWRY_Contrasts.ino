///
/// @file BWRY_Contrasts.ino
/// @brief Example of features for basic edition
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

#if (SCREEN_EPD_EXT3_RELEASE < 800)
#error Required SCREEN_EPD_EXT3_RELEASE 800
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
        Serial.print(formatString(" > %i  \r", i));
        delay(1000);
    }
    Serial.print("         \r");
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
    Serial.begin(115200);
    delay(500);
    Serial.println();
    Serial.println("=== " __FILE__);
    Serial.println("=== " __DATE__ " " __TIME__);
    Serial.println();

    Serial.print("begin... ");
    myScreen.begin();
    Serial.println(formatString("%s %ix%i", myScreen.WhoAmI().c_str(), myScreen.screenSizeX(), myScreen.screenSizeY()));

#if (DISPLAY_CONTRASTS_BWRY == 1)

    Serial.println("DISPLAY_CONTRASTS_BWRY... ");
    myScreen.clear();
    displayContrastsBWRY();
    wait(8);

#endif // DISPLAY_CONTRASTS_BWRY

    Serial.print("White... ");
    myScreen.clear();
    myScreen.flush();

    Serial.println("=== ");
    Serial.println();
}

// Add loop code
///
/// @brief Loop, empty
///
void loop()
{
    delay(1000);
}
