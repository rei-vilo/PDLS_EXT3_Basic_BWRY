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
// For exclusive use with Pervasive Displays screens
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
// Release 801: Improved OTP implementation
// Release 802: Added references to application notes
// Release 802: Refactored CoG functions
// Release 803: Added types for string and frame-buffer
// Release 804: Improved power management
//

// Library header
#include "Screen_EPD_EXT3.h"

//
// === COG section
//
/// @cond
/// @see
/// * ApplicationNote_Spectra_4_smallSize_EPD_v01_20230522.pdf
/// * ApplicationNote_E5_SE_smallSize_EPD_v01_20230522
//

//
// --- Small screens with Q film
//
void Screen_EPD_EXT3::COG_SmallQ_reset()
{
    // Application note § 2. Power on COG driver
    b_reset(10, 10, 20, 40, 10); // BWRY specific
    b_waitBusy(); // BWRY specific
}

void Screen_EPD_EXT3::COG_SmallQ_getDataOTP()
{
    // 1.6 Read OTP memory mapping data
    uint16_t _chipId;
    uint16_t _readBytes = 0;
    u_flagOTP = false;

    // Size cSize cType Driver
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_154_QS_0F: // 1.54”
        case eScreen_EPD_213_QS_0F: // 2.13”
        case eScreen_EPD_266_QS_0F: // 2.66”

            _chipId = 0x0302;
            _readBytes = 48;
            break;

        case eScreen_EPD_417_QS_0A: // 4.17”

            _chipId = 0x0605;
            _readBytes = 112;
            break;

        default:

            break;
    }

    // GPIO
    // s_reset();
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
    if (ui16 == _chipId)
    {
        mySerial.println(formatString("hV . OTP check 1 passed - Chip ID %04x as expected", ui16));
    }
    else
    {
        mySerial.println();
        mySerial.println(formatString("hV * OTP check 1 failed - Chip ID 0x%04x, expected 0x%04x", ui16, _chipId));
        while (0x01);
    }

    // Read OTP
    // uint8_t ui8 = 0;
    uint16_t offset = 0x0000;

    if (_chipId == 0x0302)
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
        hV_HAL_SPI3_read(); // Dummy
        // ui8 = hV_HAL_SPI3_read();
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);

        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_data[0] = hV_HAL_SPI3_read(); // First byte for check
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        // Check table start and set bank offset
        if (COG_data[0] == 0xa5)
        {
            offset = 0x00;
        }
        else
        {
            mySerial.println();
            mySerial.println(formatString("hV * OTP check 2 failed - Bank %i, first 0x%02x, expected 0x%02x", 0, COG_data[0], 0xa5));
            while (0x01);
        }
    }
    else if (_chipId == 0x0605)
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
        hV_HAL_SPI3_read(); // Dummy
        // ui8 = hV_HAL_SPI3_read(); // Dummy
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
        // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);

        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_data[0] = hV_HAL_SPI3_read(); // First byte for check
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        if (COG_data[0] == 0xa5) // First byte check = 0xa5
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
            COG_data[0] = hV_HAL_SPI3_read(); // First byte for check
            digitalWrite(b_pin.panelCS, HIGH); // Unselect

            if (COG_data[0] == 0xa5) // First byte check = 0xa5
            {
                offset = 0x70;
            }
            else
            {
                mySerial.println();
                mySerial.println(formatString("hV * OTP check 2 failed - Bank %i, first 0x%02x, expected 0x%02x", 0, COG_data[0], 0xa5));
                while (0x01);
            }
        }
    }

    mySerial.println(formatString("hV . OTP check 2 passed - Bank %i, first 0x%02x as expected", (offset > 0x00), COG_data[0]));

    // Populate COG_data
    for (uint16_t index = 1; index < _readBytes; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // Select
        COG_data[index] = hV_HAL_SPI3_read(); // Read OTP
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
    }

    u_flagOTP = true;
}

void Screen_EPD_EXT3::COG_SmallQ_initial()
{
    // Application note § 3. COG initial
    // Work settings
    b_sendCommandData8(0x01, COG_data[16]); // PWR
    b_sendIndexData(0x00, &COG_data[17], 2); // PSR
    b_sendIndexData(0x03, &COG_data[30], 3); // PFS

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_417_QS_0A:

            if (COG_data[2] == 0x86)
            {
                b_sendIndexData(0x06, &COG_data[23], 3); // BTST_P
            }
            else if (COG_data[2] == 0x82)
            {
                b_sendIndexData(0x06, &COG_data[23], 4); // BTST_P
            }
            else
            {
                mySerial.println();
                mySerial.println("hV * ERROR - CoG type not supported");
                while (true);;
            }
            break;

        default:

            b_sendIndexData(0x06, &COG_data[23], 7); // BTST_P
            break;
    }

    b_sendCommandData8(0x50, COG_data[39]); // CDI
    b_sendIndexData(0x60, &COG_data[40], 2); // TCON
    b_sendIndexData(0x61, &COG_data[19], 4); // TRES
    b_sendCommandData8(0xe7, COG_data[33]); //
    b_sendCommandData8(0xe3, COG_data[42]); // PWS

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_417_QS_0A:

            b_sendIndexData(0x65, &COG_data[34], 4); // TRES
            break;

        default:

            b_sendCommandData8(0x4d, COG_data[43]); //
            b_sendCommandData8(0xb4, COG_data[44]); //
            b_sendCommandData8(0xb5, COG_data[45]); //
            break;
    }

    b_sendCommandData8(0xe9, 0x01); //
    b_sendCommandData8(0x30, 0x08); // PLL

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_417_QS_0A:

            b_sendCommand8(0x04); // Power on
            digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
            b_waitBusy();
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3::COG_SmallQ_sendImageData()
{
    // Application note § 4. Input image to the EPD
    b_sendIndexData(0x10, s_newImage, u_pageColourSize); // First frame, blackBuffer
}

void Screen_EPD_EXT3::COG_SmallQ_update()
{
    // Application note § 5. Send updating command
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_417_QS_0A:

            break;

        default: // All except 417: 0x04 after sending image

            b_sendCommand8(0x04); // Power on
            b_waitBusy();
            break;
    }

    b_sendCommandData8(0x12, 0x00); // Display Refresh
    b_waitBusy();
}

void Screen_EPD_EXT3::COG_SmallQ_powerOff()
{
    // Application note § 5. Turn-off DC/DC
    b_sendCommandData8(0x02, 0x00); // Turn off DC/DC
    b_waitBusy();

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_417_QS_0A:

            if (COG_data[43] == 0xff)
            {
                delay(5000);
                b_sendIndexData(0x00, &COG_data[26], 2); // PSR
            }
            break;

        default:

            break;
    }
}
//
// --- End of Small screens with Q film
//
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
    s_newImage = 0; // nullptr
    COG_data[0] = 0;
}

void Screen_EPD_EXT3::begin()
{
    // u_eScreen_EPD = eScreen_EPD_EXT3;
    u_codeSize = SCREEN_SIZE(u_eScreen_EPD);
    u_codeFilm = SCREEN_FILM(u_eScreen_EPD);
    u_codeDriver = SCREEN_DRIVER(u_eScreen_EPD);
    u_codeExtra = SCREEN_EXTRA(u_eScreen_EPD);
    v_screenColourBits = 2; // BWR and BWRY

    // Checks
    switch (u_codeFilm)
    {
        case FILM_Q: // BWRY

            break;

        default:

            debugVariant(FILM_Q);
            break;
    }

    //
    // === Touch section
    //

    //
    // === End of touch section
    //

    //
    // === Large screen section
    //

    //
    // === End of Large screen section
    //

    // Configure board
    b_begin(b_pin, FAMILY_SMALL, 50);

    // Sizes
    switch (u_codeSize)
    {
        case SIZE_154: // 1.54"

            v_screenSizeV = 152; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_213: // 2.13"

            v_screenSizeV = 212; // vertical = wide size
            v_screenSizeH = 104; // horizontal = small size
            break;

        case SIZE_266: // 2.66"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_417: // 4.17"

            v_screenSizeV = 300; // vertical = wide size
            v_screenSizeH = 400; // horizontal = small size
            break;

        default:

            mySerial.println();
            mySerial.println(formatString("hV * Screen %i-%cS-0%c is not supported", u_codeSize, u_codeFilm, u_codeDriver));
            while (0x01);
            break;
    } // u_codeSize
    v_screenDiagonal = u_codeSize;

    // Report
    mySerial.println(formatString("hV = Screen %s %ix%i", WhoAmI().c_str(), screenSizeX(), screenSizeY()));
    mySerial.println(formatString("hV = Number %i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver));
    mySerial.println(formatString("hV = PDLS %s v%i.%i.%i", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));
    mySerial.println();

    u_bufferDepth = 1; // 1 single buffer with 2 bits per pixel
    u_bufferSizeV = v_screenSizeV; // vertical = wide size
    u_bufferSizeH = v_screenSizeH / 4; // horizontal = small size 112 / 4; 2 bits per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(s_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    setTemperatureC(25); // 25 Celsius = 77 Fahrenheit
    b_fsmPowerScreen = FSM_OFF;
    setPowerProfile(MODE_AUTO, SCOPE_GPIO_ONLY);

    // Turn SPI on, initialise GPIOs and set GPIO levels
    // Reset panel and get tables
    resume();

    // Fonts
    hV_Screen_Buffer::begin(); // Standard

    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    // Orientation
    setOrientation(0);

    v_penSolid = false;
    u_invert = false;
}

STRING_TYPE Screen_EPD_EXT3::WhoAmI()
{
    char work[64] = {0};
    u_WhoAmI(work);

    return formatString("iTC %i.%02i\"%s", v_screenDiagonal / 100, v_screenDiagonal % 100, work);
}

void Screen_EPD_EXT3::suspend(uint8_t suspendScope)
{
    if (((suspendScope & FSM_GPIO_MASK) == FSM_GPIO_MASK) and (b_pin.panelPower != NOT_CONNECTED))
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) == FSM_GPIO_MASK)
        {
            b_suspend();
        }
    }
}

void Screen_EPD_EXT3::resume()
{
    // Target   FSM_ON
    // Source   FSM_OFF
    //          FSM_SLEEP
    if (b_fsmPowerScreen != FSM_ON)
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) != FSM_GPIO_MASK)
        {
            b_resume(); // GPIO

            s_reset(); // Reset

            b_fsmPowerScreen |= FSM_GPIO_MASK;
        }

        // Check type and get tables
        if (u_flagOTP == false)
        {
            s_getDataOTP(); // 3-wire SPI read OTP memory

            s_reset(); // Reset
        }

        // Start SPI
        hV_HAL_SPI_begin(16000000); // Fast 16 MHz, with unicity check
    }
}

uint8_t Screen_EPD_EXT3::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:
        case UPDATE_GLOBAL:

            s_flush();
            break;

        default:

            mySerial.println();
            mySerial.println("hV * UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}

void Screen_EPD_EXT3::flush()
{
    flushMode(UPDATE_GLOBAL);
}

void Screen_EPD_EXT3::s_reset()
{
    // Reset
    COG_SmallQ_reset();
}

void Screen_EPD_EXT3::s_getDataOTP()
{
    hV_HAL_SPI_end(); // With unicity check

    hV_HAL_SPI3_begin(); // Define 3-wire SPI pins

    // Get data OTP
    COG_SmallQ_getDataOTP(); // 3-wire SPI read OTP memory
}

void Screen_EPD_EXT3::s_flush(uint8_t updateMode)
{
    // Resume
    if (b_fsmPowerScreen != FSM_ON)
    {
        resume();
    }

    COG_SmallQ_initial(); // Initialise
    COG_SmallQ_sendImageData(); // Send image data
    COG_SmallQ_update(); // Update
    COG_SmallQ_powerOff(); // Power off

    // Suspend
    if (u_suspendMode == MODE_AUTO)
    {
        suspend(u_suspendScope);
    }
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
                s_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black = 0-1
        memset(s_newImage, 0b01010101, u_pageColourSize);
    }
    else if ((colour == myColours.black) xor u_invert)
    {
        // physical white = 0-0
        memset(s_newImage, 0b00000000, u_pageColourSize);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        memset(s_newImage, 0b11111111, u_pageColourSize);
    }
    else if (colour == myColours.darkRed)
    {
        // red = 1-1, black = 0-1
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b11011101 : 0b01110111; // red-black : black-red
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                s_newImage[i * u_bufferSizeH + j] = pattern;
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
                s_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        memset(s_newImage, 0b10101010, u_pageColourSize);
    }
    else if (colour == myColours.darkYellow)
    {
        // yellow = 1-0, black = 0-1
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10011001 : 0b01100110; // yellow-black : black-yellow
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                s_newImage[i * u_bufferSizeH + j] = pattern;
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
                s_newImage[i * u_bufferSizeH + j] = pattern;
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
                s_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
}

void Screen_EPD_EXT3::regenerate(uint8_t mode)
{
    clear();
    flush();
}

void Screen_EPD_EXT3::s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    if (s_orientCoordinates(x1, y1) == RESULT_ERROR)
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
    uint32_t z1 = s_getZ(x1, y1);
    uint16_t b1 = s_getB(x1, y1);

    // Basic colours
    if ((colour == myColours.black) xor u_invert)
    {
        // physical white = 0-0
        bitClear(s_newImage[z1], b1 + 1); // MSB
        bitClear(s_newImage[z1], b1);
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black = 0-1
        bitClear(s_newImage[z1], b1 + 1); // MSB
        bitSet(s_newImage[z1], b1);
    }
    else if (colour == myColours.yellow)
    {
        // physical yellow = 1-0
        bitSet(s_newImage[z1], b1 + 1); // MSB
        bitClear(s_newImage[z1], b1);
    }
    else if (colour == myColours.red)
    {
        // physical red = 1-1
        bitSet(s_newImage[z1], b1 + 1); // MSB
        bitSet(s_newImage[z1], b1);
    }
}

void Screen_EPD_EXT3::s_setOrientation(uint8_t orientation)
{
    v_orientation = orientation % 4;
}

bool Screen_EPD_EXT3::s_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR;
    switch (v_orientation)
    {
        case 3: // checked, previously 1

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                x = v_screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                x = v_screenSizeH - 1 - x;
                y = v_screenSizeV - 1 - y;
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                y = v_screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint32_t Screen_EPD_EXT3::s_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;

    z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 2); // 4 pixels per byte

    return z1;
}

uint16_t Screen_EPD_EXT3::s_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    b1 = 6 - 2 * (y1 % 4); // 4 pixels per byte

    return b1;
}

uint16_t Screen_EPD_EXT3::s_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
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

