
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

#include "esp_adc_cal.h"

// internal (user) libraries:
#include "myConstants.h"  // all constants in one file + pinout table
#include "press_type.h"   // wrapper library further abstracting Yabl / Bounce2 switch routines

//? ************** Selective Debug Scaffolding *********************
// Selective debug scaffold set-up; comment out appropriate lines below to disable debugging tests at pre-processor stage
//   Note: #ifdef preprocessor simply tests if the symbol's been defined; therefore don't use #ifdef 0
//   Ref: https://stackoverflow.com/questions/16245633/ifdef-debug-versus-if-debug
//? *****************************************************************
#define DEBUG 1  // uncomment to debug
//? ************ end Selective Debug Scaffolding ********************

//   battery operating ranges
#define HIGH_BATTERY_VOLTAGE 3.70  // 4.2 - 3.7V battery "fully" charged
#define LOW_BATTERY_VOLTAGE 3.00   // lower bound battery operating range (DW01 battery protection circuit triggers at 2.4V )

int current_battery_level = 100;  // initially set to fully charged, 100%

const byte BLE_DELAY = 10;  // Delay (milliseconds) to prevent BT congestion

// blekeyboard instantiation params: (BT device name, BT Device manufacturer, Battery Level)
BleKeyboard bleKeyboard("flipTurn", "CW Greenstreet", current_battery_level);

bool hasRun = 0;  // run flag to control single execution within loop

/*****************************************************************************
Description : Reads the battery voltage through the voltage divider at AO pin (FireBeetle-ESP32, DFR0478)
*!             Must solder-bridge zero-ohm pads to enable voltage divider hardware (ref DFR0478 Ver3 schematic)

              Uses eFuse calibrations, if present (ESP32-E), otherwise alternative characterisation used
              In comparison with a regular voltmeter, ESP32 vs. multimeter values differ only ~0.05V
Input Value : -
Return Value: battery voltage in volts
---------------------------------
Ref:  ADC1_CHANNEL_0 Enumeration
https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html#_CPPv414ADC1_CHANNEL_0)
********************************************************************************/
float readBattery() {
    uint32_t value = 0;
    int rounds = 11;
    esp_adc_cal_characteristics_t adc_chars;

    // battery voltage divided by 2 can be measured at GPIO36 (ADC1_CHANNEL0)
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    switch (esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars)) {
        case ESP_ADC_CAL_VAL_EFUSE_TP:
#ifdef DEBUG
            // Serial.println("Characterized using Two Point Value");
#endif
            break;
        case ESP_ADC_CAL_VAL_EFUSE_VREF:
#ifdef DEBUG
            // Serial.printf("Characterised using eFuse Vref (%d mV)\r\n", adc_chars.vref);
#endif
            break;
        default:
            Serial.printf("Characterised using Default Vref (%d mV)\r\n", 1100);
    }

    // to avoid noise, sample the pin several times and average the result
    for (int i = 1; i <= rounds; i++) {
        value += adc1_get_raw(ADC1_CHANNEL_0);
    }
    value /= (uint32_t)rounds;

    // due to the voltage divider (1M+1M), multiply value by 2 and convert mV to V
    return esp_adc_cal_raw_to_voltage(value, &adc_chars) * 2.0 / 1000.0;
}

/*****************************************************************************
Description : Tests for low battery charge (<=3V) and update BT Central device
                with (%) battery charge level

Input Value : battery_voltage (volts)
Return Value: true / false
********************************************************************************/
bool isBatteryLow(uint32_t battery_voltage) {
    static unsigned long updateTimer_msec = 0;

    if (millis() - updateTimer_msec > 1000) {
        bleKeyboard.setBatteryLevel(
            battery_voltage >= HIGH_BATTERY_VOLTAGE ? 100 : 10 + 90 * (battery_voltage - LOW_BATTERY_VOLTAGE) / (HIGH_BATTERY_VOLTAGE - LOW_BATTERY_VOLTAGE));
        delay(BLE_DELAY);
        // Serial.printf("Battery: %d%%\n", 10 + 90 * (battery_voltage - LOW_BATTERY_VOLTAGE) / (HIGH_BATTERY_VOLTAGE - LOW_BATTERY_VOLTAGE));
        updateTimer_msec = millis();
    }

    return battery_voltage <= LOW_BATTERY_VOLTAGE ? true : false;
}

/*****************************************************************************
Description : calculates battery level (percentage) and updates BT Central device

Input Value : battery voltage (in Volts)
Return Value: -
*******************************************************************************
int setBatteryLevel(float battery_voltage) {
    // do something
    bleKeyboard.setBatteryLevel(current_battery_level);  // update battery level
    {
        // batteryAvg - LOW_VOLTAGE) / (HI_VOLTAGE - LOW_VOLTAGE)
*/

//  Ref: https://www.w3schools.com/colors/colors_picker.asp
struct StatusColour {
    int red, green, blue;  // rgb values, permissible values 0 - 255.
};

// pre-define status notification colours
StatusColour blue_BT_connected{0, 0, 255};
StatusColour green_fully_charged_battery{0, 255, 0};
StatusColour magenta_low_battery{255, 255, 0};  // used magenta as orange colour was not distinct
StatusColour red_critically_low_battery{255, 0, 0};
StatusColour white_blink{0, 0, 0};

// TODO: explore gamma corrections to RGB luminosity (due to different voltages) for acceptable orange to replace magenta

/*****************************************************************************
Description : sets a defined colour on RGB LED by setting R, G and B values in an array
                const pass by ref avoids inefficient copying yet prevents any changes to underlying struct

Input Value : R, G and B values for a specific colour output
                see https://www.w3schools.com/colors/colors_picker.asp
Return Value: - n/a -
********************************************************************************/
void setRgbColour(const StatusColour& statusColour) {
    analogWrite(RED_LED_PIN, statusColour.red);
    analogWrite(GREEN_LED_PIN, statusColour.green);
    analogWrite(BLUE_LED_PIN, statusColour.blue);
}

/*****************************************************************************
Description : blinks red (RGB) LED

Input Value : LED state ON : OFF
Return Value: -
********************************************************************************/
void RedLedState(bool state) {
    if (state) {
        setRgbColour(red_critically_low_battery);
    } else {
        setRgbColour(white_blink);
    }
}

void setup() {
    Serial.begin(115200);

#ifdef DEBUG
    Serial.println("Preparing flipTurn for BLE connection");
#endif

    bleKeyboard.begin();

    // initialise button (eg switch) press_type set-up code (pin, pullup mode, callback function
    button.begin(SWITCH_PIN);

    // set-up RGB LED
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

}  // end setup

void loop() {
    yield();  // let ESP32 background functions play through to avoid potential WDT reset
    button.update();

    float battery_voltage = readBattery();  // in Volts
    battery_voltage = 3.0;                  //! temporary debug line - remove!

    static unsigned long flash_timer = 0;

    unsigned long current_time;
    bool flash_Led = false;

    /*
    #ifdef DEBUG  // test LED colours
        setRgbColour(blue_BT_connected);
        delay(1000);
        setRgbColour(green_fully_charged_battery);
        delay(1000);
        setRgbColour(magenta_low_battery);
        delay(1000);
        setRgbColour(red_critically_low_battery);
        delay(1000);
    #endif
    */

    // TODO:  auto-shutdown if battery_voltage < 3V

    if (bleKeyboard.isConnected()) {
        current_time = millis();
        if (isBatteryLow(battery_voltage)) {
            if (current_time - flash_timer > 1000)
                flash_timer = millis();
        } else {  // Force any current flash off if battery recovers
            flash_timer = 0;
        }

        flash_Led = (current_time - flash_timer >= 900) && (current_time - flash_timer <= 1000) ? true : false;

        //setRgbColour(blue_BT_connected);
        //delay(50);  //! debug line - remove!

        battery_voltage = 3.0;  //! temporary debug line - remove!

        RedLedState(flash_Led ? true : false);

        if (hasRun = 0) {
            Serial.println("flipTurn BLE Device now connected!");
            hasRun = 1;  // toggle flag to run connection notification only once
        }

        //! warning: delay() is blocking but necessary to prevent BT GATT overflow errors; must keep short or interferes with button presses
        delay(BLE_DELAY);  //  optimised through trial & error, where no delay gives BT GATT overflow errors

        if (button.triggered(SINGLE_TAP)) {
            yield();  // Do (almost) nothing - yield allows ESP8266 background functions
            bleKeyboard.write(KEY_DOWN_ARROW);
            Serial.println("Single Tap = Down Arrow");
        }

        if (button.triggered(DOUBLE_TAP)) {
            yield();  // Do (almost) nothing -- yield allows ESP8266 background functions
            bleKeyboard.write(KEY_UP_ARROW);
            Serial.println("Double Tap = Up Arrow");
        }

        if (button.triggered(LONG_PRESS)) {
            yield();                             // Do (almost) nothing -- yield allows ESP8266 background functions
            bleKeyboard.write(KEY_MEDIA_EJECT);  // toggles visibility of IOS virtual on-screen keyboard
            Serial.println("Long Press = Eject / show Battery Status Colour");
        }

    }  // end if ( bleKeyboard.isConnected() )

}  // end loop()
