#include "GamepadPins.h"
#include "esp32-hal-gpio.h"

int leds[4] = {LED_1, LED_2, LED_3, LED_4};

void led_set(uint8_t bits) {
    for (int led = 4; --led >= 0;) {
        // HIGH for LED off
        uint8_t set = (bits & (0x1 << led)) == 0x0;
        digitalWrite(leds[led], set);
    }
}

void led_circle() {
  int led_circle[4] = {LED_3, LED_4, LED_2, LED_1};
  int lastLed = 3;
  for (int led = 4; --led >= 0;) {
    digitalWrite(led_circle[lastLed], HIGH);
    digitalWrite(led_circle[led], LOW);
    lastLed = led;
    delay(150);
  }
}

void led_greeting() {
  led_circle();
  delay(300);
  led_set(0x1);
}
