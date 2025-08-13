# MacroPad

A custom, open-source macro pad project written for an Arduino-compatible microcontroller, featuring configurable autoclicker, jitter, and mouse hold scripts.

## Overview

MacroPad is a project designed for a custom hardware macro pad. It serves as a reference implementation for creating your own custom HID (Human Interface Device) macros.

## Features

* Modular Codebase: The project logic is separated into distinct files, making it easy to understand and modify.

### User Interface & Navigation
* Killswitch:

  * Tap: Toggles the killswitch, which pauses the execution of all scripts. You can still interact with the menu and change settings, but no macros will run.

  * Hold: Immediately disables all scripts, returning the device to a neutral state.

* Navigation Button: This button's function changes depending on whether you're in the main menu or actively using a script.

  * Tap (not in menu): Toggles the currently selected script on or off.

  * Tap (while in menu): Navigates to the next script in the list.

  * Hold (not in menu): Opens the main menu.

  * Hold (while in menu): Selects the currently highlighted script as the new active script. The display will update to show the newly selected script and its messages.

### Scripts
Note: All scripts are non-blocking and can be run at the same time.

* Gamepad:

  * Buttons 1-4 are assigned for general gamepad functions.

* Autoclicker: An autoclicker with adjustable delays.

  * Button 1: Decrements the minimum delay.

  * Button 2: Increments the minimum delay.

  * Button 3: Decrements the maximum delay.

  * Button 4: Increments the maximum delay.

* Jitter: A customizable script that simulates mouse jitter.

  * Button 1: Decrements the minimum delay.

  * Button 2: Increments the minimum delay.

  * Button 3: Decrements the maximum delay.

  * Button 4: Increments the maximum delay.

* Mouse Hold: A toggleable script to hold a mouse button down.

  * Button 1: Toggles Left Mouse Button (LMB) hold.

  * Button 2: Toggles Right Mouse Button (RMB) hold.

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

Hardware:
* Arduino Nano ESP32
* I2C 128x64 OLED display
* 6x Buttons (4 main, killswitch, nav)

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
