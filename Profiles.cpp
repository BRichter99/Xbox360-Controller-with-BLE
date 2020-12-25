#include "Profiles.h"
#include "GamepadPins.h"
#include "LedModule.h"
#include "Log.h" // enable / disable Serial output

Profiles::Profiles() : cyclingAveragePointer(0), currentProfile(0), consecutiveSwitchPresses(1) {
  this->inputsMutex = xSemaphoreCreateMutex();
  this->inputsSemaphore = xSemaphoreCreateBinary();
  this->analogAvg = (int8_t **)malloc(MOVING_AVG * sizeof(int8_t *));
  for (int i = 0; i < MOVING_AVG; i++) {
    this->analogAvg[i] = (int8_t *)malloc(N_AXES * sizeof(int8_t));
  }
}

int Profiles::scanInputs(){
  bool switchButton = !digitalRead(SWITCH_PIN);
  if (switchButton) {
    if (!consecutiveSwitchPresses) {
      // switch profile on rising edge
      currentProfile = (currentProfile + 1) % N_PROFILES;
      led_set(0x1 << currentProfile);
    }
    consecutiveSwitchPresses++;
  } else {
    consecutiveSwitchPresses = 0;
  }

  // get values using the current profile's policy
  profiles[currentProfile](&inputs_tmp, analogAvg, cyclingAveragePointer);
  cyclingAveragePointer = (cyclingAveragePointer + 1) % MOVING_AVG;

  // transfer values to shared memory
  xSemaphoreTake(inputsMutex, portMAX_DELAY);
  inputs = inputs_tmp;
  xSemaphoreGive(inputsMutex);
  xSemaphoreGive(inputsSemaphore);

  return consecutiveSwitchPresses;
}

int Profiles::getCurrentProfile() { return currentProfile; }

void Profiles::p0(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {
  index = analogAvg[0][0];
  // measure values directly
  inputs->buttons = buttons();
  inputs->dpad = dpad();
  getAxes(inputs->axes);
}

void Profiles::p1(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {}

void Profiles::p2(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {}

void Profiles::p3(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {}
