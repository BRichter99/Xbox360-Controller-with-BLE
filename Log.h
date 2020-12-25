//#define SERIAL_OUT
/* comment out this first line to disable serial output */
#ifndef LOG_H
#define LOG_H

#include <HardwareSerial.h>


#ifdef SERIAL_OUT

#define SERIAL_INIT() do { Serial.begin(115200); } while (0)

#define PRINTF(...) do { Serial.printf(__VA_ARGS__); } while (0)

#define PRINT_BINARY(x) for (int i = 0; i < 8*sizeof(x); i++) { PRINTF("%d", (x >> i & 0x1)); }

#define STOPWATCH_START() do { t0 = esp_log_timestamp(); } while (0)

#define STOPWATCH_STOP() do { PRINTF("Average period: %dms (%d measurements)\n", \
                                (esp_log_timestamp() - t0) / n, n); } while (0)

#else
#define PASS do {} while (0)
#define SERIAL_INIT() PASS
#define PRINTF(...) PASS
#define PRINT_BINARY(x) PASS
#define STOPWATCH_START() PASS
#define STOPWATCH_STOP()  PASS
#endif

#endif //LOG_H
