#include "HostPty.h"
#include "SpeedyStepper.h"
#include "Config.h"

#include <wiringPi.h>
#include <iostream>

SpeedyStepper stepper;

bool relativePositioning = true;
unsigned long lastMovementMS = 0;

#if SUPPORT_UP_DOWN_BUTTONS
void setSteperLowSpeed()
{
    stepper.setSpeedInMillimetersPerSecond(LOW_SPEED);
    stepper.setAccelerationInMillimetersPerSecondPerSecond(LOW_ACCELERATION);
}

void setSteperHighSpeed()
{
    stepper.setSpeedInMillimetersPerSecond(HIGH_SPEED); 
    stepper.setAccelerationInMillimetersPerSecondPerSecond(HIGH_ACCELERATION);
}

bool isButtonPressed(int btnPin)
{
    return digitalRead(btnPin) == HIGH;
}

void processBtnMovement(int btnPin, int direction = 1)
{
    // Try small movements first
    setSteperHighSpeed();
    for(int i=0; i<5; i++)
    {
        stepper.moveRelativeInMillimeters(MANUAL_MOVEMENT_MM * direction);
        delay(300);
        if(!isButtonPressed(btnPin))
            return;
    }

    // Then move at low speed for 3 sec
    setSteperLowSpeed();
    unsigned long startTime = millis();
    while(millis() < startTime + 3000)
    {
        if(stepper.motionComplete())
            stepper.setupRelativeMoveInMillimeters(1000 * direction);
        else
            stepper.processMovement();

        // Check if button released
        if(!isButtonPressed(btnPin))
        {
            stepper.setupStop();
            while(!stepper.processMovement())
                ;
            return;
        }
    }

    // Then move at high speed
    setSteperHighSpeed();
    stepper.setupRelativeMoveInMillimeters(1000 * direction);
    while(isButtonPressed(btnPin))
    {
        if(stepper.motionComplete())
            stepper.setupRelativeMoveInMillimeters(1000 * direction);
        else
            stepper.processMovement();
    }

    // Stop when button released
    stepper.setupStop();
    while(!stepper.processMovement())
        ;
}
#endif //SUPPORT_UP_DOWN_BUTTONS

void processMotorOnCmd() //M17
{
    digitalWrite(ENABLE_PIN, LOW);
}

void processMotorOffCmd() //M18
{
    digitalWrite(ENABLE_PIN, HIGH);
}

void updateLastMovement()
{
    lastMovementMS = millis();
}

bool shouldDisableMotors()
{
    return millis() - lastMovementMS > 100000; // 100 seconds
}

void processLEDButon()
{
    digitalWrite(UV_LED_PIN, !digitalRead(UV_LED_PIN));
    delay(50);
    while(isButtonPressed(LED_ON_BTN_PIN))
        ;
}


void setup()
{
    // General GPIO initialization
    if (wiringPiSetupGpio () == -1)
        throw std::runtime_error("Cannot initialize GPIO");

    // Init stepper motor
    stepper.connectToPins(STEP_PIN, DIR_PIN);
    stepper.setStepsPerMillimeter(STEPS_PER_MM);

#if SUPPORT_UP_DOWN_BUTTONS
    // Init up/down buttons
    pinMode(UP_BTN_PIN, INPUT);
    pullUpDnControl(UP_BTN_PIN, UP_BTN_PUD);
    pinMode(DOWN_BTN_PIN, INPUT);
    pullUpDnControl(DOWN_BTN_PIN, DOWN_BTN_PUD);
#endif //SUPPORT_UP_DOWN_BUTTONS

    // Init UV LED MOSFET Pin
    pinMode(UV_LED_PIN, OUTPUT);
    digitalWrite(UV_LED_PIN, LOW);

#if SUPPORT_LED_ON_BUTTON
    // Init Led On/Off button
    pinMode(LED_ON_BTN_PIN, INPUT);
    pullUpDnControl(LED_ON_BTN_PIN, LED_ON_BTN_PUD);
#endif //SUPPORT_LED_ON_BUTTON
}


int main(int argc, char** argv)
{
    setup();

    HostPty pty("/tmp/ttyNanoDLP");
    while(1)
    {
        //checkAlive();

#if SUPPORT_UP_DOWN_BUTTONS
        if(isButtonPressed(UP_BTN_PIN))
        {
            processMotorOnCmd();
            processBtnMovement(UP_BTN_PIN, 1);
            updateLastMovement();
        }

        if(isButtonPressed(DOWN_BTN_PIN))
        {
            processMotorOnCmd();
            processBtnMovement(DOWN_BTN_PIN, -1);
            updateLastMovement();
        }
#endif //SUPPORT_UP_DOWN_BUTTONS

        if(isButtonPressed(LED_ON_BTN_PIN))
            processLEDButon();

        std::string str;
        if(pty.receiveNextString(str))
            std::cout << "Received line: " << str << std::endl;

/*
        std::string str = pty.nextString();
        std::cout << "Received line: " << str << std::endl;
        processSerialInput(str);

        if(shouldDisableMotors() && !digitalRead(ENABLE_PIN))
            processMotorOffCmd();
*/
    }

    return 0;
}
