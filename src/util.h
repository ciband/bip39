#ifndef BIP39_UTIL_H
#define BIP39_UTIL_H

#include <cstdint>

#define STRING_VAR_DECL(s) const char s##_str[] PROGMEM = #s;

#define STRING_VAR(s) s##_str

#if (defined ARDUINO || defined ESP8266 || defined ESP32)

#define BIP39_PLATFORM_IOT

#include <Arduino.h>
#include <pgmspace.h>

inline uint32_t generate_random_number(uint32_t min, uint32_t max, bool /* static_seed */ = false) {
	return random(min, max);
}

#else

#include <cstring>

// Define compatibility macros for non-Arduino platforms
#define PROGMEM
#define PGM_P const char*

#define pgm_read_ptr_far(p) (*p)
#define strcpy_P strcpy

uint32_t generate_random_number(uint32_t min, uint32_t max, bool static_seed = false);

#endif

#endif
