#ifndef GAMEPAD_PINS_H
#define GAMEPAD_PINS_H

#include "BleGamepad.h"

/* ADC2 is unavailable when using Wifi/Bluetooth 

            _________________
  no-pu|lup | EN            | 23
ADC1   |-36 | VP            | 22
|      \-39 | VN         TX0|  1
|        34 |            RX0|  3
|        35 |               | 21
|        32 |   WROOM-32    | 19
ADC1     33 |    DEVKIT     | 18
ADC2 DAC1 25|               |  5
|   DAC2 26 |            TX2| 17
|        27 |            RX2| 16
|        14 |               |  4    ADC2
|        12 |               |  2       |
ADC2     13 |               | 15    ADC2
            |GND         GND|
            |VIN___USB___3V3|
*/

void setupPins();
uint16_t buttons();
void getAxes(int8_t *);
uint8_t dpad();

char dpadToChar(uint8_t);

#define N_BUTTONS 15
#define N_AXES     6
#define BUTTON_OUT 0
#define INPUT_PIN  1

// digital
#define BUTTON_LB     13  // XXX
#define BUTTON_LS     27  // TP31
#define BUTTON_LEFT    1  // TP32 DPAD
#define BUTTON_UP     19  // TP33 DPAD
#define BUTTON_RIGHT  21  // TP34 DPAD
#define BUTTON_DOWN    3  // TP35 DPAD
#define BUTTON_BACK   12  // TP41
#define BUTTON_XBOX   14  // TP42
#define BUTTON_START  23  // TP40
#define BUTTON_RB     17  // XXX
#define BUTTON_RS      5  // TP30
#define BUTTON_A      25  // TP39
#define BUTTON_B      22  // TP43
#define BUTTON_X      18  // TP44
#define BUTTON_Y      26  // TP38

// analog
#define POT_LS_X  36   // TP14
#define POT_LS_Y  39   // TP13
#define POT_LT    35   // TP 9
#define POT_RS_X  33   // TP12
#define POT_RS_Y  32   // TP11
#define POT_RT    34   // TP10
/* according to https://circuits4you.com/2018/12/31/esp32-devkit-esp32-wroom-gpio-pinout/
  analog pins on the ESP32 WROOM32 DevKit are:
  ADC1:   32, 33, 34, 35, 36, 39
  ADC2:   2, 4, 12, 13, 14, 15, 25, 26, 27
  DAC1:   25
  DAC2:   26  */

// leds
#define LED_1  2
#define LED_2 15
#define LED_3  4
#define LED_4 16

#endif  // GAMEPAD_PINS_H