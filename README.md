# Alarm-System
A simple alarm system prototyped on a microcontroller includes a lock/unlock feature with password input, an alarm siren, and a temperature monitor using interrupt driven design. Built for a computer architecture course where I learned about: microprocessors and their general architecture, CISC and RISC architectures, Microcontrollers, Embedded systems, Designing computers using microprocessors, Introduction to computer hardware software codesign.

## Built With
The alarm system was built using:
* C
* Assembly
* Motorola 68HC12 Dragon 12 Microcontroller

## Features
The following are some features implemented, use cases of such features, and a number to match to the hardware diagram to give a better understanding of the project:

![Dragon 12 Microcontroller](https://github.com/mmorg031/Alarm-System/blob/master/68HC12%20Dragon%2012%20Microcontroller.png)

1. LCD Display
Displays messages to User about the status of temperature, the status of the alarm , and the configuration and verification of pin

2. 7 Segment Display
Displays pin entered on keypad (3) 

3. Alarm with Lock & Unlock Feature
A siren will play if the system is armed, the system is triggered, and the entered pin is incorrect. You may shut the siren off when the correct pin is entered.

4. Keypad for PIN
A keypad exists to create and configure a security code/pin, as well as verify that code/pin when the alarm is triggered to disarm the system.

5. Temperature Monitor
A thermometer exists on the board and the temperature is displayed on the LCD screen when the system is armed/disarmed
