cmake_minimum_required(VERSION 2.8.9)
project (NanoDlpShield)

find_library(wiringPi_LIB wiringPi)
find_library(pthread_LIB pthread)

set(SOURCES
    Src/NanoDLPShield.cpp 
    Src/HostPty.cpp
    Src/SpeedyStepper.cpp
    Src/MotorController.cpp
    )
add_executable(NanoDlpShield ${SOURCES})
target_link_libraries(NanoDlpShield ${wiringPi_LIB} ${pthread_LIB} util)
