#include "MotorController.h"
#include "Config.h"

#include <wiringPi.h>
#include <pthread.h>
#include <stdexcept>

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
    printf("runMotorThread\n");
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
    printf("motorThread()\n");
    _enable_pin.write(false);
    _dir_pin.write(false);

    struct sched_param param;
    param.sched_priority = sched_get_priority_max (SCHED_RR);
    pthread_setschedparam(pthread_self(), SCHED_RR, &param);

    while(true)
    {
        _step_pin.write(true);
        delayMicroseconds(5);
        _step_pin.write(false);
        delayMicroseconds(120);
    }
}

void * MotorController::motorThreadWrapper(void * arg)
{
    MotorController * mc = static_cast<MotorController*>(arg);
    mc->motorThread();
    return NULL;
}


void MotorController::setAcceleration(float acceleration)
{
}

void MotorController::setAbsolutePositioning(bool absolute)
{
}

void MotorController::freeRun(float speed)
{
}

void MotorController::runTo(float dest, float speed)
{
}


