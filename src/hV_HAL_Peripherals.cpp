//
// hV_HAL_Peripherals.cpp
// C++ code
// ----------------------------------
//
// Project highView Library Suite
//
// Created by Rei Vilo, 08 Jan 2024
//
// Copyright (c) Rei Vilo, 2010-2024
// Licence All rights reserved
//
// * Basic edition: for hobbyists and for basic usage
// Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
//
// * Evaluation edition: for professionals or organisations, evaluation only, no commercial usage
// All rights reserved
//
// * Commercial edition: for professionals or organisations, commercial usage
// All rights reserved
//

#include "hV_HAL_Peripherals.h"

struct h_pinSPI3_t
{
    uint8_t pinClock;
    uint8_t pinData;
};

h_pinSPI3_t h_pinSPI3;

void hV_HAL_SPI3_begin(uint8_t pinClock, uint8_t pinData)
{
    h_pinSPI3.pinClock = pinClock;
    h_pinSPI3.pinData = pinData;
}

uint8_t hV_HAL_SPI3_read()
{
    uint8_t value = 0;

    pinMode(h_pinSPI3.pinClock, OUTPUT);
    pinMode(h_pinSPI3.pinData, INPUT);

    for (uint8_t i = 0; i < 8; ++i)
    {
        digitalWrite(h_pinSPI3.pinClock, HIGH);
        delay(1);
        value |= digitalRead(h_pinSPI3.pinData) << (7 - i);
        digitalWrite(h_pinSPI3.pinClock, LOW);
        delay(1);
    }

    return value;
}

void hV_HAL_SPI3_write(uint8_t value)
{
    pinMode(h_pinSPI3.pinClock, OUTPUT);
    pinMode(h_pinSPI3.pinData, OUTPUT);

    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(h_pinSPI3.pinData, !!(value & (1 << (7 - i))));
        delay(1);
        digitalWrite(h_pinSPI3.pinClock, HIGH);
        delay(1);
        digitalWrite(h_pinSPI3.pinClock, LOW);
        delay(1);
    }
}