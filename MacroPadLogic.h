// MacroPadLogic.h

#ifndef MACROPAD_LOGIC_H
#define MACROPAD_LOGIC_H

// --- INCLUDE ---
#include <Arduino.h>
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

// --- PIN DEFINITION ---
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3
#define BUTTON_3_PIN 4
#define BUTTON_4_PIN 5
#define KILLSWITCH_PIN 6
#define NAV_BUTTON_PIN 7

// Script Message
#define MAX_MSG_LINES 4
#define MAX_MSG_LINE_LENGTH 21

// --- STRUCT DEFINITION ---
struct Script;
typedef void (*ScriptFunction)(Script* script);
typedef void (*ScriptSetup)(Script* script);

struct Script {
  ScriptSetup setup;
  ScriptFunction run;
  const char* name;
  bool selected;
  bool enabled;
  void* data;
  char msg[MAX_MSG_LINES][MAX_MSG_LINE_LENGTH];
};

// Now that Script is defined, we can safely include other headers
#include "DisplayManager.h"
#include "scripts/Scripts.h"

// --- CONSTANT ---
const int FAST_DEBOUNCE_TIME = 50;
const int NORMAL_DEBOUNCE_TIME = 200;
const int HOLD_TIME = 1000;
const int TIMEOUT = 5000;

// --- GLOBAL ---
extern USBHIDKeyboard Keyboard;
extern USBHIDMouse Mouse;
extern bool killswitch;
extern bool menu;
extern Script scripts[];
extern const int NUM_SCRIPTS;
extern int currentScriptIndex;
extern int buttonStates[];

// --- FUNCTION PROTOTYPE ---
void macropadSetup();
void updateButtonStates();
void updateDisplay();

#endif
