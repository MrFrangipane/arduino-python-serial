// Minimal example for Raspberry Pi Pico and NHD-0216AW-IB3 Display
#include <WireOled.h>


WireOled display;


void setup() {
    if (display.detect()) {
        display.init();
        display.write("Frangitron");
        display.write(1, 0, "Hello");
    }
}


void loop() {
      display.setActive(true);

      display.setCursorBlinking(false);
      delay(500);
      display.setCursorBlinking(true);
      delay(500);
      display.setContrast(0);
      delay(500);
      display.setContrast(255);
      delay(500);

      display.setActive(false);
      delay(500);
}
