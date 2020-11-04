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
