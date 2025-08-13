// DisplayManager.cpp

// --- INCLUDE ---
#include <Arduino.h>
#include "MacroPadLogic.h"
#include "DisplayManager.h"

// Create an instance of the OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function to set up the OLED display
void displaySetup() {
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void displayUpdateMenu(Script scripts[], const int numScripts) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Title
  display.println("MacroPad Menu:");
  display.println("");

  // Menu
  for (int i = 0; i < numScripts; i++) {
    // Highlight the currently selected script
    if (scripts[i].selected) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.print(" > ");
    display.print(scripts[i].name);
    display.println(scripts[i].enabled ? " (ON)" : " (OFF)");
  }
  display.display();
}

void displayUpdateStatus(bool killswitch, Script scripts[], const int numScripts) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Title
  display.println("MacroPad Status:");
  display.println("");

  // Killswitch
  display.print("Killswitch: ");
  display.println(killswitch ? "Engaged" : "Ready");

  bool hasSelected = false;
  for (int i = 0; i < numScripts; i++) {
    if (scripts[i].selected) {
      display.print(scripts[i].name);
      display.print(": ");
      display.println(scripts[i].enabled ? "ON" : "OFF");
      hasSelected = true;

      // Separator
      display.println("");

      // Loop through the msg array and print each line
      for (int j = 0; j < MAX_MSG_LINES; j++) {
        if (strlen(scripts[i].msg[j]) > 0) {
          display.println(scripts[i].msg[j]);
        }
      }
    }
  }

  // Scripts
  if (!hasSelected) {
    display.println("Enabled Scripts:");
    for (int i = 0; i < numScripts; i++) {
      if (scripts[i].enabled) {
        display.print(" > ");
        display.println(scripts[i].name);
      }
    }
  }
  
  display.display();
}
