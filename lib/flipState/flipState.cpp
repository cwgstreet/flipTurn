/*
 * *************************************************************
 * flipState.h - implementation file for flipTurn state machine
 *
 *
 *  C W Greenstreet, Ver1, 10Dec23
 *    MIT Licence - Released into the public domain
 *
 * ************************************************************ */

//!  debug scaffold not used - remove if it remains unused
// set up debug scaffold; comment out following line if you want to "turn off" debugging to serial monitor
// #define DEBUG 1

#include "flipState.h"

// entryStates is an enum variable type defined in menu.h header file (as extern); smokerState is global
entryStates_t flipState;

void processState() {
    switch (flipState) {
        case BT_connected:
            /* code */
            break;

        case high_battery_charge:
            /* code */
            break;

        case warning_charge_battery_now:
            /* code */
            break;

        case low_battery:
            /* code */
            break;

        case auto_shut_down:
            /* code */
            break;

        default:
            break;
    }
}

/*
// RgbLed constructor for common cathode RGB LED
RgbLed::RgbLed(int red_pin, int green_pin, int blue_pin) : _red_pin(red_pin),
                                                           _green_pin(green_pin),
                                                           _blue_pin(blue_pin) {
    pinMode(_red_pin, OUTPUT);
    pinMode(_green_pin, OUTPUT);
    pinMode(_blue_pin, OUTPUT);
}*/

/*****************************************************************************
Purpose     : Blink RGB LED in designated colour

Input Value : statusColour (see RgbLed Class for designated colour choices),
              Blink interval in msec (interval = on time = off time)
Return Value: -
********************************************************************************/
/*
void RgbLed::ledBlink(const RgbLed::StatusColour& statusColour,
                      unsigned long blink_interval_msec) {
    // switch expression uses clever 1-line approach that evaluates to either 0 or 1
    //  ref: https://blog.wokwi.com/5-ways-to-blink-an-led-with-arduino/
    switch ((millis() / blink_interval_msec) % 2) {
        case 0:  // led off
            this->setRgbColour(this->led_off);
            break;
        case 1:  // led on, displayed in designated colour (passed as argument)
            this->setRgbColour(statusColour);
            break;
    }
}
*/
