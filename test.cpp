#include <iostream>
//#include <unistd.h>
//#include <sys/select.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <string.h>
//#include <termios.h>
//#include <stdlib.h>
//#include <wiringPi.h>

#include "HostPty.h"

int main(int argc, char** argv)
{
#if 0
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
#endif

    HostPty pty("/dev/ttyNanoDLP");
    while(1)
    {
        std::string str = pty.nextString();
        std::cout << "Received line: " << str << std::endl;
    }

    return 0;
}
