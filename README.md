# MacroPad

A custom, open-source macro pad project written for an Arduino-compatible microcontroller, featuring configurable autoclicker, jitter, and mouse hold scripts.

## Overview

MacroPad is a project designed for a custom hardware macro pad. It serves as a reference implementation for creating your own custom HID (Human Interface Device) macros.

## Features

* Autoclicker: A non-blocking autoclicker with adjustable min/max delay.
* Jitter: A customizable jitter script with controls for travel distance, step size, and inter-cycle delay.
* Mouse Hold: A toggleable script to hold the left mouse button.
* Killswitch: A dedicated button to immediately disable all active scripts.
* Modular Codebase: The project logic is separated into distinct files (macropad_logic, display_manager), making it easy to understand and modify.

## Installation

### Using Git (Recommended)

1. Clone the repository: git clone https://github.com/GrunclePug/MacroPad.git
2. Open the MacroPad.ino file in the Arduino IDE.
3. Ensure the necessary libraries are installed (see Requirements below).
4. Upload the sketch to your microcontroller.

### Using a ZIP File

1. Download the ZIP file containing the source code.
2. Extract the files to a directory of your choice.
3. Open the MacroPad.ino file in the Arduino IDE.
4. Ensure the necessary libraries are installed (see Requirements below).
5. Upload the sketch to your microcontroller.

## Requirements

Hardware: Arduino Nano ESP32

Software:
* Arduino IDE (install from https://www.arduino.cc/en/software)
* The standard Arduino Keyboard.h and Mouse.h libraries.
* The Adafruit SSD1306 library by Adafruit along with it’s dependencies.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request.

## Author

Chad Humphries |
[Website](https://grunclepug.com/) |
[GitHub Profile](https://github.com/GrunclePug)

## Other Projects

Check out some of my other projects on GitHub: [Here](https://github.com/GrunclePug?tab=repositories)
