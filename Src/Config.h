#pragma once

//////// Z-axis parameters ///////////////
//////////////////////////////////////////

// Pin #s where motor is connected
const int DIR_PIN = 5;
const int STEP_PIN = 6;
const int ENABLE_PIN = 12;

// Steps in 1mm along Z axis
const float STEPS_PER_MM = 400*16/4; //steps per revolution * microstepping / mm per revolution
const float DEFAULT_SPEED = 6; // mm/s
const float DEFAULT_ACCELERATION = 20; // mm/s2


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
const int UP_BTN_PUD = 2; // PUD_UP
const int DOWN_BTN_PUD = 2; // PUD_UP

#endif //SUPPORT_UP_DOWN_BUTTONS


// Enable this option if the printer has hardware LED On/Off button
#define SUPPORT_LED_ON_BUTTON 1
#if SUPPORT_LED_ON_BUTTON
const int LED_ON_BTN_PIN = 13;
const int LED_ON_BTN_PUD = 2; // PUD_UP
#endif //SUPPORT_LED_ON_BUTTON

// Pin number where UV LED is connected to
const int UV_LED_PIN = 17;

// Signaling LED pin
const int LED_PIN = 16;

// FAN pin
const int FAN_PIN = 18;