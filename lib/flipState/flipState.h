/*
 * *************************************************************
 * flipState.h - Header file for flipTurn state machine
 *
 *
 *  C W Greenstreet, Ver1, 10Dec23
 *    MIT Licence - Released into the public domain
 *
 * ************************************************************ */

#ifndef FLIP_STATE_H  // begin header guard
#define FLIP_STATE_H 

#if ARDUINO >= 100  // this if-else block manages depreciated versions of Arduino IDE
#include <Arduino.h>
#else
#include <WConstants.h>
#include <WProgram.h>
#include <pins_arduino.h>
#endif  // end if-block

/*
class xxx {
   public:
    RgbLed(int red_pin, int green_pin, int blue_pin);  // constructor prototype

    
    // method prototypes:
    void setRgbColour(const RgbLed::StatusColour& statusColour);

    void ledBlink(const RgbLed::StatusColour& statusColour,
                  unsigned long blink_interval_msec);

    void functionTest();

   private:
    //  private var and methods
};
*/

#endif  // end header guard