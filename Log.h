#define SERIAL_OUT
#include <HardwareSerial.h>

#define PASS do {} while (0)

#ifdef SERIAL_OUT

#define INIT() do { Serial.begin(115200); } while (0)

#define PRINTF(...) do { Serial.printf(__VA_ARGS__); } while (0)

#define PRINT_BINARY(x) for (int i = 0; i < 8*sizeof(x); i++) { PRINTF("%d", (b >> i & 0x1)); }

#else
#define INIT() PASS
#define PRINTF(...) PASS
#define PRINT_BINARY(x) PASS
#endif

char dpadToChar(uint8_t);
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
