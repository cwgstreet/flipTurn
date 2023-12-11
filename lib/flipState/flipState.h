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

// Prepare case names for flipTurn State Machine switch-case stucture
enum entryStates_t { BT_connected = 1,  // set enum 1 to 10 rather than default 0 for first element
                     high_battery_charge,
                     warning_charge_battery_now,
                     low_battery, 
                     auto_shut_down};  // 5

/*
// smokerState type defined in implementation smokerState.cpp, hence extern keyword
extern entryStates_t smokerState;

// globals, defined in main
extern long unsigned startCookTime_ms;
extern long unsigned previousMillis;
extern long unsigned transmitInterval;
extern long unsigned previousDisplayMillis;
extern long unsigned displayInterval;


/******************************************************
// Helper function prototype:
******************************************************

// process_state() prototype
void processState(CWG_LCD &lcd);



StatusColour blue_BT_connected{0, 0, 255};
    StatusColour green_fully_charged_battery{0, 255, 0};
    StatusColour magenta_charge_battery_warning{255, 255, 0};  // used magenta as orange colour was not distinct
    StatusColour red_critically_low_battery{255, 0, 0};
    StatusColour led_off{0, 0, 0};


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