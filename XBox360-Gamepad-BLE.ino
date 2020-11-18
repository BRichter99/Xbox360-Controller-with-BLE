/**
 * Project to reanimate a dead Xbox360 Controller
 * by using an ESP32 to read the inputs and connecting to Windows via BLE
 */

#include "nvs_flash.h"
#include "esp_log.h" // for time measurements
#include "BleGamepad.h"
#include "LedModule.h"
#include "Profiles.h"
#include "GamepadPins.h"
#include "Log.h"

#define N_MEASUREMENTS  200
#define FREQUENCY 30
const int DELAY_MS = 1000 / FREQUENCY;
const int DELAY_NOT_CONNECTED = 750;
uint8_t ledOn = 0;

BleGamepad bleGamepad("Xbox-ESP", "Richter", 100);
Profiles *p;

void led_greeting();
void sleep_until_multiple_of(uint16_t);

void setup() {
  nvs_flash_init();
  p = new Profiles(&bleGamepad);
  SERIAL_INIT();
  setupPins();
  PRINTF("Boot complete\n");
  led_greeting();
}

int n, t0, t1;

void loop() {
  STOPWATCH_START();
  for (n=1; n<=N_MEASUREMENTS; n++) {
    if (bleGamepad.isConnected()) {
      p->handleInputs();
      sleep_until_multiple_of(DELAY_MS);
    } else {
      // blink all LEDs
      led_set(ledOn);
      ledOn = ~ledOn;
      sleep_until_multiple_of(DELAY_NOT_CONNECTED);
    }
  }
  STOPWATCH_STOP();
}

void sleep_until_multiple_of(uint16_t n_ms) {
  delay(n_ms  - (esp_log_timestamp() % n_ms));
}
