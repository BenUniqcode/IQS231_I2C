# IQS231_I2C

## Arduino library for using Azoteq IQS231x capacitative proximity sensor over I2C bus

The IQS231 series proximity sensors are unique among Azoteq's single-channel capacitative proximity sensors in their ability to be controlled 
and read over a shared I2C bus. Other sensors can use I2C, but either do so in a non-standard way, so can't share the bus, or only listen for 
I2C requests during a narrow time window, so require repeated polling. 

Because the IQS231 (when correctly set up) listens for I2C requests even while asleep, this gives the ability for the host MCU to take a more 
active role in managing its configuration at runtime - for example, to reduce its sensitivity settings if it is producing lots of false detections. 

This library allows both reading events (proximity, touch, movement), and adjusting the sensitivity for the detection of those events, over I2C.

Author: Ben Wheeler <ben@uniqcode.com>

