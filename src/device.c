#include "device.h"

void in(uint8_t input) {}
uint8_t out() {
  return 0x0;
}

Device device = {
  .id = 0x0,
  .in = in,
  .out = out
};
