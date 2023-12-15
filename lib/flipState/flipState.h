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

// required in header file as BLEKeyboard object referred to as extern; otherwise compiler error
#include <BleKeyboard.h>  



// declare case names for flipTurn State Machine switch-case stucture
enum entryStates_t { check_BT_connection = 1,  // set enum 1 to 10 rather than default 0 for first element
                     high_battery_charge,
                     warning_charge_battery_now,
                     low_battery,
                     battery_status,
                     auto_shut_down };  // 6

// make flipState global (visible everywhere)
extern entryStates_t flipState;  

extern BleKeyboard bleKeyboard; 

//timer - declared as global in flipTurn-main.cpp
extern unsigned long ledTimer_msec;

// run-once flag
extern bool hasRun;

/******************************************************
// Function prototypes:
******************************************************/
void processState();
float readBattery();
bool isBatteryLow(uint32_t battery_voltage);
// int setBatteryLevel(float battery_voltage);  //* not used



#endif  // end header guard