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

   private:
    int _red_pin, _green_pin, _blue_pin;
};

#endif  // end header guard