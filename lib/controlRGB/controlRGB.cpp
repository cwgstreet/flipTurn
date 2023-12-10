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

/*****************************************************************************
Description : Displays a defined RGB LED colour by passing R, G and B values through a struct
                const pass by ref avoids inefficient copying yet prevents any changes to underlying struct

Input Value : R, G and B values for a specific colour output
                see https://www.w3schools.com/colors/colors_picker.asp
Return Value: - n/a -
********************************************************************************/
void RgbLed::setRgbColour(const RgbLed::StatusColour& statusColour) {
    
    analogWrite(_red_pin, statusColour.red);  
    analogWrite(_green_pin, statusColour.green);  
    analogWrite(_blue_pin, statusColour.blue);    
}
