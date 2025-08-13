// DisplayManager.h

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

// --- INCLUDE ---
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
struct Script;

// --- OLED DEFINITION ---
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // I2C address of the display

// --- FUNCTION PROTOTYPE ---
void displaySetup();
void displayUpdateMenu(Script scripts[], const int numScripts);
void displayUpdateStatus(bool killswitch, Script scripts[], const int numScripts);

#endif