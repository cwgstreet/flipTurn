
/** *************************************************************************************
 *?    flipTurn-main.cpp
 *
 *       (note:  comments are formatted with VSCode Better Comments extension)
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
 *?   Purpose:  ESP-Arduino software to send BLE pagnation commands to sheet music Apps like Unreal Book
 *
 *    Project Repository:  https://github.com/cwgstreet/flipTurn
 *    Project Wiki:        https://github.com/cwgstreet/flipTurn/wiki
 *
 *    Code designed to work with the following hardware
 *      DFRobot ESP-32 dfr0478 Ver3 module
 *      3.7V LiPo rechargeable battery
 *      microswitches
 *
 *    Credits (3rd Party Libraries, code snippets, etc)
 *    -------------------------------------------------
 *     This application uses or adapts Open Source components. You can find the source code of their open source projects
 *     along with license information below. We acknowledge and are grateful to these developers for their
 *     contributions to open source.
 *
 **      Project: LOLIN32-BT-Page-Turner
 **      https://github.com/raichea/LOLIN32-BT-Page-Turner
 **      https://www.thingiverse.com/thing:4880077/files
 *       Use: Overall project inspiration
 *       Copyright (c) 2021 raichea
 *       License: CC4.0 International Attribution; Creative Commons - Attribution license
 *
 **      Project: Bounce2  https://github.com/thomasfredericks/Bounce2
 *       Use: Debouncing library for Arduino and Wiring
 *       Copyright (c) 2013 thomasfredericks
 *       License (MIT) https://github.com/thomasfredericks/Bounce2/blob/master/LICENSE
 *
 **      Project: ESP32-BLE-Keyboard  https://github.com/T-vK/ESP32-BLE-Keyboard
 *       Use: BLE Keyboard library for ESP32 devices; used to send pagnation commands to iPad
 *       Copyright (c) 2019 T-vK
 *       License (MIT / GPL3) licence discussion: https://github.com/T-vK/ESP32-BLE-Keyboard/issues/60
 *       Fork used in this project: https://github.com/cwgstreet/ESP32-BLE-Keyboard-with-EJECT
 *       Fork enables KEY_MEDIA_EJECT keypress, necessary to toggle on virtual onscreen keyboard in IOS
 *
 **      Project: Firebeetle-2-ESP32-E motion sensor https://github.com/Torxgewinde/Firebeetle-2-ESP32-E
 *       Incorporated / adapted code snippets on measuring and managing LiPo battery voltage with ESP-32
 *       Copyright (C) 2021 Tom Stöveken
 *       License (GPL2): https://github.com/Torxgewinde/Firebeetle-2-ESP32-E/blob/main/LICENSE
 *
 *    Revisions:
 *      2022.10.31   Ver1 - under development
 *
 * *************************************************************************************/

/*
 * *************************************************************
 *   Pin-out Summaries
 *     Refer to myConstants.h for pin-out table plus also see github flipTurn
 *       wiki for device schematic and breadboard hookup pictures
 * *************************************************************/

// external libraries:
#include <Arduino.h>  // IDE requires Arduino framework to be explicitly included
#include <BleKeyboard.h>
#include <Bounce2.h>

#include "esp_adc_cal.h"

// internal (user) libraries:
// #include "periphials.h"    // serial monitor function tests and usuage routines
#include "press_type.h"  // wrapper library abstracting Yabl / Bounce2 routines

//? ************** Selective Debug Scaffolding *********************
// Selective debug scaffold set-up; comment out appropriate lines below to disable debugging tests at pre-proccessor stage
//   Note: #ifdef preprocessor simply tests if the symbol's been defined; therefore don't use #ifdef 0
//   Ref: https://stackoverflow.com/questions/16245633/ifdef-debug-versus-if-debug
//? *****************************************************************
#define DEBUG 1  // uncomment to debug
//? ************ end Selective Debug Scaffolding ********************

// Li-Po battery management, per Firebeetle-2-ESP32-E motion sensor project (GPL2);
//   code snippets adapted to Firebeetle DFR0478 (Ver1)
#define LOW_BATTERY_VOLTAGE 3.20  // battery protection board should trigger off at this level
#define VERY_LOW_BATTERY_VOLTAGE 3.10
#define CRITICALLY_LOW_BATTERY_VOLTAGE 3.00  // battery damage at this level!

int currentBattLevel = 99;  // initially set to 99%

const byte BLE_DELAY = 10;  // Delay to prevent BT congestion

// blekeyboard instantiation params: (BT device name, BT Device manufacturer, Battery Level)
BleKeyboard bleKeyboard("flipTurn", "CW Greenstreet", currentBattLevel);

bool hasRun = 0;  // run flag to control single execution in loop

/*****************************************************************************
    function name : readBattery()
    Code adapted from Firebeetle-2-ESP32-E motion sensor project (changes made)
----------------
Description : Reads the battery voltage through the voltage divider at AO pin (FireBeetle-ESP32 Ver4)
               note: must physically bridge zero ohm pads on board to enable voltage divider hardware
               (see DFR0478 Ver3 schematic)

              If the ESP32-E has calibration eFused those will be used.
              In comparison with a regular voltmeter the values of ESP32 and
              multimeter differ only about 0.05V
Input Value : -
Return Value: battery voltage in volts

Ref:  ADC1_CHANNEL_0 Enumeration
https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html#_CPPv414ADC1_CHANNEL_0)
********************************************************************************/
float readBattery() {
    uint32_t value = 0;
    int rounds = 11;
    esp_adc_cal_characteristics_t adc_chars;

    // battery voltage divided by 2 can be measured at GPIO36, which equals ADC1_CHANNEL0 for v1
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    switch (esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars)) {
        case ESP_ADC_CAL_VAL_EFUSE_TP:
#ifdef DEBUG
            Serial.println("Characterized using Two Point Value");
#endif
            break;
        case ESP_ADC_CAL_VAL_EFUSE_VREF:
#ifdef DEBUG
            Serial.printf("Characterized using eFuse Vref (%d mV)\r\n", adc_chars.vref);
#endif
            break;
        default:
            Serial.printf("Characterized using Default Vref (%d mV)\r\n", 1100);
    }

    // to avoid noise, sample the pin several times and average the result
    for (int i = 1; i <= rounds; i++) {
        value += adc1_get_raw(ADC1_CHANNEL_0);
    }
    value /= (uint32_t)rounds;

    // due to the voltage divider (1M+1M) values must be multiplied by 2
    // and convert mV to V
    return esp_adc_cal_raw_to_voltage(value, &adc_chars) * 2.0 / 1000.0;
}

void setup() {
    Serial.begin(115200);

#ifdef DEBUG
    Serial.println("Preparing flipTurn for BLE connection");
#endif

    bleKeyboard.begin();

    // initialise button press_type set-up code (pin, pullup mode, callback function)
    button.begin(SWITCH_PIN);

#ifdef DEBUG_PRESSTYPE  // *****  debug - button press_type function tests *****
    button.functionTest();
#endif
}

void loop() {
    yield();  // let ESP32 background functions play through to avoid potential WDT reset
    button.update();

#ifdef DEBUG  // connection status
    if (hasRun) {
        if (bleKeyboard.isConnected()) {
            Serial.println("flipTurn BLE Device is now Connected");
            bleKeyboard.print("flipTurn is connected");
        }
        hasRun = 1;  // toggle flag to run only once
    }
#endif

    delay(100);  //! trying to avoid BT congestion -  note this is blocking!

    if (button.triggered(SINGLE_TAP)) {
        yield();  // Do (almost) nothing -- yield allows ESP8266 background functions
        // bleKeyboard.write(KEY_DOWN_ARROW);
        bleKeyboard.press(KEY_DOWN_ARROW);
        bleKeyboard.print("Single Tap = Down Arrow");
    }

    // delay(1000);

    if (button.triggered(DOUBLE_TAP)) {
        yield();  // Do (almost) nothing -- yield allows ESP8266 background functions
        // bleKeyboard.write(KEY_UP_ARROW);
        bleKeyboard.press(KEY_UP_ARROW);
        bleKeyboard.print("Double Tap = Arrow");
    }

    // delay(1000);

    if (button.triggered(LONG_PRESS)) {
        yield();  // Do (almost) nothing -- yield allows ESP8266 background functions
        //bleKeyboard.write(KEY_MEDIA_EJECT);
        bleKeyboard.press(KEY_MEDIA_EJECT);
        bleKeyboard.print("Long Press = Eject");
    }

    bleKeyboard.releaseAll();

    // delay(1000);

    bleKeyboard.setBatteryLevel(currentBattLevel);  // update battery level
}
