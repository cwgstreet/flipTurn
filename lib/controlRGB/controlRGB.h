/*
 * *************************************************************
 * controlRGB.h - Header file for common cathode RGB LED control library
 *
 *
 *  C W Greenstreet, Ver1, 10Dec23
 *    MIT Licence - Released into the public domain
 *
 *
 * ************************************************************ */

#ifndef CONTROL_RGB_H
#define CONTROL_RGB_H

#if ARDUINO >= 100  // this if-else block manages depreciated versions of Arduino IDE
#include <Arduino.h>
#else
#include <WConstants.h>
#include <WProgram.h>
#include <pins_arduino.h>
#endif  // end if-block



class RgbLed {
   public:
    RgbLed(int red_pin, int green_pin, int blue_pin);  // constructor prototype

    struct StatusColour {
        int red, green, blue;  // rgb values, permissible values 0 - 255.
    };                         //  Colour Picker Ref: https://www.w3schools.com/colors/colors_picker.asp

    // pre-define status notification colours
    StatusColour blue_BT_connected{0, 0, 255};
    StatusColour green_fully_charged_battery{0, 255, 0};
    StatusColour magenta_low_battery{255, 255, 0};  // used magenta as orange colour was not distinct
    StatusColour red_critically_low_battery{255, 0, 0};
    StatusColour led_off{0, 0, 0};  // common cathode - current sourcing

    // TODO: explore gamma corrections to RGB luminosity (due to different voltages) for acceptable orange to replace magenta

    // method prototypes:
    void setRgbColour(const RgbLed::StatusColour& statusColour);

    void ledBlink(const RgbLed::StatusColour& statusColour,
                          unsigned long blink_interval_msec);

   private:
    int _red_pin,
        _green_pin,
        _blue_pin;

    //unsigned long currentInterval_;  // time till we change state
    //unsigned long _start_time;        // when we last changed state
};

//extern RgbLed rgbled;

#endif  // end header guard