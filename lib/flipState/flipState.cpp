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
// #define DEBUG 1

#include "flipState.h"
#include <BleKeyboard.h>
#include "esp_adc_cal.h"  // Espressif Analog to Digital Converter (ADC) Calibration Driver library


//   battery operating ranges
#define HIGH_BATTERY_VOLTAGE 3.70            // 4.2 - 3.7V battery "fully" charged
#define CHARGE_WARNING_TRIGGER_VOLTAGE 3.20  // trigger voltage to warn that device requires charging
#define LOW_BATTERY_VOLTAGE 3.00             // lower bound battery operating range (DW01 battery protection circuit triggers at 2.4V )


int current_battery_level = 100;  // initially set to fully charged, 100%


BleKeyboard bleKeyboard("flipTurn", "CW Greenstreet", current_battery_level);

const byte BLE_DELAY = 10;  // Delay (milliseconds) to prevent BT congestion


// entryStates is an enum variable type defined in menu.h header file (as extern); smokerState is global
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

void processState(int flipState) {
    switch (flipState) {
        case check_BT_connection:
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
