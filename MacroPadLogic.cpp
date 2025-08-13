// MacroPadLogic.cpp

// --- INCLUDE ---
#include "MacroPadLogic.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

// --- GLOBAL ---
bool killswitch = false;
bool menu = false;
int currentScriptIndex = 0;
int buttonStates[4];

// --- GLOBAL SCRIPT ARRAY ---
Script scripts[] = { // ScriptSetup, ScriptFunction, name, selected, enabled, data, msg
  {setupGamepad, gamepad, "Gamepad", false, false, nullptr},
  {setupAutoclicker, autoclicker, "Autoclicker", false, false, nullptr},
  {setupJitter, jitter, "Jitter", false, false, nullptr},
  {setupMouseHold, mouseHold, "Mouse Hold", false, false, nullptr}
};
const int NUM_SCRIPTS = sizeof(scripts) / sizeof(scripts[0]);

// Set up the macro pad components (buttons, etc.)
void macropadSetup() {
  // Initialize Scripts
  for (int i = 0; i < NUM_SCRIPTS; i++) {
    scripts[i].setup(&scripts[i]);
  }

  // Initialize the display using custom display manager
  displaySetup();

  // Status Light
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up the button pins as inputs with pull-up resistors
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);
  pinMode(KILLSWITCH_PIN, INPUT_PULLUP);
  pinMode(NAV_BUTTON_PIN, INPUT_PULLUP);
  
  // HID
  Keyboard.begin();
  Mouse.begin();
  USB.begin();

  // Set the first script as selected
  scripts[currentScriptIndex].selected = true;

  // Initial display update
  updateDisplay();
}

// Check button states and update script toggles
void updateButtonStates() {
  // --- STATIC VARIABLES FOR DEBOUNCE AND STATE TRACKING ---
  static unsigned long lastKillswitchDebounce = 0;
  static unsigned long lastNavDebounce = 0;
  static unsigned long killswitchPressTime = 0;
  static unsigned long navButtonPressTime = 0;
  static bool killswitchWasPressed = false;
  static bool navButtonWasPressed = false;
  unsigned long currentTime = millis();
  
  // Read the state of the button
  buttonStates[0] = digitalRead(BUTTON_1_PIN);
  buttonStates[1] = digitalRead(BUTTON_2_PIN);
  buttonStates[2] = digitalRead(BUTTON_3_PIN);
  buttonStates[3] = digitalRead(BUTTON_4_PIN);
  int killswitchState = digitalRead(KILLSWITCH_PIN);
  int navButtonState = digitalRead(NAV_BUTTON_PIN);

  // Killswitch
  if (killswitchState == LOW && !killswitchWasPressed) { // Killswitch pressed
    killswitchWasPressed = true;
    killswitchPressTime = currentTime;
  }

  if (killswitchState == HIGH && killswitchWasPressed) { // Killswitch released
    killswitchWasPressed = false;

    if (currentTime - killswitchPressTime > HOLD_TIME) { // Held
      // Loop through all scripts and set enabled = false
      for (int i = 0; i < NUM_SCRIPTS; i++) {
        scripts[i].enabled = false;
      }
    } else if (currentTime - lastKillswitchDebounce > NORMAL_DEBOUNCE_TIME) { // Tapped
      killswitch = !killswitch;
      digitalWrite(LED_BUILTIN, killswitch ? LOW : HIGH);
    }
    lastKillswitchDebounce = currentTime;
  }

  // Navigation
  if (navButtonState == LOW && !navButtonWasPressed) { // Nav button pressed
    navButtonWasPressed = true;
    navButtonPressTime = currentTime;
  }

  if(navButtonState == HIGH && navButtonWasPressed) { // Nav button released
    navButtonWasPressed = false;

    if (currentTime - navButtonPressTime > HOLD_TIME) { // Held
      if (menu) { // Confirm
        menu = false;
      } else { // Open Menu
        menu = true;
      }
      scripts[currentScriptIndex].selected = true;
    } else if (currentTime - lastNavDebounce > NORMAL_DEBOUNCE_TIME) { // Tapped
      if(menu) { // Cycle to next script
        scripts[currentScriptIndex].selected = false;
        currentScriptIndex = (currentScriptIndex + 1) % NUM_SCRIPTS;
        scripts[currentScriptIndex].selected = true;
      } else { // Toggle Script
        if (scripts[currentScriptIndex].selected) {
          scripts[currentScriptIndex].enabled = !scripts[currentScriptIndex].enabled;
        }
      }
    }
    lastNavDebounce = currentTime;
  }

  if (navButtonState == HIGH && menu && (currentTime - lastNavDebounce > TIMEOUT)) { // Menu timeout, 
    menu = false;
    scripts[currentScriptIndex].selected = false;
    currentScriptIndex = 0;
  }

  // Update the display to show the new states
  updateDisplay();
}

// Update the display with the current macro states using custom display manager
void updateDisplay() {
  if(menu) {
    displayUpdateMenu(scripts, NUM_SCRIPTS);
  } else {
    displayUpdateStatus(killswitch, scripts, NUM_SCRIPTS);
  }
}
