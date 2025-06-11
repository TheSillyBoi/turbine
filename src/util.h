#ifndef INCLUDE_SRC_UTIL_H_
#define INCLUDE_SRC_UTIL_H_

#include <stdint.h>
void u16_split(uint16_t value, uint8_t *low_byte, uint8_t *high_byte);
uint16_t u16_combine(uint8_t left, uint8_t right);

#endif // INCLUDE_SRC_UTIL_H_
