#include "HostPty.h"
#include "SpeedyStepper.h"
#include "Config.h"

#include <wiringPi.h>
#include <iostream>
#include <cstring>

using namespace std;

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

void updateLastMovement()
{
    lastMovementMS = millis();
}

bool shouldDisableMotors()
{
    return millis() - lastMovementMS > 100000; // 100 seconds
}

void processMotorOnCmd() //M17
{
    updateLastMovement();
    digitalWrite(ENABLE_PIN, LOW);
}

void processMotorOffCmd() //M18
{
    digitalWrite(ENABLE_PIN, HIGH);
}

void processLEDOnCmd() // M3 or M106
{
    digitalWrite(UV_LED_PIN, HIGH);
}

void processLEDOffCmd() // M5 or M107
{
    digitalWrite(UV_LED_PIN, LOW);
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
    pinMode(ENABLE_PIN, OUTPUT);
    processMotorOffCmd();

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

    // Signaling (general purpose) LED
    pinMode(LED_PIN, OUTPUT);
    
    // FAN
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, 1);
}

int parseInt(const char * buf, char prefix, int value)
{
    const char * ptr = buf;

    while(ptr && *ptr)
    {
        if(*ptr == prefix)
            return atoi(ptr + 1);

        ptr = strchr(ptr, ' ');
        if(ptr == NULL)
            break;

        ptr++;
    }
    return value;
}

float parseFloat(const char * buf, char prefix, float value)
{
    const char * ptr = buf;

    while(ptr && *ptr)
    {
        if(*ptr == prefix)
            return atof(ptr + 1);

        ptr = strchr(ptr, ' ');
        if(ptr == NULL)
            break;

        ptr++;
    }
    return value;
}

void processMoveCmd(float position, float speed)
{
    if(speed != 0)
        stepper.setSpeedInMillimetersPerSecond(speed / 60);

    if(relativePositioning)
        stepper.moveRelativeInMillimeters(position);
    else
        stepper.moveToPositionInMillimeters(position);
}

void processPauseCmd(int duration)
{
    delay(duration);
}

bool parseGCommand(const char * cmd)
{
    int cmdID = parseInt(cmd, 'G', 0);
    switch(cmdID)
    {
        case 1: // G1 Move
        {
            float len = parseFloat(cmd, 'Z', 0);
            float speed = parseFloat(cmd, 'F', 0);
            processMotorOnCmd();
            processMoveCmd(len, speed);
            updateLastMovement();

            // NanoDLP waits for a confirmation that movement was completed
            //Serial.write("Z_move_comp\n");
            cout << "Z_move_comp" << endl;
            return true;
        }
        case 4: // G4 Pause
        {
            int duration = parseInt(cmd, 'P', 0);
            processPauseCmd(duration);
            return true;
        }

        case 90: // G90 - Set Absolute Positioning
            relativePositioning = false;
            return true;

        case 91: // G91 - Set Relative Positioning
            relativePositioning = true;
            return true;

    }

    return false;
}

bool parseMCommand(const char * cmd)
{
    int cmdID = parseInt(cmd, 'M', 0);
    switch(cmdID)
    {
    case 3: // M3/M106 - UV LED On
    case 106: 
        processLEDOnCmd();
        return true;

    case 5: // M5/M107 - UV LED Off
    case 107:
        processLEDOffCmd();
        return true;

    case 17: // M17 - Motor on
        processMotorOnCmd();
        return true;

    case 18: // M18 - Motor off
        processMotorOffCmd();
        return true;

    case 114: // M114 - Get current position
        float pos = stepper.getCurrentPositionInMillimeters();
        // Serial.print("Z:");
        // Serial.print(pos, 2);
        // Serial.write('\n');
        cout << "Z: " << pos << endl;
        return true;
    }

    return false;
}

bool parseCommand(const char * cmd)
{
    switch(*cmd)
    {
    case 'G':
        return parseGCommand(cmd);

    case 'M':
        return parseMCommand(cmd);

    default:
        break;
    }

    return false;
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

        string cmd;
        if(pty.receiveNextString(cmd))
        {
            cout << "Received line: " << cmd << endl;

            if(parseCommand(cmd.c_str()))
            {
                //Serial.write("ok\n");
                cout << "ok" << endl;
            }
            else
            {
                // Serial.print("Invalid or unsupported command: ");
                // Serial.print(cmdBuf);
                // Serial.write('\n');
                cout << "Invalid or unsupported command: " << cmd << endl;
            }
        }

        if(shouldDisableMotors() && !digitalRead(ENABLE_PIN))
            processMotorOffCmd();
    }

    return 0;
}
