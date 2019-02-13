#pragma once

//////// Z-axis parameters ///////////////
//////////////////////////////////////////

// Pin #s where motor is connected
const int DIR_PIN = 5;
const int STEP_PIN = 6;
const int ENABLE_PIN = 12;

// Steps in 1mm along Z axis
//const float STEPS_PER_MM = 200*16/8; //steps per revolution * microstepping / mm per revolution
const float STEPS_PER_MM = 200*16; //DEBUG: steps in one revolution

// Top/Bottom end stops
const int ZTOP_PIN = 21;
const int ZBOT_PIN = 20;

// Top/Bottom end stops pull up/down modes (see PUD_UP/PUD_DOWN/PUD_OFF values)
const int ZTOP_PIN_PUD = 2; // PUD_UP
const int ZBOT_PIN_PUD = 2; // PUD_UP

// Enable this option if the printer has hardware up/down button.
#define SUPPORT_UP_DOWN_BUTTONS 1

#if SUPPORT_UP_DOWN_BUTTONS
// a single z-axis movement using HW buttons 
const float MANUAL_MOVEMENT_MM = 0.1; //mm

// After a few single moves Z-axis will move with low speed/acceleration
const float LOW_SPEED = 2; // mm/s
const float LOW_ACCELERATION = 5; // mm/s2

// If pressing up or down button for >5 seconds z-axis will move faster
const float HIGH_SPEED = 8; // mm/s
const float HIGH_ACCELERATION = 20; // mm/s2

// Pin #s for up/down buttons
const int UP_BTN_PIN = 26;
const int DOWN_BTN_PIN = 19;

// Pull up/down modes for up/down buttons (see PUD_UP/PUD_DOWN/PUD_OFF values)
const int UP_BTN_PUD = 1; // PUD_DOWN
const int DOWN_BTN_PUD = 1; // PUD_DOWN

#endif //SUPPORT_UP_DOWN_BUTTONS


#if 0

#ifdef JAP_PRINTER

const int LED_ON_BTN_PIN = A2;

const int UV_LED_PIN = A6;

#else // Wanhao D7

const int DIR_PIN = 48;
const int STEP_PIN = 76;
const int ENABLE_PIN = A8;

const int LED_ON_BTN_PIN = 18;

const int UV_LED_PIN = 9;
#endif//


const int CMD_TIMEOUT = 5000;
const int BUF_LEN = 100;


#endif //0