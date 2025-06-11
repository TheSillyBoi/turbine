#include "util.h"
#include <stdint.h>

void u16_split(uint16_t value, uint8_t *low_byte, uint8_t *high_byte) {
  *low_byte = (uint8_t)(value & 0xFF);
  *high_byte = (uint8_t)(value >> 8);
}

uint16_t u16_combine(uint8_t left, uint8_t right) {
  return ((uint16_t)right << 8) | left;
}
