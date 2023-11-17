///
/// @file BWRY_Palette.ino
/// @brief Example for Pervasive Displays Library Suite
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 31 Aug 2023
/// @version 614
///
/// @copyright (c) Rei Vilo, 2010-2023
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

// Configuration
#include "hV_Configuration.h"

// Include application, user and local libraries
// #include <SPI.h>

// Set parameters
#define DISPLAY_COLOURS_BWRY 0
#define DISPLAY_CONTRASTS_BWRY 1
#define DISPLAY_PALETTE_BWRY 1

// Define structures and classes

// Define variables and constants
Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_154_BWRY, boardRaspberryPiPico_RP2040);
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_213_BWRY, boardRaspberryPiPico_RP2040);
// Screen_EPD_EXT3 myScreen(eScreen_EPD_EXT3_266_BWRY, boardRaspberryPiPico_RP2040);

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

#if (DISPLAY_PALETTE_BWRY == 1)

    Serial.print("DISPLAY_PALETTE_BWRY... ");
    myScreen.clear();
    displayPaletteBWRY();
    Serial.println("done");
    wait(8);

#endif // DISPLAY_PALETTE_BWRY

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
