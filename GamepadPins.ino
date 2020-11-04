#include "GamepadPins.h"

#define ADC_MAX 4095
#define AXIS_MIN -127
#define AXIS_MAX 127
#define TRIGGER_MIN 0
#define TRIGGER_MAX AXIS_MAX

uint16_t mapping[N_BUTTONS][2] = {
    {BUTTON_1,  BUTTON_A},
    {BUTTON_2,  BUTTON_B},
    {BUTTON_3,  BUTTON_X},
    {BUTTON_4,  BUTTON_Y},
    {BUTTON_5,  BUTTON_LB},
    {BUTTON_6,  BUTTON_RB},
    {BUTTON_7,  BUTTON_BACK},
    {BUTTON_8,  BUTTON_START},
    {BUTTON_9,  BUTTON_XBOX},
    {BUTTON_10, BUTTON_LS},
    {BUTTON_11, BUTTON_RS},
};

uint8_t axisPins[N_AXES] = {POT_LS_X, POT_LS_Y, POT_LT, POT_RS_X, POT_RS_Y, POT_RT};
/* for analog calibration */
uint16_t minVal[N_AXES] =  {       0,        0,    450,        0,        0,    820};
uint16_t maxVal[N_AXES] =  {    4095,     4095,   3300,     4095,     3500,   3500};
uint16_t resting[N_AXES];

void setupPins() {
  memset(resting, 0, N_AXES);
  for (int i = N_AXES; --i >= 0; ) {
    pinMode(axisPins[i], INPUT);
    for (int n = 10; --n >= 0; ) {
      resting[i] += (analogRead(axisPins[i])) / 10;
    }
  }
  for (int i=N_BUTTONS; --i>=0; ) {
    pinMode(mapping[i][INPUT_PIN], INPUT_PULLUP);
  }
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_UP,   INPUT_PULLUP);
  pinMode(BUTTON_RIGHT,INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

uint16_t buttons() {
  uint16_t b=0x0;
  int inputPin,
  buttonOut;
  for (int i=N_BUTTONS; --i>=0; ) {
    b |= !digitalRead(mapping[i][INPUT_PIN]) * mapping[i][BUTTON_OUT];
  }
  return b;
}

void getAxes(int8_t *analogs) {
  for (int i = N_AXES; --i >= 0;) {
    int v = variance(analogRead(axisPins[i]), resting[i], minVal[i], maxVal[i]);

    uint8_t mode = i % 3;
    if (mode == 2) {
      // trigger
      analogs[i] = map(v, 0, minVal[i] - resting[i], TRIGGER_MIN, TRIGGER_MAX);
    } else {
      // stick                        Y-axis     X-axis
      int upperLimit = (mode == 1) ? AXIS_MIN : AXIS_MAX;
      int lowerLimit = (mode == 1) ? AXIS_MAX : AXIS_MIN;
      if (v < 0) {
        analogs[i] = map(v, minVal[i] - resting[i], 0, upperLimit, 0);
      } else {
        analogs[i] = map(v, 0, maxVal[i] - resting[i], 0, lowerLimit);
      }
    }
  }
}

int16_t variance(int16_t value, int16_t resting, int16_t min, int16_t max) {
  int16_t narrowed = value;
  if (value > max) {
    narrowed = max;
  } else if (value < min) {
    narrowed = min;
  }
  return narrowed - resting;
}

uint8_t dpad() {
  uint8_t direction, left, up, right, down = 0x0;
  left = ! digitalRead(BUTTON_LEFT);
  up   = ! digitalRead(BUTTON_UP);
  right= ! digitalRead(BUTTON_RIGHT);
  down = ! digitalRead(BUTTON_DOWN);
  direction = (left << 3) | (up << 2) | (right << 1) | down;
  switch (direction & 0xF) {
    // inspect only the 4 least significant bits
    case 0x1:   // b_0001
      return DPAD_DOWN;
    case 0x2:   // b_0010
      return DPAD_RIGHT;
    case 0x3:   // b_0011
      return DPAD_DOWN_RIGHT;
    case 0x4:   // b_0100
      return DPAD_UP;
    case 0x6:   // b_0110
      return DPAD_UP_RIGHT;
    case 0x8:  // b_1000
      return DPAD_LEFT;
    case 0x9:  // b_1001
      return DPAD_DOWN_LEFT;
    case 0xC:  // b_1100
      return DPAD_UP_LEFT;
    case 0x0:
    default:
      return DPAD_CENTERED;
  }
}
