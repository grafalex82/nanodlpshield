#pragma once

#include "Pin.h"
#include "Queue.h"

#include <atomic>

enum MotorCommand
{
    MC_STOP,
    MC_SET_SPEED,
    MC_SET_ACCEL,
    MC_RUN,
    MC_RUN_TO
};


struct MotorMsg
{
    MotorCommand command;
    long value;
};

class MotorController
{
    Pin _step_pin;
    Pin _dir_pin;
    Pin _enable_pin;
    Pin _z_top_pin;
    Pin _z_bottom_pin;

    std::atomic<float> _z_position;

    Queue<MotorMsg> _cmd_queue;

    void runMotorThread();
    void stopMotorThread();

    void motorThread();
    void moveWithAcceleration(float speed, float distance, float acceleration);
    static void *motorThreadWrapper(void * arg);

public:
    MotorController();
    ~MotorController();

    void setSpeed(float speed);
    void setAcceleration(float acceleration);
    void setAbsolutePositioning(bool absolute);
    void freeRun(float speed);
    void runTo(float dest, float speed);
};