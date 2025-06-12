#include "device.h"
#include <stdio.h>

void in(uint8_t input) {
  printf("%c", input);
}

Device console = {
  .id = 0x0,
  .in = in,
  .out = NULL
};
