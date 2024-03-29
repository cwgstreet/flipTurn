
/**
 ** *************************************************************************************
 *?    flipTurn-main.cpp
 *       (comments are formatted with VSCode Better Comments extension)
 **  ---------------------------------------------------------------------------------
 **  flipTurn - ESP-Arduino software to send BLE keyboard pagnation commands to
 **    sheet music Apps like Unreal Book
 **  Copyright (C) 2022-2023 Carl W Greenstreet
 *
 **  This program is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 2 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License along
 **  with this program; if not, write to the Free Software Foundation, Inc.,
 **  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **  ---------------------------------------------------------------------------------
 *
 *?   Purpose:  Send BLE pagnation commands to sheet music App Unreal Book
 *      Project Repository:  https://github.com/cwgstreet/flipTurn
 *      Project Wiki:        https://github.com/cwgstreet/flipTurn/wiki
 *
 * *  flipTurn foot-switch operation:
 * *     1) Short Press - Page Down
 * *     2) Double Press - Page Up
 * *     3) Press Hold (long Press):  trigger onscreen virtual keyboard in IOS, and
 * *         show battery charge status colour (green = fully charged, magenta = low charge, red = critically low charge)
 *
 *?   Pin-out Summary: Refer to myConstants.h for pin-out table plus also see github flipTurn wiki
 *
 *?   Credits (3rd Party Libraries, code snippets, etc)
 *    -------------------------------------------------
 *     This application uses or adapts Open Source code. You can find the source code of their open source projects
 *     along with license information below. We acknowledge and are grateful to these developers for their
 *     contributions to open source.
 *
 **      Project: LOLIN32-BT-Page-Turner
 **        https://github.com/raichea/LOLIN32-BT-Page-Turner
 **        https://www.thingiverse.com/thing:4880077/files
 *         Use: Overall project inspiration
 *         Copyright (c) 2021 raichea
 *         License: CC4.0 International Attribution; Creative Commons - Attribution license
 *
 **      Project: Bounce2  https://github.com/thomasfredericks/Bounce2
 *         Use: Debouncing library for Arduino and Wiring
 *         Copyright (c) 2013 thomasfredericks
 *         License (MIT) https://github.com/thomasfredericks/Bounce2/blob/master/LICENSE
 *
 **      Project: ESP32-BLE-Keyboard  https://github.com/T-vK/ESP32-BLE-Keyboard
 *         Use: BLE Keyboard library for ESP32 devices; used to send pagnation commands to iPad
 *         Copyright (c) 2019 T-vK
 *         License (MIT / GPL3) licence discussion: https://github.com/T-vK/ESP32-BLE-Keyboard/issues/60
 *         Fork used in this project: https://github.com/cwgstreet/ESP32-BLE-Keyboard-with-EJECT
 *          Fork enables KEY_MEDIA_EJECT keypress, necessary to toggle on virtual onscreen keyboard in IOS
 *
 **      Project: Firebeetle-2-ESP32-E motion sensor https://github.com/Torxgewinde/Firebeetle-2-ESP32-E
 *         Incorporated / adapted code snippets on measuring and managing LiPo battery voltage with ESP-32
 *         Copyright (C) 2021 Tom Stöveken
 *         License (GPL2): https://github.com/Torxgewinde/Firebeetle-2-ESP32-E/blob/main/LICENSE
 *
 *?    Revisions:
 *       2023.11.27   Ver1 - code under development
 *
 ** *************************************************************************************/

// external libraries:
#include <Arduino.h>  // IDE requires Arduino framework to be explicitly included
#include <BleKeyboard.h>
#include <Bounce2.h>

// internal (user) libraries:
#include "flipState.h"    //  library to manage flipTurn state machine
#include "myConstants.h"  // all constants in one file + pinout table
#include "press_type.h"   // wrapper library further abstracting Yabl / Bounce2 switch routines

//? ************** Selective Debug Scaffolding *********************
// Selective debug scaffold: comment out  lines below to disable debugging tests at pre-processor stage
//   Note: #ifdef preprocessor simply tests if the symbol's been defined; therefore #ifdef 0 will not work!
//   Ref: https://stackoverflow.com/questions/16245633/ifdef-debug-versus-if-debug
//? *****************************************************************
// #define DEBUG 1  // uncomment to debug
//? ************ end Selective Debug Scaffolding ********************

extern const byte BLE_DELAY;       // Delay (milliseconds) to prevent BT congestion
extern int current_battery_level;  // initially set to fully charged, 100%

// timer - global
unsigned long ledTimer_msec = 0;

// run-once flags
bool hasRun = 0;           // run flag to control single execution within loop
bool flipStateHasRun = 0;  // run flag to run flipState config once

void setup() {
    Serial.begin(115200);
    delay(STARTUP_DELAY_MSEC);  // give serial monitor time to initialise to display early status messages

    // flipState = battery_status;  // show battery status at power-up

#ifdef DEBUG
    Serial.println("Preparing flipTurn for BLE connection");
#endif

    bleKeyboard.begin();

    // initialise button (eg foot switch); see press_type set-up code
    button.begin(SWITCH_PIN);

}  // end setup

void loop() {
    yield();  // let ESP32 background functions play through to avoid potential WDT reset

    // automatically show battery status on LED at device start-up
    if (!flipStateHasRun) {  // flag ensures this runs once only
        ledTimer_msec = millis();  // get timer mark for flipState
        flipState = battery_status;
#ifdef DEBUG
        Serial.println("--------------------------");
        Serial.print("flipStateHasRun; flipState = ");
        Serial.println(flipState);
        Serial.println("--------------------------");
#endif
        flipStateHasRun = 1;  // toggle flag to run connection notification only once
    }

    processState();

    // monitor switch button with response depending on designated pressTypes (Single Press, Double Press, Hold Press)
    if (button.update()) {
        // true = when a switch (button press) event triggered

        if (button.triggered(SINGLE_TAP)) {
            bleKeyboard.write(KEY_DOWN_ARROW);
            Serial.println("Single Tap = Down Arrow");
        }

        else if (button.triggered(DOUBLE_TAP)) {
            bleKeyboard.write(KEY_UP_ARROW);
            Serial.println("Double Tap = Up Arrow");
        }

        else if (button.triggered(HOLD)) {
            bleKeyboard.write(KEY_MEDIA_EJECT);  // toggles visibility of IOS virtual on-screen keyboard
            ledTimer_msec = millis();            //! update times; trying to debug flipState
            flipState = battery_status;

            Serial.println("Long Press = Eject / show Battery Status Colour");
        }
    }

}  // end loop()
