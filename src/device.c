#include "device.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

void console_on_cycle(VirtualMachine *vm, Device* self) {
  uint8_t change_indicator = vm->memory[console.start];
  if (change_indicator != self->prev_change_indic) {
    console.prev_change_indic = change_indicator;
    printf("%c", vm->memory[self->end]);
  }
}

// first register is used to monitor changes. for all intents and purposes, it
// will just cycle between 1 and 0 for true and false. if the first register
// changes from last time, then the end register character will be printed as
// ascii to standard output
Device console = {.start = 0x0, .end = 0x1, .on_cycle = console_on_cycle};
Device *devices[] = {&console};
