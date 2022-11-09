/* *************************************************************************************
 *    flipTurn-main.cpp
 *
 *   ---------------------------------------------------------------------------------
 *   flipTurn - Rechargeable bluetooth foot switch pedal to page-turn sheet music apps
 *   Copyright (C) 2022 Carl W Greenstreet
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *   ---------------------------------------------------------------------------------
 *
 *    Purpose:  ESP-Arduino software to send BLE pagnation commands to music Apps like Unreal Book
 *
 *    Project Repository:  https://github.com/cwgstreet/flipTurn
 *    Project Wiki:        https://github.com/cwgstreet/flipTurn/wiki
 *
 *    Code designed to work with the following hardware
 *      DFRobot ESP-32 dfr0478 module
 *      3.7V LiPo rechargeable battery
 *      microswitches
 *
 *
 *    Credits (3rd Party Libraries, code snippets, etc)
 *
 *     This application uses Open Source components. You can find the source code of their open source projects
 *     along with license information below. We acknowledge and are grateful to these developers for their
 *     contributions to open source.
 *
 *       Project: Bounce2  https://github.com/thomasfredericks/Bounce2
 *       Use: Debouncing library for Arduino and Wiring
 *       Copyright (c) 2013 thomasfredericks
 *       License (MIT) https://github.com/thomasfredericks/Bounce2/blob/master/LICENSE
 *
 *       Project: ESP32-BLE-Keboard  https://github.com/T-vK/ESP32-BLE-Keyboard
 *       Use: BLE Keboard library for ESP32 devices; used to send pagnation commands to iPad
 *       Copyright (c) 2019 T-vK
 *       License (MIT / GPL3) licence discussion: https://github.com/T-vK/ESP32-BLE-Keyboard/issues/60
 *
 *      https://github.com/Torxgewinde/Firebeetle-2-ESP32-E
 *      Project: Firebeetle-2-ESP32-E motion sensor https://github.com/Torxgewinde/Firebeetle-2-ESP32-E
 *      Incorporated / adapted code snippets on measuring and managing LiPo battery voltage with ESP-32
 *      Copyright (C) 2021 Tom Stöveken
 *      License (GPL2): https://github.com/Torxgewinde/Firebeetle-2-ESP32-E/blob/main/LICENSE
 *
 *    Revisions:
 *      2022.10.31   V1 under development
 *
 * ************************************************************************************
 */

/* ******************************************************
 *   Pin-out Summaries
 *     Refer to xxxxxx for pin-out table plus also see github flipTurn wiki
 *       for device schematic and breadboard hookup pictures
 * *******************************************************/

// external libraries:
#include <Arduino.h>  //* IDE requires Arduino framework to be explicitly included
#include <Bounce2.h>
#include <BleKeyboard.h>

#include "esp_adc_cal.h"

// Li-Po battery management, per Firebeetle-2-ESP32-E motion sensor project (GPL2)
#define LOW_BATTERY_VOLTAGE 3.20
#define VERY_LOW_BATTERY_VOLTAGE 3.10
#define CRITICALLY_LOW_BATTERY_VOLTAGE 3.00  // battery damage at this level!

int currentBattLevel = 0;

// blekeyboard instantiation params: (BT device name, BT Device manufacturer, Battery Level)
BleKeyboard bleKeyboard("flipTurn", "CW Greenstreet", currentBattLevel);

/******************************************************************************
Adapted from Firebeetle-2-ESP32-E motion sensor project (changes made)
----------------
Description.: reads the battery voltage through the voltage divider at AO pin (FireBeetle-ESP32 Ver4)
                must bridge zero ohm pads on board to enable voltage divider

              if the ESP32-E has calibration eFused those will be used.
              In comparison with a regular voltmeter the values of ESP32 and
              multimeter differ only about 0.05V
Input Value.: -
Return Value: battery voltage in volts
******************************************************************************/
float readBattery() {
    uint32_t value = 0;
    int rounds = 11;
    esp_adc_cal_characteristics_t adc_chars;

    // battery voltage divided by 2 can be measured at GPIO34, which equals ADC1_CHANNEL6
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    switch (esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars)) {
        case ESP_ADC_CAL_VAL_EFUSE_TP:
            Serial.println("Characterized using Two Point Value");
            break;
        case ESP_ADC_CAL_VAL_EFUSE_VREF:
            Serial.printf("Characterized using eFuse Vref (%d mV)\r\n", adc_chars.vref);
            break;
        default:
            Serial.printf("Characterized using Default Vref (%d mV)\r\n", 1100);
    }

    // to avoid noise, sample the pin several times and average the result
    for (int i = 1; i <= rounds; i++) {
        value += adc1_get_raw(ADC1_CHANNEL_6);
    }
    value /= (uint32_t)rounds;

    // due to the voltage divider (1M+1M) values must be multiplied by 2
    // and convert mV to V
    return esp_adc_cal_raw_to_voltage(value, &adc_chars) * 2.0 / 1000.0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleKeyboard.begin();
}

void loop() {
    // loop code here

    bleKeyboard.setBatteryLevel(currentBattLevel);  // update battery level
}
