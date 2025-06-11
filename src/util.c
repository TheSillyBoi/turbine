#include "util.h"
#include <stdint.h>

void u16_split(uint16_t value, uint8_t *left, uint8_t *right) {
  *left = (uint8_t)(value & 0xFF);
  *right = (uint8_t)(value >> 8);
}

uint16_t u16_combine(uint8_t left, uint8_t right) {
  return ((uint16_t)right << 8) | left;
}
