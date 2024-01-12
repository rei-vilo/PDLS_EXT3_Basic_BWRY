//
// Screen_EPD_EXT3.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Rei Vilo, 2010-2024
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// Portions (c) Pervasive Displays, 2010-2024
//
// Release 508: Added support for E2969CS0B and E2B98CS0B
// Release 511: Improved stability for external SPI SRAM
// Release 529: Added support for pixmap file
// Release 530: Added support for external SRAM
// Release 531: Added getBoardPins
// Release 601: Added support for screens with embedded fast update
// Release 604: Improved optional external power management
// Release 607: Improved screens names consistency
// Release 608: Added screen report
// Release 609: Added temperature management
// Release 610: Removed partial update
// Release 611: Added support for red and yellow colour screens
// Release 700: Refactored screen and board functions
// Release 701: Improved functions names consistency
// Release 702: Added support for xE2417QS0Ax
// Release 800: Read OTP memory
//

// Library header
#include "Screen_EPD_EXT3.h"

#if defined(ENERGIA)
///
/// @brief Proxy for SPISettings
/// @details Not implemented in Energia
/// @see https://www.arduino.cc/en/Reference/SPISettings
///
struct _SPISettings_s
{
    uint32_t clock; ///< in Hz, checked against SPI_CLOCK_MAX = 16000000
    uint8_t bitOrder; ///< LSBFIRST, MSBFIRST
    uint8_t dataMode; ///< SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3
};
///
/// @brief SPI settings for screen
///
_SPISettings_s _settingScreen;
#else
///
/// @brief SPI settings for screen
///
SPISettings _settingScreen;
#endif // ENERGIA

#ifndef SPI_CLOCK_MAX
#define SPI_CLOCK_MAX 16000000
#endif

//
// === COG section
//
/// @cond
/// @see
/// * ApplicationNote_Spectra_4_smallSize_EPD_v01_20230522.pdf
//
// Use of delay in µs before and after SPI
// 0 = no, 1..500 = yes; recommended = 1
#define USE_DELAY_US 1

void Screen_EPD_EXT3::COG_initial()
{
    // Application note § 3. COG initial
    // Work settings
    b_sendCommandData8(0x01, COG_initialData[16]); // PWR
    b_sendIndexData(0x00, &COG_initialData[17], 2); // PSR
    b_sendIndexData(0x03, &COG_initialData[30], 3); // PFS

    if (u_eScreen_EPD == eScreen_EPD_417_QS_0A)
    {
        if (COG_initialData[2] == 0x86)
        {
            b_sendIndexData(0x06, &COG_initialData[23], 3); // BTST_P
        }
        else if (COG_initialData[2] == 0x82)
        {
            b_sendIndexData(0x06, &COG_initialData[23], 4); // BTST_P
        }
        else
        {
            mySerial.println();
            mySerial.println("* ERROR - CoG type not supported");
            while(true);;
        }
    }
    else
    {
        b_sendIndexData(0x06, &COG_initialData[23], 7); // BTST_P
    }

    b_sendCommandData8(0x50, COG_initialData[39]); // CDI
    b_sendIndexData(0x60, &COG_initialData[40], 2); // TCON
    b_sendIndexData(0x61, &COG_initialData[19], 4); // TRES
    b_sendCommandData8(0xE7, COG_initialData[33]); //
    b_sendCommandData8(0xE3, COG_initialData[42]); // PWS

    if (u_eScreen_EPD == eScreen_EPD_417_QS_0A)
    {
        b_sendIndexData(0x65, &COG_initialData[34], 4); // TRES
    }
    else
    {
        b_sendCommandData8(0x4D, COG_initialData[43]); //
        b_sendCommandData8(0xB4, COG_initialData[44]); //
        b_sendCommandData8(0xB5, COG_initialData[45]); //
    }

    b_sendCommandData8(0xE9, 0x01); //
    b_sendCommandData8(0x30, 0x08); // PLL
}

void Screen_EPD_EXT3::COG_getUserData()
{
#if defined(ARDUINO_FEATHER_ESP32)

    hV_HAL_SPI3_begin(SCK, MOSI) // SCK SDA

#elif defined(ARDUINO_XIAO_ESP32C3)

    // Board Xiao ESP32-C3 crashes if pins are not specified.
    hV_HAL_SPI3_begin(8, 9) // SCK SDA

#elif defined(ARDUINO_NANO_ESP32)

    // Board Arduino Nano ESP32 arduino_nano_nora v2.0.11
    hV_HAL_SPI3_begin(SCK, MOSI) // SCK SDA

#elif defined(ARDUINO_ARCH_ESP32)

    // void begin(int8_t sck=-1, int8_t miso=-1, int8_t mosi=-1, int8_t ss=-1);
    // Board ESP32-Pico-DevKitM-2 crashes if pins are not specified.
    hV_HAL_SPI3_begin(14, 12) // SCK SDA

#else

    hV_HAL_SPI3_begin(SCK, MOSI); // SCK SDA

#endif // ARDUINO

    // 1.6 Read OTP memory mapping data
    uint16_t u_chipId;
    uint16_t u_readBytes;

    // Size cSize cType Driver
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_154_QS_0F: // 1.54”
        case eScreen_EPD_213_QS_0F: // 2.13”
        case eScreen_EPD_266_QS_0F: // 2.66”

            u_chipId = 0x0302;
            u_readBytes = 48;
            break;

        case eScreen_EPD_417_QS_0A: // 4.17”

            u_chipId = 0x0605;
            u_readBytes = 112;
            break;

        default:

            break;
    }

    // GPIO
    digitalWrite(b_pin.panelReset, HIGH);

    // Check
    digitalWrite(b_pin.panelDC, LOW); // Command
    digitalWrite(b_pin.panelCS, LOW); // Select
    hV_HAL_SPI3_write(0x70);
    digitalWrite(b_pin.panelCS, HIGH); // Unselect
    delay(8); // Added

    uint16_t ui16 = 0;
    digitalWrite(b_pin.panelDC, HIGH); // Data
    digitalWrite(b_pin.panelCS, LOW); // Select
    ui16 = hV_HAL_SPI3_read();
    digitalWrite(b_pin.panelCS, HIGH); // Unselect
    ui16 <<= 8;
    digitalWrite(b_pin.panelDC, HIGH); // Data
    digitalWrite(b_pin.panelCS, LOW); // Select
    ui16 |= hV_HAL_SPI3_read();
    digitalWrite(b_pin.panelCS, HIGH); // Unselect

    mySerial.println();
    if (ui16 == u_chipId)
    {
        mySerial.println(formatString(". INFO - Check 1 passed - Chip ID %04x as expected", ui16));
    }
    else
    {
        mySerial.println(formatString("* ERROR - Check 1 failed - Chip ID 0x%04x, expected 0x%04x", ui16, u_chipId));
        while(0x01);
    }

    // Read OTP
    uint8_t ui8 = 0;
    uint16_t offset = 0x0000;

    if (u_chipId == 0x0302)
    {
        digitalWrite(b_pin.panelDC, LOW); // Command
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0xa4);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        digitalWrite(b_pin.panelDC, HIGH); // Data
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x15);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x00);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x01);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        delay(8);

        digitalWrite(b_pin.panelDC, LOW); // Command
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0xa1);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        digitalWrite(b_pin.panelDC, HIGH); // Data
        digitalWrite(b_pin.panelCS, LOW); // Select
        ui8 = hV_HAL_SPI3_read();
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);

        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_initialData[0] = hV_HAL_SPI3_read(); // First byte for check
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        // Check table start and set bank offset
        if (COG_initialData[0] == 0xa5)
        {
            offset = 0x00;
        }
        else
        {
            mySerial.println(formatString("* ERROR - Check 2 failed - Bank %i, first 0x%02x, expected 0x%02x", 0, COG_initialData[0], 0xa5));
            while(0x01);
        }
    }
    else if (u_chipId == 0x0605)
    {
        digitalWrite(b_pin.panelDC, LOW); // Command
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0xa2);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        digitalWrite(b_pin.panelDC, HIGH); // Data
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x00);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x15);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x00);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        digitalWrite(b_pin.panelDC, LOW); // Command
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0xa0);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        digitalWrite(b_pin.panelDC, LOW); // Command
        digitalWrite(b_pin.panelCS, LOW); // Select
        hV_HAL_SPI3_write(0x92);
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        delay(10);

        digitalWrite(b_pin.panelDC, HIGH); // Data
        digitalWrite(b_pin.panelCS, LOW); // Select
        ui8 = hV_HAL_SPI3_read(); // Dummy
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);

        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_initialData[0] = hV_HAL_SPI3_read(); // First byte for check
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        if (COG_initialData[0] == 0xa5) // First byte check = 0xa5
        {
            offset = 0x00;
        }
        else
        {
            offset = 0x70;
            for (uint16_t index = 1; index < offset; index += 1)
            {
                digitalWrite(b_pin.panelCS, LOW); // Select
                hV_HAL_SPI3_read(); // Ignore bytes 1..offset
                digitalWrite(b_pin.panelCS, HIGH); // Unselect
            }

            digitalWrite(b_pin.panelCS, LOW); // Select
            COG_initialData[0] = hV_HAL_SPI3_read(); // First byte for check
            digitalWrite(b_pin.panelCS, HIGH); // Unselect

            if (COG_initialData[0] == 0xa5) // First byte check = 0xa5
            {
                offset = 0x70;
            }
            else
            {
            mySerial.println(formatString("* ERROR - Check 2 failed - Bank %i, first 0x%02x, expected 0x%02x", 0, COG_initialData[0], 0xa5));
            while(0x01);
            }
        }
    }

    mySerial.println(formatString(". INFO - Check 2 passed - Bank %i, first 0x%02x as expected", (offset > 0x00), COG_initialData[0]));

    // Populate COG_initialData
    for (uint16_t index = 1; index < u_readBytes; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_initialData[index] = hV_HAL_SPI3_read(); // Read OTP
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
    }

    u_flagOTP = true;

    /*
    // Debug COG_initialData
    mySerial.print(formatString("const uint8_t COG_initialData[%i] =", u_readBytes));
    mySerial.println();
    mySerial.print("{");
    for (uint16_t index = 0; index < u_readBytes; index += 1)
    {
        if ((index % 8) == 0)
        {
            mySerial.println();
            mySerial.print("   ");
        }

        mySerial.print(formatString("0x%02x", COG_initialData[index]));
        mySerial.print(formatString("%s ", (index + 1 < u_readBytes ? "," : ""))); // no comma on last value
    }
    mySerial.println();

    mySerial.print(formatString("} // %i", u_readBytes));
    mySerial.println();
    */
}

void Screen_EPD_EXT3::COG_sendImageDataGlobal()
{
    // Application note § 4. Input image to the EPD
    if (u_eScreen_EPD == eScreen_EPD_417_QS_0A) // 417: 0x04 before sending image
    {
        b_sendCommand8(0x04); // Power on
        digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
        b_waitBusy();
    }

#if (SRAM_MODE == USE_INTERNAL_MCU)

    b_sendIndexData(0x10, u_newImage, u_frameSize); // First frame, blackBuffer

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    b_sendIndexDataSRAM(0x10, u_newImage, u_frameSize); // First frame

#endif // SRAM_MODE
}

void Screen_EPD_EXT3::COG_update()
{
    // Application note § 5. Send updating command
    if (u_eScreen_EPD == eScreen_EPD_417_QS_0A)
    {
    }
    else // All except 417: 0x04 after sending image
    {
        b_sendCommand8(0x04); // Power on
        digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
        b_waitBusy();
    }

    b_sendCommandData8(0x12, 0x00); // Display Refresh
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
    b_waitBusy();
}

void Screen_EPD_EXT3::COG_powerOff()
{
    // Application note § 5. Turn-off DC/DC
    b_sendCommandData8(0x02, 0x00); // Turn off DC/DC
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
    b_waitBusy();

    if (u_eScreen_EPD == eScreen_EPD_417_QS_0A)
    {
        if (COG_initialData[43] == 0xff)
        {
            delay(5000);
            b_sendIndexData(0x00, &COG_initialData[26], 2); // PSR
        }
    }
}
/// @endcond
//
// === End of COG section
//

//
// === Class section
//
Screen_EPD_EXT3::Screen_EPD_EXT3(eScreen_EPD_t eScreen_EPD_EXT3, pins_t board)
{
    u_eScreen_EPD = eScreen_EPD_EXT3;
    b_pin = board;
    u_newImage = 0; // nullptr
}

void Screen_EPD_EXT3::begin()
{
    // u_eScreen_EPD = eScreen_EPD;
    u_codeSize = SCREEN_SIZE(u_eScreen_EPD);
    u_codeFilm = SCREEN_FILM(u_eScreen_EPD);
    u_codeDriver = SCREEN_DRIVER(u_eScreen_EPD);
    u_codeExtra = SCREEN_EXTRA(u_eScreen_EPD);
    _screenColourBits = 2; // BWR and BWRY

    // Checks
    if (u_codeFilm != FILM_Q)
    {
        mySerial.println();
        mySerial.println("* ERROR - Not a black-white-red-yellow screen");
        while (true);
    }

    switch (u_codeSize)
    {
        case SIZE_154: // 1.54"

            _screenSizeV = 152; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            // _screenDiagonal = 154;
            break;

        case SIZE_213: // 2.13"

            _screenSizeV = 212; // vertical = wide size
            _screenSizeH = 104; // horizontal = small size
            // _screenDiagonal = 213;
            break;

        case SIZE_266: // 2.66"

            _screenSizeV = 296; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            // _screenDiagonal = 266;
            break;

        case SIZE_417: // 4.17"

            _screenSizeV = 300; // vertical = wide size
            _screenSizeH = 400; // horizontal = small size
            // _screenDiagonal = 417;
            break;

        default:

            break;
    } // u_codeSize
    _screenDiagonal = u_codeSize;

    _screenMarginV = 0;
    _screenMarginH = 0;

    u_bufferDepth = 1; // 1 single buffer with 2 bits per pixel
    u_bufferSizeV = _screenSizeV; // vertical = wide size
    u_bufferSizeH = _screenSizeH / 4; // horizontal = small size 112 / 4; 2 bits per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

    // u_frameSize = u_pageColourSize, except for 9.69 and 11.98
    u_frameSize = u_pageColourSize;
    // 9.69 and 11.98 combine two half-screens, hence two frames with adjusted size

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (u_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        u_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (u_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        u_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(u_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    // Initialise the /CS pins
    pinMode(b_pin.panelCS, OUTPUT);
    digitalWrite(b_pin.panelCS, HIGH); // CS# = 1

    // New generic solution
    pinMode(b_pin.panelDC, OUTPUT);
    pinMode(b_pin.panelReset, OUTPUT);
    pinMode(b_pin.panelBusy, INPUT); // All Pins 0

    // Initialise Flash /CS as HIGH
    if (b_pin.flashCS != NOT_CONNECTED)
    {
        pinMode(b_pin.flashCS, OUTPUT);
        digitalWrite(b_pin.flashCS, HIGH);
    }

    // Initialise slave panel /CS as HIGH
    if (b_pin.panelCSS != NOT_CONNECTED)
    {
        pinMode(b_pin.panelCSS, OUTPUT);
        digitalWrite(b_pin.panelCSS, HIGH);
    }

    // Initialise slave Flash /CS as HIGH
    if (b_pin.flashCSS != NOT_CONNECTED)
    {
        pinMode(b_pin.flashCSS, OUTPUT);
        digitalWrite(b_pin.flashCSS, HIGH);
    }

    // Initialise SD-card /CS as HIGH
    if (b_pin.cardCS != NOT_CONNECTED)
    {
        pinMode(b_pin.cardCS, OUTPUT);
        digitalWrite(b_pin.cardCS, HIGH);
    }

    // Read OTP memory
    COG_getUserData();

    // Initialise SPI
    _settingScreen = {16000000, MSBFIRST, SPI_MODE0};

#if defined(ENERGIA)

    SPI.begin();
    SPI.setBitOrder(_settingScreen.bitOrder);
    SPI.setDataMode(_settingScreen.dataMode);
    SPI.setClockDivider(SPI_CLOCK_MAX / min(SPI_CLOCK_MAX, _settingScreen.clock));

#else

#if defined(ARDUINO_XIAO_ESP32C3)

    // Board Xiao ESP32-C3 crashes if pins are specified.
    SPI.begin(8, 9, 10); // SCK MISO MOSI

#elif defined(ARDUINO_NANO_ESP32)

    // Board Arduino Nano ESP32 arduino_nano_nora v2.0.11
    SPI.begin();

#elif defined(ARDUINO_ARCH_ESP32)

    // Board ESP32-Pico-DevKitM-2 crashes if pins are not specified.
    SPI.begin(14, 12, 13); // SCK MISO MOSI

#else

    SPI.begin();

#endif // ARDUINO_ARCH_ESP32

    SPI.beginTransaction(_settingScreen);

#endif // ENERGIA

    // Reset
    b_reset(10, 10, 20, 40, 10); // specific
    b_waitBusy(); // specific

    // Check type and get tables
    // COG_getUserData(); // nothing sent to panel

    // Standard
    hV_Screen_Buffer::begin();

    setOrientation(0);
    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    _penSolid = false;
    u_invert = false;

    // Report
    mySerial.println(formatString("= Screen %s %ix%i", WhoAmI().c_str(), screenSizeX(), screenSizeY()));
    mySerial.println(formatString("= Number %i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver));
    mySerial.println(formatString("= PDLS %s v%i.%i.%i", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));

    clear();
}

String Screen_EPD_EXT3::WhoAmI()
{
    char work[64] = {0};
    u_WhoAmI(work);

    return formatString("iTC %i.%02i\"%s", _screenDiagonal / 100, _screenDiagonal % 100, work);
}

uint8_t Screen_EPD_EXT3::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:
        case UPDATE_GLOBAL:

            _flushGlobal();
            break;

        default:

            mySerial.println("* PDLS - UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}

void Screen_EPD_EXT3::flush()
{
    flushMode(UPDATE_GLOBAL);
}

void Screen_EPD_EXT3::_flushGlobal()
{
    // Initialise
    COG_initial();

    // Send image data
    COG_sendImageDataGlobal();

    // Update
    COG_update();
    COG_powerOff();
}

void Screen_EPD_EXT3::clear(uint16_t colour)
{
    if (colour == myColours.grey)
    {
        // black = 0-1, white = 0-0
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b01000100 : 0b00010001; // black-white : white-black
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black = 0-1
        memset(u_newImage, 0b01010101, u_pageColourSize);
    }
    else if ((colour == myColours.black) xor u_invert)
    {
        // physical white = 0-0
        memset(u_newImage, 0b00000000, u_pageColourSize);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        memset(u_newImage, 0b11111111, u_pageColourSize);
    }
    else if (colour == myColours.darkRed)
    {
        // red = 1-1, black = 0-1
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b11011101 : 0b01110111; // red-black : black-red
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.lightRed)
    {
        // red = 1-1, white = 0-0
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b11001100 : 0b00110011; // red-white : white-red
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        memset(u_newImage, 0b10101010, u_pageColourSize);
    }
    else if (colour == myColours.darkYellow)
    {
        // yellow = 1-0, black = 0-1
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10011001 : 0b01100110; // yellow-black : black-yellow
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.lightYellow)
    {
        // yellow = 1-0, white = 0-0
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10001000 : 0b00100010; // yellow-white : white-yellow
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.orange)
    {
        // yellow = 1-0, red = 1-1
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10111011 : 0b11101110; // yellow-red : red-yellow
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                u_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
}

void Screen_EPD_EXT3::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    // _orientCoordinates() returns false = success, true = error
    if (_orientCoordinates(x1, y1) == RESULT_ERROR)
    {
        return;
    }

    // Convert combined colours into basic colours
    bool flagOdd = ((x1 + y1) % 2 == 0);

    if (colour == myColours.grey)
    {
        if (flagOdd)
        {
            colour = myColours.black; // black
        }
        else
        {
            colour = myColours.white; // white
        }
    }
    else if (colour == myColours.darkRed)
    {
        if (flagOdd)
        {
            colour = myColours.red; // red
        }
        else
        {
            colour = u_invert ? myColours.white : myColours.black; // white
        }
    }
    else if (colour == myColours.lightRed)
    {
        if (flagOdd)
        {
            colour = myColours.red; // red
        }
        else
        {
            colour = u_invert ? myColours.black : myColours.white; // black
        }
    }
    else if (colour == myColours.darkYellow)
    {
        if (flagOdd)
        {
            colour = myColours.yellow; // yellow
        }
        else
        {
            colour = myColours.black; // black
        }
    }
    else if (colour == myColours.lightYellow)
    {
        if (flagOdd)
        {
            colour = myColours.yellow; // yellow
        }
        else
        {
            colour = myColours.white; // white
        }
    }
    else if (colour == myColours.orange)
    {
        if (flagOdd)
        {
            colour = myColours.yellow; // yellow
        }
        else
        {
            colour = myColours.red; // red
        }
    }

    // Coordinates
    uint32_t z1 = _getZ(x1, y1);
    uint16_t b1 = _getB(x1, y1);

    // Basic colours
    if ((colour == myColours.black) xor u_invert)
    {
        // physical white = 0-0
        bitClear(u_newImage[z1], b1 + 1); // MSB
        bitClear(u_newImage[z1], b1);
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black = 0-1
        bitClear(u_newImage[z1], b1 + 1); // MSB
        bitSet(u_newImage[z1], b1);
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        bitSet(u_newImage[z1], b1 + 1); // MSB
        bitClear(u_newImage[z1], b1);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        bitSet(u_newImage[z1], b1 + 1); // MSB
        bitSet(u_newImage[z1], b1);
    }
}

void Screen_EPD_EXT3::_setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
}

bool Screen_EPD_EXT3::_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR; // false = success, true = error
    switch (_orientation)
    {
        case 3: // checked, previously 1

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                x = _screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                x = _screenSizeH - 1 - x;
                y = _screenSizeV - 1 - y;
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                y = _screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint16_t Screen_EPD_EXT3::_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}

uint32_t Screen_EPD_EXT3::_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;
    // According to 11.98 inch Spectra Application Note
    // at http:// www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=245146

    z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 2); // 4 pixels per byte

    return z1;
}

uint16_t Screen_EPD_EXT3::_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    b1 = 6 - 2 * (y1 % 4); // 4 pixels per byte

    return b1;
}

void Screen_EPD_EXT3::regenerate()
{
    clear(myColours.black);
    flush();

    delay(100);

    clear(myColours.white);
    flush();
}
//
// === End of Class section
//

//
// === Touch section
//

//
// === End of Touch section
//

