*RC Servo Programmer/Tester on Cheap Yellow Display*

A project to use a "Cheap Yellow Display" (based on ESP32) as Servo Programmer/Tester
Project Description

This repository contains the code and instructions to turn a simple ESP32-based "Cheap Yellow Display" into a practical servo programmer and tester for RC Servo:
The tester work on any servo while at the moment the programmer works only on PTK Servos (other are planned to be supported). 
It allows you to change servo parameters as neutral angle, min/max PPM Values, Gain, Deadband and other.
The tester allow to place the servo at any position as well to perform continuos span between a minimum and maximun angle

The goal is to provide a practical, on field usable tool to program and test servos.
Hardware Requirements

To build this project, you will need:

    A "Cheap Yellow Display" module (typically ESP32 with a 2.8" or 3.2" TFT display).
    A 470 Ohm resistor
    If tou decide to print the box for the 2.8 Display you will need also:
    1 - XT-30 Connector
    1 - Male RC Servo connector

*Software Requirements*

    Arduino IDE
    Esp32 Framework for arduino ide
    Necessary libraries for display management (install all of them from arduino library manager):
      TFT_eSPI - https://github.com/Bodmer/TFT_eSPI
      LVGL 9.4 - https://github.com/lvgl/lvgl
      XPT2046_Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen
    Library for servo driving under ESP32 (install from arduino library manager)
      ESP32Servo - https://madhephaestus.github.io/ESP32Servo/annotated.html
    
    EEZ Studio to customize the interface

*Installation and Setup*

    Clone this repository to your computer.
    Open the project Arduino.
    Install the required libraries (check the specific instructions in the code or accompanying documentation).
    Select ESP32 Dev Module as target board
    Configure TFT_eSPI copying the user_setup.h from Config\(Display name) directory in your TFT_eSPI library folder, normally Documents\arduino\libraries\TFT_eSPI\
    Configure lvgl copying the lv_conf.h in the library folder, normally Documents\arduino\libraries\
    Compile to check that environment is OK
    Solder a 470 Ohm resistor between GPIO35 and GPIO22 (See Photo FOLDER)
    Connect signal wire from the servo connector and GPIO35
    Connect + and - from the xt30 connector to the CYD
    Connect the CYD to the USB port of your PC
    Upload the code to your Cheap Yellow Display.

*Usage*

    Connect the xt30 connector to a 2S LiPo battery (or even to a 4S NiMh for standard servos)
    With the touch pen turn around to the display to autocalibrate touch (need to be redone at any firwmare update)
    Connect the servo to the servo connector Press "READ SERVO" to Read current conviguratio and Write servo to update
    Swipe to the right to go to the servotester page, swipe left to go back to servo configuration page
    
*Contributing*

Contributions are welcome! If you have ideas for improving the project, bug fixes, or new features, feel free to open an issue or submit a pull request.
