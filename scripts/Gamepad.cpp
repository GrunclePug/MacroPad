// scripts/Gamepad.cpp

// --- INCLUDE ---
#include "Scripts.h"
#include "../MacroPadLogic.h"
#include "USBHIDKeyboard.h"

// --- DATA ---
struct GamepadData {
  bool isKey1Pressed = false;
  bool isKey2Pressed = false;
  bool isKey3Pressed = false;
  bool isKey4Pressed = false;
  unsigned long lastButton1Debounce = 0;
  unsigned long lastButton2Debounce = 0;
  unsigned long lastButton3Debounce = 0;
  unsigned long lastButton4Debounce = 0;
};

// --- GLOBAL ---
extern bool killswitch;
extern USBHIDKeyboard Keyboard;
extern int buttonStates[];
extern const int FAST_DEBOUNCE_TIME;

// --- CONSTANT ---
const char BUTTON_1_KEY = 'D';
const char BUTTON_2_KEY = 'F';
const char BUTTON_3_KEY = 'J';
const char BUTTON_4_KEY = 'K';

// --- HELPER ---
static void handleInactiveState(GamepadData* data, Script* thisScript, unsigned long currentTime);
static void handleActiveState(GamepadData* data, Script* thisScript, unsigned long currentTime);
static void updateScriptDisplay(GamepadData* data, Script* thisScript);

// --- FUNCTION DEFINITION ---
void setupGamepad(Script* thisScript) {
  thisScript->data = new GamepadData;
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, " %s %s %s %s", "[ ]", "[ ]", "[ ]", "[ ]");
}

void gamepad(Script* thisScript) {
  // Cast the void* data pointer back to our specific data struct.
  GamepadData* data = static_cast<GamepadData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.

  unsigned long currentTime = millis();

  if (killswitch || !thisScript->enabled) {
    handleInactiveState(data, thisScript, currentTime);
  } else {
    handleActiveState(data, thisScript, currentTime);
  }
  
  updateScriptDisplay(data, thisScript);
}

static void handleInactiveState(GamepadData* data, Script* thisScript, unsigned long currentTime) {
  // If the killswitch is engaged or the script is disabled, ensure all keys are released and the state is reset.
  if (data->isKey1Pressed) {
    Keyboard.release(BUTTON_1_KEY);
    data->isKey1Pressed = false;
  }
  if (data->isKey2Pressed) {
    Keyboard.release(BUTTON_2_KEY);
    data->isKey2Pressed = false;
  }
  if (data->isKey3Pressed) {
    Keyboard.release(BUTTON_3_KEY);
    data->isKey3Pressed = false;
  }
  if (data->isKey4Pressed) {
    Keyboard.release(BUTTON_4_KEY);
    data->isKey4Pressed = false;
  }
}

static void handleActiveState(GamepadData* data, Script* thisScript, unsigned long currentTime) {
  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && !data->isKey1Pressed && (currentTime - data->lastButton1Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.press(BUTTON_1_KEY);
    data->isKey1Pressed = true;
    data->lastButton1Debounce = currentTime;
  } else if (buttonStates[0] == HIGH && data->isKey1Pressed && (currentTime - data->lastButton1Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.release(BUTTON_1_KEY);
    data->isKey1Pressed = false;
    data->lastButton1Debounce = currentTime;
  }
  
  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && !data->isKey2Pressed && (currentTime - data->lastButton2Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.press(BUTTON_2_KEY);
    data->isKey2Pressed = true;
    data->lastButton2Debounce = currentTime;
  } else if (buttonStates[1] == HIGH && data->isKey2Pressed && (currentTime - data->lastButton2Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.release(BUTTON_2_KEY);
    data->isKey2Pressed = false;
    data->lastButton2Debounce = currentTime;
  }
  
  // --- Handle Button 3 ---
  if (buttonStates[2] == LOW && !data->isKey3Pressed && (currentTime - data->lastButton3Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.press(BUTTON_3_KEY);
    data->isKey3Pressed = true;
    data->lastButton3Debounce = currentTime;
  } else if (buttonStates[2] == HIGH && data->isKey3Pressed && (currentTime - data->lastButton3Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.release(BUTTON_3_KEY);
    data->isKey3Pressed = false;
    data->lastButton3Debounce = currentTime;
  }
  
  // --- Handle Button 4 ---
  if (buttonStates[3] == LOW && !data->isKey4Pressed && (currentTime - data->lastButton4Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.press(BUTTON_4_KEY);
    data->isKey4Pressed = true;
    data->lastButton4Debounce = currentTime;
  } else if (buttonStates[3] == HIGH && data->isKey4Pressed && (currentTime - data->lastButton4Debounce > FAST_DEBOUNCE_TIME)) {
    Keyboard.release(BUTTON_4_KEY);
    data->isKey4Pressed = false;
    data->lastButton4Debounce = currentTime;
  }
}

static void updateScriptDisplay(GamepadData* data, Script* thisScript) {
  // Check if we're in an active or inactive state to show the correct message.
  if (killswitch || !thisScript->enabled) {
    snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, " %s %s %s %s", "[ ]", "[ ]", "[ ]", "[ ]");
  } else {
    snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, " %s %s %s %s",
    data->isKey1Pressed ? "[#]" : "[ ]",
    data->isKey2Pressed ? "[#]" : "[ ]",
    data->isKey3Pressed ? "[#]" : "[ ]",
    data->isKey4Pressed ? "[#]" : "[ ]"
  );
  }
}
