#ifndef CSTR_H
#define CSTR_H

#include <stdint.h>

const char* to_string(uint64_t value);
const char* to_string(int64_t value);

const char* to_string(double value);
const char* to_string(double value, uint8_t decimals);

const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);

#endif // CSTR_H