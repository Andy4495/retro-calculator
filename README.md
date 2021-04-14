Retro Calculator
================
![Retro Calculator Image][8]

After seeing an Arduino Project Hub [article][5], I purchased a handful of [NSA1166][2] bubble LED displays from [Jameco][3] and decided to build a [retro calculator][8].

The calculator uses an [MSP430G2553][9] microcontroller and [ICM7228][10] LED driver along with a generic 4x4 matrix keypad.

To make the project as small as possible and to simplify the wiring, I created created a PCB design and ordered it from [OshPark][11].

Due to limitations of my keypad and the display (6 characters with a single fixed decimal point), the calculator only supports limited functions and integer math.

## Hardware ##
The Eagle BRD and SCH files can be found in the [hardware folder][12], along with a PDF of the [schematic][14].

## External Libraries ##
The calculator uses my [matrix keypad library][13] and [ICM7218 LED driver library][15].

## References ##
* Arduino Project Hub [article][5]
* NSA-1166 [datasheet][2]
* NSA-1166 devices at [Jameco][3]
* [MSP430G2553][9]
* [ICM7228][10]
* [Schematic][14]
* [Library][4] written specifically for NSA1166, without using discrete controller IC
* Vintage Novus 650 Mathbox [calculator][6] that used the NSA1166 display, including [interior pictures][7]

## License ##
The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]: https://github.com/Andy4495/matrix-keypad-library
[2]: https://www.jameco.com/Jameco/Products/ProdDS/2210976NAT.pdf
[3]: https://www.jameco.com/z/NSA1166-National-Semiconductor-LED-Display-7-Segment-Red-0-10-Inch-6-Digit-1-65V-5mA-2-Inch-Long_2210976.html
[4]: https://github.com/8bitkick/SevSegPlus
[5]: https://create.arduino.cc/projecthub/8bitkick/retro-led-bubble-display-d357e9
[6]: https://www.keesvandersanden.nl/calculators/novus650.php
[7]: https://www.keesvandersanden.nl/calculators/novus650_inside.php
[8]: ./hardware/Calculator.jpeg
[9]: https://www.ti.com/product/MSP430G2553
[10]: https://www.renesas.com/us/en/www/doc/datasheet/icm7228.pdf
[11]: https://oshpark.com/
[12]: ./hardware
[13]: https://github.com/Andy4495/matrix-keypad-library
[14]: ./hardware/retro-calculator-schematic.pdf
[15]: https://github.com/Andy4495/ICM7218
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
