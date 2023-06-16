///
/// @file BWRY_Palette.ino
/// @brief Example for Pervasive Displays Library Suite
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jun 2023
/// @version 611
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// @see ReadMe.txt for references
/// @n
///

// SDK
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#else // Arduino general
#include "Arduino.h"
#endif // SDK

// Set parameters
#define DISPLAY_PALETTE_BWRY 1
#define DISPLAY_COLOURS_BWRY 0

// Include application, user and local libraries
#include "SPI.h"
#include "PDLS_EXT3_Basic_BWRY.h"

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

    Serial.print("Colours BWRY... ");
    myScreen.clear();
    displayColoursBWRY();

    wait(8);

    Serial.println("done");

#endif // DISPLAY_PALETTE_BWRY

    Serial.print("White... ");
    myScreen.clear();
    myScreen.flush();

    Serial.println("=== ");
    Serial.println();

#if defined(ARDUINO_ARCH_PIDUINO)
    exit(0);
#endif
}

// Add loop code
void loop()
{
    delay(1000);
}
