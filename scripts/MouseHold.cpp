// scripts/MouseHold.cpp

// --- INCLUDE ---
#include "Scripts.h"
#include "../MacroPadLogic.h"
#include "USBHIDMouse.h"

// --- DATA ---
struct MouseHoldData {
  bool isHoldingLMB = false;
  bool isHoldingRMB = false;
  bool wasHoldingLMB = false;
  bool wasHoldingRMB = false;
  unsigned long lastLMBPressTime = 0;
  unsigned long lastRMBPressTime = 0;
};

// --- GLOBAL ---
extern bool killswitch;
extern USBHIDMouse Mouse;
extern int buttonStates[];
extern const int NORMAL_DEBOUNCE_TIME;

// --- HELPER ---
static void handleInactiveState(MouseHoldData* data, Script* thisScript, unsigned long currentTime);
static void handleActiveState(MouseHoldData* data, Script* thisScript, unsigned long currentTime);
static void updateScriptDisplay(MouseHoldData* data, Script* thisScript);

// --- FUNCTION DEFINITION ---
void setupMouseHold(Script* thisScript) {
  thisScript->data = new MouseHoldData;
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "LMB: OFF");
  snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "RMB: OFF");
}

void mouseHold(Script* thisScript) {
  // Cast the void* data pointer back to our specific data struct.
  MouseHoldData* data = static_cast<MouseHoldData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.
  
  unsigned long currentTime = millis();
  
  if (killswitch || !thisScript->enabled) {
    handleInactiveState(data, thisScript, currentTime);
  } else {
    handleActiveState(data, thisScript, currentTime);
  }
  
  updateScriptDisplay(data, thisScript);
}

static void handleInactiveState(MouseHoldData* data, Script* thisScript, unsigned long currentTime) {
  // Ensure the physical mouse buttons are released.
  if (data->isHoldingLMB) {
    Mouse.release(MOUSE_LEFT);
    data->isHoldingLMB = false;
    data->wasHoldingLMB = true;
  }
  if (data->isHoldingRMB) {
    Mouse.release(MOUSE_RIGHT);
    data->isHoldingRMB = false;
    data->wasHoldingRMB = true;
  }

  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastLMBPressTime > NORMAL_DEBOUNCE_TIME)) {
    data->wasHoldingLMB = !data->wasHoldingLMB;
    data->lastLMBPressTime = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastRMBPressTime > NORMAL_DEBOUNCE_TIME)) {
    data->wasHoldingRMB = !data->wasHoldingRMB;
    data->lastRMBPressTime = currentTime;
  }
}

static void handleActiveState(MouseHoldData* data, Script* thisScript, unsigned long currentTime) {
  // Resume any previously configured holds.
  if (data->wasHoldingLMB) {
    Mouse.press(MOUSE_LEFT);
    data->isHoldingLMB = true;
    data->wasHoldingLMB = false; // Reset the flag
  }
  if (data->wasHoldingRMB) {
    Mouse.press(MOUSE_RIGHT);
    data->isHoldingRMB = true;
    data->wasHoldingRMB = false; // Reset the flag
  }

  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastLMBPressTime > NORMAL_DEBOUNCE_TIME)) {
    if (data->isHoldingLMB) {
      Mouse.release(MOUSE_LEFT);
    } else {
      Mouse.press(MOUSE_LEFT);
    }
    data->isHoldingLMB = !data->isHoldingLMB;
    data->lastLMBPressTime = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastRMBPressTime > NORMAL_DEBOUNCE_TIME)) {
    if (data->isHoldingRMB) {
      Mouse.release(MOUSE_RIGHT);
    } else {
      Mouse.press(MOUSE_RIGHT);
    }
    data->isHoldingRMB = !data->isHoldingRMB;
    data->lastRMBPressTime = currentTime;
  }
}

static void updateScriptDisplay(MouseHoldData* data, Script* thisScript) {
  // Check if we're in an active or inactive state to show the correct message.
  if (killswitch || !thisScript->enabled) {
    snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "LMB: %s", data->wasHoldingLMB ? "PREP" : "OFF");
    snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "RMB: %s", data->wasHoldingRMB ? "PREP" : "OFF");
  } else {
    snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "LMB: %s", data->isHoldingLMB ? "HELD" : "OFF");
    snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "RMB: %s", data->isHoldingRMB ? "HELD" : "OFF");
  }
}
