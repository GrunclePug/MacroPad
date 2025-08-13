// scripts/Jitter.cpp

// --- INCLUDE ---
#include "Scripts.h"
#include "../MacroPadLogic.h"
#include "USBHIDMouse.h"

// --- DATA ---
struct JitterData {
  unsigned long lastMoveTime = 0;
  unsigned long lastDelayTime = 0;
  int currentMoves = 0;
  bool movingPositive = true;
  int halfCyclesCompleted = 0;
  int minDelay = 4000; // Min delay between full jitter cycles
  int maxDelay = 7000; // Max delay between full jitter cycles
  unsigned long lastButton1Debounce = 0;
  unsigned long lastButton2Debounce = 0;
  unsigned long lastButton3Debounce = 0;
  unsigned long lastButton4Debounce = 0;
};

// --- GLOBAL ---
extern bool killswitch;
extern USBHIDMouse Mouse;

// --- CONSTANT ---
const int JITTER_DELAY_STEP_SIZE = 100;
const int JITTER_AMOUNT = 100; // Mouse travel distance
const int JITTER_STEP_SIZE = 10; // How many pixels to move per step
const int JITTER_MIN_ACTION_DELAY = 0; // Min delay between mouse moves
const int JITTER_MAX_ACTION_DELAY = 2; // Max delay between mouse moves

// --- HELPER ---
static void handleInactiveState(JitterData* data, Script* thisScript, unsigned long currentTime);
static void handleActiveState(JitterData* data, Script* thisScript, unsigned long currentTime);
static void updateScriptDisplay(JitterData* data, Script* thisScript);

// --- FUNCTION DEFINITION ---
void setupJitter(Script* thisScript) {
  thisScript->data = new JitterData;

  // Cast the void* data pointer back to our specific data struct.
  JitterData* data = static_cast<JitterData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "Min Delay: %dms", data->minDelay);
  snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "Max Delay: %dms", data->maxDelay);
}

void jitter(Script* thisScript) {
  if (killswitch || !thisScript->enabled) {
    if (thisScript->data) {
      JitterData* data = static_cast<JitterData*>(thisScript->data);
      // Reset the state to allow a clean restart later.
      data->currentMoves = 0;
      data->movingPositive = true;
      data->halfCyclesCompleted = 0;
    }
  }

  // Cast the void* data pointer back to our specific data struct.
  JitterData* data = static_cast<JitterData*>(thisScript->data);
  if (!data) return; // Exit if data is not initialized.

  unsigned long currentTime = millis();

  if (killswitch || !thisScript->enabled) {
    handleInactiveState(data, thisScript, currentTime);
  } else {
    handleActiveState(data, thisScript, currentTime);
  }
  
  updateScriptDisplay(data, thisScript);
}

static void handleInactiveState(JitterData* data, Script* thisScript, unsigned long currentTime) {
  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastButton1Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay - JITTER_DELAY_STEP_SIZE;
    data->lastButton1Debounce = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastButton2Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay + JITTER_DELAY_STEP_SIZE;
    data->lastButton2Debounce = currentTime;
  }

  // --- Handle Button 3 ---
  if (buttonStates[2] == LOW && (currentTime - data->lastButton3Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay - JITTER_DELAY_STEP_SIZE;
    data->lastButton3Debounce = currentTime;
  }

  // --- Handle Button 4 ---
  if (buttonStates[3] == LOW && (currentTime - data->lastButton4Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay + JITTER_DELAY_STEP_SIZE;
    data->lastButton4Debounce = currentTime;
  }

  // Safety checks to prevent invalid delay ranges.
  if (data->minDelay < JITTER_DELAY_STEP_SIZE) data->minDelay = JITTER_DELAY_STEP_SIZE;
  if (data->maxDelay < data->minDelay) data->maxDelay = data->minDelay;
  if (data->minDelay > data->maxDelay) data->minDelay = data->maxDelay;
}

static void handleActiveState(JitterData* data, Script* thisScript, unsigned long currentTime) {
  // Check if we need to pause between full jitter cycles.
  if (data->halfCyclesCompleted >= 2) {
    if (currentTime - data->lastDelayTime < random(data->minDelay, data->maxDelay)) {
      return;
    }
    data->halfCyclesCompleted = 0;
  }

  // Check if the individual move delay has passed.
  if (currentTime - data->lastMoveTime > random(JITTER_MIN_ACTION_DELAY, JITTER_MAX_ACTION_DELAY)) {
    // Perform a single mouse move.
    if (data->movingPositive) {
      Mouse.move(JITTER_STEP_SIZE, 0);
    } else {
      Mouse.move(-JITTER_STEP_SIZE, 0);
    }
    
    // Increment the move counter.
    data->currentMoves += JITTER_STEP_SIZE;
    
    // Check if we have completed a half-cycle.
    if (data->currentMoves >= JITTER_AMOUNT) {
      data->movingPositive = !data->movingPositive; // Reverse direction.
      data->currentMoves = 0; // Reset the counter for the new half-cycle.
      data->halfCyclesCompleted++;
      data->lastDelayTime = currentTime;
    }
    
    data->lastMoveTime = currentTime; // Reset the timer for the next individual move.
  }

  if (!thisScript->selected) return;

  // --- Handle Button 1 ---
  if (buttonStates[0] == LOW && (currentTime - data->lastButton1Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay - JITTER_DELAY_STEP_SIZE;
    data->lastButton1Debounce = currentTime;
  }

  // --- Handle Button 2 ---
  if (buttonStates[1] == LOW && (currentTime - data->lastButton2Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->minDelay = data->minDelay + JITTER_DELAY_STEP_SIZE;
    data->lastButton2Debounce = currentTime;
  }

  // --- Handle Button 3 ---
  if (buttonStates[2] == LOW && (currentTime - data->lastButton3Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay - JITTER_DELAY_STEP_SIZE;
    data->lastButton3Debounce = currentTime;
  }

  // --- Handle Button 4 ---
  if (buttonStates[3] == LOW && (currentTime - data->lastButton4Debounce > NORMAL_DEBOUNCE_TIME)) {
    data->maxDelay = data->maxDelay + JITTER_DELAY_STEP_SIZE;
    data->lastButton4Debounce = currentTime;
  }

  // Safety checks to prevent invalid delay ranges.
  if (data->minDelay < JITTER_DELAY_STEP_SIZE) data->minDelay = JITTER_DELAY_STEP_SIZE;
  if (data->maxDelay < data->minDelay) data->maxDelay = data->minDelay;
  if (data->minDelay > data->maxDelay) data->minDelay = data->maxDelay;
}

static void updateScriptDisplay(JitterData* data, Script* thisScript) {
  snprintf(thisScript->msg[0], MAX_MSG_LINE_LENGTH, "Min Delay: %dms", data->minDelay);
  snprintf(thisScript->msg[1], MAX_MSG_LINE_LENGTH, "Max Delay: %dms", data->maxDelay);
}