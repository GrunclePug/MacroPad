// MacroPad.ino

// --- INCLUDE ---
#include "MacroPadLogic.h"

// Scripts
#include "scripts/Gamepad.cpp"
#include "scripts/Autoclicker.cpp"
#include "scripts/Jitter.cpp"
#include "scripts/MouseHold.cpp"

void setup() {
  // Initialize the Serial monitor for debugging purposes
  Serial.begin(115200);
  Serial.println("MacroPad Starting...");

  // Call the setup function from our custom logic file
  macropadSetup();
  Serial.println("MacroPad Online.");
}

void loop() {
  updateButtonStates();
  for (int i = 0; i < NUM_SCRIPTS; i++) {
    if (scripts[i].enabled) {
      scripts[i].run(&scripts[i]);
    }
  }
}
