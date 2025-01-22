# Pervasive Displays Library Suite - Basic edition - Black-White-Red-Yellow

[![Release](https://img.shields.io/github/v/release/rei-vilo/PDLS_EXT3_Basic_BWRY)](https://github.com/rei-vilo/PDLS_EXT3_Basic_BWRY/releases) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/77ecc9fa99834e299505f5f2a7c83e53)](https://app.codacy.com/gh/rei-vilo/PDLS_EXT3_Basic_BWRY/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rei-vilo/PDLS_EXT3_Basic&amp;utm_campaign=Badge_Grade) ![Arduino lint](https://github.com/rei-vilo/PDLS_EXT3_Basic_BWRY/actions/workflows/main.yml/badge.svg) [![GitHub issues](https://img.shields.io/github/issues/rei-vilo/PDLS_EXT3_Basic_Global)](https://github.com/rei-vilo/PDLS_EXT3_Basic/issues)

[![](https://img.shields.io/badge/-Wiki-blue)](https://docs.pervasivedisplays.com/) [![](https://img.shields.io/badge/-User_Guide-blue)](https://pdls.pervasivedisplays.com/userguide/index.html) [![](https://img.shields.io/badge/-Reference_manual-blue)](https://pdls.pervasivedisplays.com/reference/html/index.html)

The Pervasive Displays Library Suite is specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com) e-paper screens, extension boards and evaluation kits.

![](https://pdls.pervasivedisplays.com/userguide/img/Logo_PDI_text_320.png)

> ### The library **PDLS_EXT3_Basic_BWRY** is deprecated. 
> 
> Use the library [PDLS_Basic](https://github.com/rei-vilo/PDLS_Basic) with the driver [Pervasive_BWRY_Small](https://github.com/PervasiveDisplays/Pervasive_BWRY_Small) instead.
>
> For more information, please refer to the technical note [Migrate from release 8 to release 9](https://pdls.pervasivedisplays.com/userguide/Volume5/Chapter1/Part6/Section22/).

The **PDLS_EXT3_Basic_BWRY** supports the screens

* iTC [black-white-red-yellow colour](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red-yellow) screens (film `Q`);

the boards

* [EPD Extension Kit Gen 3 (EXT3 or EXT3.1)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/). 

## Features

Basic colours | Ten colours | Contrasts 
--- | --- | ---
![](img/BWRY_Colours.png) | ![](img/BWRY_Palette.png) | ![](img/BWRY_Contrasts.png)

*By combining the four **basic colours**, black, white, red and yellow, six other colours are obtained: grey, light red, dark red, light yellow, dark yellow and orange, totalling **ten colours**, with high **contrasts** for high-impact messages.*

The Pervasive Displays Library Suite provides a high-level interface to drive the e-paper screens.

* Global update
* Graphics routines
* Text routines
* Extended colours
* Four extended fonts with double-sized variants

## Documentation

* [Wiki](https://docs.pervasivedisplays.com/) 
* [User guide](https://pdls.pervasivedisplays.com/userguide/index.html) with commented examples
* [Reference manual](https://rei-vilo.github.io/PDLS_EXT3_Basic_Documentation/index.html) 

## Support

* Please refer to [PDLS_EXT3_Basic_Global](https://github.com/rei-vilo/PDLS_EXT3_Basic_Global/issues) for reporting issues and contributing pull requests.

## Installation

Software

* [Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://arduino.github.io/arduino-cli/)
* [Installing Additional Arduino Libraries](https://www.arduino.cc/en/guide/libraries)

    + Using the Library Manager
    + Importing a `.zip` Library

Hardware

* [Connecting the Pervasive Displays e-Paper EPD Extension Kit Gen 3](https://embeddedcomputing.weebly.com/connecting-the-e-paper-epd-extension-kit-gen-3.html)

## Configuration

Software

* Arduino SDK
* SPI library

Hardware

* Pervasive Displays EPD Extension Kit Gen 3 (EXT3 or EXT3.1)
* Pervasive Displays iTC colour (black-white-red-yellow) screens (film `Q`)

## Licence

**Copyright** &copy; Rei Vilo, 2010-2025

For exclusive use with Pervasive Displays screens

**Licence** [Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)](./LICENCE.md)

![](img/by-sa.svg)

**Portions** &copy; Pervasive Displays, 2010-2025

