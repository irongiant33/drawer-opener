# Drawer Opener


## Resources

- https://lastminuteengineers.com/drv8826-stepper-motor-driver-arduino-tutorial/
- https://hackaday.io/project/183279-accelstepper-the-missing-manual/details
- https://www.airspayce.com/mikem/arduino/AccelStepper/index.html

## TODO

- What happens when something hits the drawer in the middle of movement? 
    - potentially try and measure the "fault" pin on DRV8825
    - potentially use an ultrasonic sensor to compare expected state to actual state and close the loop
- What happens when the drawer is moved when the state is "closed" or "open"?
    - potentially use an ultrasonic sensor to provide some feedback on the open loop stepper motor
- user-defined settings for speed and acceleration
- how to send commands to the system to open, close, configure, and reset?
- how to tell if the motor does not have a reasonable amount of torque to move the drawer?
    - current motor has 16N-cm of torque
- how does the system change when using an ESP32 as the main MCU rather than an arduino?
