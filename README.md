# powerwheels-car-controller
Controller for my PowerWheels car mod

This was made in VSCode, using the PlatformIO extension.

I don't have a schematic to include, but the design is a 12-volt system, with two motors, one on each wheel. The controller is an ESP32, any should work fine. I used the DoIt DevKit V1 board. As for motor drivers, it uses two BTS7960 modules, one for each motor. I also added brake and reverse lighting to the car, and this controller does the lighting appropriately for left/right brake (dim/bright), center brake (off/on), and reverse (off/on).

The module can use either off/on or variable-speed gas pedals. It implements a system that limits the maximum rate the pedal value can change, so you could set it to take at least 2 seconds to go from "full off" to "full on", and the same for on to off. For variable pedals, the motors will be in "free" or "coast" mode as long as there is any pressure on the gas pedal, but as soon as it is full-off, the controller gradually applies the motor e-brake to slow the car down.
