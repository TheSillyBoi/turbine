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

// Device screen;

// void screen_on_cycle(VirtualMachine *vm) {
//   uint8_t buffer = vm->memory[screen.start];
//   size_t start = (buffer) ? screen.start + 64^2 : screen.start;
//   size_t end = (buffer) ? screen.end - 64^2 : screen.end;
//     for (size_t i = screen.start + 1; i <= screen.end; i++) {
//     }
// }

Device console = {.start = 0x0, .end = 0x0, .on_cycle = console_on_cycle};
// first byte is for double buffering. it's basically a bool which gets flipped
// to specify whether or not to use the 64x64 screen or the second one.
// Device screen = {.start = 0x1, .end = 0x2001, .on_cycle = screen_on_cycle};
Device *devices[] = {&console};
