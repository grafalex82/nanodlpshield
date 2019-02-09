#pragma once

#include "Pin.h"

#include <atomic>

enum MotorCommand
{
    MC_STOP,
    MC_SET_SPEED,
    MC_SET_ACCEL,
    MC_RUN,
    MC_RUN_TO
};

class MotorController
{
    Pin _step_pin;
    Pin _dir_pin;
    Pin _enable_pin;
    Pin _z_top_pin;
    Pin _z_bottom_pin;

    std::atomic<float> _z_position;

    void runMotorThread();
    void stopMotorThread();

    void motorThread();
    static void *motorThreadWrapper(void * arg);

public:
    MotorController();
    ~MotorController();

    void setAcceleration(float acceleration);
    void setAbsolutePositioning(bool absolute);
    void freeRun(float speed);
    void runTo(float dest, float speed);
};