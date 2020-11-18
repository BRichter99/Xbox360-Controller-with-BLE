#ifndef PROFILES_H
#define PROFILES_H

#include <stdint.h>
#include "GamepadPins.h"
#include "BleGamepad.h"

#define N_PROFILES  4
#define SWITCH_PIN  BUTTON_XBOX
#define MOVING_AVG  5

struct {
  uint16_t buttons;
  uint8_t dpad;
  int8_t axes[N_AXES];
} typedef inputs_t;

class Profiles {
private:
  BleGamepad bleGamepad;
  int currentProfile;
  bool switchButton;
  inputs_t inputs;
  int8_t **analogAvg;           // 2-dimensional array for average calculation
  uint8_t cyclingAveragePointer;// points to the current analog-array
  static void p0(inputs_t*, int8_t**, uint8_t);
  static void p1(inputs_t*, int8_t**, uint8_t);
  static void p2(inputs_t*, int8_t**, uint8_t);
  static void p3(inputs_t*, int8_t**, uint8_t);
  void (*profiles[N_PROFILES])(inputs_t*, int8_t**, uint8_t)
    = { &p0, &p1, &p2, &p3, };
public:
  Profiles(BleGamepad*);
  void handleInputs();
};

#endif
