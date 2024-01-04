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
 *   GND         GND        Split line ground between switch and RGB LED
 *   A0 (IO36)   BATT_PIN   Read battery voltage (must bridge Rx and Ry zero ohm resistor pads on Firebeetle voltage divider)
 *   D6 (IO10)   SWITCH_PIN Microswitch; enable built-in pullup resistor, eg pinMode(D6, INPUT_PULLUP);
 *   19 (IOxx)   R-LED      Red anode RGB LED (80 ohm current limiting resistor)
 *   23 (IOxx)   G-LED      Green anode RGB LED (12 ohm current limiting resistor)
 *   18 (IOxx )  B-LED      Blue anode RGB LED (12 ohm current limiting resistor)
 *  *******************************************************
 *    Note:  See Firebeetle pin mapping table for full board pin-out; pin number 35 is same as GPIO35 / IO35
 *
 *
 *   Initial prototype (this pin configuration not used in final device):
 *   -------    --------    -----------------------------------------------
 *   D2 (IO25)   R-LED      Red anode RGB LED (82 ohm current limiting resistor)
 *   D3 (IO26)   G-LED      Green anode RGB LED (12 ohm current limiting resistor)
 *   D4 (IO26 )  B-LED      Blue anode RGB LED (12 ohm current limiting resistor)
 */

//!  A0 Battery divider (GPIO36)
//!  ----------------------------------------------
//!   Another caveat is the usage of pin 36 A0 may conflict with wifi (note: not confirmed)
//!   To avoid that, the battery must be measured before the wifi is activated.

/*
 *  ************************************************************************************
 *   5mm RGB LED - Common Cathode   Source: AliExpress
 * 
 *    Pin    Function     Comment                       Current limiting resistor 
 *    ----   --------     ------------------------      -------------------------
 *    1      Red anode    Typ Vf (@ If=20mA) = 2.0V      82 ohm
 *    2      GND          Common Cathode
 *    3      Green anode  Typ Vf (@ If=20mA) = 3.2V      12 ohm
 *    4      Blue anode   Typ Vf (@ If=20mA) = 3.1V      12 ohm
 * ************************************************************************************/

//******************************************************
//? Constants
//*******************************************************/

// ---------------------------------------------------------
// Pin set-up listed below are for nodeMCU ESP8266
//? Note to self:  constexp better than const for variable values that should be known at compile
//?    time -> more memory efficient.  Also better than simple #define
//! cannot use "extern constexp", must use "const" instead, as with constexp "...it must be immediately constructed or assigned a value"
// ---------------------------------------------------------
constexpr int BATT_PIN = A0;    // Read battery voltage (must bridge Rx and Ry zero ohm resistor pads on Firebeetle voltage divider)
constexpr int SWITCH_PIN = D6;  // microswitch (wired NO; need to enable internal pullup)

// RGB pwm pin assignments
constexpr int RED_LED_PIN = 19;    // GPIO15
constexpr int GREEN_LED_PIN = 23;  // GPIO35
constexpr int BLUE_LED_PIN = 18;   // GPIO34

// ---------------------------------------------------------
// Baudrate:  Recommend 74480 baud rate for ESP8266 devices to match ESP8266 fixed bootloader initialisation speed
//  (otherwise you will get startup gibberish characters on serial monitor before serial speed syncs)
//  https://forum.arduino.cc/t/serial-monitor-only-shows-strange-symbols-arduino-mega-with-esp8266/640490/5
//  note: may have to manually reset board after flashing for code to work correctly
// #define SERIAL_MONITOR_SPEED 74880  // change to constexpr
constexpr int SERIAL_MONITOR_SPEED = 115200;

//   battery operating ranges
constexpr float HIGH_BATTERY_VOLTAGE = 3.70;  // 4.2 - 3.7V battery "fully" charged
constexpr float CHARGE_NOW_VOLTAGE = 3.20;    // trigger voltage to warn that device requires charging
constexpr float LOW_BATTERY_VOLTAGE = 3.00;   // lower bound battery operating range (DW01 battery protection circuit triggers at 2.4V )

// led status light duration
constexpr float LED_DURATION_MSEC = 3000;  // 3 seconds

// startup delay
constexpr float STARTUP_DELAY_MSEC = 4000;  // 4 seconds

// *******************************************************
//   Other constants
// *******************************************************

#endif  // end header guard