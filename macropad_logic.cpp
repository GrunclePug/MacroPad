// macropad_logic.cpp

// --- INCLUDE ---
#include "macropad_logic.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

// --- GLOBAL ---
int debounceTime = 200;
bool killswitch = false;
bool script1Active = false;
bool script2Active = false;
bool script3Active = false;

// Autoclicker
int autoclickMinDelay = 82;
int autoclickMaxDelay = 103;

// Jitter
int jitter_amount = 100; // Mouse travel distance
int jitter_step_size = 10; // New variable: how many pixels to move per step
int jitterMinDelay = 0; // Min delay between mouse moves
int jitterMaxDelay = 2; // Max delay between mouse moves
int jitterMinActionDelay = 4000; // Min delay between full jitter cycles
int jitterMaxActionDelay = 7000; // Max delay between full jitter cycles

// Set up the macro pad components (buttons, etc.)
void macropad_setup() {
  // Initialize the display using custom display manager
  display_setup();

  // Status Light
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up the button pins as inputs with pull-up resistors
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);
  
  // HID
  Keyboard.begin();
  Mouse.begin();
  USB.begin();

  // Initial display update
  update_display();
}

// Function to check the state of the buttons
void check_buttons() {
  // Debounce
  static unsigned long last_button_press = 0;
  unsigned long current_time = millis();
  
  // Read the state of the button
  int buttonState1 = digitalRead(BUTTON_1_PIN);
  int buttonState2 = digitalRead(BUTTON_2_PIN);
  int buttonState3 = digitalRead(BUTTON_3_PIN);
  int buttonState4 = digitalRead(BUTTON_4_PIN);

  // Killswitch
  if(buttonState1 == LOW && (current_time - last_button_press > debounceTime)) {
    last_button_press = current_time;
    killswitch = !killswitch;
    if (killswitch) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  // Script 1
  if(buttonState2 == LOW && (current_time - last_button_press > debounceTime)) {
    last_button_press = current_time;
    script1Active = !script1Active;
  }

  // Script 2
  if(buttonState3 == LOW && (current_time - last_button_press > debounceTime)) {
    last_button_press = current_time;
    script2Active = !script2Active;
  }

  // Script 3
  if(buttonState4 == LOW && (current_time - last_button_press > debounceTime)) {
    last_button_press = current_time;
    script3Active = !script3Active;
  }

  // Update the display to show the new states
  update_display();
}

// Update the display with the current macro states using custom display manager
void update_display() {
  display_update_status(killswitch, script1Active, script2Active, script3Active);
}

void autoclicker() {
  if (killswitch) return;

  // Use a static variable to keep track of the last time a click occurred
  static unsigned long lastClickTime = 0;
  
  if (script1Active) {
    // Check if the random delay time has passed
    if (millis() - lastClickTime > random(autoclickMinDelay, autoclickMaxDelay)) {
      Mouse.click();
      lastClickTime = millis();
    }
  }
}

void jitter() {
  if (killswitch) return;
  
  // Static variables to manage the state of the jitter
  static unsigned long lastMoveTime = 0;
  static unsigned long lastActionDelayTime = 0;
  static int currentMoves = 0;
  static bool movingPositive = true;
  static int halfCyclesCompleted = 0;

  // If the jitter script is not active, reset the state and return.
  if (!script2Active) {
    currentMoves = 0;
    movingPositive = true;
    halfCyclesCompleted = 0;
    return;
  }

  // Check if we need to pause between full jitter cycles.
  if (halfCyclesCompleted >= 2) {
    if (millis() - lastActionDelayTime < random(jitterMinActionDelay, jitterMaxActionDelay)) {
      return;
    }
    halfCyclesCompleted = 0;
  }

  // Check if the individual move delay has passed.
  if (millis() - lastMoveTime > random(jitterMinDelay, jitterMaxDelay)) {
    // Perform a single mouse move.
    if (movingPositive) {
      Mouse.move(jitter_step_size, 0);
    } else {
      Mouse.move(-jitter_step_size, 0);
    }
    
    // Increment the move counter.
    currentMoves += jitter_step_size;
    
    // Check if we have completed a half-cycle.
    if (currentMoves >= jitter_amount) {
      movingPositive = !movingPositive; // Reverse direction.
      currentMoves = 0; // Reset the counter for the new half-cycle.
      halfCyclesCompleted++;
      lastActionDelayTime = millis();
    }
    
    lastMoveTime = millis(); // Reset the timer for the next individual move.
  }
}

// Function to handle mouse hold/release
void mouse_hold() {
  // Use a static variable to track the current state of the mouse button
  static bool isHolding = false;
  
  // If killswitch is engaged, release the mouse button immediately.
  // The rest of the function will be skipped by the if statements.
  if (killswitch) {
    if (isHolding) {
      Mouse.release(MOUSE_LEFT);
      isHolding = false;
    }
  }
  
  if (!killswitch && script3Active && !isHolding) {
    Mouse.press(MOUSE_LEFT);
    isHolding = true;
  } else if (!killswitch && !script3Active && isHolding) {
    Mouse.release(MOUSE_LEFT);
    isHolding = false;
  }
}
