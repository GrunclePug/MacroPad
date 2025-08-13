// scripts/Autoclicker.cpp

// --- INCLUDE ---
#include "Scripts.h"
#include "../MacroPadLogic.h"
#include "USBHIDMouse.h"

// --- DATA ---
struct AutoclickerData {
  unsigned long lastClickTime = 0;
  int minDelay = 82;
  int maxDelay = 103;
  unsigned long lastButton1Debounce = 0;
  unsigned long lastButton2Debounce = 0;
  unsigned long lastButton3Debounce = 0;
  unsigned long lastButton4Debounce = 0;
};

// --- GLOBAL ---
extern bool killswitch;
extern USBHIDMouse Mouse;
extern int buttonStates[];
extern const int NORMAL_DEBOUNCE_TIME;

// --- CONSTANT ---
const int AUTOCLICKER_DELAY_STEP_SIZE = 1;

// --- HELPER ---
static void handleInactiveState(AutoclickerData* data, Script* thisScript, unsigned long currentTime);
static void handleActiveState(AutoclickerData* data, Script* thisScript, unsigned long currentTime);
static void updateScriptDisplay(AutoclickerData* data, Script* thisScript);

// --- FUNCTION DEFINITION ---
void setupAutoclicker(Script* thisScript) {
  thisScript->data = new AutoclickerData;

  // Cast the void* data pointer back to our specific data struct.
  AutoclickerData* data = static_cast<AutoclickerData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "Min Delay: %dms", data->minDelay);
  snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "Max Delay: %dms", data->maxDelay);
}

void autoclicker(Script* thisScript) {
  // Cast the void* data pointer back to our specific data struct.
  AutoclickerData* data = static_cast<AutoclickerData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.
  
  unsigned long currentTime = millis();

  if (killswitch || !thisScript->enabled) {
    handleInactiveState(data, thisScript, currentTime);
  } else {
    handleActiveState(data, thisScript, currentTime);
  }
  
  updateScriptDisplay(data, thisScript);
}

static void handleInactiveState(AutoclickerData* data, Script* thisScript, unsigned long currentTime) {
  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastButton1Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay - AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton1Debounce = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastButton2Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay + AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton2Debounce = currentTime;
  }

  // --- Handle Button 3 ---
  if (buttonStates[2] == LOW && (currentTime - data->lastButton3Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay - AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton3Debounce = currentTime;
  }

  // --- Handle Button 4 ---
  if (buttonStates[3] == LOW && (currentTime - data->lastButton4Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay + AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton4Debounce = currentTime;
  }

  // Safety checks to prevent invalid delay ranges.
  if (data->minDelay < AUTOCLICKER_DELAY_STEP_SIZE) data->minDelay = AUTOCLICKER_DELAY_STEP_SIZE;
  if (data->maxDelay < data->minDelay) data->maxDelay = data->minDelay;
  if (data->minDelay > data->maxDelay) data->minDelay = data->maxDelay;
}

static void handleActiveState(AutoclickerData* data, Script* thisScript, unsigned long currentTime) {
  // Check if we need to click.
  if (currentTime - data->lastClickTime > random(data->minDelay, data->maxDelay)) {
    Mouse.click();
    data->lastClickTime = currentTime;
  }

  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastButton1Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay - AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton1Debounce = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastButton2Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay + AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton2Debounce = currentTime;
  }

  // --- Handle Button 3 ---
  if (buttonStates[2] == LOW && (currentTime - data->lastButton3Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay - AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton3Debounce = currentTime;
  }

  // --- Handle Button 4 ---
  if (buttonStates[3] == LOW && (currentTime - data->lastButton4Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay + AUTOCLICKER_DELAY_STEP_SIZE;
    data->lastButton4Debounce = currentTime;
  }

  // Safety checks to prevent invalid delay ranges.
  if (data->minDelay < AUTOCLICKER_DELAY_STEP_SIZE) data->minDelay = AUTOCLICKER_DELAY_STEP_SIZE;
  if (data->maxDelay < data->minDelay) data->maxDelay = data->minDelay;
  if (data->minDelay > data->maxDelay) data->minDelay = data->maxDelay;
}

static void updateScriptDisplay(AutoclickerData* data, Script* thisScript) {
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "Min Delay: %dms", data->minDelay);
  snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "Max Delay: %dms", data->maxDelay);
}
