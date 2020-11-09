/**
 * Project to reanimate a dead Xbox360 Controller
 * by using an ESP32 to read the inputs and connecting to Windows via BLE
 */

#include "BleGamepad.h"
#include "GamepadPins.h"
#include "Log.h"            // enable / disable Serial output
#include "esp_log.h"        // for time measurements

#define N_MEASUREMENTS  200
#define FREQUENCY 100
const int DELAY_MS = 1000 / FREQUENCY;

BleGamepad bleGamepad("Xbox-ESP", "Richter", 100);

int8_t analogs[N_AXES];

void led_greeting();
void sleep_until_multiple_of(uint16_t);

void setup() {
  INIT(); // Serial
  setupPins();
  led_greeting();
  bleGamepad.begin();
  PRINTF("\nBoot complete\n");
}

void loop() {
  int n = N_MEASUREMENTS;
  if (bleGamepad.isConnected()) {
    int t0 = esp_log_timestamp();
    for (int i=n; --i>=0; ) {
      // measure
      uint16_t b = buttons();
      uint8_t hat = dpad();
      getAxes(analogs);

      // set values to bleGamepad
      bleGamepad.release(~b);
      bleGamepad.press(b);
      bleGamepad.setAxes(analogs[0], analogs[1], analogs[2], analogs[5], analogs[3], analogs[4],
                         hat);

      // output
      PRINT_BINARY(b);
      PRINTF(" | %c | {lX=%+4d}, {lY=%+4d}, {lZ=%+4d}, {rZ=%+4d}, {rX=%+4d}, {rY=%+4d}\n",
        dpadToChar(hat), analogs[0], analogs[1], analogs[2], analogs[5], analogs[3], analogs[4]);

      // delay
      delay(DELAY_MS);
    }
    int t1 = esp_log_timestamp();
    PRINTF("Average period: %dms (%d measurements)\n", (t1 - t0) / n, n);
  }
}

int leds[4] = {LED_3, LED_4, LED_2, LED_1};
void led_greeting() {
  for (int led = 4; --led >= 0;)
    digitalWrite(leds[led], HIGH);
  for (int led = 4; --led >= 0;) {
    digitalWrite(leds[led], LOW);
    delay(75);
  }
  delay(300);
  for (int led = 4; --led >= 0;)
    digitalWrite(leds[led], HIGH);
}

void sleep_until_multiple_of(uint16_t n_ms) {
  delay(n_ms  - (esp_log_timestamp() % n_ms));
}
