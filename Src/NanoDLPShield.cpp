#include "HostPty.h"
#include "SpeedyStepper.h"
#include "MotorController.h"
#include "Config.h"

#include <wiringPi.h>
#include <iostream>

#if 0
SpeedyStepper stepper;

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

void processMotorOnCmd() //M17
{
    digitalWrite(ENABLE_PIN, LOW);
}

void processMotorOffCmd() //M18
{
    digitalWrite(ENABLE_PIN, HIGH);
}
#endif //0

int main(int argc, char** argv)
{
    if (wiringPiSetupGpio () == -1)
        return 1;

#if 0
    stepper.connectToPins(STEP_PIN, DIR_PIN);
    stepper.setStepsPerMillimeter(STEPS_PER_MM);
    setSteperHighSpeed();

    pinMode(UP_BTN_PIN, INPUT);
    pullUpDnControl(UP_BTN_PIN, UP_BTN_PUD);
    pinMode(DOWN_BTN_PIN, INPUT);
    pullUpDnControl(DOWN_BTN_PIN, DOWN_BTN_PUD);

    HostPty pty("/dev/ttyNanoDLP");
    while(1)
    {
        if(isButtonPressed(UP_BTN_PIN))
        {
            processMotorOnCmd();
            processBtnMovement(UP_BTN_PIN, 1);
        }

        if(isButtonPressed(DOWN_BTN_PIN))
        {
            processMotorOnCmd();
            processBtnMovement(DOWN_BTN_PIN, -1);
        }

//        std::string str = pty.nextString();
//        std::cout << "Received line: " << str << std::endl;
    }
#endif//0

    MotorController mc;
    while(true)
    {
        //mc.runTo(10., 10.);
        delay(10000);
    }


    return 0;
}
