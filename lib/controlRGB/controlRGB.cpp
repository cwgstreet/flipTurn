/*
 * *************************************************************
 * controlRGB.h - implementation file for common cathode 
 *   RGB LED control library
 *
 *
 *  C W Greenstreet, Ver1, 10Dec23
 *    MIT Licence - Released into the public domain
 *
 *
 * ************************************************************ */

#include "controlRGB.h"

/*
constexpr int RED_LED_PIN = D2;
constexpr int GREEN_LED_PIN = D3;
constexpr int BLUE_LED_PIN = D4;
*/

// RgbLed constructor for common cathode RGB LED
RgbLed::RgbLed(int red_pin, int green_pin, int blue_pin) : _red_pin(red_pin),
                                                           _green_pin(green_pin),
                                                           _blue_pin(blue_pin) {
    pinMode(_red_pin, OUTPUT);
    pinMode(_green_pin, OUTPUT);
    pinMode(_blue_pin, OUTPUT);
}

RgbLed::StatusColour statusColour;  