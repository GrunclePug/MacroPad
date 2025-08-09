// display_manager.h

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

// --- INCLUDE ---
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED DEFINITION ---
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // I2C address of the display

// --- FUNCTION PROTOTYPE ---
// Functions to manage the display's state and content
void display_setup();
void display_update_status(bool killswitch, bool script1, bool script2, bool script3);

#endif
