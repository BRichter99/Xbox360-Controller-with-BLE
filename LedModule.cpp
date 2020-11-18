#include "GamepadPins.h"
#include "esp32-hal-gpio.h"

int leds[4] = {LED_1, LED_2, LED_3, LED_4};

void led_greeting() {
  int led_circle[4] = {LED_3, LED_4, LED_2, LED_1};
  for (int led = 4; --led >= 0;)
    digitalWrite(led_circle[led], HIGH);
  for (int led = 4; --led >= 0;) {
    digitalWrite(led_circle[led], LOW);
    delay(75);
  }
  delay(300);
  for (int led = 4; --led >= 0;)
    digitalWrite(led_circle[led], HIGH);
}

void led_set(uint8_t bits) {
    for (int led = 4; --led >= 0;) {
        // HIGH for LED off
        uint8_t set = (bits & (0x1 << led)) == 0x0;
        digitalWrite(leds[led], set);
    }
}
