//#include <unistd.h>
//#include <sys/select.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <string.h>
//#include <termios.h>
//#include <stdlib.h>

#include "HostPty.h"
#include "SpeedyStepper.h"

#include <wiringPi.h>
#include <iostream>

int main(int argc, char** argv)
{
    if (wiringPiSetupGpio () == -1)
        return 1;

#if 0

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(12, 0);

    bool dir = 0;
    for (;;)
    {
        dir = !dir;
        printf("Direction %d\n", dir);
        digitalWrite(5, dir);

        for (int i=0; i<5*200*16; i++)
        {
            digitalWrite(6, 1);
            delayMicroseconds(10);
            digitalWrite(6, 0);
            delayMicroseconds(300);
        }

        delay(2000);
    }
#endif

    const float STEPS_PER_MM = 200*16/1; //steps per revolution * microstepping / mm per revolution
    const float HIGH_SPEED = 2;
    const float HIGH_ACCELERATION = 1;

    SpeedyStepper stepper;
    stepper.connectToPins(6, 5);
    stepper.setStepsPerMillimeter(STEPS_PER_MM);
    stepper.setSpeedInMillimetersPerSecond(HIGH_SPEED);
    stepper.setAccelerationInMillimetersPerSecondPerSecond(HIGH_ACCELERATION);


    HostPty pty("/dev/ttyNanoDLP");
    while(1)
    {
        std::string str = pty.nextString();
        std::cout << "Received line: " << str << std::endl;

        stepper.moveRelativeInMillimeters(5);
    }

    return 0;
}
