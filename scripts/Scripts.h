// scripts/Scripts.h

#ifndef SCRIPTS_H
#define SCRIPTS_H

// This header is a central place to declare all script functions.

#include <Arduino.h>
struct Script;

// --- FUNCTION PROTOTYPE ---
extern void setupGamepad(Script* thisScript);
extern void setupAutoclicker(Script* thisScript);
extern void setupJitter(Script* thisScript);
extern void setupMouseHold(Script* thisScript);

extern void gamepad(Script* thisScript);
extern void autoclicker(Script* thisScript);
extern void jitter(Script* thisScript);
extern void mouseHold(Script* thisScript);

#endif
