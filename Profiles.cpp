#include "Profiles.h"
#include "GamepadPins.h"
#include "LedModule.h"
#include "Log.h" // enable / disable Serial output

char dpadToChar(uint8_t dpad) {
  switch (dpad) {
    case DPAD_CENTERED:     return '0';
    case DPAD_UP:           return '^';
    case DPAD_UP_RIGHT:     return '/';
    case DPAD_RIGHT:        return '>';
    case DPAD_DOWN_RIGHT:   return 'l';
    case DPAD_DOWN:         return 'v';
    case DPAD_DOWN_LEFT:    return ',';
    case DPAD_LEFT:         return '<';
    case DPAD_UP_LEFT:      return '`';
    default:                return 'X';
  }
}

Profiles::Profiles(BleGamepad *bleGamepad) :
    bleGamepad(*bleGamepad),
    cyclingAveragePointer(0),
    currentProfile(0),
    switchButton(false) {
      this->analogAvg = (int8_t**) malloc(MOVING_AVG * sizeof(int8_t*));
  for (int i=0; i<MOVING_AVG; i++) {
    this->analogAvg[i] = (int8_t*) malloc(N_AXES * sizeof(int8_t));
  }
  this->bleGamepad.begin();
}

void Profiles::handleInputs(){
  bool lastSwitchButton = switchButton;
  switchButton = !digitalRead(SWITCH_PIN);
  // switch profile on rising edge
  if (switchButton && !lastSwitchButton) {
    currentProfile = (currentProfile + 1) % N_PROFILES;
  }


  // get values using the current profile's policy
  profiles[currentProfile](&inputs, analogAvg, cyclingAveragePointer);
  cyclingAveragePointer = (cyclingAveragePointer + 1) % MOVING_AVG;

  // set values to bleGamepad
  bleGamepad.release(~inputs.buttons);
  bleGamepad.press(inputs.buttons);
  int8_t *ax = inputs.axes;
  bleGamepad.setAxes(ax[0], ax[1], ax[2], ax[5], ax[3], ax[4], inputs.dpad);

  // output
  PRINT_BINARY(inputs.buttons);
  PRINTF(" | %c | {lX=%+4d}, {lY=%+4d}, {lZ=%+4d}, {rZ=%+4d}, {rX=%+4d}, {rY=%+4d}\n",
      dpadToChar(inputs.dpad), ax[0], ax[1], ax[2], ax[5], ax[3], ax[4]);

}

void Profiles::p0(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {
  led_set(0x1);

  index = analogAvg[0][0];
  // measure values directly
  inputs->buttons = buttons();
  inputs->dpad = dpad();
  getAxes(inputs->axes);
}

void Profiles::p1(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {
  led_set(0x1 << 1);
}

void Profiles::p2(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {
  led_set(0x1 << 2);
}

void Profiles::p3(inputs_t *inputs, int8_t **analogAvg, uint8_t index) {
  led_set(0x1 << 3);
}
