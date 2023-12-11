/* ***************************************************************
 * myConstants.h - library listing all user-defined constants
 *
 *  C W Greenstreet, Ver1, 27Sep21
 *    MIT Licence - Released into the public domain
 *
 ** ************************************************************* */

#ifndef MY_CONSTANTS_H  // header guard
#define MY_CONSTANTS_H

#if ARDUINO >= 100  // this if-else block manages depreciated versions of Arduino IDE
#include <Arduino.h>
#else
#include <WConstants.h>
#include <WProgram.h>
#include <pins_arduino.h>
#endif  // end if-block


/* 
 * ******************************************************
 *   Pin-out Summaries
 *     See github flipTurn wiki for schematic and hardware details
 * *******************************************************/

/*
 *  DFR0478 (Firebeetle ESP32 Ver1) microcontroller pinout:
 *   Pin        Function    Comment
 *   -------    --------    -----------------------------------------------
 *   D2 (IO25)   R-LED      Red anode RGB LED (xx ohm current limiting resistor)
 *   D3 (IO26)   G-LED      Green anode RGB LED (xx ohm current limiting resistor)
 *   D4 (IO26 )  B-LED      Blue anode RGB LED (xx ohm current limiting resistor)
 *   GND         GND        Split line ground between switch and RGB LED
 *   A0 (IO36)   BATT_PIN   Read battery voltage (must bridge Rx and Ry zero ohm resistor pads on Firebeetle voltage divider)
 *   D6 (IO10)   SWITCH_PIN Microswitch; enable built-in pullup resistor, eg pinMode(D6, INPUT_PULLUP);
 *   xx (IOxx)   R-LED      (future) Red anode RGB LED (xx ohm current limiting resistor)
 *   xx (IOxx)   G-LED      (future) Green anode RGB LED (xx ohm current limiting resistor)
 *   xx (IOxx )  B-LED      (future) Blue anode RGB LED (xx ohm current limiting resistor)
 *  *******************************************************
 */

//!  A0 Battery divider (GPIO36)
//!  ----------------------------------------------
//!   Another caveat is the usage of pin 36 A0 may conflict with wifi. 
//!   To avoid that, the battery must be measured before the wifi is activated.

/*
 *
 *  5mm RGB LED - Common Cathode   Source: AliExpress
 *   Pin        Function      Comment
 *   -------    --------      ------------------------         
 *   1          Red anode     Typ Vf (@ If=20mA) = 2.0V
 *   2          GND           Common Cathode
 *   3          Green anode   Typ Vf (@ If=20mA) = 3.2V
 *   4          Blue anode    Typ Vf (@ If=20mA) = 3.1V               
 * *******************************************************/



//******************************************************
//? Constants
//*******************************************************/

// ---------------------------------------------------------
// Pin set-up listed below are for nodeMCU ESP8266
//? Note to self:  constexp better than const for variable values that should be known at compile
//?    time -> more memory efficient.  Also better than simple #define
//! cannot use "extern constexp", must use "const" instead, as with constexp "...it must be immediately constructed or assigned a value"
// ---------------------------------------------------------
constexpr int BATT_PIN = A0;     // Read battery voltage (must bridge Rx and Ry zero ohm resistor pads on Firebeetle voltage divider)
constexpr int SWITCH_PIN = D6;   // microswitch (wired NO; need to enable internal pullup)

// RGB pwm pin assignments
constexpr int RED_LED_PIN = D2;
constexpr int GREEN_LED_PIN = D3;
constexpr int BLUE_LED_PIN = D4;


// ---------------------------------------------------------
// Baudrate:  Recommend 74480 baud rate for ESP8266 devices to match ESP8266 fixed bootloader initialisation speed
//  (otherwise you will get startup gibberish characters on serial monitor before serial speed syncs)
//  https://forum.arduino.cc/t/serial-monitor-only-shows-strange-symbols-arduino-mega-with-esp8266/640490/5
//  note: may have to manually reset board after flashing for code to work correctly
//#define SERIAL_MONITOR_SPEED 74880  // change to constexpr
constexpr int SERIAL_MONITOR_SPEED = 115200;





// *******************************************************
//   Other constants
// *******************************************************



#endif  // end header guard