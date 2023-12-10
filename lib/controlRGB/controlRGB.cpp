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

// extern RgbLed rgbled;

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

Input Value : Class struct instantiation for pre-defined status colours, containing
              R, G and B values for a specific colour output:
                 blue_BT_connected{0, 0, 255}
                 green_fully_charged_battery{0, 255, 0}
                 magenta_low_battery{255, 255, 0}
                 red_critically_low_battery{255, 0, 0}
                 led_off{0, 0, 0}
Return Value: - n/a -
********************************************************************************/
void RgbLed::setRgbColour(const RgbLed::StatusColour& statusColour) {
    analogWrite(_red_pin, statusColour.red);
    analogWrite(_green_pin, statusColour.green);
    analogWrite(_blue_pin, statusColour.blue);
}

/*****************************************************************************
Description : Displ

Input Value :
Return Value:
********************************************************************************/
//!  Need to fix statusColour argument passing as method is not right
void RgbLed::ledBlink(const RgbLed::StatusColour& statusColour,
                      unsigned long blink_interval_msec) {
    switch ((millis() / blink_interval_msec) % 2) {
        case 0:
            this->setRgbColour(this->led_off);
            break;
        case 1:
            this->setRgbColour(this->red_critically_low_battery);

            break;
    }
}

/*
  unsigned long now = millis();

  if (((now - _start_time) >= blink_interval_msec)) {
      _start_time = now;
 */