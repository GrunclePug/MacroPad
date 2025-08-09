// macropad_logic.h

#ifndef MACROPAD_LOGIC_H
#define MACROPAD_LOGIC_H

// --- INCLUDE ---
#include <Arduino.h>
#include "display_manager.h"

// --- PIN DEFINITION ---
#define BUTTON_1_PIN 2 // Killswitch
#define BUTTON_2_PIN 3 // Script 1
#define BUTTON_3_PIN 4 // Script 2
#define BUTTON_4_PIN 5 // Script 3

// --- GLOBAL ---
extern bool killswitch;
extern bool script1Active;
extern bool script2Active;
extern bool script3Active;

// --- FUNCTION PROTOTYPE ---
void macropad_setup();
void update_button_states();
void update_display();
void autoclicker();
void jitter();
void mouse_hold();

#endif
