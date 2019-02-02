# NanoDLP Shield Firmware

This is a firmare for custom RaspberryPi shield to drive LCD 3d printer based on NanoDLP software.

[NanoDLP](https://www.nanodlp.com/) is a web-based control for LCD or DLP 3D printer. It allows user to upload
a model to print, slice it, and then drive 3D printer hardware (LCD, motors, shutters) in order to print the model.

Unfortunately NanoDLP software is closed source and therefore has limited extension capabilities. Out of the box
it can drive Z-axis motor, display slices on LCD or DLP projector, can drive 2x16 LCD screen. But that is it. If
you want more you have to write external scripts. Well, this is a good option, but it can't be used for low-latency
user input (e.g. adding more hadrware buttons).

A good option is to delegate dealing with the hardware to an Arduino. User can significantly extend hadrware capabilities.
The drawback of this approach is having multiple external components (Stepper motor drivers, UV LED drivers, I2C and UART
level converters) and wires which may lead to hardware instability. Moreover multiple component require a space to mount
them in the printer.

Aim of this project is to create a small all-in one extendable RaspberryPI shield that would include:
- Stepper Motor driver
- two Z-axis end stops (incl optional pull-up or pull-down resistors)
- Powerful MOSFET to drive UV LED
- GPIO for user needs (buttons, driving external hardware components)
- 1-wire DS18B20 thermometer support
- PWM output
- Fan output with PWM support
- I2C and UART interfaces with integrated level converters (3.3V or 5V selectable)
- low power LED output
- Buzzer
- Provide various voltage levels for external components (3.3V, 5V, 12V)

This shield would take responsibility of driving all external hardware, replacing Arduino with number of external components.
Communication between the shield and NanoDLP software is implemented via pseudo-COM port, so that NanoDLP software treats is
as external Arduino.

# Hardware

Schematics and PCB can be found [here](https://easyeda.com/editor#id=1c84f9033af4487bb82d24a9e845125c|2ef221c521474696ba044a7bebf7602c).

BOM:
- TBD (look at the easyeda project)

# Software

Software is based on WiringPi library which provides a Arduino-like interface to drive RasPi's GPIOs.

Installing prerequisites
```bash
sudo apt-get install cmake g++ wiringpi
```

Building
TBD