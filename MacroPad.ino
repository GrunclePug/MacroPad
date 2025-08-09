// MacroPad.ino

// --- INCLUDE ---
#include "macropad_logic.h"

void setup() {
  // Initialize the Serial monitor for debugging purposes
  Serial.begin(115200);
  Serial.println("MacroPad Starting...");

  // Call the setup function from our custom logic file
  macropad_setup();
  Serial.println("MacroPad Online.");
}

void loop() {
  update_button_states();

  // Macro
  autoclicker();
  jitter();
  mouse_hold();
}
