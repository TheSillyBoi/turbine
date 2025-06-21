#include "device.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

void console_on_cycle(VirtualMachine *vm) {
  uint8_t *data = &vm->memory[console.start];
  if (*data != 0) {
    printf("%c", *data);
    *data = 0;
  }
}

Device console = {.start = 0x0, .end = 0x0, .on_cycle = console_on_cycle};
Device *devices[] = {&console};
