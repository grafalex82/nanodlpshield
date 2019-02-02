#include <stdio.h>
#include <wiringPi.h>

int main(int argc, char** argv)
{
    if (wiringPiSetupGpio () == -1)
        return 1;

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

    return 0;
}
