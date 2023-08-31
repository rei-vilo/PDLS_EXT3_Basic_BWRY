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
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
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
// Release 614: Added support for Arduino Nano ESP32 board 
//

// Library header
#include "SPI.h"
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
// Common settings
const uint8_t COG_initialData154QS[48] =
{
    0xa5, 0x18, 0x84, 0x04, 0x01, 0x53, 0x50, 0x51, 0x30, 0x31, 0x31, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0f, 0x29, 0x00, 0x98, 0x00, 0x98, 0x0f, 0x00, 0x3f, 0x0a, 0x25, 0x0a, 0x20, 0x10, 0x54,
    0x44, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x02, 0x02, 0x22, 0x78, 0xd0, 0x03, 0x00, 0x00,
};

const uint8_t COG_initialData213QS[48] =
{
    0xa5, 0x18, 0x84, 0x04, 0x01, 0x53, 0x50, 0x51, 0x30, 0x31, 0x44, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0f, 0x29, 0x00, 0x68, 0x00, 0xd4, 0x05, 0x00, 0x3f, 0x0a, 0x25, 0x0d, 0x16, 0x10, 0x54,
    0x44, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x02, 0x02, 0x22, 0x78, 0xd0, 0x03, 0x00, 0x00,
};

const uint8_t COG_initialData266QS[48] =
{
    0xa5, 0x18, 0x84, 0x04, 0x01, 0x53, 0x50, 0x51, 0x30, 0x5a, 0x5a, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0f, 0x29, 0x00, 0x98, 0x01, 0x28, 0x05, 0x00, 0x3f, 0x0a, 0x25, 0x12, 0x1a, 0x10, 0x54,
    0x44, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x02, 0x02, 0x22, 0x78, 0xd0, 0x00, 0x00, 0x00,
};

void Screen_EPD_EXT3::COG_initial()
{
    // Work settings
    // _sendIndexData(0x01, &COG_initialData[16], 1); // PWR
    _sendCommandData8(0x01, COG_initialData[16]); // PWR
    _sendIndexData(0x00, &COG_initialData[17], 2); // PSR
    _sendIndexData(0x03, &COG_initialData[30], 3); // PFS
    _sendIndexData(0x06, &COG_initialData[23], 7); // BTST_P
    // _sendIndexData(0x50, &COG_initialData[39], 1); // CDI
    _sendCommandData8(0x50, COG_initialData[39]); // CDI
    _sendIndexData(0x60, &COG_initialData[40], 2); // TCON
    _sendIndexData(0x61, &COG_initialData[19], 4); // TRES
    // _sendIndexData(0xE7, &COG_initialData[33], 1); //
    _sendCommandData8(0xE7, COG_initialData[33]); //
    // _sendIndexData(0xE3, &COG_initialData[42], 1); // PWS
    _sendCommandData8(0xE3, COG_initialData[42]); // PWS
    // _sendIndexData(0x4D, &COG_initialData[43], 1); //
    _sendCommandData8(0x4D, COG_initialData[43]); //
    // _sendIndexData(0xB4, &COG_initialData[44], 1); //
    _sendCommandData8(0xB4, COG_initialData[44]); //
    // _sendIndexData(0xB5, &COG_initialData[45], 1); //
    _sendCommandData8(0xB5, COG_initialData[45]); //

    // uint8_t index_E9[] = {0x01};
    // _sendIndexData(0xE9, index_E9, 1); //
    _sendCommandData8(0xE9, 0x01); //
    // uint8_t index_30[] = {0x08};
    // _sendIndexData(0x30, index_30, 1); // PLL
    _sendCommandData8(0x30, 0x08); // PLL
}

void Screen_EPD_EXT3::COG_getUserData()
{
    uint16_t _codeSizeType = _eScreen_EPD_EXT3 & 0xffff;

    // Size cSize cType Driver
    switch (_codeSizeType)
    {
        case 0x150f: // 1.54”

            COG_initialData = COG_initialData154QS;
            break;

        case 0x210f: // 2.13”

            COG_initialData = COG_initialData213QS;
            break;

        case 0x260f: // 2.66”

            COG_initialData = COG_initialData266QS;
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3::COG_sendImageDataGlobal()
{
    _sendIndexData(0x10, _newImage, _frameSize); // First frame, blackBuffer
}

void Screen_EPD_EXT3::COG_update()
{
    _sendCommand8(0x04); // Power on
    digitalWrite(_pin.panelCS, HIGH); // CS# = 1
    _waitBusy();

    // uint8_t data12[] = {0x00};
    // _sendIndexData(0x12, data12, 1); // Display Refresh
    _sendCommandData8(0x12, 0x00); // Display Refresh
    digitalWrite(_pin.panelCS, HIGH); // CS# = 1
    _waitBusy();
}

void Screen_EPD_EXT3::COG_powerOff()
{
    uint8_t data02[] = {0x00};
    // _sendIndexData(0x02, data02, 1); // Turn off DC/DC
    _sendCommandData8(0x02, 0x00); // Turn off DC/DC
    digitalWrite(_pin.panelCS, HIGH); // CS# = 1
    _waitBusy();
}
/// @endcond
//
// === End of COG section
//

// Utilities

// Class
Screen_EPD_EXT3::Screen_EPD_EXT3(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board)
{
    _eScreen_EPD_EXT3 = eScreen_EPD_EXT3;
    _pin = board;
    _newImage = 0; // nullptr
}

void Screen_EPD_EXT3::begin()
{
    _codeExtra = (_eScreen_EPD_EXT3 >> 16) & 0xff;
    _codeSize = (_eScreen_EPD_EXT3 >> 8) & 0xff;
    _codeType = _eScreen_EPD_EXT3 & 0xff;
    _screenColourBits = 2; // BWRY

    // Check
    if (not(_codeExtra & FEATURE_RED_YELLOW))
    {
        Serial.println();
        Serial.println("ERROR - Not a red and yellow screen");
        while (true);
    }

    switch (_codeSize)
    {
        case 0x15: // 1.54"

            _screenSizeV = 152; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            _screenDiagonal = 154;
            break;

        case 0x21: // 2.13"

            _screenSizeV = 212; // vertical = wide size
            _screenSizeH = 104; // horizontal = small size
            _screenDiagonal = 213;
            break;

        case 0x26: // 2.66"

            _screenSizeV = 296; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            _screenDiagonal = 266;
            break;

        default:

            break;
    } // _codeSize

    _bufferDepth = 1; // 1 buffer with 2 bits per pixel
    _bufferSizeV = _screenSizeV; // vertical = wide size
    _bufferSizeH = _screenSizeH / 4; // horizontal = small size 112 / 4; 2 bits per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    _pageColourSize = (uint32_t)_bufferSizeV * (uint32_t)_bufferSizeH;

    // _frameSize = _pageColourSize, except for 9.69 and 11.98
    _frameSize = _pageColourSize;

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(_pageColourSize * _bufferDepth);
        _newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[_pageColourSize * _bufferDepth];
        _newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(_newImage, 0x00, _pageColourSize * _bufferDepth);

    // Initialise the /CS pins
    pinMode(_pin.panelCS, OUTPUT);
    digitalWrite(_pin.panelCS, HIGH); // CS# = 1

    // New generic solution
    pinMode(_pin.panelDC, OUTPUT);
    pinMode(_pin.panelReset, OUTPUT);
    pinMode(_pin.panelBusy, INPUT); // All Pins 0

    // Initialise Flash /CS as HIGH
    if (_pin.flashCS != NOT_CONNECTED)
    {
        pinMode(_pin.flashCS, OUTPUT);
        digitalWrite(_pin.flashCS, HIGH);
    }

    // Initialise slave panel /CS as HIGH
    if (_pin.panelCSS != NOT_CONNECTED)
    {
        pinMode(_pin.panelCSS, OUTPUT);
        digitalWrite(_pin.panelCSS, HIGH);
    }

    // Initialise slave Flash /CS as HIGH
    if (_pin.flashCSS != NOT_CONNECTED)
    {
        pinMode(_pin.flashCSS, OUTPUT);
        digitalWrite(_pin.flashCSS, HIGH);
    }

    // Initialise SD-card /CS as HIGH
    if (_pin.cardCS != NOT_CONNECTED)
    {
        pinMode(_pin.cardCS, OUTPUT);
        digitalWrite(_pin.cardCS, HIGH);
    }

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
    _reset(10, 10, 20, 40, 10); // specific
    _waitBusy(); // specific

    // Check type and get tables
    COG_getUserData(); // nothing sent to panel

    _screenWidth = _screenSizeH;
    _screenHeigth = _screenSizeV;

    // Standard
    hV_Screen_Buffer::begin();

    setOrientation(0);
    if (_f_fontMax() > 0)
    {
        _f_selectFont(0);
    }
    _f_fontSolid = false;

    _penSolid = false;
    _invert = false;

    // Report
    Serial.println(formatString("= Screen %s %ix%i", WhoAmI(), screenSizeX(), screenSizeY()));
    Serial.println(formatString("= PDLS v%i", SCREEN_EPD_EXT3_RELEASE));

    clear();
}

void Screen_EPD_EXT3::_reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5)
{
    delay_ms(ms1); // delay_ms 5ms
    digitalWrite(_pin.panelReset, HIGH); // RES# = 1
    delay_ms(ms2); // delay_ms 5ms
    digitalWrite(_pin.panelReset, LOW);
    delay_ms(ms3);
    digitalWrite(_pin.panelReset, HIGH);
    delay_ms(ms4);
    digitalWrite(_pin.panelCS, HIGH); // CS# = 1
    delay_ms(ms5);
}

String Screen_EPD_EXT3::WhoAmI()
{
    String text = "iTC ";
    text += String(_screenDiagonal / 100);
    text += ".";
    text += String(_screenDiagonal % 100);
    text += "\" -";

#if (FONT_MODE == USE_FONT_HEADER)

    text += "H";

#elif (FONT_MODE == USE_FONT_FLASH)

    text += "F";

#elif (FONT_MODE == USE_FONT_TERMINAL)

    text += "T";

#else

    text += "?";

#endif // FONT_MODE

    return text;
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
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b01000100 : 0b00010001; // black-white : white-black
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor _invert)
    {
        // physical black = 0-1
        memset(_newImage, 0b01010101, _pageColourSize);
    }
    else if ((colour == myColours.black) xor _invert)
    {
        // physical white = 0-0
        memset(_newImage, 0b00000000, _pageColourSize);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        memset(_newImage, 0b11111111, _pageColourSize);
    }
    else if (colour == myColours.darkRed)
    {
        // red = 1-1, black = 0-1
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b11011101 : 0b01110111; // red-black : black-red
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.lightRed)
    {
        // red = 1-1, white = 0-0
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b11001100 : 0b00110011; // red-white : white-red
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        memset(_newImage, 0b10101010, _pageColourSize);
    }
    else if (colour == myColours.darkYellow)
    {
        // yellow = 1-0, black = 0-1
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b10011001 : 0b01100110; // yellow-black : black-yellow
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.lightYellow)
    {
        // yellow = 1-0, white = 0-0
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b10001000 : 0b00100010; // yellow-white : white-yellow
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.orange)
    {
        // yellow = 1-0, red = 1-1
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b10111011 : 0b11101110; // yellow-red : red-yellow
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
}

void Screen_EPD_EXT3::invert(bool flag)
{
    _invert = flag;
}

void Screen_EPD_EXT3::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

uint16_t Screen_EPD_EXT3::readPixel(uint16_t x1, uint16_t y1)
{
    return _getPoint(x1, y1);
}

void Screen_EPD_EXT3::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    // _orientCoordinates() returns false = success, true = error
    if (_orientCoordinates(x1, y1))
    {
        return;
    }

    uint32_t z1 = _getZ(x1, y1);
    uint8_t b1 = 6 - 2 * (y1 % 4);

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
            colour = _invert ? myColours.white : myColours.black; // white
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
            colour = _invert ? myColours.black : myColours.white; // black
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

    // Basic colours
    if ((colour == myColours.black) xor _invert)
    {
        // physical white = 0-0
        bitClear(_newImage[z1], b1 + 1); // MSB
        bitClear(_newImage[z1], b1);
    }
    else if ((colour == myColours.white) xor _invert)
    {
        // physical black = 0-1
        bitClear(_newImage[z1], b1 + 1); // MSB
        bitSet(_newImage[z1], b1);
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        bitSet(_newImage[z1], b1 + 1); // MSB
        bitClear(_newImage[z1], b1);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        bitSet(_newImage[z1], b1 + 1); // MSB
        bitSet(_newImage[z1], b1);
    }
}

void Screen_EPD_EXT3::_setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
}

bool Screen_EPD_EXT3::_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool flagError = true; // false = success, true = error
    switch (_orientation)
    {
        case 3: // checked, previously 1

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                x = _screenSizeV - 1 - x;
                flagError = false;
            }
            break;

        case 2: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                x = _screenSizeH - 1 - x;
                y = _screenSizeV - 1 - y;
                swap(x, y);
                flagError = false;
            }
            break;

        case 1: // checked, previously 3

            if ((x < _screenSizeV) and (y < _screenSizeH))
            {
                y = _screenSizeH - 1 - y;
                flagError = false;
            }
            break;

        default: // checked

            if ((x < _screenSizeH) and (y < _screenSizeV))
            {
                swap(x, y);
                flagError = false;
            }
            break;
    }

    return flagError;
}

uint32_t Screen_EPD_EXT3::_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;
    // According to 11.98 inch Spectra Application Note
    // at http:// www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=245146

    z1 = (uint32_t)x1 * _bufferSizeH + (y1 >> 2); // 4 pixels per byte

    return z1;
}

uint16_t Screen_EPD_EXT3::_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}

// Utilities
void Screen_EPD_EXT3::_sendCommand8(uint8_t command)
{
    digitalWrite(_pin.panelDC, LOW);
    digitalWrite(_pin.panelCS, LOW);

    SPI.transfer(command);

    digitalWrite(_pin.panelCS, HIGH);
}

void Screen_EPD_EXT3::_sendCommandData8(uint8_t command, uint8_t data)
{
    digitalWrite(_pin.panelDC, LOW); // LOW = command
    digitalWrite(_pin.panelCS, LOW);

    SPI.transfer(command);

    digitalWrite(_pin.panelDC, HIGH); // HIGH = data
    SPI.transfer(data);

    digitalWrite(_pin.panelCS, HIGH);
}

void Screen_EPD_EXT3::_waitBusy()
{
    // LOW = busy, HIGH = ready
    while (digitalRead(_pin.panelBusy) != HIGH)
    {
        delay(100);
    }
}

void Screen_EPD_EXT3::_sendIndexData(uint8_t index, const uint8_t * data, uint32_t size)
{
    digitalWrite(_pin.panelDC, LOW); // DC Low = Command
    digitalWrite(_pin.panelCS, LOW); // CS Low = Select

    delayMicroseconds(50);
    SPI.transfer(index);
    delayMicroseconds(50);

    digitalWrite(_pin.panelCS, HIGH); // CS High = Unselect
    digitalWrite(_pin.panelDC, HIGH); // DC High = Data
    digitalWrite(_pin.panelCS, LOW); // CS Low = Select

    delayMicroseconds(50);
    for (uint32_t i = 0; i < size; i++)
    {
        SPI.transfer(data[i]);
    }
    delayMicroseconds(50);

    digitalWrite(_pin.panelCS, HIGH); // CS High
}

//
// === Miscellaneous section
//
//
// === End of Miscellaneous section
//

//
// === Temperature section
//
void Screen_EPD_EXT3::setTemperatureC(int8_t temperatureC)
{
    _temperature = temperatureC;

    uint8_t _temperature2;
    if (_temperature < 0)
    {
        _temperature2 = -_temperature;
        _temperature2 = (uint8_t)(~_temperature2) + 1; // 2's complement
    }
    else
    {
        _temperature2 = _temperature;
    }
    // indexE5_data[0] = _temperature2;
}

void Screen_EPD_EXT3::setTemperatureF(int16_t temperatureF)
{
    int8_t temperatureC = ((temperatureF - 32) * 5) / 9; // C = (F - 32) * 5 / 9
    setTemperatureC(temperatureC);
}

uint8_t Screen_EPD_EXT3::checkTemperatureMode(uint8_t updateMode)
{
    // #define FEATURE_FAST 0x01 ///< With embedded fast update
    // #define FEATURE_TOUCH 0x02 ///< With capacitive touch panel
    // #define FEATURE_OTHER 0x04 ///< With other feature
    // #define FEATURE_WIDE_TEMPERATURE 0x08 ///< With wide operating temperature
    // #define FEATURE_RED 0x10 ///< With red colour
    // #define FEATURE_RED_YELLOW 0x20 ///< With red and yellow colours

    updateMode = UPDATE_GLOBAL;

    switch (_codeExtra & 0x19)
    {
        case FEATURE_FAST: // PS series

            // Fast 	PS 	Embedded fast update 	FU: +15 to +30 °C 	GU: 0 to +50 °C
            if ((_temperature < 0) or (_temperature > 50))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        case (FEATURE_FAST | FEATURE_WIDE_TEMPERATURE): // KS series

            // Wide 	KS 	Wide temperature and embedded fast update 	FU: 0 to +50 °C 	GU: -15 to +60 °C
            if ((_temperature < -15) or (_temperature > 60))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        case FEATURE_WIDE_TEMPERATURE: // HS series

            // Freezer 	HS 	Global update below 0 °C 	FU: - 	GU: -25 to +30 °C
            if ((_temperature < -25) or (_temperature > 30))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        case FEATURE_RED: // JS series
        case FEATURE_RED_YELLOW: // QS series

            // Red  JS 	Red colour 	FU: - 	GU: 0 to +40 °C
            // Red  QS 	Red and yellow colours 	FU: - 	GU: 0 to +40 °C
            if ((_temperature < 0) or (_temperature > 40))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        default: // CS series

            // Normal 	CS 	Global update above 0 °C 	FU: - 	GU: 0 to +50 °C
            updateMode = UPDATE_GLOBAL;
            if ((_temperature < 0) or (_temperature > 50))
            {
                updateMode = UPDATE_NONE;
            }
            break;
    }

    return updateMode;
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

            Serial.println("* PDLS - UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}
//
// === End of Temperature section
//

