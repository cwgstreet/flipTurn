/*
 * *************************************************************
 * flipState.h - implementation file for flipTurn state machine
 *
 *
 *  C W Greenstreet, Ver1, 10Dec23
 *    MIT Licence - Released into the public domain
 *
 * ************************************************************ */

// set up debug scaffold; comment out following line if you want to "turn off" debugging to serial monitor
#define DEBUG 1

#include "flipState.h"

#include <BleKeyboard.h>

#include "controlRGB.h"   // rgb led control functions
#include "esp_adc_cal.h"  // Espressif Analog to Digital Converter (ADC) Calibration Driver library
#include "myConstants.h"  // all constants in one file + pinout table

int current_battery_level = 100;  // initially set to fully charged, 100%

BleKeyboard bleKeyboard("flipTurn", "CW Greenstreet", current_battery_level);
// rgb led instantiation
RgbLed rgbLed(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);

const byte BLE_DELAY = 10;  // Delay (milliseconds) to prevent BT congestion

// entryStates is an enum variable type defined in menu.h header file (as extern); flipState is global
entryStates_t flipState;

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

    // battery voltage divided by 2 can be measured at GPIO36 / A0 pin (ADC1_CHANNEL0)
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
Description : state machine, primarily to process status LED states

Input Value : -
Return Value: -
*******************************************************************************/
void processState() {
    float battery_voltage = readBattery();  // in Volts

    // battery_voltage = 3.8;               // !debug test line

    //! code over-ride to auto shut-down if battery is critically low (<3V)
    if (battery_voltage < LOW_BATTERY_VOLTAGE) {
        flipState = auto_shut_down;
    }

    switch (flipState) {
        case check_BT_connection:
            if (bleKeyboard.isConnected()) {
                if (!hasRun) {  // prints message to serial monitor once only
                    Serial.println("Entered flipState : check_BT_connection");
                    Serial.println("flipTurn BLE Device connected!");
                    hasRun = 1;  // toggle flag to run connection notification only once
                }
                rgbLed.setRgbColour(rgbLed.blue_BT_connected);  // solid blue LED if connected
            } else {
                rgbLed.ledBlink(rgbLed.blue_BT_connected, 750);  // flash blue LED if no connection
            }
            break;

        case high_battery_charge:
            if ((millis() - ledTimer_msec) <= LED_DURATION_MSEC) {
                rgbLed.setRgbColour(rgbLed.green_high_battery_charge);
            }
            if ((millis() - ledTimer_msec) > LED_DURATION_MSEC) {
                Serial.println(F("Battery charged: battery voltage above 3.7V"));
                flipState = check_BT_connection;
#ifdef DEBUG
                Serial.println();
                Serial.println("--------------------------");
                Serial.print("flipState high battery charge; battery voltage = ");
                Serial.println(battery_voltage);
                Serial.println("--------------------------");
#endif
            }
            break;

        case warning_charge_battery_now:
            if ((millis() - ledTimer_msec) <= LED_DURATION_MSEC) {
                rgbLed.setRgbColour(rgbLed.magenta_charge_battery_warning);
            }
            if ((millis() - ledTimer_msec) > LED_DURATION_MSEC) {
                Serial.println(F("Battery adequate: battery voltage 3.7 to 3.2V"));
                flipState = check_BT_connection;
#ifdef DEBUG
                Serial.println();
                Serial.println("--------------------------");
                Serial.print("flipState warning battery charge; battery voltage = ");
                Serial.println(battery_voltage);
                Serial.println("--------------------------");
#endif
            }
            break;

        case low_battery:
            if ((millis() - ledTimer_msec) <= LED_DURATION_MSEC) {
                rgbLed.ledBlink(rgbLed.red_critically_low_battery, 500);
            }
            if ((millis() - ledTimer_msec) > LED_DURATION_MSEC) {
                Serial.println(F("Charge Battery NOW"));
                flipState = check_BT_connection;
#ifdef DEBUG
                Serial.println();
                Serial.println("--------------------------");
                Serial.print("flipState low battery charge; battery voltage = ");
                Serial.println(battery_voltage);
                Serial.println("--------------------------");
#endif
            }
            break;

        case battery_status:
            if (battery_voltage >= HIGH_BATTERY_VOLTAGE) {
                flipState = high_battery_charge;
                Serial.println(F("Battery status case: Battery charge high"));

            } else if ((battery_voltage >= CHARGE_NOW_VOLTAGE) && (battery_voltage < HIGH_BATTERY_VOLTAGE)) {
                flipState = warning_charge_battery_now;
                Serial.println(F("Battery status case: Battery adequate - battery voltage 3.7 to 3.2V"));

            } else if ((battery_voltage >= LOW_BATTERY_VOLTAGE) && (battery_voltage < CHARGE_NOW_VOLTAGE)) {
                flipState = low_battery;
                Serial.println(F("Battery status case: Battery charge low.  Charge battery now!"));
            }
            break;

        case auto_shut_down:
            ledTimer_msec = millis();
            do {
                rgbLed.ledBlink(rgbLed.red_critically_low_battery, 250);

            } while (((millis() - ledTimer_msec) <= 10000));  //? flash red warning for 10 sec before shutdown
            Serial.println(F("Battery critically low.  Commencing auto-shutdown!"));
            // trigger auto shut-down (deep sleep) ref: https://esp32.com/viewtopic.php?t=5624
            //! ESP32 will only wake-up on restart (cycle power switch or manual press reset button)
            esp_deep_sleep_start();
            break;

        default:
            flipState = check_BT_connection;
            Serial.println(F("processState() default switch-case triggered; check for code bug!"));
            break;
    }
}

//*  ************** not used *******************

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
