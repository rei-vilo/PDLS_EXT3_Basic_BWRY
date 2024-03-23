///
/// @file BWRY_Palette.ino
/// @brief Example for Pervasive Displays Library Suite
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
#define DISPLAY_PALETTE_BWRY 1

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
#if (DISPLAY_PALETTE_BWRY == 1)

void displayPaletteBWRY()
{
    // Grid = 5
    // .  .  .  Y  Y
    // .  .  R  O  R
    // .  W  lR lY W
    // B  G  dR dY B
    // B  W  R  Y

    // Grid = 4
    // .  .  .  Y
    // .  .  R  O
    // .  W  lR lY
    // B  G  dR dY

    const uint8_t grid = 4; // 4 or 5

    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);

    uint16_t x0, y0, dx, dy, dz;

    dz = min(myScreen.screenSizeX(), myScreen.screenSizeY()) / grid;
    x0 = (myScreen.screenSizeX() - dz * grid) / 2;
    y0 = (myScreen.screenSizeY() - dz * grid) / 2;

    myScreen.setPenSolid(true);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 0, y0 + dz * 4, dz, dz, myColours.black);
        myScreen.dRectangle(x0 + dz * 1, y0 + dz * 4, dz, dz, myColours.white);
        myScreen.dRectangle(x0 + dz * 2, y0 + dz * 4, dz, dz, myColours.red);
        myScreen.dRectangle(x0 + dz * 3, y0 + dz * 4, dz, dz, myColours.yellow);
    }
    myScreen.dRectangle(x0 + dz * 0, y0 + dz * 3, dz, dz, myColours.black);
    myScreen.dRectangle(x0 + dz * 1, y0 + dz * 3, dz, dz, myColours.grey);
    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 3, dz, dz, myColours.darkRed);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 3, dz, dz, myColours.darkYellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 3, dz, dz, myColours.black);
    }

    myScreen.dRectangle(x0 + dz * 1, y0 + dz * 2, dz, dz, myColours.white);
    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 2, dz, dz, myColours.lightRed);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 2, dz, dz, myColours.lightYellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 2, dz, dz, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 1, dz, dz, myColours.red);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 1, dz, dz, myColours.orange);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 1, dz, dz, myColours.red);
    }

    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 0, dz, dz, myColours.yellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 0, dz, dz, myColours.yellow);
    }

    myScreen.setPenSolid(false);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 0 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.white);
        myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.black);
        myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.white);
        myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.black);
    }
    myScreen.dRectangle(x0 + dz * 0 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    }

    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 0 + 2, dz - 4, dz - 4, myColours.black);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 0 + 2, dz - 4, dz - 4, myColours.black);
    }

    myScreen.setFontSolid(false);
    myScreen.selectFont(Font_Terminal8x12);

    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 0 + 4, y0 + dz * 4 + 4, "B", myColours.white);
        myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 4 + 4, "W", myColours.black);
        myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 4 + 4, "R", myColours.white);
        myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 4 + 4, "Y", myColours.black);
    }
    myScreen.gText(x0 + dz * 0 + 4, y0 + dz * 3 + 4, "B", myColours.white);
    myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 3 + 4, "G", myColours.white);
    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 3 + 4, "dR", myColours.white);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 3 + 4, "dY", myColours.white);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 3 + 4, "B", myColours.white);
    }

    myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 2 + 4, "W", myColours.black);
    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 2 + 4, "lR", myColours.black);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 2 + 4, "lY", myColours.black);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 2 + 4, "W", myColours.black);
    }

    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 1 + 4, "R", myColours.white);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 1 + 4, "O", myColours.white);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 1 + 4, "R", myColours.white);
    }

    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 0 + 4, "Y", myColours.black);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 0 + 4, "Y", myColours.black);
    }

    myScreen.flush();
}

#endif // DISPLAY_PALETTE_BWRY

// Add setup code
///
/// @brief Setup
///
void setup()
{
    mySerial.begin(115200);
    delay(500);
    mySerial.println();
    mySerial.println("=== " __FILE__);
    mySerial.println("=== " __DATE__ " " __TIME__);
    mySerial.println();

    mySerial.print("begin... ");
    myScreen.begin();
    mySerial.println(formatString("%s %ix%i", myScreen.WhoAmI().c_str(), myScreen.screenSizeX(), myScreen.screenSizeY()));

#if (DISPLAY_PALETTE_BWRY == 1)

    mySerial.println("DISPLAY_PALETTE_BWRY... ");
    myScreen.clear();
    displayPaletteBWRY();
    wait(8);

#endif // DISPLAY_PALETTE_BWRY

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
