cmake_minimum_required(VERSION 2.8.9)
project (NanoDlpShield)

find_library(wiringPi_LIB wiringPi)

set(SOURCES
    NanoDLPShield.cpp 
    HostPty.cpp
    SpeedyStepper.cpp
    )
add_executable(NanoDlpShield ${SOURCES})
target_link_libraries(NanoDlpShield ${wiringPi_LIB} util)
