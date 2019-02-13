#include "MotorController.h"
#include "Config.h"

#include <wiringPi.h>
#include <pthread.h>
#include <mqueue.h>
#include <stdexcept>
#include <unistd.h>
#include <math.h>

#include <vector>

void mynanosleep(unsigned long delay)
{
    delayMicroseconds(delay - 60);
}

MotorController::MotorController()
    : _step_pin(STEP_PIN, OUTPUT)
    , _dir_pin(DIR_PIN, OUTPUT)
    , _enable_pin(ENABLE_PIN, OUTPUT)
    , _z_top_pin(ZTOP_PIN, INPUT, ZTOP_PIN_PUD)
    , _z_bottom_pin(ZBOT_PIN, INPUT, ZBOT_PIN_PUD)
{
    runMotorThread();
}

MotorController::~MotorController()
{
}

void MotorController::runMotorThread()
{

    // Now run the processing thread
    pthread_t myThread;
    int res = pthread_create(&myThread, NULL, motorThreadWrapper, static_cast<void *>(this));
    if(res != 0)
        throw std::runtime_error("Cannot create motor thread");
}

void MotorController::stopMotorThread()
{
}

void MotorController::motorThread()
{
    _enable_pin.write(false);
    _dir_pin.write(false);

    struct sched_param param;
    param.sched_priority = sched_get_priority_max (SCHED_RR);
    pthread_setschedparam(pthread_self(), SCHED_RR, &param);

    printf("Running motor loop\n");
#if 0
    long curPositionInSteps = 0;
    long targetPositionInSteps = 0;
    float curSpeedInSteps = 0;
    float targetSpeedInSteps = 0;
    float accelerationInSteps = 0;

    int d = 150;
    while(true)
    {
//        MotorMsg msg = _cmd_queue.pop();
//        printf("Received msg: %d)\n", msg.command);

//        switch(msg.command)
//        {
//            case MC_SET_SPEED:
//                targetSpeedInSteps = msg.value / 1E6; // Convert to steps/uS
//                break;
//            case MC_SET_ACCEL:
//                accelerationInSteps = (float)msg.value / 1E12; // Convert to steps/uS^2;
//                break;
//            case MC_RUN:
//                break;
//        }

        unsigned long accumPulseTime = 0;
        unsigned long accumDelayTime = 0;
        unsigned long accumExpDelayTime = 0;
        unsigned long accumCycleTime = 0;
        unsigned long iterations = STEPS_PER_MM*10;
        for(unsigned long i=0; i<iterations; i++)
        {
            unsigned long pulseStart = micros();
            _step_pin.write(true);
            delayMicroseconds(15);
            _step_pin.write(false);
            unsigned long now = micros();

            accumPulseTime += now - pulseStart;

            unsigned long delayStart = now;
            unsigned long nextDelay = d - (now - pulseStart);
            accumExpDelayTime += nextDelay;
            mynanosleep(nextDelay);
            now = micros();
            accumDelayTime += now - delayStart;
            accumCycleTime += now - pulseStart;
        }
        
        printf("Average pulse time: %d\n", accumPulseTime / iterations);
        printf("Average expected delay time: %d\n", accumExpDelayTime / iterations);
        printf("Average delay time: %d\n", accumDelayTime / iterations);
        printf("Average cycle time: %d\n", accumCycleTime / iterations);
        printf("Typical increase time: %d\n", (accumDelayTime - accumExpDelayTime) / iterations);
        
        d += 20;
    }
#endif //0

#if 1
    while(true)
    {
        moveWithAcceleration(0, 8, 0.25);
        moveWithAcceleration(2, 6, 0);
        moveWithAcceleration(2, 8, -0.25);
        delay(2000);
    }
#endif
}

void MotorController::moveWithAcceleration(float speed, float distance, float acceleration)
{
    unsigned long iterations = distance * STEPS_PER_MM;
    printf("Distance: %f\n", distance);
    printf("Iterations: %d\n", iterations);

    float v0 = speed * STEPS_PER_MM;
    printf("V0 = %f\n", v0);
    float a = acceleration * STEPS_PER_MM;
    printf("a = %f\n", a);

    std::vector<float> delays(iterations);
    float prevT = 0;
    for(unsigned int i=0; i<iterations; i++)
    {
        float nextX = i+1;
        float nextT = a != 0 ? ( -v0 + sqrt(v0 * v0 + 2 * a * nextX)) / a : nextX / v0;
        float d = (nextT - prevT) * 1000000;
        delays[i] = d;
        prevT = nextT;
    }

    for(unsigned int i=0; i<iterations; i++)
    {
        _step_pin.write(true);
        delayMicroseconds(15);
        _step_pin.write(false);
        mynanosleep(round(delays[i]-15));
    }
}

void * MotorController::motorThreadWrapper(void * arg)
{
    MotorController * mc = static_cast<MotorController*>(arg);
    mc->motorThread();
    return NULL;
}


void MotorController::setSpeed(float speed)
{
    // Recalculate acceleration from mm/s to steps/s
    long speedInSteps = speed * STEPS_PER_MM;

    MotorMsg msg = {MC_SET_SPEED, speedInSteps};
    _cmd_queue.push(msg);
}

void MotorController::setAcceleration(float acceleration)
{
    // Recalculate acceleration from mm/s^2 to steps/s^2
    long accelInSteps = acceleration * STEPS_PER_MM;

    MotorMsg msg = {MC_SET_ACCEL, accelInSteps};
    _cmd_queue.push(msg);
}

void MotorController::setAbsolutePositioning(bool absolute)
{
}

void MotorController::freeRun(float speed)
{
    MotorMsg msg = {MC_RUN, 0};
    _cmd_queue.push(msg);
}

void MotorController::runTo(float dest, float speed)
{
    printf("Sending msg\n");
    MotorMsg msg = {MC_RUN, 10};
    _cmd_queue.push(msg);
}


