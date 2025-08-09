// display_manager.cpp

// --- INCLUDE ---
#include <Arduino.h>
#include "display_manager.h"

// Create an instance of the OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function to set up the OLED display
void display_setup() {
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

// Function to update the display with a simple status message
void display_update_status(bool killswitch, bool script1, bool script2, bool script3) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Display a title
  display.println("MacroPad Status:");
  display.println("");

  // Display the status of each script and the killswitch
  display.print("Killswitch: ");
  display.println(killswitch ? "Engaged" : "Ready");

  display.print("Autoclicker: ");
  display.println(script1 ? "ON" : "OFF");
  
  display.print("Jitter: ");
  display.println(script2 ? "ON" : "OFF");

  display.print("Mouse Hold: ");
  display.println(script3 ? "ON" : "OFF");

  display.display();
}
